// Behaviour test for CAIStateGroup_HoverMovementBase::AIAction_MoveToAndFaceThingAtDistance (0x0091D320): model copy, scripted stubs.
#include <cstdio>
#include <cmath>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); if (l > 0.0f) { X /= l; Y /= l; Z /= l; } }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CThing {
    unsigned char m_flags91;
    C3DVector pos;
    const C3DVector& GetPosition() { return pos; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CAIBrain { CThing* PCreature; };

static int g_calls, g_results[8], g_ri, g_lastRepeat, g_lastMovement;
static C3DVector g_lastPos, g_lastFace;
struct CAIStateGroup_HoverMovementBase {
    CAIBrain* PBrain;
    CThing* GetCreature() const { return PBrain->PCreature; }
    long AIAction_MoveToPosAndFacePos(long repeat, const C3DVector& pos, const C3DVector& facePos, bool& arrived, long movement)
    {
        ++g_calls; g_lastRepeat = repeat; g_lastPos = pos; g_lastFace = facePos; g_lastMovement = movement; arrived = true;
        return g_results[g_ri++];
    }
    long AIAction_MoveToAndFaceThingAtDistance(long repeat, CBaseIntelligentPointer& thing, float distance, bool& arrived, long movement);
};

long CAIStateGroup_HoverMovementBase::AIAction_MoveToAndFaceThingAtDistance(long repeat, CBaseIntelligentPointer& thing,
                                                                            float distance, bool& arrived, long movement)
{
    if (!IsThingAlive(thing))
        return 3;
    if (repeat == 2) {
        CThing* me = GetCreature();
        const C3DVector& thingPos = thing.GetPItem()->GetPosition();
        const C3DVector& myPos = me->GetPosition();
        C3DVector dir(myPos.X - thingPos.X, myPos.Y - thingPos.Y, 0.0f);
        dir.Normalise();
        C3DVector offset = dir * distance;
        C3DVector target = thing.GetPItem()->GetPosition() + offset;
        return AIAction_MoveToPosAndFacePos(2, target, thing.GetPItem()->GetPosition(), arrived, 8);
    }
    if (repeat == 1) {
        for (;;) {
            CThing* me = GetCreature();
            const C3DVector& thingPos = thing.GetPItem()->GetPosition();
            const C3DVector& myPos = me->GetPosition();
            C3DVector dir(myPos.X - thingPos.X, myPos.Y - thingPos.Y, 0.0f);
            dir.Normalise();
            C3DVector offset = dir * distance;
            C3DVector target = thing.GetPItem()->GetPosition() + offset;
            long result = AIAction_MoveToPosAndFacePos(1, target, thing.GetPItem()->GetPosition(), arrived, movement);
            if (result != 4)
                return result;
        }
    }
    return 3;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static void Reset() { g_calls = g_ri = 0; for (int i = 0; i < 8; ++i) g_results[i] = 2; }

int main()
{
    int ok = 1;
    CThing me = { 0, C3DVector(10.0f, 0.0f, 5.0f) };
    CThing target = { 0, C3DVector(0.0f, 0.0f, 1.0f) };
    CAIBrain brain = { &me };
    CAIStateGroup_HoverMovementBase s; s.PBrain = &brain;
    CBaseIntelligentPointer p = { &target };
    bool arrived = false;
    // once: 3 units from the target towards the creature, flattened; movement forced to 8
    Reset(); g_results[0] = 7;
    if (s.AIAction_MoveToAndFaceThingAtDistance(2, p, 3.0f, arrived, 5) != 7) ok = 0;
    if (g_calls != 1 || g_lastRepeat != 2 || g_lastMovement != 8 || !arrived) ok = 0;
    if (!Near(g_lastPos.X, 3.0f) || !Near(g_lastPos.Y, 0.0f) || !Near(g_lastPos.Z, 1.0f) || !Near(g_lastFace.X, 0.0f)) ok = 0;
    // repeat until the move stops reporting 4; caller's movement passed through
    Reset(); g_results[0] = 4; g_results[1] = 4; g_results[2] = 1;
    if (s.AIAction_MoveToAndFaceThingAtDistance(1, p, 2.0f, arrived, 5) != 1) ok = 0;
    if (g_calls != 3 || g_lastRepeat != 1 || g_lastMovement != 5 || !Near(g_lastPos.X, 2.0f)) ok = 0;
    // other repeat modes: 3, nothing called
    Reset();
    if (s.AIAction_MoveToAndFaceThingAtDistance(0, p, 2.0f, arrived, 5) != 3 || g_calls != 0) ok = 0;
    // dead or missing thing: 3
    target.m_flags91 = 1; Reset();
    if (s.AIAction_MoveToAndFaceThingAtDistance(2, p, 2.0f, arrived, 5) != 3 || g_calls != 0) ok = 0;
    p.m_pItem = 0;
    if (s.AIAction_MoveToAndFaceThingAtDistance(1, p, 2.0f, arrived, 5) != 3 || g_calls != 0) ok = 0;
    if (!ok) { std::printf("0091d320_TEST FAIL\n"); return 1; }
    std::printf("0091d320_TEST PASS\n");
    return 0;
}
