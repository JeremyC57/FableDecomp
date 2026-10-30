// CV_BordelloScript::IsHeroWearingTash() -- true when the hero wears any of the six moustaches (the hairstyle slot holds the tash)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E44CC0, 478 bytes (480 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; through the interface (+0x40)
//   GetHero (slot 70, +0x118) and IsWearingHairstyle (slot 193, +0x304).
// Strings: "OBJECT_HERO_TASHSMITH_01" 0x012DC788, "OBJECT_HERO_TASHTRADER_01" 0x012E49A8, "OBJECT_HERO_TASHKHG_01" 0x012E4990, "OBJECT_HERO_TASHSHERIFF_01" 0x012E4974, "OBJECT_HERO_TASHCHINESE_01" 0x012E4958, "OBJECT_HERO_TASHSMALL_01" 0x012E493C.
// Shape: an `||` chain of implicit string conversions into a named bool: VC7.1 keeps one construction-flag bit per
//   conditional temporary in ebx (mov ebx,1/3/7/0Fh/1Fh/3Fh) and destroys the built ones after the chain.
// Layout: interface pointer at +0x40 (CQuestScriptBase, landed relatives). Local model: the generated CGameScriptInterface.h
//   carries no virtual slots.
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
struct CV_BordelloScript {
    unsigned char _base[0x40];                              // CScriptBase
    CGameScriptInterface* Game;                             // +0x40
    bool IsHeroWearingTash();
};
bool CV_BordelloScript::IsHeroWearingTash()
{
    bool wearing = Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSMITH_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHTRADER_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHKHG_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSHERIFF_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHCHINESE_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSMALL_01");
    return wearing;
}