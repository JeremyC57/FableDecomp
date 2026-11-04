// NScript::CV_BordelloScript::AdjustTavernPrices() -- each time the Bordello region loads: cache the bordello village thing, stop the barmaid and barman screaming at the hero, apply the hero tavern price multiplier once the hero owns the bordello, wait for the region to unload, then reset prices to 1.0 and the beer count
// Leaf: ?AdjustTavernPrices@CV_BordelloScript@NScript@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3A6B0, 1013 bytes, plain `ret`; same TU as the landed CreateCustomers (0x00E3A350), whose model this reuses.
// Identity: the CV_BordelloScript spawner stores 0x00E3A6B0 as a CSpawnedFunc member-function pointer (+0x34, vtable 0x012E38C4, at
//   0x00E39FDD) named "AdjustTavernPrices" (0x012E38E0); Ego_r.pdb and FableWin.pdb both list ?AdjustTavernPrices@CV_BordelloScript@NScript@@QAEXXZ.
//   The catalog label "CScriptGameResourceObjectScriptedThingBase_HandleBordelloNPC" is wrong.
// Layout (retail = Ego_r.pdb - 0x14): PlayerOwned +0x4c, BordelloVillage (CScriptThing) +0x5c, BeersDrunk +0x6c; CScriptDef
//   BordelloHeroTavernPriceMultiplier +0x4fc (as landed elsewhere).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString 0x0099EBF0 / ~ 0x0099EAE0; ~CBaseClass 0x0099A2E0 and
//   operator delete 0x00BFE9BC (inline ~CScriptThing and CScriptThing::operator= via CCountedPointer::ShareData); Game slots 7 NewScriptFrame,
//   12 IsRegionLoaded, 70 GetHero, 81 GetNearestWithDefName (by value), 611 EntitySetOpinionReactionMask, 880 SetTradingPriceMult;
//   pGlobalScriptDef 0x0143E90C.
// Shape: `while (!x)` waits give neg/sbb/inc, `while (!(!x))` gives setne (GOTCHAS chunk 248).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
// CDefPointer<const CScriptDef>: local model (engine/CDefPointer.h + CParentDefClassBase.h pull <stddef.h>, which hangs cl in the lane).
template <class T> class CDefPointer { public:
    T* Data;
    T* operator->() const { return Data; }
};
class CScriptDef {
    unsigned char _pad_0x0[0x4fc];
public:
    float BordelloHeroTavernPriceMultiplier;            // +0x4fc
};
extern CDefPointer<const CScriptDef> pGlobalScriptDef;  // 0x0143E90C
#include "engine/CScriptBase.h"
namespace NScript {
class CV_BordelloScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*         Owner;                                // +0x44
    unsigned char _pad48[0x4c - 0x48];
    bool          PlayerOwned;                          // +0x4c
    bool          BecomeNunnery;                        // +0x4d
    unsigned char _pad4e[0x5c - 0x4e];
    CScriptThing  BordelloVillage;                      // +0x5c
    unsigned char _pad68[0x6c - 0x68];
    long          BeersDrunk;                           // +0x6c
    void AdjustTavernPrices();
};
} // namespace NScript
void NScript::CV_BordelloScript::AdjustTavernPrices()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("Bordello")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        BordelloVillage = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BORDELLO");
        Game->EntitySetOpinionReactionMask(Game->GetNearestWithDefName(Game->GetHero(), "CREATURE_OAKVALE_VILLAGER_FEMALE_BARMAID"), "OPINION_REACTION_MASK_DONT_SCREAM");
        Game->EntitySetOpinionReactionMask(Game->GetNearestWithDefName(Game->GetHero(), "CREATURE_OAKVALE_VILLAGER_MALE_BARMAN"), "OPINION_REACTION_MASK_DONT_SCREAM");
        while (Game->IsRegionLoaded("Bordello")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (PlayerOwned) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetTradingPriceMult(pGlobalScriptDef->BordelloHeroTavernPriceMultiplier);
                break;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        while (!(!Game->IsRegionLoaded("Bordello"))) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetTradingPriceMult(1.0f);
        BeersDrunk = 0;
        Game->NewScriptFrame();
    }
}