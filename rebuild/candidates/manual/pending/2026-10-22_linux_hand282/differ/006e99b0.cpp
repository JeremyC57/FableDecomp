// CCombatManager::GetDefenderZone(const CThingCreatureBase&, const CThingCreatureBase&, long) const
// Grade: DIFFER(852v852) -- toPredicted.Z = 0 `mov dword ptr [esp+2Ch],0` lands between `fsub [eax]` and `fstp [esp+24h]`; retail puts it after `fstp [esp+24h]; fld [esp+64h]` (before `fsub [eax+4]`); all else exact (frame 0x60, slots, x87 shapes, dots) | tried (~200 grades over three runs): explicit member stores in every X/Y/Z order with and without (float) casts, ctor C3DVector(x,y,0), Set/ClearZ/Flatten/operator[]/SetComponent/pointer writes, -= on a copy and in place on predictedPos (different slots), direct-init / assignment (894), free/NRVO/__forceinline operator- and ctors, trivial copy ctor (defenderPos block copy -> zero store right after fstp X, 866 bytes; memberwise defenderPos init restores the early slot), /Oa, QFE 4035; both-component casts put it after the Y reload (`fld [esp+28h]`) and swap the Dot(right) operand order | next: lane gap: kind=immediate-store scheduling (the slot follows block-level context such as the defenderPos copy shape, not the toPredicted statement form)
// Leaf: ?GetDefenderZone@CCombatManager
#include <math.h>
#pragma intrinsic(cos)
extern float g_VectorEpsilon;                       // 0x0129BA3C (0.0001f)

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    struct C3DVector Scaled3D(float s) const;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    explicit C3DVector(const C2DVector& v) : X(v.X), Y(v.Y), Z(0.0f) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    float Dot(const C3DVector& o) const { float d = X * o.X + Y * o.Y; d += Z * o.Z; return d; }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float GetMagnitudeAndNormalise()
    {
        float magnitude = GetMagnitude();
        if (magnitude > g_VectorEpsilon)
            *this *= 1.0f / magnitude;
        return magnitude;
    }
    C3DVector& Normalise();                         // 0x00A14440
};

inline C3DVector C2DVector::Scaled3D(float s) const { return C3DVector(X * s, Y * s, 0.0f); }
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;            // slot 70 (+0x118)
    unsigned char _pad04[0xc - 4];
    C3DVector Position;                             // +0x0c
    C3DVector OldPosition;                          // +0x18
    long      OldPositionLastFrameSet;              // +0x24
    C3DVector Velocity;                             // +0x28
    float     Radius;                               // +0x34
};

struct CCombatTypeDef {
    unsigned char _pad00[0x6c];
    float FrontFarLimit;                            // +0x6c (PDB +0x7c)
    float FrontMiddleLimit;                         // +0x70
    float FrontNearLimit;                           // +0x74
    float SideLimit;                                // +0x78
    float RearLimit;                                // +0x7c
};

struct CThingCreatureBase {
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                      // +0x18
    unsigned char _pad20[0x60 - 0x20];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
    unsigned __int64 GetUID() const { return UniqueID; }
    const CCombatTypeDef& GetCombatTypeDef() const; // 0x00666D10
    const C3DVector& GetPosition() const;           // 0x006A4D00
};

struct CAttackingCombatantInfo {                    // retail 0x30 bytes
    CThingCreatureBase* Attacked;                   // +0x00
    unsigned char _rest[0x2c];
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                     // +0x00
    CAttackingCombatantInfo AttackerInfo;           // +0x04
    unsigned char DefenderInfo[0x3c];               // +0x34
};
struct CRetailRbMap_UID_CombatantInfo {             // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                       // +0x10
        CCombatantInfo value;                       // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;  // 0x006F0250
    node* end() const { return header; }
};

