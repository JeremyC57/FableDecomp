// Grade: DIFFER(426v426) -- instruction-identical apart from stack-slot packing: retail's frame is 0x1c (the find key temporary, outerRadius and the prong vector share words 4..0xf; direction at 0x10), ours 0x24 (key/prong at 0xc, direction at 0x18). Tried: all five callees same-TU (425v426: the map find sinks below the ring test), find declaration-only (428v426), GetRing inlined in the GetRingRadius body (426v426), early return on the ring index (this file, 426v426).
// CCombatManager::IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const -- true when the attacker stands inside its assigned ring of the defender's combat wheel (between the inner and outer ring radius, padded by the defender's radius) and within its prong's angular sector
// Leaf: ?IsCreatureInAttackRingSector@CCombatManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E94D0, 426 bytes, __thiscall, `ret 4`, bool in al.
// PDB: ?IsCreatureInAttackRingSector@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z (catalog
//   "Combat_CheckRingRangeToTarget" was descriptive). Callers 0x0093DBF6 / 0x0093DCFA / 0x0093DE7A.
// Callees: the combatant map find 0x006F0250 (twice), CCombatWheel::GetRingRadius 0x006E8850 (twice), CThing::GetPosition
//   0x004C73D0 (twice), CCombatWheel::GetRing const 0x006E8520, CCombatRing::GetAttackerProngPosition 0x006E8810,
//   CCombatRing::GetProngSeparationHalfAngle 0x006E8460. The five combat callees are same-TU noinline bodies (as in
//   the landed 0x006E9680 / 0x006E9D90): retail keeps the dot product on the x87 stack across the half-angle call.
// Data: 0.6f 0x01261D68, 1e-4f 0x0129BA3C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, (double)2pi 0x0124F2B8.
// Layout: as the landed GetCombatWheelRadiusForAttacker: Combatants +0x10 (node value +0x18); CCombatantInfo
//   AttackerInfo +4 {Attacked, AttackerNumber, CombatWheelRingIndex +8}, CombatWheel +0x40; CThing physics TC +0x60
//   (CTCPhysicsBase::Radius +0x34); CThingCreatureBase UID +0x18.
#include <math.h>
#pragma intrinsic(sqrt, cos)

extern const double g_twoPi;                                // 0x0124F2B8 ((double)6.2831855f)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float DotProduct(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

class CTCPhysicsBase {
public:
    unsigned char _pad00[0x34];
    float Radius;                                           // +0x34
};

class CThing {
public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned char    _pad20[0x60 - 0x20];
    CTCPhysicsBase*  PhysicsTC;                             // +0x60
    unsigned __int64 GetUID() const { return UniqueID; }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
class CThingCreatureBase : public CThing {};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

class CDefendingCombatantInfo {
public:
    class CProng {                                          // 28 bytes
    public:
        bool          Blocked;                              // +0x00
        float         Prong[3];                             // +0x04 C3DVector
        long          ProngBlockageTimer;                   // +0x10
        unsigned char POccupier[8];                         // +0x14 CIntelligentPointer<CThingCreatureBase>
        C3DVector GetRotatedPosition(float angle) const;    // 0x006E7FD0
    };
    class CCombatRing {                                     // 28 bytes in retail
    public:
        float                 RotationAngle;                // +0x00
        float                 DeltaRotation;                // +0x04
        long                  Index;                        // +0x08
        float                 RingThickness;                // +0x0C
        CRetailVector<CProng> Prongs;                       // +0x10
        const CProng* GetAttackerProng(const CThing* attacker) const;          // 0x006E87D0
        C3DVector GetAttackerProngPosition(const CThing* attacker) const;     // 0x006E8810
        float GetProngSeparationHalfAngle() const;                             // 0x006E8460
    };
    class CCombatWheel {
    public:
        CRetailVector<CCombatRing> CombatRings;             // +0x00
        const CCombatRing* GetRing(long index) const;       // 0x006E8520
        const CCombatRing* GetRingInline(long index) const  // the same body, inlined into GetRingRadius
        {
            if (index >= 0 && (unsigned long)index < CombatRings.size())
                return &CombatRings.first[index];
            return 0;
        }
        float GetRingRadius(long ring) const;               // 0x006E8850
    };
};

#pragma auto_inline(off)
// Same-TU bodies of the combat-ring helpers (landed separately at the addresses above).
__declspec(noinline) const CDefendingCombatantInfo::CCombatRing* CDefendingCombatantInfo::CCombatWheel::GetRing(long index) const
{
    if (index >= 0 && (unsigned long)index < CombatRings.size())
        return &CombatRings.first[index];
    return 0;
}
__declspec(noinline) float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long ring) const
{
    float radius = 0.0f;
    for (long i = ring; i >= 0; --i)
        radius += GetRingInline(i)->RingThickness;
    return radius;
}
__declspec(noinline) C3DVector CDefendingCombatantInfo::CCombatRing::GetAttackerProngPosition(const CThing* attacker) const
{
    const CProng* prong = GetAttackerProng(attacker);
    if (prong)
        return prong->GetRotatedPosition(RotationAngle);
    return C3DVector(1.0f, 0.0f, 0.0f);
}
__declspec(noinline) float CDefendingCombatantInfo::CCombatRing::GetProngSeparationHalfAngle() const
{
    long count = Prongs.size();
    return (1.0f / count) * 0.5f;
}
#pragma auto_inline(on)

