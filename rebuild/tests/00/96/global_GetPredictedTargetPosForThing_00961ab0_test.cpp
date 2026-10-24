#include <cstdio>
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector& operator+=(const C3DVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
};

struct CSnowTrollDef {
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
struct CDefClassBase {
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
struct CTCPhysicsBase {
    C3DVector Position;
    C3DVector OldPosition;
};
struct CThing {
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

// Behaviour test for 0x00961AB0 GetPredictedTargetPosForThing (the copy above): the target's position extrapolated by
// its last-frame movement times the def's prediction time (2.0 without a snow-troll def); counts stay balanced.
int main()
{
    CDefClassBase defClass;
    CSnowTrollDef def; def.SecondsIcicleAttackTargetPositionPredictionTime = 1.5f;
    CTCPhysicsBase creaturePhysics; creaturePhysics.Position = C3DVector(0, 0, 0); creaturePhysics.OldPosition = C3DVector(0, 0, 0);
    CTCPhysicsBase targetPhysics; targetPhysics.Position = C3DVector(10, 2, 0); targetPhysics.OldPosition = C3DVector(9, 2, 1);
    CThing creature(&creaturePhysics, &defClass);
    CThing target(&targetPhysics, 0);

    C3DVector p = GetPredictedTargetPosForThing(creature, target);
    if (!Near(p.X, 12.0f) || !Near(p.Y, 2.0f) || !Near(p.Z, -2.0f)) { std::printf("00961ab0_TEST FAIL\n"); return 1; }
    defClass.SubDef = &def;
    p = GetPredictedTargetPosForThing(creature, target);
    if (!Near(p.X, 11.5f) || !Near(p.Y, 2.0f) || !Near(p.Z, -1.5f)) { std::printf("00961ab0_TEST FAIL\n"); return 1; }
    if (defClass.RefCount != 1 || defClass.Deleted != 0 || def.RefCount != 1 || def.Deleted != 0) {
        std::printf("00961ab0_TEST FAIL\n");
        return 1;
    }
    std::printf("00961ab0_TEST PASS\n");
    return 0;
}