#include <cstdio>
static int g_strings = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
struct CBase { virtual void v00() {} CBase() {} };
namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBase { bool m_bTriggered; CActionEventBase() : CBase(), m_bTriggered(false) { } };
struct CActionEventRemoveObject : CActionEventBase { unsigned char _pad[0x0c]; CCharString m_name; virtual void v00() {} CActionEventRemoveObject(); };
}
NCreatureAction::CActionEventRemoveObject::CActionEventRemoveObject() : CActionEventBase(), m_name() { }
int main()
{
    NCreatureAction::CActionEventRemoveObject e;
    if (e.m_bTriggered != false || e.m_name.m_pData != 0 || g_strings != 1) { std::printf("006922c0_TEST FAIL\n"); return 1; }
    std::printf("006922c0_TEST PASS\n");
    return 0;
}
