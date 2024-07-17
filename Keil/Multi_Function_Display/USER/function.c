//**********************************************************************
//功能函数
#include<header.h>


void delay(int z)				//延时程序
{
    int x, y;
    for (x = 55; x > 0; x--)
        for (y = z; y > 0; y--);
}

void wordByt(char b)			//写（拆分）一个字节数据  传给595芯片
{
    char i;
    for (i = 0; i < 8; i++)
    {
        b = b >> 1;			//右移一位，暂存近CY
        R = CY;				//取出最低位
        CLK = 0;			//上升沿	
        CLK = 1;
    }
}

void seg_switch(uchar num) {		 //让138译码器选中位选
    seg3 = num / 4 % 2;
    seg2 = num / 2 % 2;
    seg1 = num % 2;
}
delay_ms(uint x)    //ms延时函数
{
    uint i, j;
    for (j = 0; j < x; j++)
        for (i = 0; i < 123; i++);
}
void Init_Timer0(void)  //定时器0  初始化   用于提供时间 10ms中断一次
{
    TMOD |= 0x01;    //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响
    TH0 = (65536 - 9216) / 256;  //给定初值，
    TL0 = (65536 - 9216) % 256;
    EA = 1;          //总中断打开
    ET0 = 1;         //定时器中断打开
    TR0 = 1;         //定时器开关打开
}




int i = 0;

