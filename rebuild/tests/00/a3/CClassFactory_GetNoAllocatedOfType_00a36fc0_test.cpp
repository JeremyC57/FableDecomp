// Test for 0x00A36FC0 CClassFactory::GetNoAllocatedOfType: reads NoAllocated of the indexed class type info.
#include <stdio.h>
class CClassTypeInfo { public: void* TypeList; unsigned int NoAllocated; bool Used; bool Local; void* AllocFunc; void* Name; };
class CClassFactory {
public:
    unsigned int GetNoAllocatedOfType(unsigned int type) const { return ClassTypeInfo[type].NoAllocated; }
    CClassTypeInfo* ClassTypeInfo;
};
int main()
{
    CClassTypeInfo infos[3]; infos[0].NoAllocated = 4; infos[1].NoAllocated = 0; infos[2].NoAllocated = 19;
    CClassFactory f; f.ClassTypeInfo = infos;
    bool ok = f.GetNoAllocatedOfType(0) == 4 && f.GetNoAllocatedOfType(1) == 0 && f.GetNoAllocatedOfType(2) == 19;
    puts(ok ? "00a36fc0_TEST PASS" : "00a36fc0_TEST FAIL");
    return ok ? 0 : 1;
}