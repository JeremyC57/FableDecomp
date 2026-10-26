// CCombatActionBase::~CCombatActionBase() -- member-by-member destruction of the combat action (last-weapon pointer, animation vector, strike-event list, melee ability def, camera point, target), then ~CCreatureActionBase
// Leaf: ??1CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062E900, length 123 bytes (tail `jmp` into the base dtor). Catalog label
// "CTCLightningOrb::~CTCLightningOrb" is propagated: the body works on the CCombatActionBase members (Ego_r.pdb) and is called by
// the combat-action deleting dtors (landed 0x008C9690, 0x008CED90, 0x008CEDD0) and by the restart temporaries of the landed
// FinishAction rows (e.g. 0x008C5370), which already name it the action destructor.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x3), free @ 0x00BFEA14 (vector storage, list sentinel),
// the StrikeEvents list clear @ 0x0062E2B0 (out of line: walks the nodes releasing CCountedPointer<CStrikeEvent>), the def's
// slot-1 release, CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (tail jump).
// Layout (retail CCreatureActionBase is 0xA8, PDB 0xB4; retail STLport list is 4 bytes, PDB 12): PTarget +0xA8, PCameraPoint +0xB0,
// PMeleeAbilityDef +0xB8, StrikeEvents +0xBC, LastStrikeEventEndTime..FrameUpdatePauseFrames +0xC0..+0xEC, CombatAnimations +0xF0,
// ActiveStrikeEventIndex +0xFC, LastWeaponUsedInStrike +0x100, PreferredMovementVector +0x108 (sizeof 0x114).
// Shape: no vtable store, so CCombatActionBase is `__declspec(novtable)`; empty body, members destroyed in reverse order; the
// CDefPointer dtor clears Object inside its null test (landed ~CTCLight 0x00640F80 form).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                             // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer { virtual void v00(); };   // CIntelligentPointer<CThing>
struct CIntelligentPointerConstThing : CBaseIntelligentPointer { virtual void v00(); };   // CIntelligentPointer<const CThing>
class CDefPointeeBase { public:
    virtual void Destroy();
    virtual void Delete();                                  // slot 1
    long m_refCount;                                        // +0x04
};
template<class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};
struct SStrikeEventNode;
struct CStrikeEventList {                                   // STLport list<CCountedPointer<CStrikeEvent>>: sentinel pointer only
    SStrikeEventNode* Node;
    void clear();                                           // 0x0062E2B0
    ~CStrikeEventList()
    {
        clear();
        if (Node)
            free(Node);
    }
};
class CAnimationEntry;
template<class T> struct CPtrVector {                       // STLport vector<T*> (12 bytes, malloc allocator)
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    ~CPtrVector() { if (_M_start) free(_M_start); }
};
class C3DVector { public: float X, Y, Z; };
class CMeleeCombatAbilityDef;
class CCreatureActionBase { public:                                // retail sizeof 0xA8
    virtual ~CCreatureActionBase();                         // 0x00693EF0
    unsigned char _body[0xa8 - 4];
};
struct __declspec(novtable) CCombatActionBase : CCreatureActionBase {
    CIntelligentPointerThing      PTarget;                  // +0xa8
    CIntelligentPointerConstThing PCameraPoint;             // +0xb0
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef;   // +0xb8
    CStrikeEventList StrikeEvents;                          // +0xbc
    long  LastStrikeEventEndTime;                           // +0xc0
    float StrikeEventInterruptTime;                         // +0xc4
    float StrikeEventResetComboTime;                        // +0xc8
    long  MovementEventInterruptFrame;                      // +0xcc
    float StrikeSpeedVariationStartTime;                    // +0xd0
    float StrikeSpeedVariationEndTime;                      // +0xd4
    bool  HasStrikeEventInterrupt;                          // +0xd8
    bool  HasStrikeEventResetCombo;
    bool  HasMovementEventInterrupt;
    bool  StrikeEventInterruptActive;
    bool  MovementEventInterruptActive;
    bool  PreferredFinalPosSet;
    bool  HasPerformAction;
    bool  SentMissedEvent;
    bool  HasResetMeleeCombatAbilityEffects;                // +0xe0
    long  LastAnimStage;                                    // +0xe4
    bool  HitThing;                                         // +0xe8
    bool  HitCreature;                                      // +0xe9
    long  FrameUpdatePauseFrames;                           // +0xec
    CPtrVector<const CAnimationEntry> CombatAnimations;     // +0xf0
    long  ActiveStrikeEventIndex;                           // +0xfc
    CIntelligentPointerThing LastWeaponUsedInStrike;        // +0x100
    C3DVector PreferredMovementVector;                      // +0x108
    virtual ~CCombatActionBase();
};

CCombatActionBase::~CCombatActionBase()
{
}