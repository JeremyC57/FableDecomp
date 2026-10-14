#include <cstdio>

#include <cstddef>
static int g_scriptBase = 0, g_names = 0;
struct CScriptBase { virtual void v00() { } unsigned char _body[0x3c]; CScriptBase() { ++g_scriptBase; } };
struct CCharString { char* m_p; CCharString() : m_p(0) { ++g_names; } };
struct __declspec(novtable) CScriptObject : CScriptBase {
    void* m_pThing; void* m_pOwner;
    CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};
struct CVtable_012e8080 : CScriptObject {
    unsigned char _pad[0x40];
    CCharString m_name;
    unsigned char _tail[0x4];
    virtual void v00() { }
    CVtable_012e8080(void* owner, void* thing) : CScriptObject(owner, thing), m_name() { }
};
CVtable_012e8080* __fastcall Alloc_CVtable_012e8080(void* owner, void* thing) { return new CVtable_012e8080(owner, thing); }
int main()
{
    int o = 1, t = 2;
    CVtable_012e8080* p = Alloc_CVtable_012e8080(&o, &t);
    if (!p || g_scriptBase != 1 || g_names != 1 || p->m_pOwner != &o || p->m_pThing != &t || p->m_name.m_p != 0
        || offsetof(CVtable_012e8080, m_name) != 0x88 || sizeof(CVtable_012e8080) != 0x90) { std::printf("00e6e230_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00e6e230_TEST PASS\n"); return 0;
}