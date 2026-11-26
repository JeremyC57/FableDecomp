// CInputProcessProjectileTargetingAnalogueZoom::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- for a live hero: the analogue action 0x54 zooms the projectile-targeting camera by its Y value / 255, presses of actions 0x6e / 0x6f zoom by +-196/255; a zoom marks the input handled
// Leaf: ?ProcessInput@CInputProcessProjectileTargetingAnalogueZoom@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the non-virtual ?ProcessInput@CInputProcessProjectileTargetingAnalogueZoom@@QAE... on a local class
//   model; respelt onto engine/CInputProcessProjectileTargetingAnalogueZoom.h FABLE_RETAIL_INPUTPROCESS_VTABLE (Ego_r.pdb / FableWin.pdb public virtual, retail
//   vftable slot 4), so the family vftables 0x00488D10 emits name this row. The base queries come from the retail view of
//   engine/CInputProcessGameBase.h (the same PDB spellings). No body change.
// NF64 (round 7, q10652): the local IsEventButtonPressed / Held / Released redeclarations on the derived class are gone; the calls bind
//   the Ego_r.pdb + FableWin.pdb protected const CInputProcessGameBase members (overlay opt-in FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068F320, length 323 bytes, `ret 8`.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), CInputProcessGameBase::GetEventGameActionVector(long,
//   const CInputEvent&, C2DVector*) @ 0x00687D00 (landed name), IsEventGameAction @ 0x00687CF0, IsEventButtonPressed
//   @ 0x00687A90, the component map's LowerBound @ 0x0040F020, the zoom component's AdjustZoom(float) @ 0x00789A10
//   (adds delta * k to +0x40 and clamps it to [0,1]; manifest "ZoomControl_Update"), CProcessedInput::SetAsPerformedEvent @ 0x00A0D390.
//   Float pool: 1/255 @ 0x01231724; the +-196/255 deltas are pushed as immediates.
// Layout: `this` is engine/CInputProcessGameBase.h (MyPlayer +0x18); CThing dead flag bit 0 of +0x91, component bitset +0x20
//   (0x81 = word 4 bit 1, read as `test al,2` on +0x30) and map +0x44 as in the landed CInputProcessBetting 0x00691560.
//   Component 0x81's class is unconfirmed (named CTCProjectileZoom here).
// Shape: GetTC is a `__forceinline` `T* c = 0; if (HasTC(k)) c = Find(k); return c;` inlined at each of the three zoom
//   calls (the `xor ecx,ecx` preload each time); its key shares the dead `event` argument slot.
#include "fable_compiler.h"
#include "engine/EGameAction.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#include "engine/CInputProcessGameBase.h"
class C2DVector { public:
    float x, y;
};
class CInputEvent;
class CProcessedInput {
public:
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
struct CTCProjectileZoom {
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);   // 0x0040F020
    FABLE_FORCEINLINE CTCProjectileZoom* Find(long kind)
    {
        STCEntry* it = LowerBound((ETCInterfaceType)kind);
        return ((CTCProjectileZoom*)((it == m_pLast || kind < it->first) ? m_pLast : it)->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    CTCMap        m_tcs;                           // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    FABLE_FORCEINLINE CTCProjectileZoom* GetTC(long kind)
    {
        CTCProjectileZoom* tc = 0;
        if (HasTC(kind))
            tc = m_tcs.Find(kind);
        return tc;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
#include "engine/CInputProcessProjectileTargetingAnalogueZoom.h"

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00789A10 ?SetProjectileTargetingZoomDelta@CTCHeroRangedAttacks@@QAEXM@Z (was ?AdjustZoom@CTCProjectileZoom@@QAEXM@Z)
class CTCHeroRangedAttacks { public: void SetProjectileTargetingZoomDelta(float); };
void CInputProcessProjectileTargetingAnalogueZoom::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (!hero || hero->m_dead)
        return;
    C2DVector zoom;
    if (GetControlMovementFromGameActionEvent(GAME_ACTION_PROJECTILE_TARGETING_ANALOGUE_ZOOM, event, zoom))
        reinterpret_cast<CTCHeroRangedAttacks*>(hero->GetTC(0x81))->SetProjectileTargetingZoomDelta(zoom.y * (1.0f / 255.0f));
    else if (IsEventGameAction(GAME_ACTION_DIGITAL_ANALOGUE_ZOOM_IN, event)) {
        if (!IsEventButtonPressed(event))
            return;
        reinterpret_cast<CTCHeroRangedAttacks*>(hero->GetTC(0x81))->SetProjectileTargetingZoomDelta(196.0f / 255.0f);
    } else if (IsEventGameAction(GAME_ACTION_DIGITAL_ANALOGUE_ZOOM_OUT, event) && IsEventButtonPressed(event))
        reinterpret_cast<CTCHeroRangedAttacks*>(hero->GetTC(0x81))->SetProjectileTargetingZoomDelta(-196.0f / 255.0f);
    else
        return;
    processed.SetAsPerformedEvent();
}