// CCombatActionBase::ProcessStrikeEvents() -- run the active strike event (damage, weapon aug effect), clear it when it ends, then arm the interrupt / reset-combo / movement-interrupt flags
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
//   Data: g_FramesPerSecond @ 0x01375550 (long).
// Shape: the frame rate is copied into a local first (it is the fild operand of both time tests). The strike-event list test is
//   begin() != end() (`cmp [head],head`). The ended-event path calls IS_THING_ALIVE out of line; the no-strike path spells the
//   same test inline (GetPItem twice, Dead bit at +0x91), and VC7.1 tail-merges the two aug-effect clears. The combat
//   component pointer is not checked (miss path reads the uninitialised out word, which shares the LowerBound key's slot).
// Layout: engine/CCombatActionBase.h is quarantined (PDB build: PCameraPoint, list and vector sizes differ); retail offsets
//   follow the landed FrameUpdate 0x00859D80 model: FramesRunning +0x1c, list head +0xbc, interrupt/reset-combo times +0xc4/+0xc8,
//   movement-interrupt frame +0xcc, flag bools +0xd8..+0xdc, ActiveStrikeEventIndex +0xfc, LastWeaponUsedInStrike +0x100.
//   CThing: interface bitset +0x20, interface map +0x44, ThingTypeFlags +0x6c, Dead bit +0x91 (PDB - 0x10). CStrikeEvent +0x08 =
//   StrikeEventIndex (PDB).
extern long g_FramesPerSecond;                         // 0x01375550

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);              // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCObjectAugmentations { public:                        // interface 0xbf
    void FireCombatAugEffect();                        // 0x00766A60
    void EndStrikeAction();                            // 0x00765D40 (`mov byte [ecx+64h],0`: InStrikeAction = false)
};
class CTCCombat { public:                                     // interface 0x30
    void ResetComboChainPos();                         // 0x00733210
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                      // +0x6c (PDB +0x7c)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                            // +0x91 bit 0 (PDB +0xa1)
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pData;
    virtual ~CBaseIntelligentPointer();                        // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CIntelligentPointer(CThing* item);                 // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer& thing);   // 0x004CC340

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
    CIntelligentPointer LastWeaponUsedInStrike;        // +0x100
    unsigned char    _pad108[0x114 - 0x108];
    CStrikeEvent* GetFirstActiveStrikeEvent();         // 0x00856E50
    CThing* GetWeaponUsedInStrikeAction();             // 0x00857070
    void PerformPossibleDamageAction(float fraction);  // 0x008591E0
    void ProcessStrikeEvents();
};

void CCombatActionBase::ProcessStrikeEvents()
{
    long framesPerSecond = g_FramesPerSecond;
    GetPCreatureAnimationOwner();
    if (StrikeEvents.HasEvents() && IsStrikeEventActive()) {
        PerformPossibleDamageAction(0.25f);
        CStrikeEvent* strikeEvent = GetFirstActiveStrikeEvent();
        if (strikeEvent) {
            if (strikeEvent->StrikeEventIndex != ActiveStrikeEventIndex) {
                ActiveStrikeEventIndex = strikeEvent->StrikeEventIndex;
                if (((CThing*)PCreature.GetPItem())->ThingTypeFlags & 0x20) {
                    CThing* weapon = GetWeaponUsedInStrikeAction();
                    if (weapon) {
                        LastWeaponUsedInStrike = weapon;
                        CTCObjectAugmentations* augmentations;
                        if (weapon->GetInterface(&augmentations, 0xbf))
                            augmentations->FireCombatAugEffect();
                    }
                }
            }
        } else if (ActiveStrikeEventIndex >= 0) {
            if (IS_THING_ALIVE(LastWeaponUsedInStrike)) {
                CTCObjectAugmentations* augmentations;
                if (((CThing*)LastWeaponUsedInStrike.GetPItem())->GetInterface(&augmentations, 0xbf))
                    augmentations->EndStrikeAction();
            }
            ActiveStrikeEventIndex = -1;
        }
    } else if (ActiveStrikeEventIndex >= 0) {
        if (((CThing*)LastWeaponUsedInStrike.GetPItem()) && !((CThing*)LastWeaponUsedInStrike.GetPItem())->Dead) {
            CTCObjectAugmentations* augmentations;
            if (((CThing*)LastWeaponUsedInStrike.GetPItem())->GetInterface(&augmentations, 0xbf))
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
        ((CThing*)PCreature.GetPItem())->GetInterface(&combat, 0x30);
        combat->ResetComboChainPos();
    }
    if (HasMovementEventInterrupt && !MovementEventInterruptActive && MovementEventInterruptFrame == FramesRunning)
        MovementEventInterruptActive = true;
}