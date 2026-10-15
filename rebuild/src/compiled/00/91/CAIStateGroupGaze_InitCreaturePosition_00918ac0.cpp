// CAIStateGroup_Gaze::InitCreaturePosition() -- when runnable, with a 30% chance places the creature near a free gaze marker (within 1 unit); returns whether it was placed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00918AC0, length 129 bytes (+ int3 padding).
// Callees: `this`'s virtual slot 11 (+0x2c), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890
// (__fastcall), CAIStateGroup_Gaze::FindFreeGazePos @ 0x00918880, CThing::GetPosition @ 0x004C73D0,
// CThingCreatureBase::AutoPlaceNear(const C3DVector&, float) @ 0x00665DF0. Constants: 2^32 @ 0x0122DCB4,
// 30.0f @ 0x0128FDD0.
// Shape: the landed world-random step inline (`seed = seed * 0x24A1 + 0x24DF ; seed = GFROR13(seed) ; % 100`,
// the zero-range test folded) compared as a float against 30 (`fild ; jge ; fadd 2^32 ; fcomp ; test ah,5 ; jp` =
// the ordered `<`); the marker is a named local taken before the creature (also a named local, esi) so the
// creature load precedes the `push 1.0f` and the GetPosition call; the placement's bool is returned directly.
// Model: local (engine/CAIStateGroup_Gaze.h flattens the base).
struct CThingGameObject;

unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

struct C3DVector { float X, Y, Z; };

struct CThing {
    const C3DVector* GetPosition() const;                   // 0x004C73D0
};

struct CThingAICreature {
    bool AutoPlaceNear(const C3DVector& position, float radius);    // 0x00665DF0 (CThingCreatureBase)
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                             // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                              // slot 11 (+0x2c)
    CAIBrain* PBrain;                                       // +0x04
    long ScriptAIPriority;                                  // +0x08
    void* Name_PStringData;                                 // +0x0c
    unsigned char Terminated;                               // +0x10
    unsigned char Priority;                                 // +0x11
    unsigned char Enabled;                                  // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();                         // 0x008FCFD0
};

struct CAIStateGroup_Gaze : CAIStateGroupBase {
    unsigned char GazeMarker[8];                            // +0x14 CIntelligentPointer<CThingGameObject>
    float ChanceOfRunning;                                  // +0x1c
    float MinJourneyForRunning;                             // +0x20
    float MaxDistanceForCreaturesToLookAt;                  // +0x24
    long PrimaryMarkerDef;                                  // +0x28
    long SecondaryMarkerDef;                                // +0x2c
    CThingGameObject* FindFreeGazePos();                    // 0x00918880
    bool InitCreaturePosition();
};

bool CAIStateGroup_Gaze::InitCreaturePosition()
{
    if (IsRunnable()) {
        if (WorldRandomRange(*GetPWorldSeed(), 100) < 30.0f) {
            CThingGameObject* marker = FindFreeGazePos();
            CThingAICreature* creature = PBrain->Creature;
            return creature->AutoPlaceNear(*((CThing*)marker)->GetPosition(), 1.0f);
        }
    }
    return false;
}