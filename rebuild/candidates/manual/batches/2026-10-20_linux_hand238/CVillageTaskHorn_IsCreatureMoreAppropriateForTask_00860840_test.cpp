// Behaviour test for CVillageTaskHorn::IsCreatureMoreAppropriateForTask (0x00860840): the candidate wins only when its squared distance
// to the PHorn thing times 1.5625 is below the assigned creature's (i.e. more than 1.25x nearer).
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { C3DVector r; r.X = X - o.X; r.Y = Y - o.Y; r.Z = Z - o.Z; return r; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing { C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
struct CVillageTaskHorn {
    CIntelligentPointer PHorn;
    CIntelligentPointer CreatureAssignedToTask;
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const
    {
        C3DVector goal = PHorn.GetPItem()->GetPosition();
        C3DVector assigned = CreatureAssignedToTask.GetPItem()->GetPosition();
        C3DVector position = creature.GetPosition();
        C3DVector assignedDelta = goal - assigned;
        C3DVector creatureDelta = goal - position;
        return creatureDelta.GetLengthSquared() * 1.5625f < assignedDelta.GetLengthSquared();
    }
};
int main()
{
    CThing goal = { C3DVector(0, 0, 0) }, assigned = { C3DVector(10, 0, 0) };
    CVillageTaskHorn t; t.PHorn.Item = &goal; t.CreatureAssignedToTask.Item = &assigned;
    CThing nearer = { C3DVector(0, 7, 0) };      // 49*1.5625 = 76.6 < 100
    CThing similar = { C3DVector(0, 0, 9) };     // 81*1.5625 = 126.6 > 100
    CThing exact = { C3DVector(8, 0, 0) };       // 64*1.5625 = 100, not strictly less
    bool ok = t.IsCreatureMoreAppropriateForTask(nearer) && !t.IsCreatureMoreAppropriateForTask(similar)
        && !t.IsCreatureMoreAppropriateForTask(exact) && !t.IsCreatureMoreAppropriateForTask(assigned);
    if (!ok) { std::printf("00860840_TEST FAIL\n"); return 1; }
    std::printf("00860840_TEST PASS\n");
    return 0;
}
