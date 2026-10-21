// CAIStateGroup_MinionWorkInInn::Update() -- with a workplace: one time in five (Random(100) < 21) clean a bench; otherwise walk to a random point 0..6m from the workplace position (random heading), retrying while the move is in progress, and idle on arrival; terminate otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090BED0, length 400 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the creature's workplace pointer at +0x170),
//          CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CleanBench @ 0x0090BD40, CThing::GetPosition
//          @ 0x004C73D0, GFFloatRandom(float, unsigned long*) @ 0x004978A0 (__fastcall: the seed in ecx),
//          C3DVector::RotateAboutZ @ 0x00A14260, CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (C3DVector by
//          value), AIAction_MinionIdle @ 0x0097FA00, Terminate @ 0x008FCF40.
// Globals: 0.0001f @ 0x0129BA3C.
// Shape: the *seed-form Random; the workplace position is a user-copy-ctor copy, the offset a (r + 0.0001, 0, 0) vector
//          rotated in place, the target `offset + position` built by a three-float ctor (Z first); a do/while re-tests
//          the workplace before each try (the sibling of CAIStateGroup_IdleAtHome::Update 0x00917380).
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04); the creature's
//          workplace intelligent pointer at +0x170 as in the landed TavernServer rows.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    void RotateAboutZ(float angle, bool flag);  // 0x00A14260
};
struct CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};
struct CThingAICreature {
    unsigned char _pad[0x170];
    CBaseIntelligentPointer PWorkplace;         // +0x170 (descriptive)
    CThing* GetPWorkplace() { return PWorkplace.GetPItem(); }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    long AIAction_MinionIdle();                 // 0x0097FA00
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MinionWorkInInn : CAIStateGroupBase {
    void CleanBench();                          // 0x0090BD40
    void Update();
};

void CAIStateGroup_MinionWorkInInn::Update()
{
    if (GetCreature()->GetPWorkplace() == 0) {
        Terminate();
        return;
    }
    if (Random(100) < 21) {
        CleanBench();
        return;
    }
    C3DVector workplace = GetCreature()->GetPWorkplace()->GetPosition();
    C3DVector offset(GFFloatRandom(6.0f, GetPWorldSeed()) + 0.0001f, 0.0f, 0.0f);
    offset.RotateAboutZ(GFFloatRandom(1.0f, GetPWorldSeed()), false);
    C3DVector target = offset + workplace;
    long result;
    do {
        if (GetCreature()->GetPWorkplace() == 0) {
            Terminate();
            return;
        }
        bool reached;
        result = AIAction_MoveToPosProximity(2, target, 1.0f, &reached, 3, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 2 && GetCreature()->GetPWorkplace() != 0) {
        AIAction_MinionIdle();
        return;
    }
    Terminate();
}
