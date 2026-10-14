#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CBaseObjectPointer;
struct CBaseObject {
    virtual void v00() {}
    CBaseObjectPointer* IntelligentPointer;
    CBaseObject() : IntelligentPointer((CBaseObjectPointer*)0x4321) {}
    CBaseObject(const CBaseObject& other) : IntelligentPointer(0) {}
};
static bool Check()
{
    CBaseObject a; CBaseObject b(a);
    if (a.IntelligentPointer != (CBaseObjectPointer*)0x4321) return false;
    if (b.IntelligentPointer != 0) return false;     // the copy does not share the record
    return true;
}
int main()
{
    if (!Check()) { std::printf("0099a390_TEST FAIL\n"); return 1; }
    std::printf("0099a390_TEST PASS\n");
    return 0;
}
