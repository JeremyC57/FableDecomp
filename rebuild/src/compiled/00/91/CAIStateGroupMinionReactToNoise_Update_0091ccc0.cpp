// CAIStateGroup_MinionReactToNoise::Update() -- wait (yielding) until the creature has finished its current action,
// turn to look at the noise, then walk to within 6 units of it; on arrival clear the investigate/found flags.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091CCC0, length 183 bytes.
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, CAIStateGroupBase::IsStopping @ 0x008FD010, the
// brain's virtual slot 2 (+0x08, the entity-script yield), AIAction_LookAtPos @ 0x0097D580 (repeat 1, by-value
// C3DVector, flags 0x23, speed 0.05), AIAction_MoveToPosProximity @ 0x0097E890 (repeat 2, 6.0, &reached, movement 3,
// true, false, true, 0, true, 0.0) repeated while it returns 4, the finish call @ 0x008FCF40.
// Shape: the rotated `while (IsPerformingAnyAction()) { if (IsStopping()) break-to-finish; yield; }` loop, the
// landed MoveToCuriosityProximity do/while, by-value C3DVector copied memberwise into the argument block.
// Engine header names (NoisePosition +0x14, InvestigatingNoise +0x20, FoundNoiseMaker +0x21); the header flattens
// the base, so the class is modelled locally.
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CThingCreatureBase {
    bool IsPerformingAnyAction();               // 0x006622A0
};

struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad[0x14];                   // +0x04..+0x17
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    bool IsStopping();                          // 0x008FD010
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    void FinishUpdate();                        // 0x008FCF40
};

struct CAIStateGroup_MinionReactToNoise : CAIStateGroupBase {
    C3DVector NoisePosition;                    // +0x14
    bool InvestigatingNoise;                    // +0x20
    bool FoundNoiseMaker;                       // +0x21
    long FrameOfLastInteraction;                // +0x24
    void Update();
};

void CAIStateGroup_MinionReactToNoise::Update()
{
    while (PBrain->PCreature->IsPerformingAnyAction()) {
        if (IsStopping()) {
            FinishUpdate();
            return;
        }
        PBrain->Yield();
    }
    if (AIAction_LookAtPos(1, NoisePosition, 0x23, 0.05f) == 2) {
        bool reached;
        long result;
        do {
            result = AIAction_MoveToPosProximity(2, NoisePosition, 6.0f, &reached, 3, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (result == 2) {
            InvestigatingNoise = false;
            FoundNoiseMaker = false;
        }
    }
    FinishUpdate();
}