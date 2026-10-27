// behaviour test for 0x008B8A10 CCreatureAction_ProjectileCrouch::CCreatureAction_ProjectileCrouch(const CCreatureAction_ProjectileCrouch&) (model; the base copy constructor is a stub)
#include <cstdio>
#include <cstring>
static int g_copies = 0;
class CCreatureActionBase { public: virtual ~CCreatureActionBase() { } };
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase() { }
    CCombatActionBase(const CCombatActionBase&) { ++g_copies; }
};
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
class CCreatureAction_ProjectileCrouch : public CCombatActionBase {
public:
    CCreatureAction_ProjectileCrouch() { }
    CCreatureAction_ProjectileCrouch(const CCreatureAction_ProjectileCrouch& other);
    bool TurnToFaceTarget;                    // +0x114 (PDB +300)
    C3DVector FacePos;                        // +0x115 (PDB +301: the 1-aligned C3DVector)
};
CCreatureAction_ProjectileCrouch::CCreatureAction_ProjectileCrouch(const CCreatureAction_ProjectileCrouch& other) : CCombatActionBase(other)
    , TurnToFaceTarget(other.TurnToFaceTarget)
    , FacePos(other.FacePos) { }
int main()
{
    CCreatureAction_ProjectileCrouch a;
    a.TurnToFaceTarget = true;
    a.FacePos.X = 1.5f; a.FacePos.Y = 2.5f; a.FacePos.Z = 4.0f;
    CCreatureAction_ProjectileCrouch b(a);
    int ok = g_copies == 1 && b.TurnToFaceTarget && b.FacePos.X == 1.5f && b.FacePos.Z == 4.0f;
    if (!ok) { std::printf("008b8a10_TEST FAIL\n"); return 1; }
    std::printf("008b8a10_TEST PASS\n");
    return 0;
}