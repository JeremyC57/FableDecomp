// Grade: DIFFER(601v612) -- retail holds the CIntelligentPointer vptr constant in ebp (push ebp, frame 0x58, output at +0x20) and ORs the type flag in memory; ours keeps it immediate (frame 0x54), this/zero registers swapped
// CAIStateGroup_JackOfBladesStage1::MeleeCombat() -- mark Jack ready to attack when he can, then each tick (target and arena centre alive, not stopping) issue an attack-and-look-at-target brain output and yield, until the target leaves combat range or the hits have caused a rush; terminates when the preconditions fail at the top of a tick; always returns 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972CC0, length 612 bytes (+ int3 padding).
// Callees: CThingCreatureBase::CanBeReadyToAttack @ 0x00662410, GetPWorld @ 0x008FCFC0, CCombatManager::
// SetAttackerAsReadyToAttack @ 0x006EA450, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem
// @ 0x00A01B90 / dtor @ 0x00A01C10, IsStopping @ 0x008FD010, CAIBrain::SetBrainOutput @ 0x0088B8F0, the brain's virtual
// slot 2 (+8, yield), CThing::GetPosition @ 0x004C73D0, operator delete @ 0x00BFE9BC, CThingCreatureBase::GetCombatTypeDef
// @ 0x00666D10, HasHitsCausedRush @ 0x00972340, Terminate @ 0x008FCF40.
// Layout: CBrainOutput per the landed 0x0097E890 model (Types +0, LookFlags +4, NavigationInput +8, PAction +0x20, MoveToPos
// +0x28, LookAtPos +0x34, LookAtThing +0x40); Ego_r.pdb CAIStateGroup_JackOfBladesBase PArenaCentre +0x1c, PTargetThing +0x24.
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CCombatTypeDef {
    unsigned char _pad0[0x70];
    float Range;                                // +0x70
};
struct CThingCreatureBase : CThing {
    bool CanBeReadyToAttack();                  // 0x00662410
    const CCombatTypeDef* GetCombatTypeDef();   // 0x00666D10
};
struct CBaseIntelligentPointer {
    void* vftable;
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
extern void* const CIntelligentPointer_CThing_vftable;   // 0x01237F1C
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing() { *(void* const**)this = &CIntelligentPointer_CThing_vftable; }
    CIntelligentPointerThing(CThing* item) { *(void* const**)this = &CIntelligentPointer_CThing_vftable; SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& o) { SetPItem(o.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    CCountedPointer() : Object(0), Block(0) { }
    void Release()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
    ~CCountedPointer() { Release(); }
};
struct CNavigationInput {
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long  MovementType;                         // +0x10
    float Speed;                                // +0x14
    CNavigationInput() : Proximity(0.0f), MovementType(0), Speed(0.0f) { }
};
struct __declspec(align(8)) CBrainOutput {
    unsigned long           Types;              // +0x00
    long                    LookFlags;          // +0x04
    CNavigationInput        NavigationInput;    // +0x08
    CCountedPointer         PAction;            // +0x20
    C3DVector               MoveToPos;          // +0x28
    C3DVector               LookAtPos;          // +0x34
    CIntelligentPointerThing LookAtThing;       // +0x40
    CBrainOutput() { Types = 0; LookFlags = 0; PAction.Release(); }
    void SetAsLookAtThing(CThing* thing, long flags)
    {
        Types |= 4;
        LookAtThing = CIntelligentPointerThing(thing);
        LookFlags = flags;
    }
};
struct CCombatManager {
    void SetAttackerAsReadyToAttack(CThingCreatureBase* creature, bool ready);   // 0x006EA450
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
};
struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x14];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_JackOfBladesStage1 : CAIStateGroupBase {
    CIntelligentPointerThing PSwordEmitter;     // +0x14
    CIntelligentPointerThing PArenaCentre;      // +0x1c
    CIntelligentPointerThing PTargetThing;      // +0x24
    bool HasHitsCausedRush();                   // 0x00972340
    long MeleeCombat();
};

long CAIStateGroup_JackOfBladesStage1::MeleeCombat()
{
    if (GetCreature()->CanBeReadyToAttack()) {
        CThingCreatureBase* creature = GetCreature();
        CWorld* world = GetPWorld();
        world->CombatManager->SetAttackerAsReadyToAttack(creature, true);
    }
    for (;;) {
        if (!IS_THING_ALIVE(PTargetThing) || IsStopping() || !IS_THING_ALIVE(PArenaCentre)) {
            Terminate();
            return 2;
        }
        float distance;
        {
            CBrainOutput output;
            output.Types |= 0x20;
            output.SetAsLookAtThing(PTargetThing.GetPItem(), 0x11);
            PBrain->SetBrainOutput(output);
            PBrain->Yield();
            const C3DVector& me = GetCreature()->GetPosition();
            const C3DVector& them = PTargetThing.GetPItem()->GetPosition();
            float dx = them.X - me.X;
            float dy = them.Y - me.Y;
            distance = sqrt(dy * dy + dx * dx);
        }
        if (!IS_THING_ALIVE(PTargetThing) || IsStopping() || !IS_THING_ALIVE(PArenaCentre))
            return 2;
        if (!(distance < GetCreature()->GetCombatTypeDef()->Range))
            return 2;
        if (HasHitsCausedRush())
            return 2;
    }
}
