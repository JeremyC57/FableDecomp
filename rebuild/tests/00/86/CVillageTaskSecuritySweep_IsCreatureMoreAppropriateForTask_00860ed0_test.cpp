// Behaviour test for 0x00860ED0 CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask: 1.25x nearer threshold.
#include <cstdio>
struct CBaseObject { };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { C3DVector r; r.X = X - o.X; r.Y = Y - o.Y; r.Z = Z - o.Z; return r; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing : CBaseObject {
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
};
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const { return m_pItem; }
};
struct CVillageTaskSecuritySweep {
    bool available;
    CBaseIntelligentPointer CreatureAssignedToTask;
    C3DVector PositionToGoTo;
    virtual ~CVillageTaskSecuritySweep() { }
    virtual bool IsTaskAvailable() const { return available; }
    bool IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) const
    {
        if (other && IsTaskAvailable()) {
            C3DVector position = thing.GetPosition();
            C3DVector myDelta = PositionToGoTo - position;
            C3DVector rivalDelta = other->PositionToGoTo - position;
            return myDelta.GetLengthSquared() < rivalDelta.GetLengthSquared();
        }
        return IsTaskAvailable();
    }
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const
    {
        C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPosition();
        C3DVector position = creature.GetPosition();
        C3DVector assignedDelta = PositionToGoTo - assigned;
        C3DVector creatureDelta = PositionToGoTo - position;
        return creatureDelta.GetLengthSquared() * 1.5625f < assignedDelta.GetLengthSquared();
    }
};
int main()
{
    CThing assigned; assigned.Pos = C3DVector(10.0f, 0.0f, 0.0f);   // 100 squared
    CVillageTaskSecuritySweep s; s.available = true; s.PositionToGoTo = C3DVector(0.0f, 0.0f, 0.0f);
    s.CreatureAssignedToTask.m_pItem = &assigned;
    CThing c; bool ok = true;
    c.Pos = C3DVector(0.0f, 7.0f, 0.0f);  ok = ok && s.IsCreatureMoreAppropriateForTask(c);    // 49 * 1.5625 = 76.6 < 100
    c.Pos = C3DVector(0.0f, 0.0f, 8.0f);  ok = ok && !s.IsCreatureMoreAppropriateForTask(c);   // 64 * 1.5625 = 100, not <
    c.Pos = C3DVector(9.0f, 0.0f, 0.0f);  ok = ok && !s.IsCreatureMoreAppropriateForTask(c);
    if (!ok) { std::printf("00860ed0_TEST FAIL\n"); return 1; }
    std::printf("00860ed0_TEST PASS\n");
    return 0;
}