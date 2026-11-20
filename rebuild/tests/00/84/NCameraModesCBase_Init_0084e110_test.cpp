// Behaviour test for CCameraModeBase::Init (0x0084E110): reset, capture parameters when tracking is
// valid, then frame-update + camera roll for type-8 modes or valid tracking.
#include <cstdio>

class CCamera { public: float x, y, z; };
struct CCameraTracking {
    CCamera Current, Last; bool Valid;
    void RollCamera() { if (Valid) { Last = Current; Valid = true; } }
};
struct CMode {
    CCameraTracking* m_pTracking; bool m_parametersCaptured; float a, b, c; long type; int resets, updates;
    virtual ~CMode() {}
    virtual long GetType() { return type; }
    virtual float GetParameterA() { return 1.0f; }
    virtual float GetParameterB() { return 2.0f; }
    virtual float GetParameterC() { return 3.0f; }
    virtual void Reset() { ++resets; }
    void FrameUpdate() { ++updates; m_pTracking->Current.x += 10.0f; }
    void Init()
    {
        Reset();
        if (m_pTracking->Valid) { a = GetParameterA(); b = GetParameterB(); c = GetParameterC(); m_parametersCaptured = true; }
        if (GetType() == 8 || m_pTracking->Valid) { FrameUpdate(); m_pTracking->RollCamera(); }
    }
};

int main()
{
    CCameraTracking t = { { 1, 2, 3 }, { 0, 0, 0 }, false };
    CMode m; m.m_pTracking = &t; m.m_parametersCaptured = false; m.a = m.b = m.c = 0; m.type = 3; m.resets = m.updates = 0;
    m.Init();
    if (m.resets != 1 || m.updates != 0 || m.m_parametersCaptured) { std::printf("0084e110_TEST FAIL\n"); return 1; }
    m.type = 8; m.Init();
    if (m.updates != 1 || t.Last.x != 0.0f) { std::printf("0084e110_TEST FAIL\n"); return 1; }
    t.Valid = true; m.type = 3; m.Init();
    if (!m.m_parametersCaptured || m.c != 3.0f || m.updates != 2 || t.Last.x != 21.0f) { std::printf("0084e110_TEST FAIL\n"); return 1; }
    std::printf("0084e110_TEST PASS\n");
    return 0;
}