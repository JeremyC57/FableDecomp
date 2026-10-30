// Behaviour test for 0x0042f722: the candidate source below is embedded verbatim, followed by stand-ins for its callees
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; printf("check failed line %d\n", __LINE__); } } while (0)
#define private public
// ---- embedded candidate source ----
// CNewFrontendGameComponent::InitialiseDefs() -- compile the defs, then fetch the "FRONT_END" front-end def
// Leaf: ?InitialiseDefs@CNewFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe 0x0042F722, 60 bytes, frame-pointer size TU (`push ebp ; mov ebp,esp ; push ecx` ... `leave`).
// Names (Ego_r.pdb + FableWin.pdb): ?InitialiseDefs@CNewFrontendGameComponent@@AAEXXZ (private), ?CompileDefs@
//   CNewFrontendGameComponent@@SIXXZ (static __fastcall, 0x0042F70B), ?Get@CStartupDefinitionManager@@SIAAV1@XZ (0x0043368D:
//   `mov eax,[0x013B8760] ; ret`, the startup manager singleton), ??$GetDef@VCFrontEndDef@@@CDefinitionManager@@QBE_NABV
//   CCharString@@JPAV?$CDefPointer@$$CBVCFrontEndDef@@@@@Z (0x004300FB), CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: GetDef takes the def pointer BY POINTER (`&m_pFrontEndDef`, `add esi,3Ch ; push esi`) as the FableWin PDB spells it;
//   with that real signature VC7.1 schedules `push 1` before the `lea` of the "FRONT_END" temporary as retail does (the old
//   by-reference model left one lea/push swap through 12 grades). The arguments are pushed before Get() runs (VC evaluates
//   the object expression after the arguments). m_pFrontEndDef +0x3C is the Ego_r.pdb member (no engine header).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#define CDefinitionManager CDefinitionManagerPdb
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CCharString
{
public:
    CCharString(const char* text, long length = -1);                     // 0x0099EBF0
    ~CCharString();                                                      // 0x0099EAE0
private:
    void* PStringData;
};
class CFrontEndDef;
class CDefinitionManager : public CDefinitionManagerPdb
{
public:
    template <class T> bool GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const;   // 0x004300FB
};
class CStartupDefinitionManager : public CDefinitionManager
{
public:
    static CStartupDefinitionManager& __fastcall Get();                 // 0x0043368D
};
class CNewFrontendGameComponent
{
public:
    static void __fastcall CompileDefs();                                // 0x0042F70B
private:
    void InitialiseDefs();
    unsigned char _pad00[0x3c];
    CDefPointer<const CFrontEndDef> m_pFrontEndDef;                      // +0x3C
};
void CNewFrontendGameComponent::InitialiseDefs()
{
    CompileDefs();
    CStartupDefinitionManager::Get().GetDef("FRONT_END", 1, &m_pFrontEndDef);
}

// ---- end of embedded source ----
// ---- stand-ins ----
static int g_step = 0, g_compiledAt = 0, g_getDefAt = 0, g_liveStrings = 0;
static char g_name[32];
static long g_index = 0;
static const void* g_defArg = 0;
static const void* g_manager = 0;
static unsigned char g_managerStorage[sizeof(CStartupDefinitionManager)];
CCharString::CCharString(const char* text, long)
{
    strncpy(g_name, text, sizeof(g_name) - 1);
    PStringData = g_name;
    ++g_liveStrings;
}
CCharString::~CCharString() { --g_liveStrings; }
void __fastcall CNewFrontendGameComponent::CompileDefs() { g_compiledAt = ++g_step; }
CStartupDefinitionManager& __fastcall CStartupDefinitionManager::Get()
{
    return *reinterpret_cast<CStartupDefinitionManager*>(g_managerStorage);
}
template <class T> bool CDefinitionManager::GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const
{
    g_getDefAt = ++g_step;
    CHECK(name.PStringData == g_name && strcmp(g_name, "FRONT_END") == 0);
    g_index = index;
    g_defArg = def;
    g_manager = this;
    return true;
}
int main()
{
    static unsigned char storage[sizeof(CNewFrontendGameComponent)];
    CNewFrontendGameComponent* component = reinterpret_cast<CNewFrontendGameComponent*>(storage);
    component->InitialiseDefs();
    CHECK(g_compiledAt == 1 && g_getDefAt == 2);                  // CompileDefs runs before the def lookup
    CHECK(g_index == 1);
    CHECK(g_defArg == storage + 0x3c);                            // m_pFrontEndDef at +0x3C
    CHECK(g_manager == g_managerStorage);
    CHECK(g_liveStrings == 0);                                    // the "FRONT_END" temporary is destroyed
    if (g_ok) printf("0042f722_TEST PASS\n");
    return g_ok ? 0 : 1;
}
