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
    TEST_IMGPROC_TYPE_RBG2GRAY,         /* �ҶȻ� */
    TEST_IMGPROC_TYPE_MERGE,            /* ����ͼ����� */
    TSET_IMGPROC_TYPE_ROBERTEDGE,       /* robert��Ե��ȡ */
    TSET_IMGPROC_TYPE_PREWITTEDGE,      /* prewitt��Ե��ȡ */    
    TSET_IMGPROC_TYPE_SOBELEDGE,        /* Sobel��Ե��ȡ */
    TEST_IMGPROC_TYPE_HISTOGRAM,        /* ��ȡͼ��ĻҶ�ֱ��ͼ */
    TEST_IMGPROC_TYPE_LINEARTRANS,      /* �������Ա仯 */
    TEST_IMGPROC_TYPE_Morphology,       /* ͼ��ʴ */       

    
    TEST_IMGPROC_TYPE_STRING,           /* ������ַ��� */
    TEST_IMGPROC_TYPE_DLOPEN,           /* ����dlopenϵ�к��� */
    TEST_IMGPROC_TYPE_ZOMBIE,           /* ���Խ�ʬ���� */
    TEST_IMGPROC_TYPE_WAITID,           /* ����waitidϵͳ���� */
    TEST_IMGPROC_TYPE_SQL3,             /* ����sqlite3 */
    TEST_IMGPROC_TYPE_NICE,             /* ���Խ���niceֵ */
    TEST_IMGPROC_TYPE_SIGNAL,           /* �����ź����signal */
    TEST_IMGPROC_TYPE_SIGACTION,        /* �����ź����sigaction */
    TEST_IMGPROC_TYPE_SIGORDER,         /* �����źŵ���˳�� */
    TEST_IMGPROC_TYPE_RDLOCKSEQ,        /* ���Զ�д��˳�� */
    TEST_IMGPROC_TYPE_THREADDATA,       /* �����߳����� */
    TEST_IMGPROC_TYPE_POPEN,            /* ���Թܵ�popen */
    TEST_IMGPROC_TYPE_FIFO,             /* ���������ܵ�fifo--д��ԭ���Բ��� */
    TEST_IMGPROC_TYPE_FLOAT,            /* ���Ը����� */
    TEST_IMGPROC_TYPE_THREADJOIN,       /* �����߳�join */
    TEST_IMGPROC_TYPE_CONVERT,          /* ��������ת�� */

    
    TEST_IMGPROC_TYPE_MAX,
}TEST_IMGPROC_TYPE_E;

typedef struct stcvImgProc_TestType
{
    TEST_IMGPROC_TYPE_E enTye;      /* �������� */
    const CHAR *pcTestTitle;              /* ���Ա��� */
    IMGPROC_TEST_PF pfTestFunc;     /* ���Իص� */
}CVIMGPROC_TESTTYPE_S;

IMGPROC_TEST_PF CVTEST_GetTestFunc(TEST_IMGPROC_TYPE_E enType);
const CHAR *CVTEST_GetTestTitle(TEST_IMGPROC_TYPE_E enType);
UINT CVTEST_GetTestCount();

#endif
