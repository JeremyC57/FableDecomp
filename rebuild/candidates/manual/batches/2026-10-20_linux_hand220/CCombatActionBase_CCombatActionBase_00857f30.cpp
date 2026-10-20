// CCombatActionBase::CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound) -- the melee/combat action base for a targeted action: creature-action base (owner, sounds, priority 100, action code), null target/camera pointers, empty strike-event list, no animations, strike index -1; then the target is assigned and the strike state initialised
// Leaf: ??0CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00857F30, 254 bytes (`ret 10h`). Own vtable 0x01259E44.
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long) @ 0x00693C70,
//          CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
//          malloc @ 0x00BFEA0E (the STLport list header node), the strike-state initialiser @ 0x008561C0.
// Vtables: CIntelligentPointer<CThing> 0x01238C6C, CIntelligentPointer<const CThing> 0x01237F1C.
// Layout (retail 0x114 bytes; Ego_r.pdb names, STLport list = 4 bytes and vector = 12): PTarget +0xa8, PCameraPoint +0xb0,
//          PMeleeAbilityDef +0xb8, StrikeEvents +0xbc, SentMissedEvent +0xdf, HasResetMeleeCombatAbilityEffects +0xe0,
//          CombatAnimations +0xf0, ActiveStrikeEventIndex +0xfc, LastWeaponUsedInStrike +0x100.
// Shape: the landed sibling constructor CCombatActionBase(CThingCreatureBase*) @ 0x00857E40 verbatim apart from the base
//          call and the PTarget temporary (built, read through GetPItem, destroyed); the base arguments are pushed right to
//          left (action code, 100, the sound pair reference, the owner). Callers: the derived combat actions (e.g. the landed
//          CCreatureAction_BanditKingStuck 0x008C94D0 -> 0x00857F30; 0x00858030 from the action-code-50/254 actions).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CThing;
struct CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    ~CBaseIntelligentPointer();             // 0x00A01C10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CConstIntelligentPointer : CBaseIntelligentPointer {    // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CConstIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CDefPointer {                        // CDefPointer<const CMeleeCombatAbilityDef>
    void* Object;
    CDefPointer() : Object(0) { }
};

struct CStrikeEventNode {                   // STLport list node of CCountedPointer<CStrikeEvent>
    CStrikeEventNode* _M_next;
    CStrikeEventNode* _M_prev;
    void* m_pObject;
    void* m_pCount;
};
struct CStrikeEventList {                   // STLport list: the header-node pointer
    CStrikeEventNode* _M_node;
    CStrikeEventList() : _M_node(0)
    {
        CStrikeEventNode* node = (CStrikeEventNode*)malloc(sizeof(CStrikeEventNode));   // 0x00BFEA0E (__stl_new)
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

struct CEndOfStorage {                      // STLport _STLP_alloc_proxy (allocator + end-of-storage word)
    void* _M_data;
    CEndOfStorage(void* data) : _M_data(data) { }
};
struct CAnimationVector {                   // STLport vector<const CAnimationEntry*>
    void* _M_start;
    void* _M_finish;
    CEndOfStorage _M_end_of_storage;
    CAnimationVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CSoundPair {                         // two sound ids
    long FirstSound;
    long LastSound;
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long interruptionGroup, long interruptionPriority);   // 0x00693C70
};

struct CCombatActionBase : CCreatureActionBase {   // vtable 0x01259E44
    CIntelligentPointer PTarget;                    // +0xa8
    CConstIntelligentPointer PCameraPoint;          // +0xb0
    CDefPointer PMeleeAbilityDef;                   // +0xb8
    CStrikeEventList StrikeEvents;                  // +0xbc
    unsigned char _pad_0xc0[0xdf - 0xc0];
    bool SentMissedEvent;                           // +0xdf
    bool HasResetMeleeCombatAbilityEffects;         // +0xe0
    unsigned char _pad_0xe1[0xf0 - 0xe1];
    CAnimationVector CombatAnimations;              // +0xf0
    long ActiveStrikeEventIndex;                    // +0xfc
    CIntelligentPointer LastWeaponUsedInStrike;     // +0x100
    unsigned char _pad_0x108[0x114 - 0x108];        // +0x108 PreferredMovementVector (not initialised)
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);
    void InitialiseStrikeState();                   // 0x008561C0
};
CCombatActionBase::CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    : CCreatureActionBase(owner, sound, 100, actionCode)
    , SentMissedEvent(false)
    , HasResetMeleeCombatAbilityEffects(false)
    , ActiveStrikeEventIndex(-1)
    , LastWeaponUsedInStrike(0)
{
    PTarget = CIntelligentPointer(target);
    InitialiseStrikeState();
}
