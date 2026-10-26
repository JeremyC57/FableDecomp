// CTCCreatureGenerator::UpdateCreatureTrigger(long frame) -- self-triggering: (re)arms the activation frame while waiting for the last batch to die, gives up when no triggers are left, then walks the triggerer creatures; a creature that enters the self-trigger radius (2D) fires a generation if the reset interval has passed since the last activation, and an external trigger fires on the interval alone; firing stamps the activation frame, spends one trigger, creates the creatures and latches ExternalTrigger to UndeadTriggered
// Leaf: UpdateCreatureTrigger
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074BAA0, 384 bytes (`ret 4`).
// Callees: CIntelligentPointer<CThingCreatureBase>::GetPItem() @ 0x00A01B50, CThing::GetPosition() @ 0x004C73D0,
// DivideByGameSpeed(float) @ 0x00415FD3 (float / frames-per-second), _ftol @ 0x00BFEA70,
// CTCCreatureGenerator::CreateCreatures(CThingCreatureBase*) @ 0x0074B2B0.
// Layout: local model as the landed OnCreate 0x00749EA0 (engine/CTCCreatureGenerator.h is the Ego_r.pdb layout, 0x10 high
// from GenParams: retail CArray 0xc, STLport list one word): CreatedCreatures (list node) +0x18, CreatureTriggerers +0x1c,
// GenParams +0x28 (SelfTriggerRadius +0x2c, SelfTriggerResetInterval +0x34), Active +0x4c, ExternalTrigger +0x4f,
// WaitForAllCreaturesDead +0x50, NumTriggersLeft +0x54, ActivationFrame +0x58, LastSpaceTest +0x5c, UndeadTriggered +0x88.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class CThing { public:
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
class CThingCreatureBase : public CThing { public:
};
struct CIntelligentPointer_Creature {                       // CIntelligentPointer<CThingCreatureBase>: 8 bytes
    void* Data[2];
    CThingCreatureBase* GetPItem() const;                   // 0x00A01B50
};
struct CListNode {
    CListNode* Next;
    CListNode* Prev;
};
float __stdcall DivideByGameSpeed(float f);                 // 0x00415FD3

class CTCCreatureGenerator { public:
    void*                          __vftable;              // +0x00
    CThing*                        ParentThing;            // +0x04
    unsigned char                  _pad08[0x10];           // +0x08
    CListNode*                     CreatedCreatures;       // +0x18 list<CIntelligentPointer<CThing> > node
    CIntelligentPointer_Creature*  TriggerersStart;        // +0x1c vector<CIntelligentPointer<CThingCreatureBase> >
    CIntelligentPointer_Creature*  TriggerersFinish;       // +0x20
    unsigned char                  _pad24[0x8];            // +0x24
    float                          SelfTriggerRadius;      // +0x2c GenParams
    unsigned char                  _pad30[0x4];            // +0x30
    long                           SelfTriggerResetInterval; // +0x34
    unsigned char                  _pad38[0x14];           // +0x38
    bool                           Active;                 // +0x4c
    bool                           Initialised;            // +0x4d
    bool                           ReadyToInitialise;      // +0x4e
    bool                           ExternalTrigger;        // +0x4f
    bool                           WaitForAllCreaturesDead;// +0x50
    unsigned char                  _pad51[0x3];            // +0x51
    long                           NumTriggersLeft;        // +0x54
    long                           ActivationFrame;        // +0x58
    bool                           LastSpaceTest;          // +0x5c
    unsigned char                  _pad5d[0x2b];           // +0x5d
    bool                           UndeadTriggered;        // +0x88
    void CreateCreatures(CThingCreatureBase* creature);    // 0x0074B2B0
    void UpdateCreatureTrigger(long frame);
};

void CTCCreatureGenerator::UpdateCreatureTrigger(long frame)
{
    if (WaitForAllCreaturesDead && !UndeadTriggered && CreatedCreatures->Next != CreatedCreatures)
        ActivationFrame = frame;
    if (!UndeadTriggered) {
        if (WaitForAllCreaturesDead && CreatedCreatures->Next != CreatedCreatures)
            return;
        if (NumTriggersLeft == 0)
            return;
    }
    for (CIntelligentPointer_Creature* it = TriggerersStart; it != TriggerersFinish; ++it) {
        CThingCreatureBase* creature = it->GetPItem();
        bool spaceTrigger = true;
        bool timeTrigger = true;
        C3DVector creaturePos(creature->GetPosition());
        C3DVector myPos(ParentThing->GetPosition());
        if ((long)DivideByGameSpeed((float)(frame - ActivationFrame)) < SelfTriggerResetInterval)
            timeTrigger = false;
        if (!UndeadTriggered) {
            float dx = myPos.X - creaturePos.X;
            float dy = myPos.Y - creaturePos.Y;
            float r = SelfTriggerRadius;
            if (dx * dx + dy * dy < r * r) {
                if (LastSpaceTest)
                    spaceTrigger = false;
                ActivationFrame = frame;
                LastSpaceTest = true;
            } else {
                LastSpaceTest = false;
                spaceTrigger = false;
            }
        }
        if ((spaceTrigger && timeTrigger && Active) || (ExternalTrigger && timeTrigger)) {
            ActivationFrame = frame;
            if (NumTriggersLeft > 0)
                NumTriggersLeft--;
            CreateCreatures(creature);
            ExternalTrigger = UndeadTriggered;
            return;
        }
    }
}