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
* $Created by: Xu Chen (email: hello_master1954@163.com) 2018-07-30
*/

#include "../core/utilities/CheckData.h"
#include "../XTensor.h"
#include "../core/arithmetic/SumDim.h"
#include "../core/getandset/SetData.h"
#include "TSumDim.h"

namespace nts { // namespace nts(NiuTrans.Tensor)

/* 
case 1: tensor summation c = a + b * \beta 
where the size of b is equal to the n-th dimension of a, 
i.e., a is summed with b by broadcasting.
In this case, (2, 4) + (2) = (2, 4), n = 0.
*/
bool TestSumDim1()
{
    /* a tensor of size (2, 4) */
    int aOrder = 2;
    int * aDimSize = new int[aOrder];
    aDimSize[0] = 2;
    aDimSize[1] = 4;

    int aUnitNum = 1;
    for (int i = 0; i < aOrder; i++)
        aUnitNum *= aDimSize[i];

    /* a tensor of size (2) */
    int bOrder = 1;
    int * bDimSize = new int[bOrder];
    bDimSize[0] = 2;

    int bUnitNum = 1;
    for (int i = 0; i < bOrder; i++)
        bUnitNum *= bDimSize[i];

    DTYPE aData[2][4] = { {0.0F, 1.0F, 2.0F, 3.0F},
                          {4.0F, 5.0F, 6.0F, 7.0F} };
    DTYPE bData[2] = {1.0F, -1.0F};
    DTYPE answer[2][4] = { {1.0F, 2.0F, 3.0F, 4.0F},
                           {3.0F, 4.0F, 5.0F, 6.0F} };

    /* CPU test */
    bool cpuTest = true;

    /* create tensors */
    XTensor * a = NewTensorV2(aOrder, aDimSize);
    XTensor * b = NewTensorV2(bOrder, bDimSize);
    XTensor * c = NewTensorV2(aOrder, aDimSize);
    XTensor * cMe = NewTensorV2(aOrder, aDimSize);
    XTensor cUser;

    /* initialize variables */
    a->SetData(aData, aUnitNum);
    cMe->SetData(aData, aUnitNum);
    b->SetData(bData, bUnitNum);
    c->SetZeroAll();

    /* call SumDim function */
    _SumDim(a, b, c, 0);
    _SumDim(cMe, b, 0);
    cUser = SumDim(*a, *b, 0);

    /* check results */
    cpuTest = _CheckData(c, answer, aUnitNum, 1e-4F) &&
              _CheckData(cMe, answer, aUnitNum, 1e-4F) &&
              _CheckData(&cUser, answer, aUnitNum, 1e-4F);

#ifdef USE_CUDA
    /* GPU test */
    bool gpuTest = true;

    /* create tensor */
    XTensor * aGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * bGPU = NewTensorV2(bOrder, bDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cMeGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor cUserGPU;

    /* Initialize variables */
    aGPU->SetData(aData, aUnitNum);
    cMeGPU->SetData(aData, aUnitNum);
    bGPU->SetData(bData, bUnitNum);
    cGPU->SetZeroAll();

    /* call sum function */
    _SumDim(aGPU, bGPU, cGPU, 0);
    _SumDim(cMeGPU, bGPU, 0);
    cUserGPU = SumDim(*aGPU, *bGPU, 0);

    /* check results */
    gpuTest = _CheckData(cGPU, answer, aUnitNum, 1e-4F) &&
              _CheckData(cMeGPU, answer, aUnitNum, 1e-4F) &&
              _CheckData(&cUserGPU, answer, aUnitNum, 1e-4F);

    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete aGPU;
    delete bGPU;
    delete cGPU;
    delete cMeGPU;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest && gpuTest;
#else
    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest;
#endif // USE_CUDA
}

/* 
case 2: tensor summation c = a + b * \beta 
where the size of b is equal to the n-th dimension of a, 
i.e., a is summed with b by broadcasting.
In this case, (2, 4) + (2, 2) = (2, 4), n = 1.
*/
bool TestSumDim2()
{
    /* a tensor of size (2, 4) */
    int aOrder = 2;
    int * aDimSize = new int[aOrder];
    aDimSize[0] = 2;
    aDimSize[1] = 4;

    int aUnitNum = 1;
    for (int i = 0; i < aOrder; i++)
        aUnitNum *= aDimSize[i];

    /* a tensor of size (2, 2) */
    int bOrder = 2;
    int * bDimSize = new int[bOrder];
    bDimSize[0] = 2;
    bDimSize[1] = 2;

    int bUnitNum = 1;
    for (int i = 0; i < bOrder; i++)
        bUnitNum *= bDimSize[i];

    DTYPE aData[2][4] = { {0.0F, 1.0F, 2.0F, 3.0F},
                          {4.0F, 5.0F, 6.0F, 7.0F} };
    DTYPE bData[2][2] = { {1.0F, -1.0F},
                          {-1.0F, 1.0F} };
    DTYPE answer[2][4] = { {1.0F, 0.0F, 1.0F, 4.0F},
                           {5.0F, 4.0F, 5.0F, 8.0F} };

    /* CPU test */
    bool cpuTest = true;

    /* create tensors */
    XTensor * a = NewTensorV2(aOrder, aDimSize);
    XTensor * b = NewTensorV2(bOrder, bDimSize);
    XTensor * c = NewTensorV2(aOrder, aDimSize);
    XTensor * cMe = NewTensorV2(aOrder, aDimSize);
    XTensor cUser;

    /* initialize variables */
    a->SetData(aData, aUnitNum);
    cMe->SetData(aData, aUnitNum);
    b->SetData(bData, bUnitNum);
    c->SetZeroAll();

    /* call SumDim function */
    _SumDim(a, b, c, 1);
    _SumDim(cMe, b, 1);
    cUser = SumDim(*a, *b, 1);

    /* check results */
    cpuTest = _CheckData(c, answer, aUnitNum, 1e-4F) &&
              _CheckData(cMe, answer, aUnitNum, 1e-4F) &&
              _CheckData(&cUser, answer, aUnitNum, 1e-4F);

#ifdef USE_CUDA
    /* GPU test */
    bool gpuTest = true;

    /* create tensor */
    XTensor * aGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * bGPU = NewTensorV2(bOrder, bDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cMeGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor cUserGPU;

    /* Initialize variables */
    aGPU->SetData(aData, aUnitNum);
    cMeGPU->SetData(aData, aUnitNum);
    bGPU->SetData(bData, bUnitNum);
    cGPU->SetZeroAll();

    /* call sum function */
    _SumDim(aGPU, bGPU, cGPU, 1);
    _SumDim(cMeGPU, bGPU, 1);
    cUserGPU = SumDim(*aGPU, *bGPU, 1);

    /* check results */
    gpuTest = _CheckData(cGPU, answer, aUnitNum, 1e-4F) &&
              _CheckData(cMeGPU, answer, aUnitNum, 1e-4F) &&
              _CheckData(&cUserGPU, answer, aUnitNum, 1e-4F);

    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete aGPU;
    delete bGPU;
    delete cGPU;
    delete cMeGPU;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest && gpuTest;
#else
    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest;
#endif // USE_CUDA
}

/* 
case 3: tensor summation c = a + b * \beta 
where the size of b is equal to the n-th dimension of a, 
i.e., a is summed with b by broadcasting.
In this case, 
(20, 40, 4000) + (40) = (20, 40, 4000), dim = 1.
*/
bool TestSumDim3()
{
    /* a tensor of size (20, 40, 4000) */
    int aOrder = 3;
    int * aDimSize = new int[aOrder];
    aDimSize[0] = 20;
    aDimSize[1] = 40;
    aDimSize[2] = 4000;

    int aUnitNum = 1;
    for (int i = 0; i < aOrder; i++)
        aUnitNum *= aDimSize[i];

    /* a tensor of size (40) */
    int bOrder = 1;
    int * bDimSize = new int[bOrder];
    bDimSize[0] = 40;

    int bUnitNum = 1;
    for (int i = 0; i < bOrder; i++)
        bUnitNum *= bDimSize[i];

    /* CPU test */
    bool cpuTest = true;

    /* create tensors */
    XTensor * a = NewTensorV2(aOrder, aDimSize);
    XTensor * b = NewTensorV2(bOrder, bDimSize);
    XTensor * c = NewTensorV2(aOrder, aDimSize);
    XTensor * cMe = NewTensorV2(aOrder, aDimSize);
    XTensor * answer = NewTensorV2(aOrder, aDimSize);
    XTensor cUser;

    /* initialize variables */
    a->SetZeroAll();
    cMe->SetZeroAll();
    b->SetDataFixed(1);
    answer->SetDataFixed(1);

    /* call SumDim function */
    _SumDim(a, b, c, 1);
    _SumDim(cMe, b, 1);
    cUser = SumDim(*a, *b, 1);
    
    /* check results */
    cpuTest = _CheckData(c, answer->data, aUnitNum) &&
              _CheckData(cMe, answer->data, aUnitNum) &&
              _CheckData(&cUser, answer->data, aUnitNum);

#ifdef USE_CUDA
    /* GPU test */
    bool gpuTest = true;

    /* create tensor */
    XTensor * aGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * bGPU = NewTensorV2(bOrder, bDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cMeGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor cUserGPU;

    /* Initialize variables */
    aGPU->SetZeroAll();
    cMe->SetZeroAll();
    bGPU->SetDataFixed(1);

    /* call sum function */
    _SumDim(aGPU, bGPU, cGPU, 1);
    _SumDim(cMeGPU, bGPU, 1);
    cUserGPU = SumDim(*aGPU, *bGPU, 1);

    /* check results */
    gpuTest = _CheckData(cGPU, answer->data, aUnitNum) &&
              _CheckData(cMeGPU, answer->data, aUnitNum) &&
              _CheckData(&cUserGPU, answer->data, aUnitNum);

    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete answer;
    delete aGPU;
    delete bGPU;
    delete cGPU;
    delete cMeGPU;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest && gpuTest;
#else
    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete answer;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest;
#endif // USE_CUDA
}

/* 
case 4: tensor summation c = a + b * \beta 
where the size of b is equal to the n-th dimension of a, 
i.e., a is summed with b by broadcasting.
In this case, 
(200, 40, 4000) + (40) = (200, 40, 4000), dim = 1.
*/
bool TestSumDim4()
{
    /* a tensor of size (200, 40, 4000) */
    int aOrder = 2;
    int * aDimSize = new int[aOrder];
    aDimSize[0] = 1000000;
    aDimSize[1] = 50;

    int aUnitNum = 1;
    for (int i = 0; i < aOrder; i++)
        aUnitNum *= aDimSize[i];

    /* a tensor of size (40) */
    int bOrder = 1;
    int * bDimSize = new int[bOrder];
    bDimSize[0] = 50;

    int bUnitNum = 1;
    for (int i = 0; i < bOrder; i++)
        bUnitNum *= bDimSize[i];

    /* CPU test */
    bool cpuTest = true;

    /* create tensors */
    XTensor * a = NewTensorV2(aOrder, aDimSize);
    XTensor * b = NewTensorV2(bOrder, bDimSize);
    XTensor * c = NewTensorV2(aOrder, aDimSize);
    XTensor * cMe = NewTensorV2(aOrder, aDimSize);
    XTensor * answer = NewTensorV2(aOrder, aDimSize);
    XTensor cUser;

    /* initialize variables */
    a->SetZeroAll();
    cMe->SetZeroAll();
    b->SetDataFixed(1);
    answer->SetDataFixed(1);

    /* call SumDim function */
    _SumDim(a, b, c, 1);
    _SumDim(cMe, b, 1);
    cUser = SumDim(*a, *b, 1);
    
    /* check results */
    cpuTest = _CheckData(c, answer->data, aUnitNum, 1e-4F) &&
              _CheckData(cMe, answer->data, aUnitNum, 1e-4F) &&
              _CheckData(&cUser, answer->data, aUnitNum, 1e-4F);

#ifdef USE_CUDA
    /* GPU test */
    bool gpuTest = true;

    /* create tensor */
    XTensor * aGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * bGPU = NewTensorV2(bOrder, bDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor * cMeGPU = NewTensorV2(aOrder, aDimSize, X_FLOAT, 1.0F, 0);
    XTensor cUserGPU;

    /* Initialize variables */
    aGPU->SetZeroAll();
    cMe->SetZeroAll();
    bGPU->SetDataFixed(1);

    /* call sum function */
    _SumDim(aGPU, bGPU, cGPU, 1);
    _SumDim(cMeGPU, bGPU, 1);
    cUserGPU = SumDim(*aGPU, *bGPU, 1);

    /* check results */
    gpuTest = _CheckData(cGPU, answer->data, aUnitNum, 1e-4F) &&
              _CheckData(cMeGPU, answer->data, aUnitNum, 1e-4F) &&
              _CheckData(&cUserGPU, answer->data, aUnitNum, 1e-4F);

    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete answer;
    delete aGPU;
    delete bGPU;
    delete cGPU;
    delete cMeGPU;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest && gpuTest;
#else
    /* destroy variables */
    delete a;
    delete b;
    delete c;
    delete cMe;
    delete answer;
    delete[] aDimSize;
    delete[] bDimSize;

    return cpuTest;
#endif // USE_CUDA
}

/* other cases */
/*
    TODO!!
*/

/* test for SumDim Function */
bool TestSumDim()
{
    XPRINT(0, stdout, "[TEST SUMDIM] tensor summation c = a + b * beta by broadcasting\n");
    bool returnFlag = true, caseFlag = true;

    /* case 1 test */
    caseFlag = TestSumDim1();
    if (!caseFlag) {
        returnFlag = false;
        XPRINT(0, stdout, ">> case 1 failed!\n");
    }
    else
        XPRINT(0, stdout, ">> case 1 passed!\n");

    /* case 2 test */
    caseFlag = TestSumDim2();
    if (!caseFlag) {
        returnFlag = false;
        XPRINT(0, stdout, ">> case 2 failed!\n");
    }
    else
        XPRINT(0, stdout, ">> case 2 passed!\n");
    
    /* case 3 test */
    caseFlag = TestSumDim3();
    if (!caseFlag) {
        returnFlag = false;
        XPRINT(0, stdout, ">> case 3 failed!\n");
    }
    else
        XPRINT(0, stdout, ">> case 3 passed!\n");
        
    ///* case 4 test */
    //caseFlag = TestSumDim4();
    //if (!caseFlag) {
    //    returnFlag = false;
    //    XPRINT(0, stdout, ">> case 4 failed!\n");
    //}
    //else
    //    XPRINT(0, stdout, ">> case 4 passed!\n");

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