void NO_1_main()
{

    int num, speed;     //num用于显示   speed用于显示的速度的
    uint temp;              //缓存 一字节数据  
    uchar hanzi_witch = 1;  //显示汉字那组数据   还是显示ASCII那组数据

    hanzi_num = 3;        //当前组数据 汉字的个数


    key_u = 1;       //按键高电平
    key_d = 1;
    key_r = 1;
    key_l = 1;
    key_m = 1;
    key_change = 1;
    order = 'm';    //开机默认逐字切换功能
    move = 0;      //移动变量为0  从头开始

    while (!CHANGE1)
    {


        if (key_u == 0) {    //向上移动的按键   让模式为 ‘u’  ，从头开始
            order = 'u';
            move = 0;
        }
        if (key_d == 0) {   //向下移动的按键按下  从头开始
            order = 'd';
            move = 0;
            for (i = 0; i < 16; i++) {   //初始化要显示的缓存数组（复制过来）
                show_temp[i * 2] = table1[hanzi_table_num - 1][i * 2];
                show_temp[i * 2 + 1] = table1[hanzi_table_num - 1][i * 2 + 1];
            }
        }
        if (key_r == 0) {   //向右移动的按键按下  从头开始
            order = 'r';
            move = 0;
            for (i = 0; i < 16; i++) {    //初始化要显示的缓存数组（复制过来） 
                show_temp[i * 2] = table1[hanzi_table_num - 1][i * 2];
                show_temp[i * 2 + 1] = table1[hanzi_table_num - 1][i * 2 + 1];
                zuoyou_temp[i] = table1[hanzi_table_num - 1][i * 2 + 32 + 1];   //以及关系到下一个汉字的后一半内容，也缓存过来   用于衔接上一个文字
            }
        }
        if (key_l == 0) { //向左移动的按键按下  从头开始
            order = 'l';
            move = 0;
            for (i = 0; i < 16; i++) {   //初始化要显示的缓存数组（复制过来）
                show_temp[i * 2] = table1[hanzi_table_num - 1][i * 2];
                show_temp[i * 2 + 1] = table1[hanzi_table_num - 1][i * 2 + 1];
                zuoyou_temp[i] = table1[hanzi_table_num - 1][i * 2 + 32];    //以及关系到下一个汉字的前一半内容，也缓存过来   用于衔接上一个文字
            }
        }
        if (key_m == 0) {  //逐个字切换的按键按下  从头开始
            order = 'm';
            move = 0;
        }
        if (key_change == 0) {   //显示内容数组变化  按键按下   切换内容序号
            move = 0;
            hanzi_witch++;
            if (hanzi_witch >= 3) hanzi_witch = 1;

            if (hanzi_witch == 1) {   //1  是汉字数组   汉字3个   索引为1
                hanzi_num = 3;
                hanzi_table_num = 1;
            }
            if (hanzi_witch == 2) { //2  是字符数组  页面4个   索引为2
                hanzi_num = 4;
                hanzi_table_num = 2;
            }
            order = 'm';                 //默认为 逐页变化的   
            while (key_change == 0);    //等待按键松开

        }

        for (num = 0; num < 16; num++) //显示处理  16*16图像    逐行显示  ，共显示16行
        {
            if (num < 8) {          //前8行
                wordByt(Hang[num]);
                wordByt(0xff);
            }
            else {         //后8行
                wordByt(0xff);
                wordByt(Hang[num - 8]);
            }
            if ((order == 'u') || (order == 'm')) {    //向上移动  和 逐字变化   时，直接计算从原始数据获取显示
                wordByt(table1[hanzi_table_num - 1][2 * num + move * 2]);
                wordByt(table1[hanzi_table_num - 1][2 * num + 1 + move * 2]);
            }
            if ((order == 'd') || (order == 'l') || (order == 'r')) {  //向下 向左 右 模式，显示处理后的缓存数组内容
                wordByt(show_temp[2 * num]);
                wordByt(show_temp[2 * num + 1]);
            }
            STB = 1;  //输出锁存中的数据，下降沿    显示一行
            STB = 0;
            delay(2);   //  延时一下
        }


        if (order == 'l') {      //向左移动
            if (++speed > 6) //速度控制（数字小速度快）
            {
                speed = 0;
                move++;   //移位  移动1列

                for (i = 0; i < 16; i++) {   //数据处理
                    if (show_temp[i * 2 + 1] & 0x01 == 0x01)  show_temp[i * 2] = ((show_temp[i * 2]) >> 1) | 0x80; //先向左移动一列    数组内容
                    else show_temp[i * 2] = ((show_temp[i * 2]) >> 1) & 0x7f;
                    if (zuoyou_temp[i] & 0x01 == 0x01)  show_temp[i * 2 + 1] = ((show_temp[i * 2 + 1]) >> 1) | 0x80;  //判断下一图像的一列，根据0或1插入上面移动1列后的数组里
                    else show_temp[i * 2 + 1] = ((show_temp[i * 2 + 1]) >> 1) & 0x7f;
                    zuoyou_temp[i] = zuoyou_temp[i] >> 1;    //下一图像跟着移动1列
                }
                if (move % 8 == 0) {   //移动当前图像的8步之内   更新下一个图像的前一半内容到缓存数组里 
                    if (move / 8 % 2 == 0) //上一边
                        for (i = 0; i < 16; i++) {
                            zuoyou_temp[i] = table1[hanzi_table_num - 1][(move / 16 + 1) * 32 + i * 2];
                        }

                    if (move / 8 % 2 == 1) //下一边   //移动当前图像的8-15步   更新下一个图像的后一半内容到缓存数组里 
                        for (i = 0; i < 16; i++) {
                            zuoyou_temp[i] = table1[hanzi_table_num - 1][(move / 16 + 1) * 32 + i * 2 + 1];
                        }

                }

                if (move > 16 * hanzi_num) { //判断是否完成一个数组数据
                    move = 0;                        //从头开始
                    for (i = 0; i < 16; i++) {    //复制数据到缓存数组中
                        show_temp[i * 2] = table1[hanzi_table_num - 1][i * 2];
                        show_temp[i * 2 + 1] = table1[hanzi_table_num - 1][i * 2 + 1];
                        zuoyou_temp[i] = table1[hanzi_table_num - 1][i * 2 + 32];    //以及关系到下一个汉字的前一半内容，也缓存过来   用于衔接上一个文字
                    }
                }
            }
        }
        if (order == 'r') {      //向右移动
            if (++speed > 6) //速度控制（数字小速度快）
            {
                speed = 0;
                move++;   //移位

                for (i = 0; i < 16; i++) { //先向右移动一个   显示数据的处理

                    temp = show_temp[i * 2] >> 7; //   移动根据后一半图像的1列数据，转换添加到右移1列后的前一半图像
                    if (temp & 0x01 == 0x01)  show_temp[i * 2 + 1] = ((show_temp[i * 2 + 1]) << 1) | 0x01;
                    else show_temp[i * 2 + 1] = ((show_temp[i * 2 + 1]) << 1) & 0xfe;

                    temp = zuoyou_temp[i] >> 7;   //根据下1个图像的前半部分的1列数据，判断添加到上1个图像的1列中
                    if (temp & 0x01 == 0x01)  show_temp[i * 2] = ((show_temp[i * 2]) << 1) | 0x01;
                    else show_temp[i * 2] = ((show_temp[i * 2]) << 1) & 0xfe;
                    zuoyou_temp[i] = zuoyou_temp[i] << 1;  //缓存也移动1列

                }

                if (move % 8 == 0) {
                    if (move / 8 % 2 == 0) //上一边//移动当前图像的8步之内   更新下一个图像的上一半内容到缓存数组里 
                        for (i = 0; i < 16; i++) {
                            zuoyou_temp[i] = table1[hanzi_table_num - 1][(move / 16 + 1) * 32 + i * 2 + 1];
                        }
                    if (move / 8 % 2 == 1) //下一边//移动当前图像的8步之内   更新下一个图像的下一半内容到缓存数组里 
                        for (i = 0; i < 16; i++) {
                            zuoyou_temp[i] = table1[hanzi_table_num - 1][(move / 16 + 1) * 32 + i * 2];
                        }
                }

                if (move > 16 * hanzi_num) { //判断是否完成一个汉字
                    move = 0;          //从头开始
                    for (i = 0; i < 16; i++) {  //复制数据  从新开始
                        show_temp[i * 2] = table1[hanzi_table_num - 1][i * 2];
                        show_temp[i * 2 + 1] = table1[hanzi_table_num - 1][i * 2 + 1];
                        zuoyou_temp[i] = table1[hanzi_table_num - 1][i * 2 + 32 + 1];   //以及关系到下一个汉字的后一半内容，也缓存过来   用于衔接上一个文字
                    }
                }
            }
        }

        if (order == 'd') {      //向下移动
            if (++speed > 5) //速度控制（数字小速度快）
            {
                speed = 0;
                move++;   //移位

                for (i = 14; i >= 0; i--) {       //缓存数据先向下移动一个
                    show_temp[(i + 1) * 2] = show_temp[i * 2];
                    show_temp[(i + 1) * 2 + 1] = show_temp[i * 2 + 1];
                }
                //第一行数据由下一个图像的最后一行数据填充
                show_temp[0] = table1[hanzi_table_num - 1][(move / 16 + 2) * 32 - (move * 2) % 32];
                show_temp[1] = table1[hanzi_table_num - 1][(move / 16 + 2) * 32 + 1 - (move * 2) % 32];

                if (move > 16 * hanzi_num) { //判断是否完成一个汉字
                    move = 0;            //从头开始
                    for (i = 0; i < 16; i++) {   //复制数据，重新开始
                        show_temp[i * 2] = table1[hanzi_table_num - 1][i * 2];
                        show_temp[i * 2 + 1] = table1[hanzi_table_num - 1][i * 2 + 1];
                    }

                }
            }
        }

        if (order == 'u') {      //向上移动    它的数组数据顺序符合向上的，所以数据不必处理
            if (++speed > 5) //速度控制（数字小速度快）
            {
                speed = 0;
                move++;   //移位
                if (move > 16 * hanzi_num) //判断是否完成一个汉字
                    move = 0; //从头开始
            }
        }
        if (order == 'm') {        //逐个字显示
            if (++speed > 25) //速度控制（数字小速度快）
            {
                speed = 0;
                move += 16; //移位   下一个图像
                if (move > 16 * hanzi_num) //判断是否完成一个汉字
                    move = 0; //从头开始
            }
        }
    }
}


