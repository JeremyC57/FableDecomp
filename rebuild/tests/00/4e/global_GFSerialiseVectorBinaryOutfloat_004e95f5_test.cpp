#include "rebuild_abi.h"
#include <stdio.h>

/* Retail advances the range in 4-byte strides (ebx*4, span>>2) and pushes each
   element as a raw dword value; model the element sink as WriteElem(int). */
struct FloatVecRange {
    int* first;
    int* last;
};

/* Spy for CDataOutputStream::WriteLong (0x00993EB0): the first call carries the count, the rest the elements. */
struct CDataOutputStream_Methods {
    int  seenCount;
    int  elemSum;
    int  elemCnt;
    int  calls;
    void WriteLong(long value);
};

void CDataOutputStream_Methods::WriteLong(long value) { if (calls++ == 0) seenCount = value; else { elemSum += value; ++elemCnt; } }

void __fastcall GFSerialiseVectorBinaryOut_float(CDataOutputStream_Methods* thisptr, void* vec);

int main(void)
{
    int data[3];
    data[0] = 10;
    data[1] = 20;
    data[2] = 30;

    FloatVecRange r;
    r.first = &data[0];
    r.last  = &data[3];

    CDataOutputStream_Methods s;
    s.calls = 0;
    s.seenCount = -1;
    s.elemSum = 0;
    s.elemCnt = 0;

    GFSerialiseVectorBinaryOut_float(&s, &r);

    if (s.seenCount == 3 && s.elemCnt == 3 && s.elemSum == 60)
        printf("GFSERIALISE_VECTOR_FLOAT_TEST PASS\n");
    else
        printf("FAIL count=%d cnt=%d sum=%d\n", s.seenCount, s.elemCnt, s.elemSum);
    return 0;
}