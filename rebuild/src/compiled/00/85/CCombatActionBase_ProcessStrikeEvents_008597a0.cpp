// CCombatActionBase::ProcessStrikeEvents() -- run the active strike event (damage, weapon aug effect), clear it when it ends, then arm the interrupt / reset-combo / movement-interrupt flags
// Leaf: ?ProcessStrikeEvents@CCombatActionBase@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008597A0, length 638 bytes (+ int3 padding), esp frame, plain `ret`; called from
//   CCombatActionBase::FrameUpdate @ 0x00859D80.
// Callees: CCreatureActionBase::GetPCreatureAnimationOwner @ 0x00692950 (result unused; manifest "_Dest_val<...>" label is
//   propagated), virtual IsStrikeEventActive (slot 14, +0x38) and GetAnimDelayTime (slot 6, +0x18),
//   CCombatActionBase::PerformPossibleDamageAction(float) @ 0x008591E0, GetFirstActiveStrikeEvent @ 0x00856E50,
//   GetWeaponUsedInStrikeAction @ 0x00857070, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10,
//   CIntelligentPointer<CThing>(CThing*) @ 0x004A96F0 and operator= @ 0x004A9760, IS_THING_ALIVE @ 0x004CC340 (`__fastcall`,
//   pointer in ecx), CInterfaceMap::LowerBound @ 0x0040F020 (the inlined GetInterface), CTCObjectAugmentations::FireCombatAugEffect
//   @ 0x00766A60 and the InStrikeAction clear @ 0x00765D40 (`mov byte [ecx+64h],0`; manifest "CXMVPlaybackInfo" is propagated),
//   CTCCombat::ResetComboChainPos @ 0x00733210 (manifest "CWADFile::Close" is propagated), _ftol @ 0x00BFEA70.
//   Data: CMainGameComponent::GetConstantFPS() @ 0x01375550 (long).
// Shape: the frame rate is copied into a local first (it is the fild operand of both time tests). The strike-event list test is
//   begin() != end() (`cmp [head],head`). The ended-event path calls IS_THING_ALIVE out of line; the no-strike path spells the
//   same test inline (GetPItem twice, Dead bit at +0x91), and VC7.1 tail-merges the two aug-effect clears. The combat
//   component pointer is not checked (miss path reads the uninitialised out word, which shares the LowerBound key's slot).
// Layout: engine/CCombatActionBase.h is quarantined (PDB build: PCameraPoint, list and vector sizes differ); retail offsets
//   follow the landed FrameUpdate 0x00859D80 model: FramesRunning +0x1c, list head +0xbc, interrupt/reset-combo times +0xc4/+0xc8,
//   movement-interrupt frame +0xcc, flag bools +0xd8..+0xdc, ActiveStrikeEventIndex +0xfc, LastWeaponUsedInStrike +0x100.
//   CThing: interface bitset +0x20, interface map +0x44, ThingTypeFlags +0x6c, Dead bit +0x91 (PDB - 0x10). CStrikeEvent +0x08 =
//   StrikeEventIndex (PDB).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCObjectAugmentations { public:                        // interface 0xbf
    void FireCombatAugEffect();                        // 0x00766A60
    void EndStrikeAction();                            // 0x00765D40 (`mov byte [ecx+64h],0`: InStrikeAction = false)
};
class CTCCombat { public:                                     // interface 0x30
    void ResetComboChainPos();                         // 0x00733210
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pData;
    virtual ~CBaseIntelligentPointer();                        // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<CThing> : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing>
    CIntelligentPointer(CThing* item);                 // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340 = IS_THING_ALIVE<CIntelligentPointer<CThing> > (Ego_r/FableWin ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z)

struct CStrikeEvent {
    bool  Legacy;                                      // +0x00
    bool  PrimaryLegacy;                               // +0x01
    bool  Weapon;                                      // +0x02
    void* HelperPointName;                             // +0x04
    long  StrikeEventIndex;                            // +0x08
};
struct CStrikeEventList {                              // std::list<CCountedPointer<CStrikeEvent> >
    struct SNode { SNode* next; SNode* prev; };
    SNode* head;
    bool HasEvents() const { return head->next != head; }
};

struct CAnimationOwner;
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    virtual void Validate() const;
    virtual void EditUpdate(float);
    virtual void PerformAction();
    virtual void FinishAction();
    virtual bool IsPlayingAnimation() const;
    virtual long GetAnimDelayTime() const;             // slot 6 (+0x18)
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual long GetAnimationTransitionOutTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsStrikeEvent(long) const;
    virtual bool IsStrikeEventActive() const;          // slot 14 (+0x38)
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0x1c - 0x10];
    long                    FramesRunning;             // +0x1c
    unsigned char           _pad20[0xa8 - 0x20];
    CAnimationOwner* GetPCreatureAnimationOwner() const;   // 0x00692950
};
class CCombatActionBase : public CCreatureActionBase { public:      // retail sizeof 0x114
    CBaseIntelligentPointer PTarget;                   // +0xa8
    CBaseIntelligentPointer PCameraPoint;              // +0xb0
    void*            PMeleeAbilityDef;                 // +0xb8
    CStrikeEventList StrikeEvents;                     // +0xbc (list head)
    long             LastStrikeEventEndTime;           // +0xc0
    float            StrikeEventInterruptTime;         // +0xc4
    float            StrikeEventResetComboTime;        // +0xc8
    long             MovementEventInterruptFrame;      // +0xcc
    float            StrikeSpeedVariationStartTime;    // +0xd0
    float            StrikeSpeedVariationEndTime;      // +0xd4
    bool             HasStrikeEventInterrupt;          // +0xd8
    bool             HasStrikeEventResetCombo;         // +0xd9
    bool             HasMovementEventInterrupt;        // +0xda
    bool             StrikeEventInterruptActive;       // +0xdb
    bool             MovementEventInterruptActive;     // +0xdc
    unsigned char    _paddd[0xfc - 0xdd];
    long             ActiveStrikeEventIndex;           // +0xfc
    CIntelligentPointer<CThing> LastWeaponUsedInStrike;        // +0x100
    unsigned char    _pad108[0x114 - 0x108];
    CStrikeEvent* GetFirstActiveStrikeEvent();         // 0x00856E50
    CThing* GetWeaponUsedInStrikeAction();             // 0x00857070
    void PerformPossibleDamageAction(float fraction);  // 0x008591E0
    void ProcessStrikeEvents();
};