enum ECombatZone {
    COMBAT_ZONE_FRONT_FAR    = 0x01,
    COMBAT_ZONE_FRONT_MIDDLE = 0x02,
    COMBAT_ZONE_FRONT_NEAR   = 0x04,
    COMBAT_ZONE_LEFT         = 0x08,
    COMBAT_ZONE_RIGHT        = 0x10,
    COMBAT_ZONE_REAR         = 0x20,
    COMBAT_ZONE_NONE         = 0x40,
    COMBAT_ZONE_ROTATING     = 0x80
};

struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];                  // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;      // +0x10
    bool IsCreatureOnRotatingCombatRing(const CThingCreatureBase& creature) const;   // 0x006E9700
    ECombatZone GetDefenderZone(const CThingCreatureBase& defender, const CThingCreatureBase& attacker, long framesAhead) const;
};

ECombatZone CCombatManager::GetDefenderZone(const CThingCreatureBase& defender, const CThingCreatureBase& attacker, long framesAhead) const
{
    CRetailRbMap_UID_CombatantInfo::node* info = Combatants.find(attacker.GetUID());
    if (info != Combatants.end() && info->value.AttackerInfo.Attacked && IsCreatureOnRotatingCombatRing(attacker))
        return COMBAT_ZONE_ROTATING;

    float frontFar = attacker.GetCombatTypeDef().FrontFarLimit + 0.6f;
    float frontFarSq = frontFar * frontFar;
    float frontMiddle = attacker.GetCombatTypeDef().FrontMiddleLimit;
    float frontMiddleSq = frontMiddle * frontMiddle;
    float frontNear = attacker.GetCombatTypeDef().FrontNearLimit;
    float frontNearSq = frontNear * frontNear;
    float rear = attacker.GetCombatTypeDef().RearLimit + 0.6f;
    float rearSq = rear * rear;
    float side = attacker.GetCombatTypeDef().SideLimit + 0.6f;
    float sideSq = side * side;

    const CTCPhysicsBase* defenderPhysics = defender.PhysicsTC;
    C3DVector defenderPos = defenderPhysics->Position;
    C2DVector velocity(defenderPhysics->Position.X - defenderPhysics->OldPosition.X, defenderPhysics->Position.Y - defenderPhysics->OldPosition.Y);
    C3DVector predictedPos = velocity.Scaled3D((float)framesAhead) + defenderPos;

    C3DVector facing = attacker.PhysicsTC->GetFacing();
    C3DVector right(facing.Y, -facing.X, 0.0f);

    C3DVector toPredicted = predictedPos - attacker.GetPosition();
    toPredicted.Z = 0.0f;
    float distance = toPredicted.GetMagnitudeAndNormalise();

    C3DVector toCurrent = defenderPos - attacker.GetPosition();
    toCurrent.Z = 0.0f;
    if (toCurrent.GetSquaredMagnitude() > g_VectorEpsilon * g_VectorEpsilon)
        toCurrent.Normalise();

    float gap = distance - defender.PhysicsTC->Radius;
    if (gap < 0.0f)
        gap = 0.0f;
    float gapSq = gap * gap;

    if (toCurrent.Dot(toPredicted) < 0.0f && gapSq < sideSq)
        return toPredicted.Dot(right) < 0.0f ? COMBAT_ZONE_LEFT : COMBAT_ZONE_RIGHT;

    float cosAngle = toPredicted.Dot(facing);
    if (cosAngle > cos(0.7853981852531433)) {
        if (gapSq < frontNearSq)
            return COMBAT_ZONE_FRONT_NEAR;
        if (gapSq < frontMiddleSq)
            return COMBAT_ZONE_FRONT_MIDDLE;
        if (gapSq < frontFarSq)
            return COMBAT_ZONE_FRONT_FAR;
        return COMBAT_ZONE_NONE;
    }
    if (cosAngle < cos(2.35619455575943)) {
        if (gapSq < rearSq)
            return COMBAT_ZONE_REAR;
        return COMBAT_ZONE_NONE;
    }
    if (gapSq < sideSq)
        return toPredicted.Dot(right) < 0.0f ? COMBAT_ZONE_LEFT : COMBAT_ZONE_RIGHT;
    return COMBAT_ZONE_NONE;
}
