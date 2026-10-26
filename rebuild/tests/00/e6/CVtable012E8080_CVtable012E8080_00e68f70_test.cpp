#include <cstdio>
static int g_strings = 0;
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
class CScriptBase { public: virtual void v00() {} unsigned char _body[0x3c]; CScriptBase() {} };
struct __declspec(novtable) CScriptObject : CScriptBase { void* m_pThing; void* m_pOwner; CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { } };
struct CVtable_012E8080 : CScriptObject { unsigned char _pad[0x40]; CCharString m_name; virtual void v00() {} CVtable_012E8080(void* owner, void* thing); };
CVtable_012E8080::CVtable_012E8080(void* owner, void* thing) : CScriptObject(owner, thing), m_name() { }
int main()
{
    int owner, thing;
    CVtable_012E8080 o(&owner, &thing);
    if (o.m_pOwner != &owner || o.m_pThing != &thing || o.m_name.m_pData != 0 || g_strings != 1) { std::printf("00e68f70_TEST FAIL\n"); return 1; }
    std::printf("00e68f70_TEST PASS\n");
    return 0;
}