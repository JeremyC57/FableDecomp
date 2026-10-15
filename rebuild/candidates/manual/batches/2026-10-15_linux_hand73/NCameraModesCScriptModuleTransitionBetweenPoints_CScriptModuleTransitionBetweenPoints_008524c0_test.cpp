// Behaviour test for 0x008524C0 (CScriptModuleTransitionBetweenPoints 5-argument constructor): PThing starts
// empty and SetUpTransitionCamera receives the arguments in the retail order.
#include <cstdio>

struct C3DVector { float X, Y, Z; };

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {
    CIntelligentPointer_CThingConst() : CBaseIntelligentPointer() { }
};

struct CScriptModuleBase {
    virtual ~CScriptModuleBase() { }
    unsigned char m_state[0x48];
};

static const C3DVector *g_a = 0, *g_b = 0, *g_c = 0, *g_d = 0;
static long g_frames = 0;
static float g_fov = 0.0f;
static int g_calls = 0;

struct CScriptModuleTransitionBetweenPoints : CScriptModuleBase {
    CIntelligentPointer_CThingConst PThing;
    C3DVector Increment, LookAtIncrement, LookAtPos, StartPos;
    long      EndFrame;
    float     FOV;
    bool      PreventSelfTermination;
    void SetUpTransitionCamera(const C3DVector& a, const C3DVector& b, const C3DVector& c, const C3DVector& d, long frames, float fov)
    {
        g_a = &a; g_b = &b; g_c = &c; g_d = &d; g_frames = frames; g_fov = fov; ++g_calls;
    }
    CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov);
};

CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov)
{
    SetUpTransitionCamera(pos, pos, startLookAt, endLookAt, frames, fov);
}

int main()
{
    C3DVector p0 = { 0, 0, 0 }, p1 = { 1, 1, 1 }, p2 = { 2, 2, 2 }, p3 = { 3, 3, 3 };
    (void)p3;
    CScriptModuleTransitionBetweenPoints m(p0, p1, p2, 12, 0.5f);
    if (g_calls != 1 || !(g_a == &p0 && g_b == &p0 && g_c == &p1 && g_d == &p2) || g_frames != 12 || g_fov != 0.5f || m.PThing.m_pItem != 0) {
        std::printf("008524c0_TEST FAIL\n");
        return 1;
    }
    std::printf("008524c0_TEST PASS\n");
    return 0;
}
