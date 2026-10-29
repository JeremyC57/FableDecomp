#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_str = 0; static void* g_strAt[4];
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { g_strAt[g_str++] = this; } };
static int g_base = 0;
class CBaseClass {
public:  virtual void v00() { } CBaseClass() { ++g_base; } };
namespace NCreatureAction { struct CActionEventBase_00692120 : CBaseClass { bool CActionEventBase_flag; CActionEventBase_00692120() : CBaseClass(), CActionEventBase_flag(false) { } };
struct CActionEventTriggerParticle : CActionEventBase_00692120 { unsigned char _pad08[0x10]; CCharString m_name; CCharString m_text; virtual void v00() { } CActionEventTriggerParticle() : CActionEventBase_00692120() { } }; }
int main() {
    NCreatureAction::CActionEventTriggerParticle* d = new NCreatureAction::CActionEventTriggerParticle; if (g_base != 1 || d->CActionEventBase_flag || g_str != 2 || g_strAt[0] != &d->m_name || g_strAt[1] != &d->m_text) { std::printf("00692120_TEST FAIL\n"); return 1; } delete d;
    std::printf("00692120_TEST PASS\n"); return 0;
}