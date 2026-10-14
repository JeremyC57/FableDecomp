#include <cstdio>

#include <cstddef>
static int g_scriptBase = 0, g_names = 0;
struct CScriptBase { virtual void v00() { } unsigned char _body[0x3c]; CScriptBase() { ++g_scriptBase; } };
struct CCharString { char* m_p; CCharString() : m_p(0) { ++g_names; } };
struct __declspec(novtable) CScriptObject : CScriptBase {
    void* m_pThing; void* m_pOwner;
    CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};
struct CVtable_012ec138 : CScriptObject {
    unsigned char _pad[0x8];
    CCharString m_name;
    unsigned char _tail[0x0];
    virtual void v00() { }
    CVtable_012ec138(void* owner, void* thing) : CScriptObject(owner, thing), m_name() { }
};
CVtable_012ec138* __fastcall Alloc_CVtable_012ec138(void* owner, void* thing) { return new CVtable_012ec138(owner, thing); }
int main()
{
    int o = 1, t = 2;
    CVtable_012ec138* p = Alloc_CVtable_012ec138(&o, &t);
    if (!p || g_scriptBase != 1 || g_names != 1 || p->m_pOwner != &o || p->m_pThing != &t || p->m_name.m_p != 0
        || offsetof(CVtable_012ec138, m_name) != 0x50 || sizeof(CVtable_012ec138) != 0x54) { std::printf("00e92900_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00e92900_TEST PASS\n"); return 0;
}
