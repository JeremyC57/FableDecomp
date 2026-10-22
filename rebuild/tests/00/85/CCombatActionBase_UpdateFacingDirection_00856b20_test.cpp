#include <cstdio>
#include <cmath>
// behaviour test: CCombatActionBase::UpdateFacingDirection (0x00856B20)
struct C3DVector { float X, Y, Z; };
static float AngleDifference(float a, float b)
{
    float d = a - b;
    while (d > 0.5f) d -= 1.0f;
    while (d < -0.5f) d += 1.0f;
    return d;
}
struct CPhysicsBody {
    C3DVector Facing;
    float SetAngle;
    int SetCount;
    C3DVector GetFacing() { return Facing; }
    void SetFacingAngle(float f) { SetAngle = f; ++SetCount; }
};
struct CThing {
    C3DVector Pos;
    CPhysicsBody* m_pPhysics;
    const C3DVector& GetPosition() const { return Pos; }
    CPhysicsBody* GetPhysics() const { return m_pPhysics; }
};
struct CAnimationEntry {
    float Rotation;
    void GetRotationInAnimation(float* p) const { *p = Rotation; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
static float FacingFraction(float x, float y)
{
    float fraction = (float)std::atan2(x, y) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)std::fmod((double)fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    return fraction;
}
struct CCombatActionBase {
    CBaseIntelligentPointer PCreature;
    CAnimationEntry* PAnimEntry;
    CBaseIntelligentPointer PTarget;
    bool UpdateFacing;
    float Distance;
    float Tolerance;
    bool IsToUpdateFacing() { return UpdateFacing; }
    float GetFacingDistance() { return Distance; }
    float GetFacingTolerance() { return Tolerance; }
    void UpdateFacingDirection();
};
void CCombatActionBase::UpdateFacingDirection()
{
    if (PTarget.GetPItem() == 0)
        return;
    if (PTarget.GetPItem() == PCreature.GetPItem())
        return;
    if (!IsToUpdateFacing())
        return;
    if (PAnimEntry) {
        float rotation;
        PAnimEntry->GetRotationInAnimation(&rotation);
        if ((float)std::fabs(rotation) > 0.0001f)
            return;
    }
    CThing* target = PTarget.GetPItem();
    CThing* creature = PCreature.GetPItem();
    const C3DVector& from = creature->GetPosition();
    const C3DVector& to = target->GetPosition();
    C3DVector delta;
    delta.X = to.X - from.X;
    delta.Y = to.Y - from.Y;
    float facing = FacingFraction(delta.X, delta.Y);
    float distance = GetFacingDistance();
    if (delta.Y * delta.Y + delta.X * delta.X > distance * distance) {
        float tolerance = GetFacingTolerance();
        if (tolerance < 0.5f) {
            C3DVector current = PCreature.GetPItem()->GetPhysics()->GetFacing();
            float currentFacing = FacingFraction(current.X, current.Y);
            if (std::fabs(AngleDifference(currentFacing, facing)) > tolerance)
                return;
        }
    }
    PCreature.GetPItem()->GetPhysics()->SetFacingAngle(facing);
}
static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); g_fail = 1; } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    CPhysicsBody phys = { { 0.0f, 1.0f, 0.0f }, -1.0f, 0 };
    CThing hero = { { 0.0f, 0.0f, 0.0f }, &phys };
    CThing foe = { { 1.0f, 0.0f, 0.0f }, 0 };
    CAnimationEntry anim = { 0.0f };
    CCombatActionBase a;
    a.PCreature.m_pItem = &hero; a.PAnimEntry = &anim; a.PTarget.m_pItem = 0;
    a.UpdateFacing = true; a.Distance = 5.0f; a.Tolerance = 0.1f;
    a.UpdateFacingDirection();
    Check(phys.SetCount == 0, "no target");
    a.PTarget.m_pItem = &hero;
    a.UpdateFacingDirection();
    Check(phys.SetCount == 0, "self target");
    a.PTarget.m_pItem = &foe;
    a.UpdateFacing = false;
    a.UpdateFacingDirection();
    Check(phys.SetCount == 0, "not to update");
    a.UpdateFacing = true;
    anim.Rotation = -0.5f;
    a.UpdateFacingDirection();
    Check(phys.SetCount == 0, "rotating anim");
    anim.Rotation = 0.0f;
    a.UpdateFacingDirection();      // within distance: snap to +X = quarter turn
    Check(phys.SetCount == 1 && Near(phys.SetAngle, 0.25f), "snap inside distance");
    foe.Pos.X = -10.0f;             // far away, facing +Y, target at -X (0.75), off by 0.25 > 0.1
    a.UpdateFacingDirection();
    Check(phys.SetCount == 1, "far and off tolerance");
    a.Tolerance = 0.3f;             // within tolerance: turn
    a.UpdateFacingDirection();
    Check(phys.SetCount == 2 && Near(phys.SetAngle, 0.75f), "far within tolerance");
    a.Tolerance = 0.6f;             // tolerance >= 0.5: always turn
    foe.Pos.X = 0.0f; foe.Pos.Y = -10.0f;
    a.UpdateFacingDirection();
    Check(phys.SetCount == 3 && Near(phys.SetAngle, 0.5f), "loose tolerance");
    a.PAnimEntry = 0;
    a.Tolerance = 0.1f;
    phys.Facing.X = 0.0f; phys.Facing.Y = -1.0f;
    a.UpdateFacingDirection();
    Check(phys.SetCount == 4 && Near(phys.SetAngle, 0.5f), "no anim entry, aligned");
    if (!g_fail) std::printf("00856b20_TEST PASS\n");
    return g_fail;
}