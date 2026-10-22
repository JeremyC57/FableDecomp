// Grade: DIFFER(862v861) -- shape aligned; remaining: retail reuses the def pointer's frame word for the jittered proximity (the def local ends its life after the runnable check, its value lives in ebp), keeps the move result test as a jump into the shared runnable/result check, and loads the def fields in a different order (4 tries)
// CAIStateGroup_MazeShieldedChase::Update() -- within 30 units of the target: chase it to a randomly jittered proximity when farther than the def's chase distance, look at it, then enflame the will (close) or cast will lightning (far), and answer a knocked-down or close target with a drain response; otherwise do nothing; terminate when the state stops being runnable or an action fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009552C0, length 861 bytes (+ int3 padding), plain `ret`.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float GetXYLength() const { return (float)sqrt(X * X + Y * Y); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct CAICreatureWillPowerIndicatorDef {
    virtual void v00();
    virtual void DeleteThis();
    long RefCount;                              // +0x04
    unsigned char _pad08[0x38];
    float SpellParameter;                       // +0x40
    unsigned char _pad44[4];
    float ChaseDistance;                        // +0x48
    float SpellRange;                           // +0x4c
};
struct CDefClassBase {
    void GetSubDef(CAICreatureWillPowerIndicatorDef** out);   // 0x008C3510
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CThing {
    unsigned char _pad0[0x6c];
    unsigned char Flags6c;                      // +0x6c
    unsigned char _pad6d[3];
    CDefClassBase* PDef;                        // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CWorld {
    unsigned long& GetRandomSeed();             // 0x0049D810
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00();
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CIntelligentPointerConstThingCopy {
    void* vptr;
    const CThing* m_pItem;
    CIntelligentPointerConstThingCopy(const CBaseIntelligentPointer& other);   // 0x007C2930
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x108];
    ~CCreatureActionBase();                     // 0x0062E900
};
struct CCombatAction_WillEnflameInto : CCreatureActionBase {
    CCombatAction_WillEnflameInto(CThingCreatureBase* creature, long count);   // 0x009567F0
};
struct CCreatureAction_CastWillLightningSpell : CCreatureActionBase {
    CCreatureAction_CastWillLightningSpell(CThingCreatureBase* creature, long count, float parameter, CThing* target);   // 0x008BCA00
};
struct CCombatAction_GenericResponseToDrain : CCreatureActionBase {
    CCombatAction_GenericResponseToDrain(CThingCreatureBase* creature, CThing* source);   // 0x00906B00
};
struct CCombatAction_GenericResponseToDrainClose : CCreatureActionBase {
    CCombatAction_GenericResponseToDrainClose(CThingCreatureBase* creature, CThing* source);   // 0x00906AC0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float proximity,
                                       bool* arrived, long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThingCopy thing, long flags, float tolerance);   // 0x0097D6F0
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_MazeBase : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    bool HasEnemyBeenKnockedDown();             // 0x00954130
};
struct CAIStateGroup_MazeShieldedChase : CAIStateGroup_MazeBase {
    bool IsStillRunnable();                     // 0x00954740
    void Update();
};

void CAIStateGroup_MazeShieldedChase::Update()
{
    CAICreatureWillPowerIndicatorDef* def = 0;
    GetCreature()->PDef->GetSubDef(&def);
    if (!IsStillRunnable()) {
        Terminate();
        if (def && --def->RefCount == 0)
            def->DeleteThis();
        return;
    }
    CAICreatureWillPowerIndicatorDef* d = def;
    float spellRange = d->SpellRange;
    float chaseDistance = d->ChaseDistance;
    float proximity;
    if (WorldRandomRange(GetPWorld()->GetRandomSeed(), 100) > 50)
        proximity = d->SpellRange + 0.5f;
    else
        proximity = d->SpellRange - 1.0f;
    float distance = (GetCreature()->GetPosition() - PTarget.GetPItem()->GetPosition()).GetLength();
    if (distance < 30.0f) {
        long result;
        if (distance > chaseDistance) {
            bool arrived;
            result = AIAction_MoveToThingProximity(1, PTarget, proximity, &arrived, 5, true, false, false, false, 0.0f);
            if (!IsStillRunnable() || result != 2) {
                Terminate();
                if (--d->RefCount == 0)
                    d->DeleteThis();
                return;
            }
        }
        if (!IsStillRunnable()) {
            Terminate();
            if (--d->RefCount == 0)
                d->DeleteThis();
            return;
        }
        AIAction_LookAtThing(1, PTarget, 0x21, 1.0f / 12.0f);
        if (!IsStillRunnable()) {
            Terminate();
            if (--d->RefCount == 0)
                d->DeleteThis();
            return;
        }
        if ((GetCreature()->GetPosition() - PTarget.GetPItem()->GetPosition()).GetLength() < spellRange)
            result = AIAction_PerformActionAndWait(&CCombatAction_WillEnflameInto(GetCreature(), 3), false);
        else
            result = AIAction_PerformActionAndWait(&CCreatureAction_CastWillLightningSpell(GetCreature(), 3, d->SpellParameter, PTarget.GetPItem()), false);
        if (!IsStillRunnable() || result != 2) {
            Terminate();
            if (--d->RefCount == 0)
                d->DeleteThis();
            return;
        }
        if (PTarget.GetPItem()->Flags6c & 8) {
            CThing* target = PTarget.GetPItem();
            if (HasEnemyBeenKnockedDown())
                AIAction_PerformActionAndWait(&CCombatAction_GenericResponseToDrain(GetCreature(), target), false);
            else if ((GetCreature()->GetPosition() - PTarget.GetPItem()->GetPosition()).GetXYLength() < chaseDistance)
                AIAction_PerformActionAndWait(&CCombatAction_GenericResponseToDrainClose(GetCreature(), target), false);
        }
    } else {
        AIAction_DoNothing();
    }
    if (--d->RefCount == 0)
        d->DeleteThis();
}
