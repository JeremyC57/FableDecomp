// Behaviour test for CAIStateGroup_KidWatchHopscotch::IsRunnable (0x00941210): model copy with stub callees.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct CThingPhysicsMeshInfo { C3DVector ToCentre; float BoundingCircleRadius; };
struct CTCPhysicsBase { CThingPhysicsMeshInfo info; const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() { return &info; } };
struct CThing { C3DVector pos; CTCPhysicsBase* physics; const C3DVector& GetPosition() const { return pos; } };
struct CPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } CPointer& operator=(CThing* t) { m_pItem = t; return *this; } };
static long g_frame;
static CThing* g_nearestBoard;
static C3DVector g_searchPos;
struct CWorld { static long GetWorldFrame() { return g_frame; } };
struct CAIBrain { CThing* PCreature; };

struct CAIStateGroup_KidWatchHopscotch {
    CAIBrain* PBrain;
    CPointer PHopscotchBoard;
    C3DVector HopscotchCentre;
    float HopscotchRadius;
    long CreationFrame, FramesPerMin;
    bool Fatigued;
    long ActivationCount;
    float ActivationFrequency;
    bool GetPBestThing(const C3DVector& pos, CThing** out) { g_searchPos = pos; if (!g_nearestBoard) return false; *out = g_nearestBoard; return true; }
    bool IsRunnable();
};

bool CAIStateGroup_KidWatchHopscotch::IsRunnable()
{
    if (CWorld::GetWorldFrame() > CreationFrame) {
        if (Fatigued) {
            ActivationFrequency = (float)ActivationCount * FramesPerMin / (float)(CWorld::GetWorldFrame() - CreationFrame);
            if (ActivationFrequency < 2.5f)
                Fatigued = false;
        } else {
            ActivationFrequency = (float)ActivationCount * FramesPerMin / (float)(CWorld::GetWorldFrame() - CreationFrame);
            if (ActivationFrequency > 3.0f)
                Fatigued = true;
        }
    }
    if (!Fatigued) {
        {
            CThing* board;
            C3DVector pos = PBrain->PCreature->GetPosition();
            if (!GetPBestThing(pos, &board))
                return false;
            PHopscotchBoard = board;
        }
        CTCPhysicsBase* physics = PHopscotchBoard.GetPItem()->physics;
        const CThingPhysicsMeshInfo* mesh = physics->GetPhysicsMeshInfo();
        HopscotchCentre = mesh->ToCentre + PHopscotchBoard.GetPItem()->GetPosition();
        HopscotchRadius = mesh->BoundingCircleRadius;
        return true;
    }
    return false;
}

int main()
{
    int ok = 1;
    CTCPhysicsBase physics = { { C3DVector(1, 2, 3), 4.5f } };
    CThing board = { C3DVector(10, 20, 30), &physics };
    CThing kid = { C3DVector(7, 8, 9), 0 };
    CAIBrain brain = { &kid };
    CAIStateGroup_KidWatchHopscotch s;
    s.PBrain = &brain; s.PHopscotchBoard.m_pItem = 0; s.CreationFrame = 100; s.FramesPerMin = 60;
    s.Fatigued = false; s.ActivationCount = 2; s.ActivationFrequency = 0;
    g_nearestBoard = &board;
    // 2 activations in 60 frames at 60 frames/min = 2 per minute: not tired; caches the board
    g_frame = 160;
    if (!s.IsRunnable() || s.ActivationFrequency != 2.0f || s.Fatigued) ok = 0;
    if (s.PHopscotchBoard.m_pItem != &board || s.HopscotchCentre.X != 11 || s.HopscotchCentre.Y != 22 || s.HopscotchCentre.Z != 33 || s.HopscotchRadius != 4.5f) ok = 0;
    if (g_searchPos.X != 7 || g_searchPos.Z != 9) ok = 0;
    // 4 in 60 frames: tired
    s.ActivationCount = 4; if (s.IsRunnable() || !s.Fatigued) ok = 0;
    // stays tired at 2.5 per minute exactly, rests below
    s.ActivationCount = 5; g_frame = 220; if (s.IsRunnable() || !s.Fatigued) ok = 0;
    s.ActivationCount = 4; if (!s.IsRunnable() || s.Fatigued) ok = 0;
    // before the creation frame nothing is recomputed
    s.Fatigued = true; g_frame = 100; if (s.IsRunnable()) ok = 0;
    s.Fatigued = false;
    // no board found
    g_frame = 220; s.ActivationCount = 0; g_nearestBoard = 0; if (s.IsRunnable()) ok = 0;
    if (!ok) { std::printf("00941210_TEST FAIL\n"); return 1; }
    std::printf("00941210_TEST PASS\n");
    return 0;
}
