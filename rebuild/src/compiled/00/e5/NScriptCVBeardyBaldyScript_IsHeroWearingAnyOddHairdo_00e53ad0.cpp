// CV_BeardyBaldyScript::IsHeroWearingAnyOddHairdo() -- true when the hero wears any of the five odd haircuts (buzz, basin, mohawk, ponytail, plaits)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E53AD0, 402 bytes (416 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; through the interface (+0x40)
//   GetHero (slot 70, +0x118) and IsWearingHairstyle (slot 193, +0x304).
// Strings: "OBJECT_HERO_HAIR_BUZZ_01" 0x012E5EC4, "OBJECT_HERO_HAIR_BASIN_01" 0x012DC7A4, "OBJECT_HERO_HAIR_MOHAWK_01" 0x012E5EA8,
//   "OBJECT_HERO_HAIR_PONYTAIL_01" 0x0124F410, "OBJECT_HERO_HAIR_PLATS_01" 0x012E5E8C.
// Shape: an `||` chain of implicit string conversions into a named bool: VC7.1 keeps one construction-flag bit per
//   conditional temporary in ebx (mov ebx,1/3/7/0Fh/1Fh) and destroys the built ones after the chain.
// Layout: interface pointer at +0x40 (CQuestScriptBase, landed relatives). Local model: the generated CGameScriptInterface.h
//   carries no virtual slots.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_BeardyBaldyScript {
public:
    unsigned char _base[0x40];                              // CScriptBase
    CGameScriptInterface* Game;                             // +0x40
    bool IsHeroWearingAnyOddHairdo();
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;
bool NScript::CV_BeardyBaldyScript::IsHeroWearingAnyOddHairdo()
{
    bool wearing = Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_BUZZ_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_BASIN_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_MOHAWK_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_PONYTAIL_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_PLATS_01");
    return wearing;
}