void CCombatActionBase::ProcessStrikeEvents()
{
    long framesPerSecond = CMainGameComponent::GetConstantFPS();
    GetPCreatureAnimationOwner();
    if (StrikeEvents.HasEvents() && IsStrikeEventActive()) {
        PerformPossibleDamageAction(0.25f);
        CStrikeEvent* strikeEvent = GetFirstActiveStrikeEvent();
        if (strikeEvent) {
            if (strikeEvent->StrikeEventIndex != ActiveStrikeEventIndex) {
                ActiveStrikeEventIndex = strikeEvent->StrikeEventIndex;
                if (((CThing*)PCreature.GetPItem())->GetTypeFlags() & 0x20) {
                    CThing* weapon = GetWeaponUsedInStrikeAction();
                    if (weapon) {
                        LastWeaponUsedInStrike = weapon;
                        CTCObjectAugmentations* augmentations;
                        if (weapon->GetTC(TCI_OBJECT_AUGMENTATIONS, &augmentations))
                            augmentations->FireCombatAugEffect();
                    }
                }
            }
        } else if (ActiveStrikeEventIndex >= 0) {
            if (IS_THING_ALIVE(LastWeaponUsedInStrike)) {
                CTCObjectAugmentations* augmentations;
                if (((CThing*)LastWeaponUsedInStrike.GetPItem())->GetTC(TCI_OBJECT_AUGMENTATIONS, &augmentations))
                    augmentations->EndStrikeAction();
            }
            ActiveStrikeEventIndex = -1;
        }
    } else if (ActiveStrikeEventIndex >= 0) {
        if (((CThing*)LastWeaponUsedInStrike.GetPItem()) && !((CThing*)LastWeaponUsedInStrike.GetPItem())->IsDead()) {
            CTCObjectAugmentations* augmentations;
            if (((CThing*)LastWeaponUsedInStrike.GetPItem())->GetTC(TCI_OBJECT_AUGMENTATIONS, &augmentations))
                augmentations->EndStrikeAction();
        }
        ActiveStrikeEventIndex = -1;
    }
    if (HasStrikeEventInterrupt && !StrikeEventInterruptActive &&
        FramesRunning > (long)(GetAnimDelayTime() + framesPerSecond * StrikeEventInterruptTime))
        StrikeEventInterruptActive = true;
    if (HasStrikeEventResetCombo &&
        FramesRunning > (long)(GetAnimDelayTime() + framesPerSecond * StrikeEventResetComboTime)) {
        HasStrikeEventResetCombo = false;
        CTCCombat* combat;
        ((CThing*)PCreature.GetPItem())->GetTC(TCI_COMBAT, &combat);
        combat->ResetComboChainPos();
    }
    if (HasMovementEventInterrupt && !MovementEventInterruptActive && MovementEventInterruptFrame == FramesRunning)
        MovementEventInterruptActive = true;
}