// Grade: DIFFER(626v739) -- first-pass model: retail's rotation-matrix x87 schedule (cos/sin products spilled into frame words, unsigned fild fix-ups) is not reproduced; control flow and calls follow retail (1 try)
// CAIStateGroup_HoverWander::Update() -- three times: pick a nearby wander position within 5 units, look at a point offset from it (the approach direction rotated by a random fraction of a turn and scaled by a random signed distance), then hover there with a random movement type 8..10 until arrived; terminate when an action fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00959BF0, length 739 bytes (+ int3 padding), virtual (`ret`).
#include <math.h>
#pragma intrinsic(cos, sin)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                           // 0x00A14440
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CTCCreatureNavigation {
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* out, bool flag);   // 0x006CB6E0
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CWorld {
    unsigned long& GetRandomSeed();             // 0x0049D810
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
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float tolerance);   // 0x0097D580
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2c(); virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50();
    virtual void v54();
    virtual long AIAction_MoveToPosProximityAndFacePos(long repeat, const C3DVector& pos, const C3DVector& facePos,
                                                       bool& arrived, long movementType);   // slot 22 (+0x58)
};
struct CAIStateGroup_HoverWander : CAIStateGroup_HoverMovementBase {
    void Update();
};

static inline unsigned long NextRandom(unsigned long& seed)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return seed;
}

void CAIStateGroup_HoverWander::Update()
{
    unsigned long& seed = GetPWorld()->GetRandomSeed();
    CThingCreatureBase* creature = GetCreature();
    CTCCreatureNavigation* nav;
    bool has = (creature->m_presentBits[0] >> 7) & 1;
    if (has) {
        long type = 7;
        CInterfaceMap* map = &creature->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        nav = (CTCCreatureNavigation*)((it == map->last || 7 < it->type) ? map->last : it)->component;
    }
    for (long i = 0; i != 3; ++i) {
        C3DVector target;
        if (!nav->GetNearbyWanderPosition(GetCreature()->GetPosition(), 5.0f, &target, true))
            continue;
        float distance = (float)(NextRandom(seed) % 100) * 0.003f + 0.1f;
        if ((NextRandom(seed) & 1) == 0)
            distance *= -1.0f;
        float turn = (float)(NextRandom(seed) % 100) * 0.02f + 3.0f;
        const C3DVector& here = GetCreature()->GetPosition();
        C3DVector dir(target.X - here.X, target.Y - here.Y, 0.0f);
        dir.Normalise();
        double angle = turn * 6.2831854820251465;
        float c = (float)cos(angle);
        float s = (float)sin(angle);
        float x = -dir.X;
        C3DVector look(target.X + (x * c - dir.Y * s) * distance,
                       target.Y + (x * s + dir.Y * c) * distance,
                       target.Z + (0.0f * s + dir.Z * c) * distance);
        if (AIAction_LookAtPos(1, look, 0x11, 0.05f) != 2) {
            Terminate();
            return;
        }
        long result;
        do {
            result = AIAction_MoveToPosProximityAndFacePos(2, target, look, has, NextRandom(GetPWorld()->GetRandomSeed()) % 3 + 8);
        } while (result == 4);
        if (result != 2) {
            Terminate();
            return;
        }
    }
}
