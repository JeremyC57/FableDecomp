// Behaviour test for CVillageTaskCarryCrate::IsCreatureMoreAppropriateForTask (0x00861530): only pickup tasks with a
// valid candidate qualify, and the candidate must be more than 1.5x nearer (squared factor 2.25) to the crate stack.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { C3DVector r; r.X = X - o.X; r.Y = Y - o.Y; r.Z = Z - o.Z; return r; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing { C3DVector Pos; bool Valid; const C3DVector& GetPosition() const { return Pos; } };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
struct CVillageTaskCarryCrate {
    CIntelligentPointer CreatureAssignedToTask, PCrateStack; bool Pickup;
    bool IsCreatureValidForTask(const CThing& c) const { return c.Valid; }
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const
    {
        bool result = false;
        if (!Pickup)
            return false;
        if (IsCreatureValidForTask(creature)) {
            C3DVector crate = PCrateStack.GetPItem()->GetPosition();
            C3DVector assigned = CreatureAssignedToTask.GetPItem()->GetPosition();
            C3DVector position = creature.GetPosition();
            float assignedDistance = (crate - assigned).GetLengthSquared();
            float creatureDistance = (crate - position).GetLengthSquared();
            if (Pickup)
                result = creatureDistance * 2.25f < assignedDistance;
            else
                result = creatureDistance > assignedDistance * 2.25f;
        }
        return result;
    }
};
int main()
{
    CThing crate = { C3DVector(0, 0, 0), true }, assigned = { C3DVector(0, 9, 0), true };
    CVillageTaskCarryCrate t; t.CreatureAssignedToTask.Item = &assigned; t.PCrateStack.Item = &crate; t.Pickup = true;
    CThing nearer = { C3DVector(5, 0, 0), true };     // 25*2.25 = 56.25 < 81
    CThing farish = { C3DVector(0, 0, 7), true };     // 49*2.25 = 110 > 81
    CThing invalid = { C3DVector(1, 0, 0), false };
    bool ok = t.IsCreatureMoreAppropriateForTask(nearer) && !t.IsCreatureMoreAppropriateForTask(farish)
        && !t.IsCreatureMoreAppropriateForTask(invalid);
    t.Pickup = false;
    ok = ok && !t.IsCreatureMoreAppropriateForTask(nearer);
    if (!ok) { std::printf("00861530_TEST FAIL\n"); return 1; }
    std::printf("00861530_TEST PASS\n");
    return 0;
}