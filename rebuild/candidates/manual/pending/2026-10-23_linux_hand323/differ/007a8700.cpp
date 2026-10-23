// CTCBattleCharge::RushForwards() -- one frame of the charge: aim (override direction, target or forward), face it, step the charging thing forward by the def's speed while it stays inside the charge range; on the first hit thing check line of sight / the hit list (and the per-level hit cap), otherwise follow the ground and stop at walls or steep drops; finish the move when out of range
// Grade: DIFFER(1205v1243) -- first full draft (aim, face, range test, hit-list / hit cap, ground follow, wall and nav checks, finish); retail keeps the step and position in x87 registers across the two range compares (frame 0x4c v our 0x60) and its hit/no-hit branches are laid out differently; x87 scheduling not yet matched
// Leaf: RushForwards
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A8700, 1248 bytes, `ret`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DCoordI { long X, Y; };
struct SOrientation { C3DVector Forward; C3DVector Up; };
struct CThingController {
    virtual void s000(); virtual void s004(); virtual void s008(); virtual void s00c(); virtual void s010(); virtual void s014(); virtual void s018(); virtual void s01c();
    virtual void s020(); virtual void s024(); virtual void s028(); virtual void s02c(); virtual void s030(); virtual void s034(); virtual void s038(); virtual void s03c();
    virtual void s040(); virtual void s044(); virtual void s048(); virtual void s04c(); virtual void s050(); virtual void s054(); virtual void s058(); virtual void s05c();
    virtual void s060(); virtual void s064(); virtual void s068(); virtual void s06c(); virtual void s070(); virtual void s074(); virtual void s078(); virtual void s07c();
    virtual void s080(); virtual void s084();
    virtual void SetPosition(const C3DVector& position, bool update);   // +0x88
    virtual void s08c(); virtual void s090(); virtual void s094(); virtual void s098(); virtual void s09c();
    virtual void s0a0(); virtual void s0a4(); virtual void s0a8(); virtual void s0ac(); virtual void s0b0(); virtual void s0b4(); virtual void s0b8(); virtual void s0bc();
    virtual void s0c0(); virtual void s0c4(); virtual void s0c8(); virtual void s0cc(); virtual void s0d0(); virtual void s0d4(); virtual void s0d8(); virtual void s0dc();
    virtual void s0e0(); virtual void s0e4(); virtual void s0e8(); virtual void s0ec(); virtual void s0f0(); virtual void s0f4(); virtual void s0f8(); virtual void s0fc();
    virtual void s100(); virtual void s104(); virtual void s108(); virtual void s10c(); virtual void s110(); virtual void s114();
    virtual C3DVector GetForward() const;               // +0x118
    virtual void s11c(); virtual void s120(); virtual void s124();
    virtual void SetOrientation(const SOrientation& orientation);   // +0x128
    unsigned char _pad04[0x18 - 0x04];
    C3DVector Velocity;                                 // +0x18
};
struct CThing {
    unsigned char _pad00[0x60];
    CThingController* Controller;                       // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char Flags6c;                              // +0x6c
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags;                              // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    float GetGroundHeight(const C3DVector& position, bool a, bool b);   // 0x00666D40
    bool IsDead() const { return (m_flags & 1) != 0; }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct CThingRef : CBaseIntelligentPointer {
    CThingRef(CThing* thing);                           // 0x004A96F0
    void Assign(const CThingRef& other);                // 0x004A9760
};
struct CHitList {
    void* First; void* Last; void* End;
    void* Find(const CBaseIntelligentPointer& thing, bool* found);   // 0x006C4060
    void Add(const CBaseIntelligentPointer& thing);     // 0x004AD520
};
struct CNavigationMap { bool IsBlocked(); };            // 0x004FCBD0
struct CWorld {
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0c(); virtual void s10(); virtual void s14(); virtual void s18(); virtual void s1c();
    virtual void s20(); virtual void s24(); virtual void s28(); virtual void s2c(); virtual void s30();
    virtual CNavigationMap* GetNavAt(const C2DCoordI& position);   // +0x34
};
struct CBattleChargeDef {
    unsigned char _pad00[0x3c];
    float Speed;                                        // +0x3c
    unsigned char _pad40[0x90 - 0x40];
    unsigned long* MaxHits;                             // +0x90
};
#pragma pack(push, 1)
class CTCBattleCharge {
public:
    void* __vftable;                                    // +0x00
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CBaseIntelligentPointer Target;                     // +0x0c
    CBattleChargeDef* PDef;                             // +0x14
    C3DVector StartPosition;                            // +0x18
    CBaseIntelligentPointer HitThing;                   // +0x24
    bool HitStarted;                                    // +0x2c
    unsigned char _pad2d;
    bool UseDirectionOverride;                          // +0x2e
    C3DVector DirectionOverride;                        // +0x2f
    float Range;                                        // +0x3b
    unsigned char _pad3f;
    long Level;                                         // +0x40
    unsigned char _pad44[0x70 - 0x44];
    CHitList HitList;                                   // +0x70
    C3DVector Moved;                                    // +0x7c
    bool Charging;                                      // +0x88
    bool IgnoreGround;                                  // +0x89
    CWorld* GetWorld();                                 // 0x00686A70
    void UpdateDirection(C3DVector* direction);         // 0x007A6E20
    void StopHit();                                     // 0x007A7D20
    void FinishMove(CThing* thing, bool hit);           // 0x007A7BF0
    void RushForwards();
};
#pragma pack(pop)
extern const float g_StepCheckScale;                    // 0x01238170
extern const float g_MaxStepDown;                       // 0x0126CBF0

void CTCBattleCharge::RushForwards()
{
    CThing* thing = ParentThing;
    CThingController* controller = thing->Controller;
    C3DVector dir;
    if (UseDirectionOverride && !Charging)
        dir = DirectionOverride;
    else if (Target.GetPItem() && !Target.GetPItem()->IsDead()) {
        CThing* target = Target.GetPItem();
        const C3DVector& from = thing->GetPosition();
        const C3DVector& to = target->GetPosition();
        dir = C3DVector(to.X - from.X, to.Y - from.Y, to.Z - from.Z);
    } else
        dir = controller->GetForward();
    UpdateDirection(&dir);
    SOrientation orientation;
    orientation.Forward = dir;
    orientation.Up = C3DVector(0.0f, 0.0f, 1.0f);
    controller->SetOrientation(orientation);
    float speed = PDef->Speed;
    C3DVector step(dir.X * speed, dir.Y * speed, dir.Z * speed);
    const C3DVector& pos = thing->GetPosition();
    C3DVector fromStart(pos.X - StartPosition.X, pos.Y - StartPosition.Y, pos.Z - StartPosition.Z);
    float rangeSq = Range * Range;
    if (!(fromStart.X * fromStart.X + fromStart.Y * fromStart.Y + fromStart.Z * fromStart.Z < rangeSq) ||
        !(Moved.X * Moved.X + Moved.Y * Moved.Y + Moved.Z * Moved.Z < rangeSq)) {
        bool hit = HitThing.GetPItem() && (HitThing.GetPItem()->Flags6c & 8);
        FinishMove(thing, hit);
        return;
    }
    if (HitStarted) {
        if (HitThing.GetPItem() && (HitThing.GetPItem()->Flags6c & 8)) {
            bool found;
            void* end = HitList.Last;
            if (HitList.Find(HitThing, &found) == end) {
                StopHit();
                HitList.Add(HitThing);
                unsigned long hits = ((char*)HitList.Last - (char*)HitList.First) >> 3;
                if (hits >= PDef->MaxHits[Level]) {
                    FinishMove(thing, true);
                    return;
                }
            }
            const C3DVector& here = thing->GetPosition();
            C3DVector next(here.X + step.X, here.Y + step.Y, here.Z + step.Z);
            float ground = thing->GetGroundHeight(next, true, true);
            if (ground > next.Z)
                next.Z = ground;
            controller->SetPosition(next, true);
            HitThing.m_pItem = 0;
            const C3DVector& now = thing->GetPosition();
            Moved.X += next.X - now.X;
            Moved.Y += next.Y - now.Y;
            Moved.Z += next.Z - now.Z;
            return;
        }
        C3DVector probe(controller->Velocity.X + step.X, controller->Velocity.Y + step.Y, controller->Velocity.Z + step.Z);
        probe.Z = thing->GetGroundHeight(probe, false, true);
        const C3DVector& here = thing->GetPosition();
        float dx = probe.X - here.X;
        float dy = probe.Y - here.Y;
        if (IgnoreGround) {
            StopHit();
            FinishMove(thing, true);
            return;
        }
        float limit = PDef->Speed * g_StepCheckScale;
        if (!(dx * dx + dy * dy < limit * limit)) {
            CWorld* world = GetWorld();
            C2DCoordI cell;
            cell.X = (long)probe.X;
            cell.Y = (long)probe.Y;
            if (!world->GetNavAt(cell)->IsBlocked()) {
                StopHit();
                FinishMove(thing, true);
                return;
            }
        }
    }
    const C3DVector& here = thing->GetPosition();
    C3DVector next(here.X + step.X, here.Y + step.Y, here.Z + step.Z);
    float ground = thing->GetGroundHeight(next, true, true);
    if (ground > next.Z && next.Z - ground > g_MaxStepDown)
        next.Z = ground;
    controller->SetPosition(next, true);
    const C3DVector& now = thing->GetPosition();
    Moved.X += next.X - now.X;
    Moved.Y += next.Y - now.Y;
    Moved.Z += next.Z - now.Z;
    thing->GetPosition();
    HitStarted = true;
}
