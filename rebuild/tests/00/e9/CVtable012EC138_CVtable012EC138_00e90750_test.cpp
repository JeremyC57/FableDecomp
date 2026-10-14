#include <cstdio>
static int g_strings = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
struct CScriptBase { virtual void v00() {} unsigned char _body[0x3c]; CScriptBase() {} };
struct __declspec(novtable) CScriptObject : CScriptBase { void* m_pThing; void* m_pOwner; CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { } };
struct CVtable_012EC138 : CScriptObject { unsigned char _pad[8]; CCharString m_name; virtual void v00() {} CVtable_012EC138(void* owner, void* thing); };
CVtable_012EC138::CVtable_012EC138(void* owner, void* thing) : CScriptObject(owner, thing), m_name() { }
int main()
{
    int owner, thing;
    CVtable_012EC138 o(&owner, &thing);
    if (o.m_pOwner != &owner || o.m_pThing != &thing || o.m_name.m_pData != 0 || g_strings != 1) { std::printf("00e90750_TEST FAIL\n"); return 1; }
    std::printf("00e90750_TEST PASS\n");
    return 0;
}