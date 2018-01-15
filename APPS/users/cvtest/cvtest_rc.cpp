#include <stdio.h>

#include <opencv2/opencv.hpp>

#include "../../../PUBILIC/include/basetype.h"
#include "cvtest_rc.h"

using namespace cv;

CVTEST_RC_S g_astCVTEST_Rc[] =
{
    {CVTEST_RC_TYPE_RGB2GRAY, (CHAR *)"This is test for rgb img convert to gray img.\n"},
};

CHAR *CVTEST_GetRcString(CVTEST_RC_TYPE_E enRcType)
{
    UINT uiSize = ARRAY_SIZE(g_astCVTEST_Rc);
    UINT uiIndex;
    CHAR *pcRetRc = NULL; 
    for (uiIndex = 0; uiIndex < uiSize; ++uiIndex)
    {
        if (g_astCVTEST_Rc[uiIndex].enType == enRcType)
        {
            pcRetRc = g_astCVTEST_Rc[uiIndex].pcRcString;
            break;
        }
    }
    return pcRetRc;
}

