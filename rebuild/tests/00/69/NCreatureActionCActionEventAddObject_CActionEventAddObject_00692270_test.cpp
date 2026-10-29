#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_str = 0; static void* g_strAt[4];
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { g_strAt[g_str++] = this; } };
static int g_base = 0;
class CBaseClass {
public:  virtual void v00() { } CBaseClass() { ++g_base; } };
namespace NCreatureAction { struct CActionEventBase_00692270 : CBaseClass { bool CActionEventBase_flag; CActionEventBase_00692270() : CBaseClass(), CActionEventBase_flag(false) { } };
struct CActionEventAddObject : CActionEventBase_00692270 { unsigned char _pad08[0xc]; CCharString m_name; CCharString m_text; virtual void v00() { } CActionEventAddObject() : CActionEventBase_00692270() { } }; }
int main() {
    NCreatureAction::CActionEventAddObject* d = new NCreatureAction::CActionEventAddObject; if (g_base != 1 || d->CActionEventBase_flag || g_str != 2 || g_strAt[0] != &d->m_name || g_strAt[1] != &d->m_text) { std::printf("00692270_TEST FAIL\n"); return 1; } delete d;
    std::printf("00692270_TEST PASS\n"); return 0;
}