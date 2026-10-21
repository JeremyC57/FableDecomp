// CCombatActionBase::CCombatActionBase(const CCombatActionBase& other) -- member-wise copy constructor: base action, target / camera-point intelligent pointers, the melee ability def (add-ref), strike-event list, strike timing fields and flags, combat animation vector, last weapon pointer and preferred movement vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062F3B0, 474 bytes; `ret 4`. Callees: CCreatureActionBase copy ctor @ 0x0062EBC0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90, list<CCountedPointer<CStrikeEvent>> copy ctor @ 0x0062F610
//          (manifest label "std_list_Initialize_Window" is propagated), vector<const CAnimationEntry*> copy ctor @ 0x0062F590.
// Vtables: CCombatActionBase 0x01259E44, CIntelligentPointer<CThing> 0x01238C6C, CIntelligentPointer<const CThing> 0x01237F1C.
// Layout: Ego_r.pdb CCombatActionBase (0x12C) with retail containers: PTarget sits at PDB 0xB4 - 0xC = +0xA8 (the
//         CCreatureActionBase base is 0xA8 in retail, the landed Clone rows 0x008C3D60 give sizeof 0x114), PCameraPoint
//         +0xB0, PMeleeAbilityDef +0xB8, StrikeEvents (STLport list: one sentinel pointer) +0xBC, then everything after
//         the list another 8 lower: LastStrikeEventEndTime +0xC0 .. HasResetMeleeCombatAbilityEffects +0xE0,
//         LastAnimStage +0xE4, HitThing/HitCreature +0xE8/+0xE9, FrameUpdatePauseFrames +0xEC, CombatAnimations
//         (12-byte STLport vector) +0xF0, ActiveStrikeEventIndex +0xFC, LastWeaponUsedInStrike +0x100,
//         PreferredMovementVector +0x108 (sizeof 0x114). No engine header exists for the class.
// Shape: the compiler's member-wise copy: base copy out of line then the vtable store; each intelligent pointer is
//        built by its base ctor, gets its own vtable and copies the pointee via GetPItem/SetPItem; the def pointer copy
//        add-refs a non-null def inline (`inc [eax+4]`); the list and vector copies are out-of-line calls.
// Leaf: ??0CCombatActionBase
#include <new>
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};

struct CThingPointer : CBaseIntelligentPointer {            // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer(const CThingPointer& other) { SetPItem(other.GetPItem()); }
};

struct CConstThingPointer : CBaseIntelligentPointer {       // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CConstThingPointer(const CConstThingPointer& other) { SetPItem(other.GetPItem()); }
};

struct CMeleeCombatAbilityDef {
    void* m_pVtable;
    long  m_refCount;                           // +0x04
};

struct CMeleeAbilityDefPointer {                // CDefPointer<const CMeleeCombatAbilityDef>
    CMeleeCombatAbilityDef* Data;
    CMeleeAbilityDefPointer(const CMeleeAbilityDefPointer& other) : Data(other.Data) { if (Data) ++Data->m_refCount; }
};

struct CStrikeEventList {                       // list<CCountedPointer<CCombatActionBase::CStrikeEvent>> (retail 4 bytes)
    void* _M_node;
    CStrikeEventList(const CStrikeEventList& other);    // 0x0062F610
};

struct CAnimationEntry;
struct CAnimationEntryVector {                  // vector<const CAnimationEntry*> (retail 12 bytes)
    const CAnimationEntry** _M_start;
    const CAnimationEntry** _M_finish;
    const CAnimationEntry** _M_end_of_storage;
    CAnimationEntryVector(const CAnimationEntryVector& other);   // 0x0062F590
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // declared memberwise copy: plain [esi+N] stores
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};

struct CCombatActionBase : CCreatureActionBase {             // vtable 0x01259E44, sizeof 0x114
    CThingPointer           PTarget;                         // +0xA8
    CConstThingPointer      PCameraPoint;                    // +0xB0
    CMeleeAbilityDefPointer PMeleeAbilityDef;                // +0xB8
    CStrikeEventList        StrikeEvents;                    // +0xBC
    long  LastStrikeEventEndTime;                            // +0xC0
    float StrikeEventInterruptTime;                          // +0xC4
    float StrikeEventResetComboTime;                         // +0xC8
    long  MovementEventInterruptFrame;                       // +0xCC
    float StrikeSpeedVariationStartTime;                     // +0xD0
    float StrikeSpeedVariationEndTime;                       // +0xD4
    bool  HasStrikeEventInterrupt;                           // +0xD8
    bool  HasStrikeEventResetCombo;                          // +0xD9
    bool  HasMovementEventInterrupt;                         // +0xDA
    bool  StrikeEventInterruptActive;                        // +0xDB
    bool  MovementEventInterruptActive;                      // +0xDC
    bool  PreferredFinalPosSet;                              // +0xDD
    bool  HasPerformAction;                                  // +0xDE
    bool  SentMissedEvent;                                   // +0xDF
    bool  HasResetMeleeCombatAbilityEffects;                 // +0xE0
    long  LastAnimStage;                                     // +0xE4
    bool  HitThing;                                          // +0xE8
    bool  HitCreature;                                       // +0xE9
    long  FrameUpdatePauseFrames;                            // +0xEC
    CAnimationEntryVector CombatAnimations;                  // +0xF0
    long  ActiveStrikeEventIndex;                            // +0xFC
    CThingPointer LastWeaponUsedInStrike;                    // +0x100
    C3DVector PreferredMovementVector;                       // +0x108
    virtual void v00();
};

// Emits the implicit copy constructor (the retail one is reached from every Clone()).
CCombatActionBase* CopyCombatAction(void* where, const CCombatActionBase& other);
CCombatActionBase* CopyCombatAction(void* where, const CCombatActionBase& other)
{
    return new (where) CCombatActionBase(other);
}
