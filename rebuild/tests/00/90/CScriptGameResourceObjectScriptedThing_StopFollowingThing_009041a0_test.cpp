// Behaviour test for 0x009041A0 CScriptGameResourceObjectScriptedThing::StopFollowingThing: for a live thing and a live target, a
// thing that is following (action type 4) terminates "CActionFollowCreature" (retail 0x0128D850); otherwise it looks up the
// "EXPRESSION_WAIT" def (0x012757DC) and posts an expression-performed-to event for the target.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined; it is called through a
// __fastcall shim aliased to the row symbol). The callees are defined under the row's own spellings; the target CScriptThing is a fixture
// whose vtable answers IsAlive (slot 75) and GetPThing (slot 11). The test asserts the names the row passes; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* PStringData; CCharString(const char* text, long length); ~CCharString(); };
class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseObject* GetPItem() const; CBaseObjectPointer* PData; };
class CTCScriptedControl { public: unsigned char _pad00[0x1c]; long CurrentActionType; bool TerminateActionByName(const CCharString& name); };
class CExpressionDef { public: virtual void v00(); virtual void Destroy(); long RefCount; };
template <class T> class CDefPointer { public: T* Object; ~CDefPointer() { } };
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();
    CDefPointer<CExpressionDef> GetPDefFromInstantiationName(const CCharString& name);
};
namespace NEntityEvents { class CEventBase { public: }; }
class CTCEntityEvents { public: void AddEvent(NEntityEvents::CEventBase* event); };
static char g_log[256]; static int g_bad = 0, g_destroyed = 0; static void* g_event = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; if (length != -1) g_bad = 1; }
CCharString::~CCharString() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
bool CTCScriptedControl::TerminateActionByName(const CCharString& name) { strcat(g_log, "terminate["); strcat(g_log, (const char*)name.PStringData); strcat(g_log, "]"); return true; }
void CExpressionDef::v00() { }
void CExpressionDef::Destroy() { ++g_destroyed; }
static CExpressionDef g_def; static double g_mgrRaw[1];
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return *(CGameDefinitionManager*)g_mgrRaw; }
CDefPointer<CExpressionDef> CGameDefinitionManager::GetPDefFromInstantiationName(const CCharString& name)
{ strcat(g_log, "def["); strcat(g_log, (const char*)name.PStringData); strcat(g_log, "]"); CDefPointer<CExpressionDef> p; p.Object = &g_def; ++g_def.RefCount; return p; }
void CTCEntityEvents::AddEvent(NEntityEvents::CEventBase* event) { strcat(g_log, "event"); g_event = event; }
// the target CScriptThing: vtable slots 11 GetPThing and 75 IsAlive (thiscall-compatible __fastcall)
static unsigned char g_targetThing[0xa0];
extern "C" void* __fastcall NF420_GetPThing(void*, int) { return g_targetThing; }
extern "C" bool __fastcall NF420_IsAlive(void*, int) { return true; }
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("CScriptThing virtual"); }
extern "C" void __fastcall NF420_Row(void* self, int edx, const void* target);
#pragma comment(linker, "/alternatename:@NF420_Row@12=?StopFollowingThing@CScriptGameResourceObjectScriptedThing_Methods@@QAEXABVCScriptThing@@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    void* vtbl[79]; for (int i = 0; i < 79; ++i) vtbl[i] = (void*)NF420_VirtualTrap;
    vtbl[11] = (void*)NF420_GetPThing; vtbl[75] = (void*)NF420_IsAlive;
    void* target[3] = { vtbl, 0, 0 };
    static CTCScriptedControl control; static CTCEntityEvents events;
    static unsigned long thing[0xa0 / 4]; unsigned char* t = (unsigned char*)thing;
    thing[0x20 / 4] = 1UL << 31; thing[0x20 / 4 + 2] = 1UL << 2;          // interfaces 0x1f and 0x42 present
    static long pairs[4] = { 0x1f, (long)&control, 0x42, (long)&events };
    *(long**)(t + 0x44) = pairs; *(long**)(t + 0x48) = pairs + 4; *(long**)(t + 0x4c) = pairs + 4;
    static unsigned char self[0x20]; *(void**)(self + 0x18) = thing;           // PThing (CIntelligentPointer<CThing>) data word
    control.CurrentActionType = 4;
    NF420_Row(self, 0, target);
    bool ok = strcmp(g_log, "terminate[CActionFollowCreature]") == 0;
    control.CurrentActionType = 1; g_log[0] = 0; g_def.RefCount = 1;
    NF420_Row(self, 0, target);
    ok = ok && strcmp(g_log, "def[EXPRESSION_WAIT]event") == 0 && g_event && *(void**)((unsigned char*)g_event + 0xc) == g_targetThing
        && *(CExpressionDef**)((unsigned char*)g_event + 0x10) == &g_def && g_def.RefCount > 0 && g_destroyed == 0 && !g_bad;
    if (ok) printf("009041a0_TEST PASS\n");
    else printf("FAIL log=%s ref=%ld destroyed=%d bad=%d\n", g_log, g_def.RefCount, g_destroyed, g_bad);
    return ok ? 0 : 1;
}
