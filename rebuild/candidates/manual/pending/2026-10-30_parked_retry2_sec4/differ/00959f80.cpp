// CAIStateGroup_HoverKeepInArea::Update() -- pick a wander position within 20 units of the area centre, look at a point offset from it (the approach direction rotated by a random fraction of a turn and scaled by a random signed distance), then hover towards it with a random movement type 8..10 while still running and at least 20 units from the centre; terminate afterwards or when the look fails
// Grade: DIFFER(747v752) nd=117 (was 716v752 nd=217) -- look point rebuilt as vector algebra: target + C3DVector(dir.Y, -dir.X, 0) * sin * distance + dir * cos * distance (operator* / operator+ temporaries give retail's frame 0x54 and z,y,x product order), C3DVector user copy ctor, loop as do-while with !(len < 20); residue: (r & 1) stored to the random slot, Y*c/X*c operand order (fld st(1); fmul [m] v fld [m]; fmul st(2)), push 1 placement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00959F80, length 752 bytes, virtual (`ret`).
#include <math.h>
#pragma intrinsic(cos, sin)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
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
struct CAIStateGroup_HoverKeepInArea : CAIStateGroup_HoverMovementBase {
    unsigned char _pad14[0x08];
    C3DVector Centre;                           // +0x1c
    void Update();
};

static inline unsigned long NextRandom(unsigned long& seed)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return seed;
}

void CAIStateGroup_HoverKeepInArea::Update()
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
    C3DVector target;
    if (!nav->GetNearbyWanderPosition(Centre, 20.0f, &target, false))
        return;
    float distance = (float)(NextRandom(seed) % 100) * 0.003f + 0.1f;
    if ((NextRandom(seed) & 1) == 0)
        distance *= -1.0f;
    float turn = (float)(NextRandom(seed) % 100) * 0.02f + 3.0f;
    const C3DVector& here = GetCreature()->GetPosition();
    C3DVector dir(target.X - here.X, target.Y - here.Y, 0.0f);
    dir.Normalise();
    float x = -dir.X;
    double angle = turn * 6.2831854820251465;
    C3DVector look = target + C3DVector(dir.Y, x, 0.0f) * (float)sin(angle) * distance + dir * (float)cos(angle) * distance;
    if (AIAction_LookAtPos(1, look, 0x11, 0.05f) == 2) {
        long movement = NextRandom(GetPWorld()->GetRandomSeed()) % 3 + 8;
        float dx;
        float dy;
        do {
            const C3DVector& me = GetCreature()->GetPosition();
            dx = me.X - Centre.X;
            dy = me.Y - Centre.Y;
        } while (AIAction_MoveToPosProximityAndFacePos(2, target, look, has, movement) == 4
                 && !((float)sqrt(dy * dy + dx * dx) < 20.0f));
    }
    Terminate();
}
