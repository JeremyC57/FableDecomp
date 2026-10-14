#include <cstdio>
#include <stddef.h>
struct CThing { int x; };
static int g_set = 0; static const CThing* g_item = (const CThing*)1;
struct CBaseIntelligentPointer { virtual void v00() { } void* PData; CBaseIntelligentPointer() : PData(0) { } void SetPItem(const CThing* p) { ++g_set; g_item = p; PData = (void*)p; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(const CThing* p) : CBaseIntelligentPointer() { SetPItem(p); } };
struct SThingPointerWithFlag_00883180 { CIntelligentPointer Thing; bool Flag; SThingPointerWithFlag_00883180() : Thing(0), Flag(false) { } };
int main()
{
    SThingPointerWithFlag_00883180 s;
    if (g_set != 1 || g_item != 0 || s.Thing.PData != 0 || s.Flag || offsetof(SThingPointerWithFlag_00883180, Flag) != 8 || sizeof(s) != 12) { std::printf("00883180_TEST FAIL\n"); return 1; }
    std::printf("00883180_TEST PASS\n"); return 0;
}