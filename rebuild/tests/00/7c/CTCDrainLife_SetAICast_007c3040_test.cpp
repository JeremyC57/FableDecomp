// Behaviour test for CTCDrainLife::SetAICast(long, long) @ 0x007C3040: SpellLevel is always stored; the definition is
// fetched into PDef only for a positive def index.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CSpecialAbilitiesDrainLifeDef { int id; };
static CSpecialAbilitiesDrainLifeDef g_def;
static int g_getDefCalls = 0;
static long g_getDefIndex = 0;
template<class T> struct CDefPointer { T* Object; };
struct CDefinitionManager_Calls {
    template<class T> bool GetDef(long globalIndex, CDefPointer<T>& out)
    { ++g_getDefCalls; g_getDefIndex = globalIndex; out.Object = &g_def; return true; }
};
struct CGameDefinitionManager : CDefinitionManager_Calls {
    static CGameDefinitionManager& __fastcall Get();
};
static CGameDefinitionManager g_manager;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
struct CTCDrainLife {
    virtual void v00() { }
    unsigned char _base[0x8];
    CDefPointer<const CSpecialAbilitiesDrainLifeDef> PDef;
    long          SpellLevel;
    void SetAICast(long spellLevel, long defIndex);
};
void CTCDrainLife::SetAICast(long spellLevel, long defIndex)
{
    SpellLevel = spellLevel;
    if (defIndex > 0)
        CGameDefinitionManager::Get().GetDef(defIndex, PDef);
}
int main()
{
    CTCDrainLife d; d.PDef.Object = 0; d.SpellLevel = 0;
    d.SetAICast(3, 0);
    if (d.SpellLevel != 3 || g_getDefCalls != 0 || d.PDef.Object != 0) { std::printf("007c3040_TEST FAIL\n"); return 1; }
    d.SetAICast(2, -5);
    if (d.SpellLevel != 2 || g_getDefCalls != 0) { std::printf("007c3040_TEST FAIL\n"); return 1; }
    d.SetAICast(4, 77);
    if (d.SpellLevel != 4 || g_getDefCalls != 1 || g_getDefIndex != 77 || d.PDef.Object != &g_def) { std::printf("007c3040_TEST FAIL\n"); return 1; }
    std::printf("007c3040_TEST PASS\n");
    return 0;
}