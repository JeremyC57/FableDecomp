#ifndef FABLE_ENGINE_CTCBATTLECHARGE_H
#define FABLE_ENGINE_CTCBATTLECHARGE_H
// CTCBattleCharge (new, chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the hero's battle-charge
// component (TCI_BATTLE_CHARGE 168). Ego_r.pdb (struct_layouts_egor.tsv, 152 bytes there): PTarget CIntelligentPointer<CThing> +0x0C
// (12 bytes there, 8 in retail: every later member -4), PDef +0x14, PositionStartedFrom +0x18, ..., IsZTargetting (bool, 140 there;
// retail +0x88: the row tests `[eax+88h]` then compares `PTarget.GetPItem()` at +0x0C with the targeted thing). Only those two are
// modelled; the rest is opaque. Retail sizeof 0x94 (152 - 4 - 16: the one pointer and the PeopleHit vector, 16 -> 12).
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ETCInterfaceType.h"
class CThing;
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_STARTUP_PASS24
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
class CSpecialAbilitiesBattleChargeDef;
#endif
#pragma pack(push, 1)
class CTCBattleCharge : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_BATTLE_CHARGE; }
    // PLACEHOLDER accessor names (no PDB on disk lists a getter; the row reads the members as inline tests):
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_HIGHLIGHT_NAMES
    // Retail opt-in (Codex pass22): both PDB records and donor Draw calls. Pointer return is not a handle reference.
    CThing* GetPTarget() const { return PTarget.Get(); }
    bool IsBattleChargeZTargetting() const { return IsZTargetting; }
#else
    const CIntelligentPointer<CThing>& GetPTarget() const { return PTarget; }
    bool GetIsZTargetting() const { return IsZTargetting; }
#endif
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_VSLOTS
// agentFAM04 (2026-11-11): retail virtuals and the private cleanup they share (Ego_r.pdb / FableWin.pdb spellings;
// TLC vtable slots aligned with the Xbox 360 Test PDB order). Declarations only (no layout change).
    virtual bool HasFrameUpdate() const;                                 // slot 8, 0x007A92B0
    virtual void OnKill();                                               // slot 14, 0x007A7BD0 (tail call to FinalCleanup)
private:
    void FinalCleanup();                                                 // 0x007A75F0 ?FinalCleanup@CTCBattleCharge@@AAEXXZ
public:
#endif // FABLE_RETAIL_CTCBATTLECHARGE_VSLOTS
private:
    CIntelligentPointer<CThing> PTarget;                                 // +0x0c
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_STARTUP_PASS24
    // Ego_d complete class and native reads at 0x007A7724–0x007A7755.
    void CreateStartUpEffect();
    CDefPointer<const CSpecialAbilitiesBattleChargeDef> PDef;             // +0x14
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_CREATEHIT_PASS27
    void CreateHitEffect();
    C3DVector PositionStartedFrom;
    CIntelligentPointer<CThing> CollidedThing;
    bool StartedMoving;
    bool JustReleased;
#else
    unsigned char _unmodelled_0x18[0x2E - 0x18];
#endif
    bool DirectionOverride;                                              // +0x2E
    C3DVector DirectionOverrideVector;                                   // +0x2F
#ifdef FABLE_RETAIL_BATTLE_CHARGE_RUSH_PASS29
    unsigned char _pad3b;
    float DashDistance;
    void RushForwards();
    void SetupFacingDirection(C3DVector&);
    void FinishMove(CThingCreatureBase&,bool);
#else
    unsigned char _unmodelled_0x3B[0x40 - 0x3B];
#endif
    long SpellLevel;                                                     // +0x40
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_CREATEHIT_PASS27
    CIntelligentPointer<CThing> PItemRemoved;
    float RadiusOfEffect;
#else
    unsigned char _unmodelled_0x44[0x50 - 0x44];
#endif
    CIntelligentPointer<CThing> ShieldEffect;                             // +0x50
    CIntelligentPointer<CThing> LeftFeetEffect;                           // +0x58
    CIntelligentPointer<CThing> RightFeetEffect;                          // +0x60
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_CREATEHIT_PASS27
    long PoseDelay;
    bool StartPoseDelay;
    bool HitSomething;
    unsigned char _pad_0x6e[2];
    std::vector<CIntelligentPointer<CThing> > PeopleHit;
    C3DVector DisplacementVector;
#else
    unsigned char _unmodelled_0x68[0x88 - 0x68];
#endif
#else
    unsigned char _unmodelled_0x14[0x88 - 0x14];                         // +0x14 PDef .. PeopleHit (not modelled)
#endif
    bool IsZTargetting;                                                  // +0x88
#ifdef FABLE_RETAIL_BATTLE_CHARGE_RUSH_PASS29
    bool FakeCollideWithSomething;
    unsigned char _pad8a[2];
    long ChargeDelayFrameCount;
    bool CompletedExitModes;
    unsigned char _pad91[3];
#else
    unsigned char _unmodelled_0x89[0x94 - 0x89];
#endif                         // +0x89 .. +0x93
};
#pragma pack(pop)
FABLE_STATIC_ASSERT_AT(38, sizeof(CTCBattleCharge) == 0x94);
// IsZTargetting at +0x88: 0x0C + 8 (PTarget) + (0x88 - 0x14) (opaque run); private, so no offsetof assert (VC7.1 refuses it).
#endif
