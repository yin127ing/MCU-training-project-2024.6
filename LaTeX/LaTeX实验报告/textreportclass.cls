\NeedsTeXFormat{LaTeX2e}
 
\ProvidesClass{testreport_class}[2023/11/18 binayi testReport]
 
 
\LoadClass[12pt,a4paper]{article}
 
\RequirePackage[T1]{fontenc}
\RequirePackage[left=3.18cm,right = 3.18cm, top = 2.54cm, bottom = 2.54cm]{geometry}
\RequirePackage{url}
\RequirePackage[heading=true]{ctex}  % 使用中文，要用到ctex宏包
\RequirePackage{graphics}
\RequirePackage{graphicx}
\RequirePackage{array}
\RequirePackage{listings} % 代码框
%\usepackage{listings}%代码框显示数学符号\begin{lstlisting}[mathescape]
\RequirePackage{framed} % listing中的分页问题
\RequirePackage{calc} % 等比例缩放
% 数学宏包
\RequirePackage{amsmath}
\RequirePackage{amsfonts}
\RequirePackage{amssymb}
\RequirePackage{bm}
\RequirePackage{titletoc}
% 首行缩进
\RequirePackage{indentfirst}
\renewcommand*{\baselinestretch}{1.38}%行间距
% 修改tabular 环境, 设置表格中的行间距为正文行间距.
\let\mcm@oldtabular\tabular
\let\mcm@endoldtabular\endtabular
\renewenvironment{tabular}%
{\bgroup%
	\renewcommand{\arraystretch}{1.5}%
	\mcm@oldtabular}%
{\mcm@endoldtabular\egroup}
% 每行缩进两个汉字
\setlength\parindent{2em}
 
 
% 数学宏包
\RequirePackage{amsmath}
\RequirePackage{amsfonts}
\RequirePackage{amssymb}
\RequirePackage{bm}
\RequirePackage{titletoc}
% 设置颜色
\RequirePackage{xcolor}
% 插入图片
\RequirePackage{graphicx}
\RequirePackage{float}
% 表格
\RequirePackage{array}
%% 长表格
\RequirePackage{longtable}
%% booktabs 提供了\toprule 等命令.
\RequirePackage{booktabs}
%% multirow 支持在表格中跨行
\RequirePackage{multirow}
%% 调整间隔, 让表格更好看些
\RequirePackage{bigstrut}
%% 在跨行表格中输入定界符
\RequirePackage{bigdelim}
% 保护脆落命令
\RequirePackage{cprotect}
\RequirePackage{enumitem}
\setenumerate[1]{itemsep=0pt,partopsep=0pt,parsep=\parskip,topsep=2pt}
\setitemize[1]{itemsep=0pt,partopsep=0pt,parsep=\parskip,topsep=2pt}
\setdescription{itemsep=0pt,partopsep=0pt,parsep=\parskip,topsep=2pt}
 
% 自定义页面风格
\RequirePackage{fancyhdr}
\fancypagestyle{bianyi}{
	\fancyhead{}  % 页眉空无下横线
	\renewcommand\headrulewidth{0pt}
	\fancyfoot{}  % 页脚空无下横线，R代表右，页脚右边有宋体五号的页码，这里页码从封面开始算
	\fancyfoot[R]{\songti\zihao{5}\thepage}
}
 
% 设置粗体
\setCJKfamilyfont{song}[AutoFakeBold]{SimSun}  % texlive升级后有些字体无法用textbf直接加粗，这里找到字体族，宋体加粗
\newcommand*{\bfsongti}{\CJKfamily{song}}   % 为粗宋体设置一个简单的名字\bfsongti
\setCJKfamilyfont{hei}[AutoFakeBold]{SimHei} % 同理设置粗体黑体
\newcommand*{\bfheiti}{\CJKfamily{hei}}
% 设置字体
\setmainfont{Times New Roman}
 
%自定义标题样式 这里设置了三级标题样式
\ctexset{
	section={
		format +=\bfsongti \zihao{4}  \raggedright  , % 加粗宋体四号，左对齐
		name={,、},   % 、分隔
		number=\chinese{section},  % 章节中文编号
		aftername =\hspace{0pt} ,
	},
	subsection={
		format +=\songti \zihao{-4}  \raggedright  ,  % 宋体小四，左对齐
		name={, \quad},   % 空格分隔
		number=\arabic{section}.\arabic{subsection},  % 章节编号.二级编号 这里编号用阿拉伯数字
		aftername =\hspace{1pt } ,
	},
	subsubsection={
		format +=\songti \zihao{-4}  \raggedright  , % 宋体，小四，左对齐
		name={, \quad},   % 章节序号前空两个空格，后为英文句号
		number=\arabic{section}.\arabic{subsection}.\arabic{subsubsection}, % 阿拉伯数字序号
		aftername =\hspace{1pt } ,
	}
}
 
 
 
% 设置浮动体的标题
\RequirePackage{caption}
% 定制列表环境
\RequirePackage{enumitem}
% 图表标题
\DeclareCaptionFont{song}{\songti}
\DeclareCaptionFont{minusfive}{\zihao{5}}
\captionsetup[figure]{%
	format=hang,   % 标题从第二行开始都有缩进, 应该和 justification=raggedright 的效果一样.
	labelsep=quad, % 分隔符是一个空格
	font={song,minusfive}, % 图的字体, 宋体五
	position=bottom % position=bottom, 不代表标题放在下面, 标题仍放在你放\caption的位置.
}
\DeclareCaptionFont{minusfour}{\zihao{4}}
\DeclareCaptionFont{bfsong}{\bfsongti}
\captionsetup[table]{%
	format=hang,   % 标题从第二行开始都有缩进, 应该和 justification=raggedright 的效果一样.
	labelsep=quad, % 分隔符是一个空格
	font={bfsong,minusfour}, % 表的字体, 宋体四
	position=top % position=bottom, 不代表标题放在下面, 标题仍放在你放\caption的位置.
}
 
 
% 设置代码环境 ，这里引用国赛模版的代码，它设置的代码框底色和代码字体颜色比较好看
\RequirePackage{listings}
\RequirePackage{xcolor}
 
\RequirePackage[titles]{tocloft}
\renewcommand{\cftdot}{$\cdot$}
\renewcommand{\cftsecdotsep}{1.5} 
\setlength{\cftbeforesecskip}{7pt}
\setlength{\cftbeforesubsecskip}{3pt}
\renewcommand{\cftsecfont}{\bfseries\zihao{-4}\songti}
\renewcommand{\cftsecleader}{\cftdotfill{\cftsecdotsep}}
\renewcommand{\cftsecaftersnumb}{\hskip.4em}
 
 
 
\definecolor{dkgreen}{rgb}{0,0.6,0}
\definecolor{gray}{rgb}{0.5,0.5,0.5}
\definecolor{mauve}{rgb}{0.58,0,0.82}
 
\lstset{
	frame=tb,
	aboveskip=3mm,
	belowskip=3mm,
	showstringspaces=false,
	columns=flexible,
	framerule=1pt,
	rulecolor=\color{gray!35},
	backgroundcolor=\color{gray!5},
	basicstyle={\small\ttfamily},
	numbers=none,
	numberstyle=\tiny\color{gray},
	keywordstyle=\color{blue},
	commentstyle=\color{dkgreen},
	stringstyle=\color{mauve},
	breaklines=true,
	breakatwhitespace=true,
	tabsize=3,
}