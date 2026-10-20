// Behaviour test for 0x0069F2B0 CCreatureAction_UseObject::FinishAction: with params, the named component is added with a
// physics params block holding the creature's position/orientation and the params are deleted and cleared.
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CTCParamsPhysicsBase { C3DVector Position; CRightHandedSet Rhs; CTCParamsPhysicsBase(const C3DVector& p, const CRightHandedSet& r) : Position(p), Rhs(r) { } };
static int g_deleted = 0;
struct CUseObjectParams { virtual ~CUseObjectParams() { ++g_deleted; } const char* GetTCName() { return "CTCUseable"; } };
struct CPhysics { C3DVector Position; CRightHandedSet rhs; const CRightHandedSet& GetRHS() { return rhs; } };
struct CThing {
    CPhysics physics; char added[32]; CTCParamsPhysicsBase* seen; float seenX, seenUpZ;
    CPhysics* GetPhysics() { return &physics; }
    void AddTC(const char* name, bool, CTCParamsPhysicsBase* p) { std::strcpy(added, name); seenX = p->Position.X; seenUpZ = p->Rhs.Up.Z; }
};
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
struct CAction {
    CPtr PCreature; CUseObjectParams* m_pParams; int baseFinish;
    void FinishAction()
    {
        if (m_pParams) {
            C3DVector position = PCreature.GetPItem()->GetPhysics()->Position;
            CRightHandedSet rhs = PCreature.GetPItem()->GetPhysics()->GetRHS();
            CTCParamsPhysicsBase params(position, rhs);
            CThing* creature = PCreature.GetPItem();
            creature->AddTC(m_pParams->GetTCName(), true, &params);
            delete m_pParams;
            m_pParams = 0;
        }
        ++baseFinish;
    }
};
int main()
{
    CThing c; c.added[0] = 0; c.physics.Position.X = 4.0f; c.physics.Position.Y = 5.0f; c.physics.Position.Z = 6.0f;
    c.physics.rhs.Up.X = 0; c.physics.rhs.Up.Y = 0; c.physics.rhs.Up.Z = 1.0f; c.physics.rhs.Forward = c.physics.Position;
    CAction a; a.PCreature.item = &c; a.m_pParams = new CUseObjectParams; a.baseFinish = 0;
    a.FinishAction();
    int ok = std::strcmp(c.added, "CTCUseable") == 0 && c.seenX == 4.0f && c.seenUpZ == 1.0f && g_deleted == 1 && a.m_pParams == 0 && a.baseFinish == 1;
    c.added[0] = 0; a.FinishAction();
    ok = ok && c.added[0] == 0 && g_deleted == 1 && a.baseFinish == 2;
    if (!ok) { std::printf("0069f2b0_TEST FAIL\n"); return 1; }
    std::printf("0069f2b0_TEST PASS\n");
    return 0;
}