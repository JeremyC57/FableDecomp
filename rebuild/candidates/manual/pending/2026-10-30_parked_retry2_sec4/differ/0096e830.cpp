// CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition() -- for a registered combatant: true when its preferred combat position is within (2/3 of twice its collision radius + 2.8) and the planar directions preferred->enemy and creature->enemy lie within 85 degrees; otherwise falls back to IsEnemyCreatureInCombatPosition()
// Grade: DIFFER(548v544) nd=38 (was 42) -- lever: Normalise in the 008E5D40 form (y*y; += z*z; += x*x; r *= 0.5f split). Tried: combat-manager chain without local (47; with form 43), Radius doubling as radius+radius / split statement (37 but 550 bytes). Residue: both 2D normalises keep r on the x87 stack (fst; fmul st(2)) where retail stores and reloads it (the Z=0 lane), the combat manager eax->ecx copy, the Radius*2 fadd st0,st0. Next: model the 2D vectors with a two-float ctor so Z is not a known 0.0f.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096E830, length 544 bytes, plain `ret`.
// Callees: IsRegisteredCombatant @ 0x00662410, GetPWorld @ 0x008FCFC0, CCombatManager::GetPreferredPos @ 0x006EA770 (C3DVector by
// hidden pointer), CThing::GetPosition @ 0x004C73D0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// IsEnemyCreatureInCombatPosition @ 0x0096E700.
// Data: the reciprocal-square-root mantissa table @ 0x013CE558; 3.0f / 0.5f / 0.0f @ 0x0122DED4 / 0x0122F59C / 0x0122DEDC;
// 0.6666667f @ 0x012925CC, 2.8f @ 0x01297070, 1.48353 (85 degrees, double) @ 0x01297068 fed to fcos.
// Layout: local model (PBrain +4; brain PCreature +0x18; PEnemy +0x14; world combat manager +0x4c; retail CThing physics body
// +0x60, its radius +0x34).
// Shape: two planar directions (Z = 0) through the landed inline fast Normalise; the angle limit is cos() of a double constant.
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = y * y;
        lengthSquared += z * z;
        lengthSquared += x * x;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        *this *= r;
    }
    __forceinline C3DVector& operator*=(const float& scale)
    {
        X *= scale;
        Y *= scale;
        Z *= scale;
        return *this;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CPhysicsBody {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
struct CThing {
    unsigned char _pad0[0x60];
    CPhysicsBody* PPhysics;                     // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    bool IsRegisteredCombatant();               // 0x00662410
};
struct CCombatManager {
    C3DVector GetPreferredPos(CThingCreatureBase* creature);   // 0x006EA770
};
struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
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
};
struct CAIStateGroup_MeleeCombatNearby : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    bool IsEnemyCreatureInCombatPosition();     // 0x0096E700
    bool IsEnemyCreatureNearCombatPosition();
};

bool CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition()
{
    if (GetCreature()->IsRegisteredCombatant()) {
    CThingCreatureBase* me = GetCreature();
    CCombatManager* combat = GetPWorld()->PCombatManager;
    C3DVector preferred = combat->GetPreferredPos(me);
    const C3DVector& here = GetCreature()->GetPosition();
    C3DVector toPreferred(preferred.X - here.X, preferred.Y - here.Y, 0.0f);
    const C3DVector& enemyPos = PEnemy.GetPItem()->GetPosition();
    C3DVector fromPreferred(enemyPos.X - preferred.X, enemyPos.Y - preferred.Y, 0.0f);
    fromPreferred.Normalise();
    const CThing* creature = GetCreature();
    const CThing* enemy = PEnemy.GetPItem();
    const C3DVector& from = creature->GetPosition();
    const C3DVector& to = enemy->GetPosition();
    C3DVector toEnemy(to.X - from.X, to.Y - from.Y, 0.0f);
    toEnemy.Normalise();
    float range = GetCreature()->PPhysics->Radius * 2.0f / 1.5f + 2.8f;
    if (toPreferred.X * toPreferred.X + toPreferred.Y * toPreferred.Y < range * range
            && Dot(toEnemy, fromPreferred) > cos(1.4835298641951802))
        return true;
    if (IsEnemyCreatureInCombatPosition())
        return true;
    }
    return false;
}
