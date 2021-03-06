/* NiuTrans.Tensor - an open-source tensor library
* Copyright (C) 2017, Natural Language Processing Lab, Northeastern University.
* All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*
* $Created by: Xu Chen (email: hello_master1954@163.com) 2018-06-13
*/

#include "../XList.h"
#include "../core/utilities/CheckData.h"
#include "TUnsqueeze.h"

namespace nts { // namespace nts(NiuTrans.Tensor)

/* 
case 1: insert a dimension by copying the blocks for x times (where x is the size of the inerted dimension)
In this case, 
(2, 3) -> (2, 2, 3), dim=1, dSize=2
(2, 3) -> (2, 3, 2), dim=2, dSize=2
*/
bool TestUnsqueeze1()
{
    /* a source tensor of size (2, 3) */
    int sOrder = 2;
    int * sDimSize = new int[sOrder];
    sDimSize[0] = 2;
    sDimSize[1] = 3;

    int sUnitNum = 1;
    for (int i = 0; i < sOrder; i++)
        sUnitNum *= sDimSize[i];

    /* a target tensor of size (2, 2, 3) */
    int tOrder1 = 3;
    int * tDimSize1 = new int[tOrder1];
    tDimSize1[0] = 2;
    tDimSize1[1] = 2;
    tDimSize1[2] = 3;

    int tUnitNum1 = 1;
    for (int i = 0; i < tOrder1; i++)
        tUnitNum1 *= tDimSize1[i];

    /* a target tensor of size (2, 3, 2) */
    int tOrder2 = 3;
    int * tDimSize2 = new int[tOrder2];
    tDimSize2[0] = 2;
    tDimSize2[1] = 3;
    tDimSize2[2] = 2;

    int tUnitNum2 = 1;
    for (int i = 0; i < tOrder2; i++)
        tUnitNum2 *= tDimSize2[i];

    DTYPE sData[2][3] = { {0.0F, 1.0F, 2.0F},
                          {3.0F, 4.0F, 5.0F} };
    DTYPE answer1[2][2][3] = { { {0.0F, 1.0F, 2.0F},
                                 {0.0F, 1.0F, 2.0F} },
                               { {3.0F, 4.0F, 5.0F},
                                 {3.0F, 4.0F, 5.0F} } };
    DTYPE answer2[2][3][2] = { { {0.0F, 0.0F}, 
                                 {1.0F, 1.0F}, 
                                 {2.0F, 2.0F} },
                               { {3.0F, 3.0F}, 
                                 {4.0F, 4.0F}, 
                                 {5.0F, 5.0F} } };
    
    /* CPU test */
    bool cpuTest = true;

    /* create tensors */
    XTensor * s = NewTensorV2(sOrder, sDimSize);
    XTensor * t1 = NewTensorV2(tOrder1, tDimSize1);
    XTensor * t2 = NewTensorV2(tOrder2, tDimSize2);
    XTensor tUser1;
    XTensor tUser2;

    /* initialize variables */
    s->SetData(sData, sUnitNum);
    t1->SetZeroAll();
    t2->SetZeroAll();

    /* call Unsqueeze function */
    _Unsqueeze(s, t1, 1, 2);
    _Unsqueeze(s, t2, 2, 2);
    tUser1 = Unsqueeze(*s, 1, 2);
    tUser2 = Unsqueeze(*s, 2, 2);

    /* check results */
    cpuTest = _CheckData(t1, answer1, tUnitNum1, 1e-4F) &&
              _CheckData(&tUser1, answer1, tUnitNum1, 1e-4F) &&
              _CheckData(t2, answer2, tUnitNum2, 1e-4F) &&
              _CheckData(&tUser2, answer2, tUnitNum2, 1e-4F);

#ifdef USE_CUDA
    /* GPU test */
    bool gpuTest = true;

    /* create tensor */
    XTensor * sGPU = NewTensorV2(sOrder, sDimSize, X_FLOAT, 1.0F, 0);
    XTensor * tGPU1 = NewTensorV2(tOrder1, tDimSize1, X_FLOAT, 1.0F, 0);
    XTensor * tGPU2 = NewTensorV2(tOrder2, tDimSize2, X_FLOAT, 1.0F, 0);
    XTensor tUserGPU1;
    XTensor tUserGPU2;

    /* Initialize variables */
    sGPU->SetData(sData, sUnitNum);
    tGPU1->SetZeroAll();
    tGPU2->SetZeroAll();

    /* call Unsqueeze function */
    _Unsqueeze(sGPU, tGPU1, 1, 2);
    _Unsqueeze(sGPU, tGPU2, 2, 2);
    tUserGPU1 = Unsqueeze(*sGPU, 1, 2);
    tUserGPU2 = Unsqueeze(*sGPU, 2, 2);

    /* check results */
    gpuTest = _CheckData(tGPU1, answer1, tUnitNum1, 1e-4F) &&
              _CheckData(&tUserGPU1, answer1, tUnitNum1, 1e-4F) &&
              _CheckData(tGPU2, answer2, tUnitNum2, 1e-4F) &&
              _CheckData(&tUserGPU2, answer2, tUnitNum2, 1e-4F);

    /* destroy variables */
    delete s;
    delete t1;
    delete t2;
    delete sGPU;
    delete tGPU1;
    delete tGPU2;
    delete[] sDimSize;
    delete[] tDimSize1;
    delete[] tDimSize2;

    return cpuTest && gpuTest;
#else
    /* destroy variables */
    delete s;
    delete t1;
    delete t2;
    delete[] sDimSize;
    delete[] tDimSize1;
    delete[] tDimSize2;

    return cpuTest;
#endif // USE_CUDA
}

/* other cases */
/*
    TODO!!
*/

/* test for Unsqueeze Function */
bool TestUnsqueeze()
{
    XPRINT(0, stdout, "[TEST Unsqueeze] insert a dimension by copying the blocks for x times\n");
    bool returnFlag = true, caseFlag = true;

    /* case 1 test */
    caseFlag = TestUnsqueeze1();

    if (!caseFlag) {
        returnFlag = false;
        XPRINT(0, stdout, ">> case 1 failed!\n");
    }
    else
        XPRINT(0, stdout, ">> case 1 passed!\n");

    /* other cases test */
    /*
    TODO!!
    */

    if (returnFlag) {
        XPRINT(0, stdout, ">> All Passed!\n");
    }
    else
        XPRINT(0, stdout, ">> Failed!\n");

    XPRINT(0, stdout, "\n");

    return returnFlag;
}

} // namespace nts(NiuTrans.Tensor)
