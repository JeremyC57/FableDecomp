// CDF_ThreeSuits::DressedAsBandit() -- true when the hero wears all five Bandit Camp clothing items (shirt, trousers,
// hat, boots, gloves), checked in that order with short-circuit.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E72F00, length 416 bytes (0x192 code + int3 pad).
// Callees: 0x0099EBF0 CCharString(const char*, long) (x5), CGameScriptInterface (at +4) slot 70 (+0x118) GetHero,
// slot 187 (+0x2ec) IsWearingClothingItem(const CScriptThing&, const CCharString&) (FSE manifest names the pair),
// 0x0099EAE0 ~CCharString (x5, each under its construction flag).
// Layout: descriptive (no PDB class): the game script interface pointer at +4 (as the landed CAC_Owner 0x00E33660).
// Shape: one `&&` chain of five implicit-literal CCharString temporaries: without C++ exceptions VC7.1 tracks the
// constructed temporaries in ebx (1, 3, 7, 0xf, 0x1f), parks the bool in a frame byte and destroys them in reverse.
// Matched first try.
// Leaf: DressedAsBandit
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

struct CDF_ThreeSuits {
    virtual void v00();
    CGameScriptInterface* m_pGame;                       // +0x04
    bool DressedAsBandit();
};

bool CDF_ThreeSuits::DressedAsBandit()
{
    return m_pGame->IsWearingClothingItem(m_pGame->GetHero(), "OBJECT_HERO_SHIRT_BANDITCAMP")
        && m_pGame->IsWearingClothingItem(m_pGame->GetHero(), "OBJECT_HERO_TROUSERS_BANDITCAMP")
        && m_pGame->IsWearingClothingItem(m_pGame->GetHero(), "OBJECT_HERO_HAT_BANDITCAMP")
        && m_pGame->IsWearingClothingItem(m_pGame->GetHero(), "OBJECT_HERO_BOOTS_BANDITCAMP")
        && m_pGame->IsWearingClothingItem(m_pGame->GetHero(), "OBJECT_HERO_GLOVES_BANDITCAMP");
}