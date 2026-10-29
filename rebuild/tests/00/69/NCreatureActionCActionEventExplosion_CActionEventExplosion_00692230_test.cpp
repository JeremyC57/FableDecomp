#include <cstdio>
static int g_strings = 0;
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
class CBaseClass {
public:  virtual void v00() {} CBaseClass() {} };
namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBaseClass { bool m_bTriggered; CActionEventBase() : CBaseClass(), m_bTriggered(false) { } };
struct CActionEventExplosion : CActionEventBase { unsigned char _pad[0x0c]; CCharString m_name; virtual void v00() {} CActionEventExplosion(); };
}
NCreatureAction::CActionEventExplosion::CActionEventExplosion() : CActionEventBase(), m_name() { }
int main()
{
    NCreatureAction::CActionEventExplosion e;
    if (e.m_bTriggered != false || e.m_name.m_pData != 0 || g_strings != 1) { std::printf("00692230_TEST FAIL\n"); return 1; }
    std::printf("00692230_TEST PASS\n");
    return 0;
}