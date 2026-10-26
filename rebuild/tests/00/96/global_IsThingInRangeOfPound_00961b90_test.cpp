#include <cstdio>
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector& operator+=(const C3DVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
};

class CSnowTrollDef { public:
    long RefCount;
    int Deleted;
    float SecondsIcicleAttackTargetPositionPredictionTime;
    float DistancePoundGroundRange;
    CSnowTrollDef() : RefCount(1), Deleted(0), SecondsIcicleAttackTargetPositionPredictionTime(2.0f), DistancePoundGroundRange(6.0f) { }
    virtual ~CSnowTrollDef() { }
    virtual void DeleteThis() { ++Deleted; }
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerSnowTroll {
    CSnowTrollDef* m_p;
    CDefPointerSnowTroll() : m_p(0) { }
    ~CDefPointerSnowTroll() { if (m_p) m_p->Release(); }
    CSnowTrollDef* operator->() const { return m_p; }
};
class CDefClassBase { public:
    long RefCount;
    int Deleted;
    CSnowTrollDef* SubDef;                      // the snow-troll def this class carries, if any (test model)
    CDefClassBase() : RefCount(1), Deleted(0), SubDef(0) { }
    virtual ~CDefClassBase() { }
    virtual void DeleteThis() { ++Deleted; }
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
    bool GetSubDef(CDefPointerSnowTroll* out) const
    {
        if (SubDef == 0)
            return false;
        SubDef->AddRef();
        if (out->m_p) out->m_p->Release();
        out->m_p = SubDef;
        return true;
    }
};
struct CDefClassPointer {
    CDefClassBase* m_p;
    CDefClassPointer(CDefClassBase* p) : m_p(p) { }
    CDefClassPointer(const CDefClassPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefClassPointer() { }
    CDefClassBase* operator->() const { return m_p; }
};
// the temporary copy made by GetPDefClass releases its count when it dies
struct CDefClassTemp : CDefClassPointer {
    CDefClassTemp(const CDefClassPointer& other) : CDefClassPointer(other) { }
    ~CDefClassTemp() { if (m_p) m_p->Release(); }
};
class CTCPhysicsBase { public:
    C3DVector Position;
    C3DVector OldPosition;
};
class CThing { public:
    CTCPhysicsBase* m_pPhysics;
    CDefClassPointer PDefClass;
    CThing(CTCPhysicsBase* physics, CDefClassBase* defClass) : m_pPhysics(physics), PDefClass(defClass) { }
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }
    CDefClassTemp GetPDefClass() const { return CDefClassTemp(PDefClass); }
    const C3DVector& GetPosition() const { return m_pPhysics->Position; }
};

namespace {
C3DVector GetPredictedTargetPosForThing(const CThing& creature, const CThing& target)
{
    float predictionTime = 2.0f;
    CDefPointerSnowTroll def;
    if (creature.GetPDefClass()->GetSubDef(&def))
        predictionTime = def->SecondsIcicleAttackTargetPositionPredictionTime;
    C3DVector position = target.GetPhysics()->Position;
    C3DVector velocity = position - target.GetPhysics()->OldPosition;
    position += velocity * predictionTime;
    return position;
}
}
static bool Near(float a, float b) { return fabs((double)(a - b)) < 1e-4; }
namespace {
bool IsThingInRangeOfPound(const CThing& creature, const CThing& target)
{
    float range = 6.0f;
    CDefPointerSnowTroll def;
    if (creature.GetPDefClass()->GetSubDef(&def))
        range = def->DistancePoundGroundRange;
    C3DVector predicted = GetPredictedTargetPosForThing(creature, target);
    const C3DVector& me = creature.GetPosition();
    float dx = me.X - predicted.X;
    float dy = me.Y - predicted.Y;
    return dx * dx + dy * dy < range * range;
}
}

// Behaviour test for 0x00961B90 IsThingInRangeOfPound (the copy above, over the copied 0x00961AB0 prediction).
int main()
{
    CDefClassBase defClass;
    CSnowTrollDef def; def.DistancePoundGroundRange = 8.0f; def.SecondsIcicleAttackTargetPositionPredictionTime = 0.5f;
    CTCPhysicsBase creaturePhysics; creaturePhysics.Position = C3DVector(0, 0, 0); creaturePhysics.OldPosition = C3DVector(0, 0, 0);
    CTCPhysicsBase targetPhysics;
    CThing creature(&creaturePhysics, &defClass);
    CThing target(&targetPhysics, 0);
    int fails = 0;

    targetPhysics.Position = C3DVector(4, 0, 9); targetPhysics.OldPosition = C3DVector(4, 0, 9);
    if (!IsThingInRangeOfPound(creature, target)) ++fails;             // 16 < 36 (Z ignored)
    targetPhysics.Position = C3DVector(7, 0, 0); targetPhysics.OldPosition = C3DVector(7, 0, 0);
    if (IsThingInRangeOfPound(creature, target)) ++fails;              // 49 > 36
    targetPhysics.Position = C3DVector(5, 0, 0); targetPhysics.OldPosition = C3DVector(4, 0, 0);
    if (IsThingInRangeOfPound(creature, target)) ++fails;              // predicted x = 5 + 1 * 2 = 7
    targetPhysics.Position = C3DVector(6, 0, 0); targetPhysics.OldPosition = C3DVector(6, 0, 0);
    if (IsThingInRangeOfPound(creature, target)) ++fails;              // 36 is not < 36
    defClass.SubDef = &def;
    targetPhysics.Position = C3DVector(7, 0, 0); targetPhysics.OldPosition = C3DVector(7, 0, 0);
    if (!IsThingInRangeOfPound(creature, target)) ++fails;             // def range 8: 49 < 64
    targetPhysics.Position = C3DVector(7, 1, 0); targetPhysics.OldPosition = C3DVector(5, 1, 0);
    if (IsThingInRangeOfPound(creature, target)) ++fails;              // predicted (8, 1): 65 > 64
    if (defClass.RefCount != 1 || def.RefCount != 1 || defClass.Deleted != 0 || def.Deleted != 0) ++fails;
    if (fails) { std::printf("00961b90_TEST FAIL\n"); return 1; }
    std::printf("00961b90_TEST PASS\n");
    return 0;
}