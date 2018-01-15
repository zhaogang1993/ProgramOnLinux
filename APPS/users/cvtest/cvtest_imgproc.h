#ifndef _CVTEST_IMGPROC_H_
#define _CVTEST_IMGPROC_H_


#define CVTEST_THREAD_NUM    50
#define CVTEST_THREAD_NUM4   4

#define CVTEST_MAX_PATH_LEN     256
#define CVTEST_MAX_PREFIX_LEN   128

#define SQLITE3_CREATE_TABLE    "create table imgdb(id integer, name varchar(10), row interger, col interger);"
#define SQLITE3_CREATE_INSERT   "insert into imgdb values(1, 'test', 100, 100);"

typedef ULONG (*IMGPROC_TEST_PF)();

typedef enum cvTest_Imgproc_Type
{
    TEST_IMGPROC_TYPE_RBG2GRAY,         /* 灰度化 */
    TEST_IMGPROC_TYPE_MERGE,            /* 两幅图像相加 */
    TSET_IMGPROC_TYPE_ROBERTEDGE,       /* robert边缘提取 */
    TSET_IMGPROC_TYPE_PREWITTEDGE,      /* prewitt边缘提取 */    
    TSET_IMGPROC_TYPE_SOBELEDGE,        /* Sobel边缘提取 */
    TEST_IMGPROC_TYPE_HISTOGRAM,        /* 获取图像的灰度直方图 */
    TEST_IMGPROC_TYPE_LINEARTRANS,      /* 基本线性变化 */
    TEST_IMGPROC_TYPE_Morphology,       /* 图像腐蚀 */       

    
    TEST_IMGPROC_TYPE_STRING,           /* 宏参数字符串 */
    TEST_IMGPROC_TYPE_DLOPEN,           /* 测试dlopen系列函数 */
    TEST_IMGPROC_TYPE_ZOMBIE,           /* 测试僵尸进程 */
    TEST_IMGPROC_TYPE_WAITID,           /* 测试waitid系统调用 */
    TEST_IMGPROC_TYPE_SQL3,             /* 测试sqlite3 */
    TEST_IMGPROC_TYPE_NICE,             /* 测试进程nice值 */
    TEST_IMGPROC_TYPE_SIGNAL,           /* 测试信号相关signal */
    TEST_IMGPROC_TYPE_SIGACTION,        /* 测试信号相关sigaction */
    TEST_IMGPROC_TYPE_SIGORDER,         /* 测试信号递送顺序 */
    TEST_IMGPROC_TYPE_RDLOCKSEQ,        /* 测试读写锁顺序 */
    TEST_IMGPROC_TYPE_THREADDATA,       /* 测试线程数据 */
    TEST_IMGPROC_TYPE_POPEN,            /* 测试管道popen */
    TEST_IMGPROC_TYPE_FIFO,             /* 测试命名管道fifo--写入原子性测试 */
    TEST_IMGPROC_TYPE_FLOAT,            /* 测试浮点数 */
    TEST_IMGPROC_TYPE_THREADJOIN,       /* 测试线程join */
    TEST_IMGPROC_TYPE_CONVERT,          /* 测试类型转换 */

    
    TEST_IMGPROC_TYPE_MAX,
}TEST_IMGPROC_TYPE_E;

typedef struct stcvImgProc_TestType
{
    TEST_IMGPROC_TYPE_E enTye;      /* 测试类型 */
    const CHAR *pcTestTitle;              /* 测试标题 */
    IMGPROC_TEST_PF pfTestFunc;     /* 测试回调 */
}CVIMGPROC_TESTTYPE_S;

IMGPROC_TEST_PF CVTEST_GetTestFunc(TEST_IMGPROC_TYPE_E enType);
const CHAR *CVTEST_GetTestTitle(TEST_IMGPROC_TYPE_E enType);
UINT CVTEST_GetTestCount();

#endif
