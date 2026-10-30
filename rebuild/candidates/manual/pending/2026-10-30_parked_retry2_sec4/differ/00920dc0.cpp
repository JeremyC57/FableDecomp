// CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace() -- when the enemy is still alive, turn the burrowing creature to face it (flat direction from its own position), then make it rise up
// Grade: DIFFER(455v455) IMPROVED (87 -> 79 difflines): the 008E5D40 Normalise form (y*y, += z*z, += x*x; r *= 0.5f as its own statement) squares dy first as retail. Left: this esi/position cache edi swap, and retail loads the facing body into edi before the Newton step and keeps r in memory. Tried: Position reference local (=), a named body local before Normalise (fixes the early load and the r spill: 62 lines, but 457v455). Next: body local + whatever removes the 2 extra bytes (the second SetOrientation body reload).
// Leaf: OnReplace@CAIStateGroup_CreatureSeekEnemyUnderground
// Layout: the base follows engine/CAIStateGroupBase.h (PBrain +4), modelled locally (landed IdleUnderground OnReplace 0x00920960);
//   the enemy CIntelligentPointer<CThing> +0x14 (item +0x18), the creature position cache +0x1c.
#include <math.h>
#pragma intrinsic(atan2, fmod)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = y * y;
        lengthSquared += z * z;
        lengthSquared += x * x;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(float upX, float upY, float upZ, const C3DVector& forward) : Up(upX, upY, upZ), Forward(forward) {}
};
inline float NormaliseAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF(30)
    virtual void GetPosition(C3DVector& out);                       // slot 31 (+0x7c)
    virtual void SetOrientation(const CRightHandedSet& set);        // slot 32 (+0x80)
    VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngle(float angle);                       // slot 66 (+0x108)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                  // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x114];
    ~CCreatureActionBase();                     // 0x0062E900
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};
struct CCreatureAction_RiseUp : CCreatureActionBase {
    CCreatureAction_RiseUp(CThingCreatureBase* creature);   // 0x008BD880
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0xc];                    // +0x08..+0x13
};
struct CAIStateGroup_CreatureSeekEnemyUnderground : CAIStateGroupBase {
    CBaseIntelligentPointer Enemy;              // +0x14
    C3DVector Position;                         // +0x1c
    virtual void OnReplace();
};

void CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace()
{
    if (IS_THING_ALIVE(Enemy)) {
        PBrain->Creature->PhysicsBody->GetPosition(Position);
        const C3DVector& target = Enemy.GetPItem()->GetPosition();
        C3DVector direction = target - Position;
        direction.Z = 0.0f;
        direction.Normalise();
        float angle = (float)atan2(direction.X, direction.Y) * 0.15915494f;
        if (angle < 0.0f || angle >= 1.0f) {
            angle = (float)fmod((double)angle, 1.0);
            if (angle < 0.0f)
                angle += 1.0f;
        }
        PBrain->Creature->PhysicsBody->SetFacingAngle(angle);
        PBrain->Creature->PhysicsBody->SetOrientation(CRightHandedSet(0.0f, 0.0f, 1.0f, direction));
    }
    CThingCreatureBase* creature = PBrain->Creature;
    creature->SetCurrentAction(&CCreatureAction_RiseUp(creature));
}
