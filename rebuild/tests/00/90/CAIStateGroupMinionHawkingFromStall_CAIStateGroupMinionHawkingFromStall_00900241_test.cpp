#include <cstdio>
#include <stddef.h>
#include <new>
static int g_base = 0; static int g_ptrbase = 0;
struct CAIStateGroupBase_Calls { virtual void v00() { } unsigned char _body[0x10]; CAIStateGroupBase_Calls() { ++g_base; } };
struct CBaseIntelligentPointer { virtual void v00() { } void* PData; CBaseIntelligentPointer() : PData(0) { ++g_ptrbase; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase_Calls {
    long NextHawkFrame; CIntelligentPointer Customer; virtual void v00() { }
    CAIStateGroup_MinionHawkingFromStall() : CAIStateGroupBase_Calls(), NextHawkFrame(0), Customer() { }
};
int main()
{
    unsigned char raw[sizeof(CAIStateGroup_MinionHawkingFromStall)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CAIStateGroup_MinionHawkingFromStall* g = new (raw) CAIStateGroup_MinionHawkingFromStall;
    if (g_base != 1 || g_ptrbase != 1 || g->NextHawkFrame != 0 || g->Customer.PData != 0 || offsetof(CAIStateGroup_MinionHawkingFromStall, NextHawkFrame) != 0x14
        || offsetof(CAIStateGroup_MinionHawkingFromStall, Customer) != 0x18 || sizeof(*g) != 0x20) { std::printf("00900241_TEST FAIL\n"); return 1; }
    std::printf("00900241_TEST PASS\n"); return 0;
}