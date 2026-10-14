#include <cstdio>
#include <stddef.h>
#include <new>
struct CClassFactory_Calls; struct CClass_Calls;
static int g_base = 0; static CClass_Calls* g_cls = 0; static unsigned long g_type = 0; static int g_local = -1; static CClassFactory_Calls* g_fac = 0;
struct CBaseIntelligentPointer { virtual void v00() { } void* PData; CBaseIntelligentPointer() : PData(0) { ++g_base; } };
struct CClass_Calls : CBaseIntelligentPointer {
    void* ClassListPos; void* TypeListPos; unsigned char Type; virtual void v00() { }
    CClass_Calls(CClassFactory_Calls* f, unsigned long type, bool local);
};
struct CClassFactory_Calls { void RegisterAllocatedClass(CClass_Calls* c, unsigned long type, bool local) { g_fac = this; g_cls = c; g_type = type; g_local = local ? 1 : 0; } };
CClass_Calls::CClass_Calls(CClassFactory_Calls* f, unsigned long type, bool local) : CBaseIntelligentPointer(), Type((unsigned char)type) { f->RegisterAllocatedClass(this, type, local); }
int main()
{
    CClassFactory_Calls fac; unsigned char raw[sizeof(CClass_Calls)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CClass_Calls* c = new (raw) CClass_Calls(&fac, 0x123, true);
    if (g_base != 1 || g_fac != &fac || g_cls != c || g_type != 0x123 || g_local != 1 || c->Type != 0x23 || c->PData != 0
        || offsetof(CClass_Calls, ClassListPos) != 8 || offsetof(CClass_Calls, TypeListPos) != 0xc || offsetof(CClass_Calls, Type) != 0x10) { std::printf("00a372d0_TEST FAIL\n"); return 1; }
    std::printf("00a372d0_TEST PASS\n"); return 0;
}
