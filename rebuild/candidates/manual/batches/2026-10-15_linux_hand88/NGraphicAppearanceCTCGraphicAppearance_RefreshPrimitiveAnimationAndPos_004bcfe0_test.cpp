#include <cstdio>

static int g_animDtors = 0, g_baseDtors = 0;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float m[6]; };
struct CInterpolatorBase { int kind; ~CInterpolatorBase() { ++g_baseDtors; } };
struct C3DAnimationInterpolator { unsigned long StartState[2]; unsigned long EndState[2]; ~C3DAnimationInterpolator() { ++g_animDtors; } };
struct C3DPositionInterpolator { CInterpolatorBase m_base; C3DVector StartPosition; C3DVector EndPosition; };
struct C3DOrientationInterpolator { CInterpolatorBase m_base; CRightHandedSet StartOrientation; CRightHandedSet EndOrientation; };
struct CEnginePrimitiveAnimation {
    C3DAnimationInterpolator Anim; C3DPositionInterpolator WorldPos; C3DOrientationInterpolator Orientation;
    CEnginePrimitiveAnimation(const C3DAnimationInterpolator& a, const C3DPositionInterpolator& p, const C3DOrientationInterpolator& o)
        : Anim(a), WorldPos(p), Orientation(o) { }
};
struct CEnginePrimitiveHandle { long m_header; void* Data; };
struct CPrimitive { long Type; CEnginePrimitiveHandle PrimitiveHandle; long BankIndex; };
static CEnginePrimitiveHandle* g_handle = 0; static float g_endX = 0.0f; static unsigned long g_state = 0; static float g_orient = 0.0f;
struct CEnginePrimitiveManager {
    virtual ~CEnginePrimitiveManager() { }
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle* p, const CEnginePrimitiveAnimation& a)
    { g_handle = p; g_endX = a.WorldPos.EndPosition.X; g_state = a.Anim.EndState[1]; g_orient = a.Orientation.EndOrientation.m[5]; }
};
struct CEngineGlobals { CEnginePrimitiveManager* m_pPrimitiveManager; };
static CEnginePrimitiveManager g_manager;
static CEngineGlobals g_globals = { &g_manager };
static CEngineGlobals* g_pEngineGlobals = &g_globals;
struct CTCGraphicAppearance {
    void RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& animation,
                                         const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation)
    {
        g_pEngineGlobals->m_pPrimitiveManager->SetPrimitiveAnimation(&primitive->PrimitiveHandle,
                                                                     CEnginePrimitiveAnimation(animation, position, orientation));
    }
};

int main()
{
    CPrimitive prim; C3DAnimationInterpolator anim; anim.EndState[1] = 77;
    C3DPositionInterpolator pos; pos.EndPosition.X = 4.5f;
    C3DOrientationInterpolator ori; ori.EndOrientation.m[5] = -1.0f;
    CTCGraphicAppearance a;
    a.RefreshPrimitiveAnimationAndPos(&prim, anim, pos, ori);
    // the temporary animation is destroyed right after the call: one anim dtor + two interpolator-base dtors
    if (g_handle != &prim.PrimitiveHandle || g_endX != 4.5f || g_state != 77 || g_orient != -1.0f || g_animDtors != 1 || g_baseDtors != 2)
        { std::printf("004bcfe0_TEST FAIL\n"); return 1; }
    std::printf("004bcfe0_TEST PASS\n");
    return 0;
}
