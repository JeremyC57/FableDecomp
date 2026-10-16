// CAIStateGroup_BalverineCornered::GetLandPos(CThing* target) -- the point beside the target, along the creature-to-target direction (flattened), at 1.2 times the sum of both physics radii, dropped to the ground height
// Grade: DIFFER(223v223) -- x87 operand order only: the landing Z is `fld [targetPos.Z] ; fadd [spilled scaled Z]` where retail loads the spilled term and adds the position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094F090, length 223 bytes (`ret 8`, C3DVector by hidden pointer).
// Callees: CThing::GetPosition @ 0x004C73D0 (target, then creature), C3DVector::Normalise @ 0x00A14440 (manifest
// "GetScaled" is propagated), CAIStateGroupBase::GetPWorld @ 0x008FCFC0, the world's virtual slot 13 (+0x34,
// GetWorldMap), CWorldMap::GetHeightZBelowPos(const C3DVector&) @ 0x0063E480. Constant 1.2f @ 0x0125A690.
// Shape: radii from each thing's physics component (+0x60, radius +0x34); the target position is copied into a local
// C3DVector; the direction is built through the three-float ctor (Z = 0 stored as an immediate); `pos + dir * d`
// through inline operators; the result takes X, Y from the landing point and Z from the height query.
struct C3DVector
{
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                               // 0x00A14440
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};

struct CTCPhysicsBase
{
    unsigned char _pad00[0x34];
    float Radius;                                   // +0x34
};

struct CThing
{
    unsigned char _pad00[0x60];
    CTCPhysicsBase* Physics;                        // +0x60
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CWorldMap
{
    float GetHeightZBelowPos(const C3DVector& pos);   // 0x0063E480
};

struct CWorld
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();               // slot 13 (+0x34)
};

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CAIStateGroup_BalverineCornered
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CWorld* GetPWorld();                            // 0x008FCFC0
    C3DVector GetLandPos(CThing* target);
};

C3DVector CAIStateGroup_BalverineCornered::GetLandPos(CThing* target)
{
    float distance = (PBrain->Creature->Physics->Radius + target->Physics->Radius) * 1.2f;
    C3DVector targetPos = target->GetPosition();
    const C3DVector& creaturePos = PBrain->Creature->GetPosition();
    C3DVector direction(targetPos.X - creaturePos.X, targetPos.Y - creaturePos.Y, 0.0f);
    direction.Normalise();
    C3DVector land = targetPos + direction * distance;
    return C3DVector(land.X, land.Y, GetPWorld()->GetWorldMap()->GetHeightZBelowPos(land));
}
