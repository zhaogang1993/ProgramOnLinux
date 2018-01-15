#ifndef _CVTEST_RC_H_
#define _CVTEST_RC_H_

#define CVTEST_TITLE_RGB2GRAY       "convert a rgb img to gray"
#define CVTEST_TITLE_MERGERIMG      "merge two img by x*img1 + y*img2"
#define CVTEST_TITLE_ROBERTEDGE     "get edge img by robert"
#define CVTEST_TITLE_PREWITTEDGE    "get edge img by prewitt"
#define CVTEST_TITLE_SOBELEDGE      "get edge img by sobel"
#define CVTEST_TITLE_HISTOGRAM      "get img's histogram"
#define CVTEST_TITLE_HONG2STR       "hong to string"
#define CVTEST_TITLE_DLOPENFUNC     "test for dlopen funcs"
#define CVTEST_TITLE_ZOMBIE         "test for zombie app"
#define CVTEST_TITLE_WAITID         "test for waitid a child process"
#define CVTEST_TITLE_SQLITE3        "test for sqlite3"
#define CVTEST_TITLE_NICE           "test for process's nice value"
#define CVTEST_TITLE_SIGNAL         "test for different 'signal'"
#define CVTEST_TITLE_SIGACTION      "test for sigaction"
#define CVTEST_TITLE_SIGORDER       "test for the order of signal"
#define CVTEST_TITLE_RDLOCKSEQ      "test for the order of rdlock"
#define CVTEST_TITLE_THREADDATA     "test for the local data of thread"
#define CVTEST_TITLE_POPEN          "test for popen"
#define CVTEST_TITLE_FIFO           "test for fifo"
#define CVTEST_TITLE_FLOAT          "test for float"
#define CVTEST_TITLE_THREADJOIN     "test for thread join"
#define CVTEST_TITLE_CONVERT        "test for type convert"
#define CVTEST_TITLE_LINEARTRANS    "test for base linear convert"
#define CVTEST_TITLE_Morphology     "test for Morphology"


typedef enum encvTest_rc_type
{
    CVTEST_RC_TYPE_RGB2GRAY,
        
    CVTEST_RC_TYPE_MAX
}CVTEST_RC_TYPE_E;

typedef struct stcvTest_Rc
{
    CVTEST_RC_TYPE_E enType;
    CHAR *pcRcString;
}CVTEST_RC_S;

CHAR *CVTEST_GetRcString(CVTEST_RC_TYPE_E enRcType);

#endif 
