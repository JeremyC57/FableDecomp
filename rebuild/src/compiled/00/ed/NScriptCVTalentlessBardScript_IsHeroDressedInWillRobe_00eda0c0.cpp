// NScript::CV_TalentlessBardScript::IsHeroDressedInWillRobe() -- true when the hero wears a full evil, plain or good Will-user robe suit (shirt, gloves, trousers and boots of one set), each set checked with short-circuit
// Leaf: IsHeroDressedInWillRobe
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDA0C0, length 971 bytes (+ int3 padding), plain `ret`.
// Label correction: catalogued as CDF_ThreeSuits::DressedAsEvilMage (propagated across the byte-identical twin
//   0x00ED9CF0 (IsHeroDressedInArmour) and the CDF_ThreeSuits family); the body reads the bard script's Game interface at +0x40 and is called
//   from CV_TalentlessBardScript::ProcessSongsClothes (0x00ED9BB0, call at 0x00ED9C27); ghidra_out chunk 238 already
//   names it IsHeroDressedInWillRobe.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x12), CGameScriptInterface slot 70 (+0x118) GetHero,
//          slot 187 (+0x2ec) IsWearingClothingItem(const CScriptThing&, const CCharString&), CCharString::~CCharString
//          @ 0x0099EAE0 (x12, each under its construction flag).
// Layout (retail): Game +0x40 (landed CV_TalentlessBardScript::ProcessSongsAge 0x00EDA8C0).
// Shape: three named bools, each one `&&` chain of four implicit-literal CCharString temporaries (the landed
//   CDF_ThreeSuits::DressedAsBandit 0x00E72F00 shape: construction flags accumulate in ebx across the three chains,
//   each chain's temporaries destroyed in reverse right after it); the result is `evil || plain || good`.
class CCharString { public:
    char* m_pString;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CV_TalentlessBardScript { public:
    unsigned char          _base[0x40];
    CGameScriptInterface*  Game;                         // +0x40
    bool IsHeroDressedInWillRobe();
};

bool CV_TalentlessBardScript::IsHeroDressedInWillRobe()
{
    bool evil = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_WILLROBE_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_WILLROBE_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_WILLROBE_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_WILLROBE_EVIL");
    bool plain = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_WILLROBE")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_WILLROBE")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_WILLROBE")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_WILLROBE");
    bool good = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_WILLROBE_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_WILLROBE_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_WILLROBE_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_WILLROBE_GOOD");
    return evil || plain || good;
}
}