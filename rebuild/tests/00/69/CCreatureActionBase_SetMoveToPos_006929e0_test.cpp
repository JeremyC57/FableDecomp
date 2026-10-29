#include <cstdio>
class C3DVector { public: float X; float Y; float Z; };
static int g_calls = 0;
static long g_worldFrame = 1234;
struct CThingCreatureBase_Calls {
    void* Call_004c7ab0() { ++g_calls; return 0; }
};
struct CBaseIntelligentPointer_Calls {
    CThingCreatureBase_Calls* m_pItem;
    void* m_pInfo;
    CThingCreatureBase_Calls* GetPItem() { return m_pItem; }
};
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
long CWorld::GetFrame() { return g_worldFrame; }
class CCreatureActionBase { public:
    virtual void v00() { }
    unsigned char _pad04[4];
    CBaseIntelligentPointer_Calls PCreature;
    unsigned char _pad10[0x54];
    long      NextMovementPosWorldFrame;
    C3DVector NextMovementPos;
    void SetMoveToPos(const C3DVector& pos);
};
void CCreatureActionBase::SetMoveToPos(const C3DVector& pos)
{
    PCreature.GetPItem()->Call_004c7ab0();
    NextMovementPosWorldFrame = CWorld::GetFrame();
    NextMovementPos = pos;
}
int main()
{
    CThingCreatureBase_Calls creature;
    CCreatureActionBase action;
    action.PCreature.m_pItem = &creature;
    C3DVector p = { 1.5f, -2.0f, 3.25f };
    action.SetMoveToPos(p);
    if (g_calls != 1 || action.NextMovementPosWorldFrame != 1234 || action.NextMovementPos.X != 1.5f || action.NextMovementPos.Y != -2.0f || action.NextMovementPos.Z != 3.25f) { std::printf("006929e0_TEST FAIL\n"); return 1; }
    std::printf("006929e0_TEST PASS\n");
    return 0;
}