void NO_2_main()
{
    //uint i;
    uint temp;			//温度
    uchar mode_num = 0;		//模式的值
    uint run_times = 0;     //主循环中运行多少次
    uchar wei_num = 0;     //时分秒位数的
    DataPort = 0xff;	   //不显示
    buzzer = 1;    //蜂鸣器不响

    //uart_init();
    Init_Timer0(); //定时器0  中断 用于数码管显示
    Init_DS18B20();		 //初始化ds18b20
    ReadTemperature();	 //读取温度，
    delay_ms(20);

    while (!CHANGE2)        //大循环
    {
        if (key_mode == 0) {	//模式切换	按钮
            delay_ms(10);
            if (key_mode == 0) {
                mode_num++;
                if (mode_num >= 3) mode_num = 0;	//对应3种模式

                setting_flag = 0;  //切换模式的时候关闭设置
                while (key_mode == 0);
                delay_ms(10);
            }
        }
        if (key_2 == 0) {		 //设置	按钮
            delay_ms(10);
            if (key_2 == 0) {
                setting_flag = 1 - setting_flag;	//取反
                if (mode_num == 2) {		//计时模式下  开始  暂停
                    jishi_flag = 1 - jishi_flag;
                }
                while (key_2 == 0);
                //delay_ms(10);
            }
        }
        if (key_3 == 0) {		 //时分秒  位数选择	 复位计时
            delay_ms(10);
            if (key_3 == 0) {
                if (mode_num == 0) {	 //会闪烁
                    wei_num++;			 //即将设置的那两位数  会闪烁
                    if (wei_num >= 3) wei_num = 0;
                }
                if (mode_num == 1) {
                    wei_num++;
                    if (wei_num >= 3) wei_num = 0;
                }
                if (mode_num == 2) {	 //复位计时
                    jishi_min = 0;
                    jishi_sec = 0;
                    jishi_5ms = 0;
                    jishi_flag = 0;
                }

                while (key_3 == 0);
                delay_ms(10);
            }
        }
        if (key_jian == 0) {		 //--  时钟、闹钟的减
            delay_ms(10);
            if (key_jian == 0) {
                if (mode_num == 0) {
                    if (wei_num == 0) {
                        if (hour == 0) hour = 23;
                        else hour--;
                    }
                    if (wei_num == 1) {
                        if (min == 0) min = 59;
                        else min--;
                    }
                    if (wei_num == 2) {
                        if (sec == 0) sec = 59;
                        else sec--;
                    }
                }
                if (mode_num == 1) {
                    if (wei_num == 0) {
                        if (naozhong_hour <= 0) naozhong_hour = 23;
                        else naozhong_hour--;
                    }
                    if (wei_num == 1) {
                        if (naozhong_min <= 0) naozhong_min = 59;
                        else naozhong_min--;
                    }
                    if (wei_num == 2) {
                        if (naozhong_flag <= 0) naozhong_flag = 1;
                        else  naozhong_flag--;
                    }
                }
                while (key_jian == 0);
                delay_ms(10);
            }
        }
        if (key_jia == 0) {		 //++    时钟、闹钟的  加
            delay_ms(10);
            if (key_jia == 0) {
                if (mode_num == 0) {
                    if (wei_num == 0) {
                        hour++;
                        if (hour >= 24) hour = 0;
                    }
                    if (wei_num == 1) {
                        min++;
                        if (min >= 60) min = 0;
                    }
                    if (wei_num == 2) {
                        sec++;
                        if (sec >= 60) sec = 0;
                    }
                }
                if (mode_num == 1) {
                    if (wei_num == 0) {
                        naozhong_hour++;
                        if (naozhong_hour >= 24) naozhong_hour = 0;
                    }
                    if (wei_num == 1) {
                        naozhong_min++;
                        if (naozhong_min >= 60) naozhong_min = 0;
                    }
                    if (wei_num == 2) {
                        naozhong_flag++;
                        if (naozhong_flag >= 2) naozhong_flag = 0;
                    }
                }
                while (key_jia == 0);
                delay_ms(10);
            }
        }
        if (naozhong_flag == 1) {    //闹钟开启	  响1分钟
            if ((naozhong_hour == hour) && (naozhong_min == min)) buzzer = 0;
            else buzzer = 1;
        }
        else buzzer = 1;

        if (mode_num == 0) {    //显示时间模式
            seg_show[2] = 0xbf;   //“-”
            seg_show[5] = 0xbf;   //“-”
            seg_show[0] = dofly_table[hour / 10 % 10];
            seg_show[1] = dofly_table[hour % 10];
            seg_show[3] = dofly_table[min / 10 % 10];
            seg_show[4] = dofly_table[min % 10];
            seg_show[6] = dofly_table[sec / 10 % 10];
            seg_show[7] = dofly_table[sec % 10];
            if ((setting_flag == 1) && (sec_temp < 100)) {   //是否为设置情况
                seg_show[3 * wei_num] = 0xff;
                seg_show[3 * wei_num + 1] = 0xff;
            }
        }

        if (mode_num == 1) {				 //闹钟模式
            seg_show[2] = 0xbf;   //“-”
            seg_show[5] = 0xff;   //“ ”
            seg_show[6] = 0xff;	//‘ ’
            seg_show[0] = dofly_table[naozhong_hour / 10 % 10];
            seg_show[1] = dofly_table[naozhong_hour % 10];
            seg_show[3] = dofly_table[naozhong_min / 10 % 10];
            seg_show[4] = dofly_table[naozhong_min % 10];

            seg_show[7] = dofly_table[naozhong_flag % 10];
            if ((setting_flag == 1) && (sec_temp < 100)) {	  //是否设置情况
                seg_show[3 * wei_num] = 0xff;
                seg_show[3 * wei_num + 1] = 0xff;
            }
        }

        if (mode_num == 2) {	  //温度+秒表  模式
            run_times++;
            if (run_times >= 50) {	 //温度不能太高频的获取
                run_times = 0;
                temp = (uint)((float)ReadTemperature() * 0.0625); //温度
                if (temp >= 1) {		 //防止因为中断出错
                    seg_show[0] = dofly_table[temp / 10 % 10];
                    seg_show[1] = dofly_table[temp / 1 % 10];
                }
            }		   //刷新秒表计时的数值
            seg_show[2] = 0xff;   //“ ”
            seg_show[3] = dofly_table[jishi_min / 10 % 10];
            seg_show[4] = dofly_table[jishi_min % 10] & 0x7f;
            seg_show[5] = dofly_table[jishi_sec / 10 % 10];
            seg_show[6] = dofly_table[jishi_sec % 10] & 0x7f;
            seg_show[7] = dofly_table[jishi_5ms % 10];
        }

        delay_ms(10);
    }

    DataPort = 0xff;

}

//定时器0中断服务函数   用于数码管的显示
void Timer0_isr(void) interrupt 1 using 1
{
    
    TH0 = (65536 - 4608) / 256;  //给定初值， 5ms
    TL0 = (65536 - 4608) % 256;
    seg_num++;	     //数码管刷新
    if (seg_num >= 8)  seg_num = 0;
    DataPort = 0xff;       //这里用于显示时间  时分秒
    seg_switch(seg_num);   //位选 
    DataPort = seg_show[seg_num];  //段选

    //有关计时功能的
    if (jishi_flag == 1) {    //计时  开始 继续
        jishi_5ms++;
        if (jishi_5ms >= 200) {
            jishi_5ms = 0;
            jishi_sec++;
        }
        if (jishi_sec >= 60) {
            jishi_sec = 0;
            jishi_min++;
        }
        if (jishi_min >= 60) {
            jishi_min = 0;
        }
    }
    //有关时钟功能的
    sec_temp++;
    if (sec_temp >= 200) {	   //200次得1秒
        sec_temp = 0;
        sec++;
    }
    if (sec >= 60) {
        sec = 0;
        min++;
    }
    if (min >= 60) {
        min = 0;
        hour++;
    }
    if (hour >= 24) {
        hour = 0;
    }
}