struct CAttackingCombatantInfo {
    const CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                                    // +0x04
    long CombatWheelRingIndex;                              // +0x08
    unsigned char _rest[0x30 - 0x0c];
};
struct CCombatantInfo {                                     // retail layout (12-byte vectors/maps)
    const CThingCreatureBase* Parent;                       // +0x00
    CAttackingCombatantInfo AttackerInfo;                   // +0x04
    unsigned char Attackers[0x0c];                          // +0x34
    CDefendingCombatantInfo::CCombatWheel CombatWheel;      // +0x40
};

struct CRetailRbMap_UID_CombatantInfo {                     // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                               // +0x10
        CCombatantInfo value;                               // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;          // 0x006F0250
};

class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;              // +0x10
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const
    {
        return Combatants.find(creature.GetUID())->value;
    }
    bool IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const;
};

bool CCombatManager::IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const
{
    const CCombatantInfo& attackerInfo = GetCombatantInfo(attacker);
    const CThingCreatureBase* attacked = attackerInfo.AttackerInfo.Attacked;
    float attackedRadius = attacked->PhysicsTC->Radius;
    const CCombatantInfo& attackedInfo = GetCombatantInfo(*attacked);
    long ringIndex = attackerInfo.AttackerInfo.CombatWheelRingIndex;
    if (ringIndex < 0)
        return false;
    const CDefendingCombatantInfo::CCombatWheel& wheel = attackedInfo.CombatWheel;
    float outerRadius = wheel.GetRingRadius(ringIndex) + attackedRadius + 0.6f;
    float innerRadius = 0.0f;
    if (ringIndex > 0)
        innerRadius = wheel.GetRingRadius(ringIndex - 1) + attackedRadius - 0.6f;
    C3DVector toAttacker = attacker.GetPosition() - attacked->GetPosition();
    toAttacker.Z = 0.0f;
    float distanceSquared = toAttacker.GetLengthSquared();
    if (innerRadius * innerRadius < distanceSquared && outerRadius * outerRadius > distanceSquared) {
        if (distanceSquared > 0.0001f) {
            float invLength = 1.0f / (float)sqrt(distanceSquared);
            toAttacker.X *= invLength;
            toAttacker.Y *= invLength;
            toAttacker.Z *= invLength;
        }
        const CDefendingCombatantInfo::CCombatRing* ring = wheel.GetRing(ringIndex);
        C3DVector prongPosition = ring->GetAttackerProngPosition(&attacker);
        if (prongPosition.DotProduct(toAttacker) > cos(ring->GetProngSeparationHalfAngle() * g_twoPi))
            return true;
    }
    return false;
}
