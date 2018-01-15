#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <sqlite3.h>
#include <opencv2/opencv.hpp>

#include "../../../PUBILIC/include/basetype.h"

#include "cvtest_imgproc.h"
#include "cvtest_rc.h"

using namespace cv;

/* 全局数组用来记录信号过来的顺序 */
static UINT g_auiSigOrder[BUF_LEN_128] = {0, };
pthread_rwlock_t g_stRdLock;
UINT g_uiRdLockNum = 0;

/* __thread申明表示每个线程一个副本 */
__thread UINT g_uiThreadData = 0;

FLOAT g_afTempPrewittY[9] = 
{
    1,  1,  1,
    0,  0,  0,
   -1, -1, -1
};
FLOAT g_afTempPrewittX[9] = 
{
    1,  0, -1,
    1,  0, -1,
    1,  0, -1
};

Mat _cvtest_prewitt(Mat stSrc)
{
    assert(1 == stSrc.channels());

    INT iIdxX;
    INT iIdxY;
    Mat stDstX;
    Mat stDstY;
    Mat stRet = Mat(stSrc.size(), stSrc.type(), stSrc.channels());;
    UCHAR ucTmpx;
    UCHAR ucTmpy;
    FLOAT fTemp;
    
    /* 构造矩阵 */
    Mat stTmpx = Mat(3, 3, CV_32F, g_afTempPrewittX);
    Mat stTmpy = Mat(3, 3, CV_32F, g_afTempPrewittY);

    /* 二维滤波 */
    filter2D(stSrc, stDstX, stSrc.depth(), stTmpx);    
    filter2D(stSrc, stDstY, stSrc.depth(), stTmpy);

    for (iIdxX = 0; iIdxX < stSrc.rows; ++iIdxX)
    {
        for (iIdxY = 0; iIdxY < stSrc.cols; ++iIdxY)
        {
            ucTmpx = stDstX.at<uchar>(iIdxX, iIdxY);
            ucTmpy = stDstY.at<uchar>(iIdxX, iIdxY);
            fTemp = sqrt(ucTmpx * ucTmpx + ucTmpy * ucTmpy);    
            stRet.at<uchar>(iIdxX, iIdxY) = fTemp;    
        }
    }
    return stRet;
}
Mat _cvtest_sobel(Mat stSrc)
{
    Mat stGradX;
    Mat stGradY;
    Mat stAbsGradx;
    Mat stAbsGrady;
    Mat stRet;
    
    assert(1 == stSrc.channels());

    /* 高斯模糊 */
    GaussianBlur(stSrc, stSrc, Size(3, 3), 0, 0, BORDER_DEFAULT);

    /* x方向梯度 */
    Sobel(stSrc, stGradX, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(stGradX, stAbsGradx);
    /* y方向梯度 */
    Sobel(stSrc, stGradY, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(stGradY, stAbsGrady);

    addWeighted(stAbsGradx, 0.5, stAbsGrady, 0.5, 0, stRet);

    return stRet;
}

Mat _cvtest_getHistImg(Mat hist)
{
    double maxVal=0;
 
    /* 找到直方图中的最大值和最小值 */
    minMaxLoc(hist, NULL, &maxVal, NULL, NULL);
    
    int histSize = hist.rows;
    Mat histImg(histSize, histSize, CV_8U, Scalar(255));
    
    /* 设置最大峰值为图像高度的90% */
    int hpt = static_cast<int>(0.9*histSize);
 
    for(int h=0; h<histSize; h++)
    {
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal * hpt / maxVal);
        line(histImg, Point(h,histSize), Point(h, histSize-intensity), Scalar::all(0));
    }
 
    return histImg;
}

Mat _cvtest_robert(Mat stSrc)
{
    assert(1 == stSrc.channels());
    Mat stResult = stSrc.clone();
    /* robet 算子:  
            1  0        0 1
            0 -1       -1 0
    */
    for(int i=0; i < stSrc.rows-1; i++)  
    {  
        for(int j=0; j < stSrc.cols-1; j++)  
        {  
            stResult.at<uchar>(i,j) = sqrt((stSrc.at<uchar>(i, j) - stSrc.at<uchar>(i+1, j+1))*
                (stSrc.at<uchar>(i,j) - stSrc.at<uchar>(i+1,j+1)) +
                (stSrc.at<uchar>(i,j+1) - stSrc.at<uchar>(i+1,j))*
                (stSrc.at<uchar>(i,j+1) - stSrc.at<uchar>(i+1,j)));    
        }  
    }
    return stResult;
}
ULONG cvtest_Test4_Rgb2Gray()
{
    ULONG ulRet = ERROR_SUCCESS;
    CHAR szPath[CVTEST_MAX_PATH_LEN] = {0, };
    CHAR szDstPath[CVTEST_MAX_PATH_LEN] = "gray.jpg";
    Mat stSrcImg;
    Mat stDst;

    printf("Please input the path of RGB img : \n");    
    scanf("%s", szPath);
    stSrcImg = imread(szPath, IMREAD_UNCHANGED);
    if (stSrcImg.empty())
    {
        printf("Load img failed, path(%s)", szPath);
        ulRet =  ERROR_FAILED;        
    }
    else
    {   
        if (3 == stSrcImg.channels())
        {
            cvtColor(stSrcImg, stDst, COLOR_BGR2GRAY);
            imwrite(szDstPath, stDst);
            ulRet = ERROR_SUCCESS;
        }
        else 
        {
            ulRet =  ERROR_FAILED;        
        }
    }
    return ulRet;
}
ULONG cvtest_Test4_Hong2Str()
{
    ULONG ulRet = ERROR_SUCCESS;

    printf("to_str = %s, append_str = %s\n", TOSTRING(1+1), STR_APPEND2_STR(ABC_, ABC));
    
    return ulRet;
}
ULONG cvtest_Test4_DlopenFunc()
{
    ULONG ulRet = ERROR_SUCCESS;

    return ulRet;
}
ULONG cvtest_Test4_GetHistogram()
{
    Mat stSrc;
    Mat stGray;
    Mat stRet;
    CHAR szPath[CVTEST_MAX_PATH_LEN] = {0, };
    CHAR szDstPath[CVTEST_MAX_PATH_LEN] = {0, };
    CHAR szPrefix[CVTEST_MAX_PREFIX_LEN] = {0, };
    CHAR *pcChrDot  = NULL;
    ULONG ulRet = ERROR_SUCCESS;
    INT iChannel;
    Mat stHist;
    int histSize = 256 ;        /* 横坐标区间数 */
    float hranges[] = {0, 256};          /* 每个区间的范围---不懂 */
    const float* ranges={hranges};
    
    printf("Please input the path of img : \n");    
    scanf("%s", szPath);
    stSrc = imread(szPath, IMREAD_UNCHANGED);
    if (stSrc.empty())
    {
        printf("Load img failed, path(%s).\n", szPath);
        ulRet =  ERROR_FAILED;        
    }
    else
    {   
        iChannel = stSrc.channels();
        if (3 == iChannel)
        {
            cvtColor(stSrc, stGray, COLOR_BGR2GRAY);
        }
        else if (1 == iChannel)
        {
            stGray = stSrc.clone();
        }
        else 
        {
            ulRet = ERROR_NOT_SUPPORT;
        }
    }
    if (ERROR_SUCCESS == ulRet)
    {
        calcHist(&stGray, 1, 0, Mat(), stHist, 1, &histSize, &ranges);
        
        stRet = _cvtest_getHistImg(stHist);
        if (stRet.empty())
        {
            ulRet = ERROR_FAILED;
        }
        else 
        {
            /* 寻找最后一个点 */
            pcChrDot = strrchr(szPath, '.');
            if (NULL != pcChrDot)
            {
                /* 提取前缀 */
                strncpy(szPrefix, szPath, pcChrDot - szPath);
                
                snprintf(szDstPath, sizeof(szDstPath), "%s_hist.jpg", szPrefix);
                imwrite(szDstPath, stRet);
            }
        }
    }
    return ulRet;
}

ULONG cvtest_Test4_Merge2Img()
{
	CHAR szPath1[BUF_LEN_256] = {0, };
    CHAR szPath2[BUF_LEN_256] = {0, };
    Mat stImg1;
    Mat stImg2;
	Mat stRet;
    INT iMaxRow, iMaxCol;
    DOUBLE dAlpha, dBeta;
    Size stRetSize;
    CHAR szDstPath[CVTEST_MAX_PATH_LEN] = "merge2.jpg";
    
    printf("Please input the path of two img : path1 path2\n");
    scanf("%s %s", szPath1, szPath2);
    
    /* 此处应该获取系统支持的最长路径名 */
	stImg1 = imread(szPath1, IMREAD_COLOR);  /* 总是以RGB色打开 */
	stImg2 = imread(szPath2, IMREAD_COLOR);  
	if (stImg1.empty() || stImg2.empty())
	{
		printf("Load img failed. path1(%s), path2(%s).\n", szPath1, szPath2);
		return ERROR_FAILED;
	}
    printf("Please input the alpha and beta for ret = alpha * img1 + beta *img2 \n");
    scanf("%lf %lf", &dAlpha, &dBeta);
    iMaxRow = MAX(stImg1.rows, stImg2.rows);
    iMaxCol = MAX(stImg1.cols, stImg2.cols);
    stRetSize = Size(iMaxCol, iMaxRow);
    
	/* 把wenli的图片放小--三次样条插值 */
    if ((iMaxCol != stImg1.cols) || (iMaxRow != stImg1.rows))
    {
        resize(stImg1, stImg1, stRetSize, 0, 0, INTER_CUBIC);   
    }
    if ((iMaxCol != stImg2.cols) || (iMaxRow != stImg2.rows))
    {
        resize(stImg2, stImg2, stRetSize, 0, 0, INTER_CUBIC);   
    }
    stRet = Mat(iMaxRow, iMaxCol, CV_8UC3);
    addWeighted(stImg1, dAlpha, stImg2, dBeta, 0, stRet, CV_8UC3);
    imwrite(szDstPath, stRet);

    #if 0
	/* 指定大图flower的兴趣区域 */
	stSubRet1 = stFlower(Rect(iFlowerCols - iWenliCols, 0, iWenliCols, iWenliRows));
	stSubRet2 = stFlower(Rect(0, iFlowerRows - iMyRows, iMyRows, iMyCols));

	/* 将小图拷贝到兴趣取悦 */
	stWenli.copyTo(stSubRet1);
	stMyself.copyTo(stSubRet2);

	/* 获取当前路径 */
	getcwd(szPath, BUF_LEN_256);
	printf("Path is : %s \n", szPath);
	ULONG ulLen = strlen(szPath);
	snprintf(szPath + ulLen, sizeof(szPath) - ulLen, "/%s", "test.jpg");
    #endif 
    
    return ERROR_SUCCESS;
}
ULONG cvtest_Test4_RobertEdge()
{
    ULONG ulRet = ERROR_SUCCESS;
    CHAR szPath[CVTEST_MAX_PATH_LEN] = {0, };
    CHAR szDstPath[CVTEST_MAX_PATH_LEN] = "robert.jpg";
    Mat stSrcImg;
    Mat stGray;
    Mat stDst;
    INT iChannels;

    printf("Please input the path of img : \n");    
    scanf("%s", szPath);
    stSrcImg = imread(szPath, IMREAD_UNCHANGED);
    if (stSrcImg.empty())
    {
        printf("Load img failed, path(%s)", szPath);
        ulRet =  ERROR_FAILED;        
    }
    else
    {   
        iChannels = stSrcImg.channels();
        if (3 == iChannels)
        {
            /* 若为彩色图像,则进行灰度化处理 */
            cvtColor(stSrcImg, stGray, COLOR_BGR2GRAY);
        }
        else if (1 == iChannels) 
        {
            stGray = stSrcImg.clone();
        }
        else
        {
            ulRet = ERROR_FAILED;
        }

        if ((ERROR_SUCCESS == ulRet) && (!stGray.empty()))
        {
            stDst = _cvtest_robert(stGray);
            imwrite(szDstPath, stDst);         
        }
    }
    return ulRet;
}
ULONG cvtest_Test4_SobelEdge()
{    
    ULONG ulRet = ERROR_SUCCESS;
    CHAR szPath[CVTEST_MAX_PATH_LEN] = {0, };
    CHAR szDstPath[CVTEST_MAX_PATH_LEN] = "sobel.jpg";
    Mat stSrcImg;
    Mat stGray;
    Mat stDst;
    INT iChannels;

    printf("Please input the path of img : \n");    
    scanf("%s", szPath);
    stSrcImg = imread(szPath, IMREAD_UNCHANGED);
    if (stSrcImg.empty())
    {
        printf("Load img failed, path(%s)", szPath);
        ulRet =  ERROR_FAILED;        
    }
    else
    {   
        iChannels = stSrcImg.channels();
        if (3 == iChannels)
        {
            /* 若为彩色图像,则进行灰度化处理 */
            cvtColor(stSrcImg, stGray, COLOR_BGR2GRAY);
        }
        else if (1 == iChannels) 
        {
            stGray = stSrcImg.clone();
        }
        else
        {
            ulRet = ERROR_FAILED;
        }

        if ((ERROR_SUCCESS == ulRet) && (!stGray.empty()))
        {
            stDst = _cvtest_sobel(stGray);
            imwrite(szDstPath, stDst);         
        }
    }
    return ulRet;
}
ULONG cvtest_Test4_PrewittEdge()
{
    ULONG ulRet = ERROR_SUCCESS;
    CHAR szPath[CVTEST_MAX_PATH_LEN] = {0, };
    CHAR szDstPath[CVTEST_MAX_PATH_LEN] = "prewitt.jpg";
    Mat stSrcImg;
    Mat stGray;
    Mat stDst;
    INT iChannels;

    printf("Please input the path of img : \n");    
    scanf("%s", szPath);
    stSrcImg = imread(szPath, IMREAD_UNCHANGED);
    if (stSrcImg.empty())
    {
        printf("Load img failed, path(%s)", szPath);
        ulRet =  ERROR_FAILED;        
    }
    else
    {   
        iChannels = stSrcImg.channels();
        if (3 == iChannels)
        {
            /* 若为彩色图像,则进行灰度化处理 */
            cvtColor(stSrcImg, stGray, COLOR_BGR2GRAY);
        }
        else if (1 == iChannels) 
        {
            stGray = stSrcImg.clone();
        }
        else
        {
            ulRet = ERROR_FAILED;
        }

        if ((ERROR_SUCCESS == ulRet) && (!stGray.empty()))
        {
            stDst = _cvtest_prewitt(stGray);
            imwrite(szDstPath, stDst);         
        }
    }
    return ulRet;   
}
ULONG cvtest_Test4_Zombie()
{
    pid_t stPid;
    stPid = fork();
    if (stPid < 0)
    {
        printf("Fork new process error : (%s)\n", strerror(errno));
        return ERROR_FAILED;
    }
    else if (stPid == 0)
    {
        printf("Child pid is %d\n", getpid());
        /* 子进程 */
        exit(0);
    }
    else
    {
        sleep(300);
        wait(NULL);     /* 获取子进程  退出信息 */
    }
    return ERROR_SUCCESS;
}
ULONG cvtest_Test4_Sqlite3()
{
    sqlite3 *pstDb = NULL;
    CHAR szSqlCmd[BUF_LEN_256] = {0, };
    INT iRet;
    CHAR *pcErrMsg = NULL;
    const char *pcName = "imgpro.db";
    iRet = sqlite3_open(pcName, &pstDb);
    if (SQLITE_OK != iRet)
    {
        printf("Open database(%s) failed.\n", pcName);
        return ERROR_FAILED;
    }
    snprintf(szSqlCmd, sizeof(szSqlCmd), "%s", SQLITE3_CREATE_TABLE);
    iRet += sqlite3_exec(pstDb, szSqlCmd, NULL, NULL, &pcErrMsg);
    szSqlCmd[0] = 0;
    snprintf(szSqlCmd, sizeof(szSqlCmd), "%s", SQLITE3_CREATE_INSERT);
    iRet += sqlite3_exec(pstDb, szSqlCmd, NULL, NULL, &pcErrMsg);
    sqlite3_free(pcErrMsg);
    sqlite3_close(pstDb);

    return ERROR_SUCCESS;
}

ULONG cvtest_Test4_WaitId()
{
    pid_t stPid;
    
    printf("Warning :: The test instance is obsolete\n");
    stPid = fork();
    if (stPid < 0)
    {
        printf("fork childe process error : (%s)\n", strerror(errno));
        return ERROR_FAILED;
    }
    else if (0 == stPid)
    {
        /* child */
        printf("Child pid is %d\n", getpid());
        sleep(100);         /* 子进程sleep---测试用信号操作子进程 */
        exit(12);
    }
    else 
    {
        /*
        iRet = waitid(P_PID, stPid, &stInfo, WSTOPPED | WCONTINUED | WNOWAIT);
        while (0 == iRet)
        {
            printf("Recv child process code(%d)\n", stInfo.si_code);
            iRet = waitid(P_PID, stPid, &stInfo, WSTOPPED | WCONTINUED | WNOWAIT);
        }
        */
        
    }
    return ERROR_SUCCESS;
}
STATIC void _test_HeavyWork()
{
    DOUBLE dSum = 0;
    ULONG ulIndex = 0;
    while (1)
    {
        dSum += sin(ulIndex++);
        
    }
    return;
}
ULONG cvtest_Test4_ProcNice()
{
    cpu_set_t stSet;
    INT iRet;
    
    CPU_ZERO(&stSet);
    CPU_SET(0, &stSet);
    iRet = sched_setaffinity(0, sizeof(stSet), &stSet);
    
    if (0 != iRet)
    {
        printf("Parent proc(%d) setaffinity failed.\n", getpid());
        return ERROR_FAILED;
    }
    iRet = fork();
    if (iRet < 0)
    {
        printf("Fork child process error.\n");
        return ERROR_FAILED;
    }
    else if(0 == iRet)
    {
        /* child proc */
        errno = 0;

        /* 子进程nice值设置为5, ---- 优先级为19 - 5 = 14 */
        iRet = setpriority(PRIO_PROCESS, 0, 5);
        if ((iRet != 0) && (0 != errno))
        {
            printf("Child setpriority failed : %s\n", strerror(errno));
            exit(1);
        }
    }

    /* 父子进程都进行死循环的工作 */
    _test_HeavyWork();
    
    return ERROR_SUCCESS;
}

VOID _test_SignalProc(INT iSigNo)
{
    const char *pcBuf1 = "==============signal begin============\n";
    const char *pcBuf2 = "==============signal end============\n";

    write(2, pcBuf1, strlen(pcBuf1));
    _test_HeavyWork();
    write(2, pcBuf2, strlen(pcBuf2));
    return;
}

ULONG cvtest_Test4_Signal()
{
    CHAR szBuf[BUF_LEN_1K] = {0, };
    ULONG ulRet = ERROR_SUCCESS;
    
    /* 编译宏
    * 测试结果：
    * SYS_signal,响应一次,恢复默认
    * sysv系统调用,响应一次,恢复默认
    * bsd系统调用,一直响应,不恢复默认
    * 默认的signal,一直响应,不恢复默认--bsd语法
    */
#if defined SYSCALL_SIGNAL_API
    if (syscall(SYS_signal, SIGINT, _test_SignalProc) == -1)
#elif defined SYSV_SIGNAL_API
    if (sysv_signal(SIGINT, _test_SignalProc) == SIG_ERR)
#elif defined BSD_SIGNAL_API
    if (bsd_signal(SIGINT, _test_SignalProc) == SIG_ERR)
#else
    if (signal(SIGINT, _test_SignalProc) == SIG_ERR)
#endif
    {
        printf("Install signal error.\n");
        return ERROR_FAILED;
    }

    printf("Please input some string : ");
    scanf("%s", szBuf);
    if (0 == strlen(szBuf))
    {
        printf("Read string failed.\n");
        ulRet =  ERROR_FAILED;
    }
    else
    {
        printf("You enter is : %s", szBuf);
    }
    return ulRet;   
}

#if 0
siginfo_t 结构前面不要带struct  
     siginfo_t {
         int      si_signo;     /* Signal number */
         int      si_errno;     /* An errno value */
         int      si_code;      /* Signal code */
         int      si_trapno;    /* Trap number that caused
                                   hardware-generated signal
                                   (unused on most architectures) */
         pid_t    si_pid;       /* Sending process ID */
         uid_t    si_uid;       /* Real user ID of sending process */
         int      si_status;    /* Exit value or signal */
         clock_t  si_utime;     /* User time consumed */
         clock_t  si_stime;     /* System time consumed */
         sigval_t si_value;     /* Signal value */
         int      si_int;       /* POSIX.1b signal */
         void    *si_ptr;       /* POSIX.1b signal */
         int      si_overrun;   /* Timer overrun count;
                                   POSIX.1b timers */
         int      si_timerid;   /* Timer ID; POSIX.1b timers */
         void    *si_addr;      /* Memory location which caused fault */
         long     si_band;      /* Band event (was int in
                                   glibc 2.3.2 and earlier) */
         int      si_fd;        /* File descriptor */
         short    si_addr_lsb;  /* Least significant bit of address
                                   (since Linux 2.6.32) */
         void    *si_call_addr; /* Address of system call instruction
                                   (since Linux 3.5) */
         int      si_syscall;   /* Number of attempted system call
                                   (since Linux 3.5) */
         unsigned int si_arch;  /* Architecture of attempted system call
                                   (since Linux 3.5) */
     }
#endif
    
VOID _test_SigAction(INT iSigNum, siginfo_t *stInfo, void *pParam)
{
//    sig_ucontext_t *pstParam = NULL;

//    pstParam = (sig_ucontext_t *)pParam;

    printf("Recv signal : %d\n", iSigNum);    
    printf("stInfo->si_signo = : %d\n", stInfo->si_signo);
    printf("stInfo->si_pid = : %d\n", stInfo->si_pid);

    /* 信号来源：SI_USER SI_KERNEL等 */
    printf("stInfo->si_code = : %d\n", stInfo->si_code);
    return;
}

ULONG cvtest_Test4_SigAction()
{
    struct sigaction stAction;
    INT iSigNum = SIGUSR1;
    volatile sig_atomic_t iFlag = 1;  /* sig_atomic_t 可保证读写操作的原子性 */
    
    memset(&stAction, 0, sizeof(stAction));
    sigemptyset(&stAction.sa_mask);
    stAction.sa_sigaction = _test_SigAction;
    stAction.sa_flags = SA_SIGINFO | SA_RESTART;

    if (-1 == sigaction(iSigNum, &stAction, NULL))
    {
        printf("Install signal(%d) failed.\n", iSigNum);
        return ERROR_FAILED;
    }
    printf("Process(%d) install signal(%d) success, iFlag = %d.\n", 
            getpid(), iSigNum, iFlag);
    while (1)
    {
        pause();
    }
    return ERROR_SUCCESS;
}
VOID _test_SigOrder(INT iSigNum)
{
    static UINT uiIndex = 0;
    if (uiIndex < BUF_LEN_128)
    {
        g_auiSigOrder[uiIndex++] = iSigNum; 
    }
    return;
}
ULONG cvtest_Test4_SigOrder()
{
    sigset_t stSigSet;
    sigset_t stSigEmptySet;
    struct sigaction stAction;
    UINT uiIndex;
    
    memset(&stAction, 0, sizeof(stAction));
    
    sigfillset(&stSigSet);
    sigemptyset(&stSigEmptySet);
    stAction.sa_handler = _test_SigOrder;
    stAction.sa_mask = stSigSet;
    stAction.sa_flags = SA_RESTART;

    /* 安装所有信号       */ 
    printf("The process's pid : %d, Tid(%ld)\n", getpid(), syscall(SYS_gettid));
    for (uiIndex = 0; uiIndex < NSIG; uiIndex++)
    {
        if ((uiIndex == SIGKILL) || (uiIndex == SIGSTOP))
        {
            continue;
        }
        if (0 != sigaction(uiIndex, &stAction, NULL))
        {
            printf("Signal(%u) install error.\n", uiIndex);
        }
    }
    if (0 != sigprocmask(SIG_SETMASK, &stSigSet, NULL))
    {
        /* 阻塞所有信号递送 */
        printf("sigprocmask fill error.\n");
        return ERROR_FAILED;
    }
    sleep(100);  /* 睡眠期间递送信号 */
    if (0 != sigprocmask(SIG_SETMASK, &stSigEmptySet, NULL))
    {
        /* 解除阻塞所有信号递送 */
        printf("sigprocmask empty error.\n");
        return ERROR_FAILED;
    }
    sleep(3);
    for (uiIndex = 0; uiIndex < ARRAY_SIZE(g_auiSigOrder); uiIndex++)
    {
        printf("#%-3d : Signum = %d \n", uiIndex, g_auiSigOrder[uiIndex]);
    }
    return ERROR_SUCCESS;
}
VOID *_test_Rdlock_Write(VOID *pParam)
{
    UINT uiIndex;
    uiIndex = (UINT)(ULONG)pParam;

    pthread_rwlock_wrlock(&g_stRdLock);
    g_uiRdLockNum++;
    printf("Thread #%d show value is (%u)\n", uiIndex, g_uiRdLockNum);
    pthread_rwlock_unlock(&g_stRdLock);

    return NULL;
}
VOID *_test_Rdlock_Read(VOID *pParam)
{
    UINT uiIndex;
    uiIndex = (UINT)(ULONG)pParam;

    pthread_rwlock_rdlock(&g_stRdLock);
    printf("Thread #%d show value is (%u)\n", uiIndex, g_uiRdLockNum);
    pthread_rwlock_unlock(&g_stRdLock);

    return NULL;
}
/* 测试读写锁唤醒顺序---
* 结果：默认是读优先，可以通过读写锁属性修改
* 测试方式有点问题----
*/
ULONG cvtest_Test4_RdLockOrder()
{
    INT uiIndex;
    UINT uiOrder;
    pthread_rwlockattr_t stAttr;
    pthread_t astThreadId[CVTEST_THREAD_NUM * 2];

    /* 请求输入读写锁顺序 */
    printf("Pleas input order of write(0)/read(1) : ");
    scanf("%d", &uiOrder);
    if ((uiOrder != 0) && (uiOrder != 1))
    {
        printf("Input lock order error.\n");
        return ERROR_FAILED;
    }
    pthread_rwlockattr_init(&stAttr);
    
    if (uiOrder == 1)
    {
        pthread_rwlockattr_setkind_np(&stAttr, PTHREAD_RWLOCK_PREFER_READER_NP);
    }
    else
    {
        pthread_rwlockattr_setkind_np(&stAttr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    }
    pthread_rwlock_init(&g_stRdLock, &stAttr);
    
    /* 先加写锁，让所有线程等待 */
    pthread_rwlock_wrlock(&g_stRdLock);
    for (uiIndex =0; uiIndex < CVTEST_THREAD_NUM; uiIndex++)
    {
        /* 传入临时变量--可能有问题 */
        pthread_create(&astThreadId[uiIndex], NULL, 
                        _test_Rdlock_Write, (VOID *)(ULONG)uiIndex);
    }
    for (uiIndex =0; uiIndex < CVTEST_THREAD_NUM; uiIndex++)
    {
        /* 传入临时变量--可能有问题 */
        pthread_create(&astThreadId[CVTEST_THREAD_NUM + uiIndex], NULL, 
                        _test_Rdlock_Read, (VOID *)(ULONG)(CVTEST_THREAD_NUM + uiIndex));
    }
    pthread_rwlock_unlock(&g_stRdLock);

    for (uiIndex = 0; uiIndex < CVTEST_THREAD_NUM * 2; uiIndex++)
    {
        pthread_join(astThreadId[uiIndex], NULL);
    }
    
    pthread_rwlockattr_destroy(&stAttr);
    pthread_rwlock_destroy(&g_stRdLock);
    
    return ERROR_SUCCESS;
}
VOID *_test_ThreadData(VOID *pParam)
{
    pthread_t stSelf = pthread_self();
    if (stSelf % 5 == 0)
    {
        g_uiThreadData++;
        printf("Thread(%lu) data is : %d\n", stSelf, g_uiThreadData);
    }

    return NULL;
}
ULONG cvtest_Test4_ThreadData()
{
    pthread_t astThreadId[CVTEST_THREAD_NUM];
    UINT uiIndex;

    for (uiIndex = 0; uiIndex < CVTEST_THREAD_NUM; uiIndex++)
    {
        pthread_create(&astThreadId[uiIndex], NULL,_test_ThreadData, NULL);
    }
    for (uiIndex = 0; uiIndex < CVTEST_THREAD_NUM; uiIndex++)
    {
        pthread_join(astThreadId[uiIndex], NULL);
    }
    return ERROR_SUCCESS;
}

ULONG cvtest_Test4_Popen()
{
    CHAR szFile[BUF_LEN_100] = {0, };
    CHAR szCmd[BUF_LEN_100] = {0, };
    CHAR szOutBuf[BUF_LEN_256] = {0, };
    FILE *pFile = NULL;
    INT iRet;
    
    printf("Please input file name : ");
    scanf("%s", szFile);
    /* 拼接命令 */
    snprintf(szCmd, sizeof(szCmd), "cat %s", szFile);
    pFile = popen(szCmd, "r");
    if (NULL == pFile)
    {
        return ERROR_FAILED;
    }

    while (fgets(szOutBuf, sizeof(szOutBuf), pFile) != NULL)
    {
        fprintf(stdout, "%s", szOutBuf);
    }
    iRet = pclose(pFile);
    if (iRet == 127)
    {
        printf("Pclose failed : cmd(%s) can't exec\n", szCmd);
    }
    else if (iRet == -1)
    {
        printf("Pclose can't get child status.\n");
    }
    else 
    {
        if (WIFEXITED(iRet))
        {
            printf("Normal exit, exit status = %d \n", WEXITSTATUS(iRet));
        }
        else if (WIFSIGNALED(iRet))
        {
            /* 由信号终止 */
            printf("Abonormal by signal(%d)\n", WTERMSIG(iRet));
        }
    }
    return ERROR_SUCCESS;
}
VOID *_test_ThreadFifo(VOID *pParam)
{
    /* 打开fifo文件,写数据 */
    
    return NULL;
}
ULONG cvtest_Test4_Fifo()
{
    pthread_rwlock_t stLock;
    pthread_t stThreadId[CVTEST_THREAD_NUM4];
    UINT uiIndex;
    
    pthread_rwlock_init(&stLock, NULL);

    mkfifo("fifo", 777);

    pthread_rwlock_wrlock(&stLock);
    for (uiIndex = 0; uiIndex < CVTEST_THREAD_NUM4; uiIndex++)
    {
        pthread_create(&stThreadId[uiIndex], NULL, _test_ThreadFifo, (VOID *)uiIndex);
    }
    pthread_rwlock_unlock(&stLock);

    for (uiIndex = 0; uiIndex < CVTEST_THREAD_NUM4; uiIndex++)
    {
        pthread_join(stThreadId[uiIndex], NULL);
    }
    
    pthread_rwlock_destroy(&stLock);
    return ERROR_SUCCESS;
}
ULONG cvtest_Test4_Float()
{
    INT iTest1 = 201;
    INT iTest2 = 299;
    INT iTest3 = 64;
    FLOAT fPram1 = 1/0.0;
    FLOAT fPram2 = 0/0.0;
    
    printf("fPram1 is %f; fPram2 is %f \n", fPram1, fPram2);
    printf("Average : %d, Sum = %d\n", 
        AVERAGE(iTest1, iTest2), SUM(iTest1, iTest2));
    printf("iTest1(%d): %d, iTest3(%d): %d\n", 
           iTest1, ISPOWER2(iTest1),
           iTest3, ISPOWER2(iTest3));

    /* 测试位运算实现两个数相加 */
    return ERROR_SUCCESS;
}
VOID *_threadJoin_main(VOID * pParam)
{
    pid_t stTid;

    stTid = syscall(SYS_gettid);
    printf("Thread TID(%d) \n", stTid);
    printf("Thread pthread_self : %p\n", (VOID *)pthread_self());
    /* 子线程sleep是为了用pmap看线程的调用栈 */
    sleep(30);
    
    return NULL;
}
ULONG cvtest_Test4_ThreadJoin()
{
    /* 未定义的值---有编译告警 */
    //UINT g_uiRdLockNum = g_uiRdLockNum;
    printf("g_uiRdLockNum is %d\n", g_uiRdLockNum);
    
    pthread_t astThread[2];
    pthread_create(&astThread[0], NULL, _threadJoin_main, NULL);

#ifdef NO_JOIN
    sleep(100);      /* sleep以等待子线程结束 */
#else
    pthread_join(astThread[0], NULL);
#endif
    pthread_create(&astThread[1], NULL, _threadJoin_main, NULL);

    sleep(40);
    return ERROR_SUCCESS;
}
ULONG cvtest_Test4_TypeConvert()
{
    UINT uiTest = 0xFFFFFFF7;
    UCHAR ucTest = (UCHAR)uiTest;
    CHAR *pcTest = (CHAR *)&uiTest;

    printf("ucTest = %08x, *pcTest = %08x\n", ucTest, *pcTest);
    
    return ERROR_SUCCESS;    
}
ULONG cvtest_Test4_BaseLinearConvert()
{
    CHAR szBuf[BUF_LEN_256] = {0, };
    Mat stSrcImg;
    Mat stRetImg;
    DOUBLE dAlpha;
    UINT uiBeta;
    INT iRowsIdx;
    INT iColsIdx;
    INT iChanIdx;
    CHAR szDstPath[CVTEST_MAX_PATH_LEN] = "BaseLinear.jpg";
    
    printf("Please input the path of srcImg : \n");
    scanf("%s", szBuf);
    
    /* 始终转换成RGB图像 */
    stSrcImg = imread(szBuf, IMREAD_COLOR);
    if (stSrcImg.empty())
    {
        return ERROR_FAILED;
    }

    /* */
    printf("Please input :  dAlpha(0.0 ~ 2.0) uiBeta(1~200) \n");
    scanf("%lf %u", &dAlpha, &uiBeta);
    printf("dAlpha = %lf uiBeta = %u \n", dAlpha, uiBeta);
    
    stRetImg = Mat(stSrcImg.size(), stSrcImg.type());
    for (iRowsIdx = 0; iRowsIdx < stSrcImg.rows; iRowsIdx++)
    {
        for (iColsIdx = 0; iColsIdx < stSrcImg.cols; iColsIdx++)
        {
            for (iChanIdx = 0; iChanIdx < 3; iChanIdx++)
            {
                
                stRetImg.at<Vec3b>(iRowsIdx, iColsIdx)[iChanIdx] = 
                    saturate_cast<uchar>(dAlpha *(stSrcImg.at<Vec3b>(iRowsIdx, iColsIdx)[iChanIdx]) + uiBeta);
            }
        }
    }
    imwrite(szDstPath, stRetImg);
    
    return ERROR_SUCCESS;
}
ULONG cvtest_Test4_Morphology()
{
    return ERROR_SUCCESS;
}
CVIMGPROC_TESTTYPE_S g_astCVTEST_Type[] = 
{
    {TEST_IMGPROC_TYPE_RBG2GRAY,    CVTEST_TITLE_RGB2GRAY,   cvtest_Test4_Rgb2Gray  },
    {TEST_IMGPROC_TYPE_MERGE,       CVTEST_TITLE_MERGERIMG,  cvtest_Test4_Merge2Img },
    {TSET_IMGPROC_TYPE_ROBERTEDGE,  CVTEST_TITLE_ROBERTEDGE, cvtest_Test4_RobertEdge},
    {TSET_IMGPROC_TYPE_PREWITTEDGE, CVTEST_TITLE_PREWITTEDGE,cvtest_Test4_PrewittEdge},
    {TSET_IMGPROC_TYPE_SOBELEDGE,   CVTEST_TITLE_SOBELEDGE,  cvtest_Test4_SobelEdge},
    {TEST_IMGPROC_TYPE_HISTOGRAM,   CVTEST_TITLE_HISTOGRAM,  cvtest_Test4_GetHistogram},
    {TEST_IMGPROC_TYPE_STRING,      CVTEST_TITLE_HONG2STR,   cvtest_Test4_Hong2Str},
    {TEST_IMGPROC_TYPE_DLOPEN,      CVTEST_TITLE_DLOPENFUNC, cvtest_Test4_DlopenFunc},
    {TEST_IMGPROC_TYPE_ZOMBIE,      CVTEST_TITLE_ZOMBIE,     cvtest_Test4_Zombie},
    {TEST_IMGPROC_TYPE_WAITID,      CVTEST_TITLE_WAITID,     cvtest_Test4_WaitId},
    {TEST_IMGPROC_TYPE_SQL3,        CVTEST_TITLE_SQLITE3,    cvtest_Test4_Sqlite3},
    {TEST_IMGPROC_TYPE_NICE,        CVTEST_TITLE_NICE,       cvtest_Test4_ProcNice},
    {TEST_IMGPROC_TYPE_SIGNAL,      CVTEST_TITLE_SIGNAL,     cvtest_Test4_Signal},
    {TEST_IMGPROC_TYPE_SIGACTION,   CVTEST_TITLE_SIGACTION,  cvtest_Test4_SigAction},
    {TEST_IMGPROC_TYPE_SIGORDER,    CVTEST_TITLE_SIGORDER,   cvtest_Test4_SigOrder},
    {TEST_IMGPROC_TYPE_RDLOCKSEQ,   CVTEST_TITLE_RDLOCKSEQ,  cvtest_Test4_RdLockOrder},
    {TEST_IMGPROC_TYPE_THREADDATA,  CVTEST_TITLE_THREADDATA, cvtest_Test4_ThreadData},
    {TEST_IMGPROC_TYPE_POPEN,       CVTEST_TITLE_POPEN,      cvtest_Test4_Popen},
    {TEST_IMGPROC_TYPE_FIFO,        CVTEST_TITLE_FIFO,       cvtest_Test4_Fifo},
    {TEST_IMGPROC_TYPE_FLOAT,       CVTEST_TITLE_FLOAT,      cvtest_Test4_Float},
    {TEST_IMGPROC_TYPE_THREADJOIN,  CVTEST_TITLE_THREADJOIN, cvtest_Test4_ThreadJoin},
    {TEST_IMGPROC_TYPE_CONVERT,     CVTEST_TITLE_CONVERT,    cvtest_Test4_TypeConvert},
    {TEST_IMGPROC_TYPE_LINEARTRANS, CVTEST_TITLE_LINEARTRANS,cvtest_Test4_BaseLinearConvert},
    {TEST_IMGPROC_TYPE_Morphology,  CVTEST_TITLE_Morphology, cvtest_Test4_Morphology},
};

/* 获取测试例标题 */
const CHAR *CVTEST_GetTestTitle(TEST_IMGPROC_TYPE_E enType)
{
    UINT uiSize = ARRAY_SIZE(g_astCVTEST_Type);
    UINT uiIndex;
    const CHAR *pcTitle = ""; 
    for (uiIndex = 0; uiIndex < uiSize; ++uiIndex)
    {
        if (g_astCVTEST_Type[uiIndex].enTye == enType)
        {
            pcTitle = g_astCVTEST_Type[uiIndex].pcTestTitle;
            break;
        }
    }
    return pcTitle;
}

/* 获取测试例数量 */
UINT CVTEST_GetTestCount()
{
    return ARRAY_SIZE(g_astCVTEST_Type);
}
/* 获取测试例主体函数 */
IMGPROC_TEST_PF CVTEST_GetTestFunc(TEST_IMGPROC_TYPE_E enType)
{
    
    UINT uiSize = ARRAY_SIZE(g_astCVTEST_Type);
    UINT uiIndex;
    IMGPROC_TEST_PF pfFunc = NULL; 
    for (uiIndex = 0; uiIndex < uiSize; ++uiIndex)
    {
        if (g_astCVTEST_Type[uiIndex].enTye == enType)
        {
            pfFunc = g_astCVTEST_Type[uiIndex].pfTestFunc;
            break;
        }
    }
    return pfFunc;
}

