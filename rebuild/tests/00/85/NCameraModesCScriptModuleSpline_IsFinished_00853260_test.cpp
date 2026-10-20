// Behaviour test for 00853260: body copied from src/00853260.cpp (NCameraModes::CScriptModuleSpline::IsFinished), GetPItem stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CBase; struct CGameCameraManager;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CThing_Retail {
    unsigned char  _pad00[0x91];
    unsigned char  m_dead : 1;
    unsigned char  m_91_1 : 7;
};
struct CIntelligentPointer_CThingConst {
    virtual void v00() {}
    void* m_pItem;
    CThing_Retail* GetPItem() const { return (CThing_Retail*)m_pItem; }
};
struct CScriptModuleBase {
    virtual void v00() {}
    CIntelligentPointer_CThingConst PCamPoint;
    CIntelligentPointer_CThingConst PThingOrigin;
    C3DVector           CoordOrigin;
    CRightHandedSet     CoordAxis;
    CBase*              POwner;
    CGameCameraManager* PCameraManager;
    bool                SelfTerminate;
    bool                ShortenOnCollision;
    long                CutOutOverride;
    float               FOVOverride;
};
namespace NCameraModes {
struct CScriptModuleSpline : CScriptModuleBase {
    float Index;
    float CurrentSplineFOV;
    float CurrentSplineRollAngle;
    bool  ReallyFinished;
    bool  FinishedSpline;
    long  NodeToStartOn;
    long  NodeToEndOn;
    unsigned char IsFinished() const;
};
unsigned char CScriptModuleSpline::IsFinished() const
{
    CThing_Retail* camPoint = PCamPoint.GetPItem();
    if (camPoint == 0)
        return true;
    if (camPoint->m_dead)
        return true;
    return ReallyFinished != 0;
}
}
int main()
{
    CThing_Retail cam; memset(&cam, 0, sizeof(cam));
    NCameraModes::CScriptModuleSpline s; s.PCamPoint.m_pItem = 0; s.ReallyFinished = false;
    // 1. no cam point: finished
    CHECK(s.IsFinished() == 1);
    // 2. dead cam point: finished
    s.PCamPoint.m_pItem = &cam; cam.m_dead = 1; CHECK(s.IsFinished() == 1);
    // 3. live cam point: the ReallyFinished flag decides
    cam.m_dead = 0; CHECK(s.IsFinished() == 0);
    s.ReallyFinished = true; CHECK(s.IsFinished() == 1);
    if (fails == 0) printf("00853260_TEST PASS\n"); else printf("00853260_TEST FAIL\n");
    return fails;
}