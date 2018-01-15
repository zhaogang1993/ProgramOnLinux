/* linux系统头文件 */
#include <stdio.h>
#include <unistd.h>   /* for getcwd */
#include <sys/prctl.h>

/* opencv头文件 */
#include <opencv2/opencv.hpp>

/* 自定义头 */
#include "../../../PUBILIC/include/basetype.h"
#include "cvtest_imgproc.h"
#include "cvtest_rc.h"

using namespace cv;

TEST_IMGPROC_TYPE_E g_enTestTest;

ULONG cvtestd_usage()
{
    UINT uiIndex;    
    UINT uiInputIndex = 0;
    UINT uiSize = CVTEST_GetTestCount();
    
    printf("********************Welcome to opencv test app.********************\n");
    for (uiIndex = 0; uiIndex < uiSize; ++uiIndex)
    {
        printf("%-3d %s\n", uiIndex, CVTEST_GetTestTitle((TEST_IMGPROC_TYPE_E)uiIndex));        
    }    
    /* 请求输入参数 */
    printf("Please input test type : ");
    scanf("%u", &uiInputIndex);
    if (uiInputIndex < uiSize)
    {
        g_enTestTest = (TEST_IMGPROC_TYPE_E)uiInputIndex;
        return ERROR_SUCCESS;
    }
    return ERROR_FAILED;
}

INT main(INT argc, CHAR **argv)
{
    /* 给线程重命名测试 */
    if (0 != prctl(PR_SET_NAME, "threadname", NULL, NULL, NULL))
    {
        printf("Set thread name error.\n");
        return ERROR_FAILED;
    }
    
    ULONG ulRet = ERROR_SUCCESS;
    IMGPROC_TEST_PF pfFunc = NULL;
    INT iRet = 0;
    ulRet = cvtestd_usage(); 
    /* 输出进程的用法 */
    while (ERROR_SUCCESS != ulRet)
    {
        ulRet = cvtestd_usage();
    }
    pfFunc = CVTEST_GetTestFunc(g_enTestTest); 
    if (NULL != pfFunc)
    {
        ulRet = pfFunc();
        if (ERROR_SUCCESS != ulRet)
        {
            printf("Test failed : type(%u),title(%s)\n", 
                   (UINT)g_enTestTest,
                   CVTEST_GetTestTitle(g_enTestTest));
            iRet = -1;
        }
        else 
        {
            iRet = 0;
        }
    }

    return iRet;
}
