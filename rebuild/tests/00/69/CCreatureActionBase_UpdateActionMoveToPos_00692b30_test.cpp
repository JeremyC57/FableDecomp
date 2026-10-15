#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static long g_frame = 1000;
struct CWorld { static long GetFrame() { return g_frame; } };
static CWorld g_world;
struct CThing { C3DVector m_pos; const C3DVector& GetPosition() const { return m_pos; } CWorld* GetWorld() const { return &g_world; } };
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() { } CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } CThing* GetPItem() const { return m_pItem; } };
struct CCreatureActionBase {
    virtual ~CCreatureActionBase() { }
    virtual bool IsUsingAnimationMovement() { return m_animMovement; }
    virtual void UpdateActionForcedMovement() { ++m_forced; }
    bool m_animMovement; int m_forced;
    CBaseIntelligentPointer PCreature;
    C3DVector AnimMovementStep;
    long      NextMovementPosWorldFrame;
    C3DVector NextMovementPos;
    CCreatureActionBase() : m_animMovement(false), m_forced(0), NextMovementPosWorldFrame(0), NextMovementPos(0, 0, 0) { }
    void SetMoveToPos(const C3DVector& pos)
    {
        NextMovementPosWorldFrame = PCreature.GetPItem()->GetWorld()->GetFrame();
        NextMovementPos = pos;
    }
    void UpdateActionMoveToPos();
};
void CCreatureActionBase::UpdateActionMoveToPos()
{
    if (IsUsingAnimationMovement())
        SetMoveToPos(AnimMovementStep + PCreature.GetPItem()->GetPosition());
    else
        UpdateActionForcedMovement();
}
int main()
{
    CThing creature; creature.m_pos = C3DVector(10.0f, 20.0f, 30.0f);
    CCreatureActionBase action; action.PCreature.m_pItem = &creature; action.AnimMovementStep = C3DVector(1.0f, 2.0f, 3.0f);
    action.UpdateActionMoveToPos();
    bool ok = action.m_forced == 1 && action.NextMovementPosWorldFrame == 0 && action.NextMovementPos.X == 0.0f;
    action.m_animMovement = true; g_frame = 77; action.UpdateActionMoveToPos();
    ok = ok && action.m_forced == 1 && action.NextMovementPosWorldFrame == 77
        && action.NextMovementPos.X == 11.0f && action.NextMovementPos.Y == 22.0f && action.NextMovementPos.Z == 33.0f;
    if (!ok) { std::printf("00692b30_TEST FAIL\n"); return 1; }
    std::printf("00692b30_TEST PASS\n");
    return 0;
}