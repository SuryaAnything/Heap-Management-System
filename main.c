//
// Created by surya on 3.2.24.
//

#include "cHEAP.h"


int main() {

    int *arr = cHEAP_AllocateNew(INT32, 10);
    int *arr2 = cHEAP_AllocateNew(INT32, 10);
    int *arr3 = cHEAP_AllocateNew(INT32, 10);
    int *arr4 = cHEAP_AllocateNew(INT32, 10);

    cHEAP_InspectFreeSpace();

    cHEAP_Release(arr2);
    cHEAP_Release(arr3);

    cHEAP_InspectFreeSpace();

    double *arr5 = cHEAP_AllocateNew(DOUBLE, 16);

    cHEAP_InspectFreeSpace();


}
