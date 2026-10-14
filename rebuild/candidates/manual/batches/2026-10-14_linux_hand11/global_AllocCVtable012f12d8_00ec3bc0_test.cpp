#include <cstdio>

static int g_scriptBase = 0;
struct CScriptBase { virtual void v00() { } CScriptBase() { ++g_scriptBase; } };
struct CVtable_012f12d8 : CScriptBase {
    unsigned char _pad[0x3c]; void* m_pThing; void* m_pOwner; unsigned char _tail[0x84 - 0x48];
    virtual void v00() { }
    CVtable_012f12d8(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};
CVtable_012f12d8* __fastcall Alloc_CVtable_012f12d8(void* owner, void* thing) { return new CVtable_012f12d8(owner, thing); }
int main()
{
    int o = 1, t = 2;
    CVtable_012f12d8* p = Alloc_CVtable_012f12d8(&o, &t);
    if (!p || g_scriptBase != 1 || p->m_pOwner != &o || p->m_pThing != &t || sizeof(CVtable_012f12d8) != 0x84) { std::printf("00ec3bc0_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00ec3bc0_TEST PASS\n"); return 0;
}
