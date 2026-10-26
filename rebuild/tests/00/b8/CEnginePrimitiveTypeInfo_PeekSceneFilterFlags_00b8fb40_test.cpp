#include <cstdio>

class CEnginePrimitiveTypeInfo;

unsigned long __fastcall CEnginePrimitiveTypeInfo_PeekSceneFilterFlags(CEnginePrimitiveTypeInfo* info);

int main()
{
    unsigned long r = CEnginePrimitiveTypeInfo_PeekSceneFilterFlags((CEnginePrimitiveTypeInfo*)0);
    if (r == 3UL)
        printf("00b8fb40_TEST PASS\n");
    else
        printf("FAIL %lu\n", r);
    return 0;
}