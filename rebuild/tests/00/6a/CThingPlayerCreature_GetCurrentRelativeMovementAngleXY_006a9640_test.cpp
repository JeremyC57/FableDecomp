#include <cstdio>
struct CTransform {
    float m_angle;
    float GetAngleXY() const { return m_angle; }
};
struct CEngineState { bool m_useTransform; };
struct CCamera {
    virtual float GetAngleXY() { return 2.5f; }
};
struct CDisplayEngine {
    virtual CTransform GetCameraTransform() { CTransform t; t.m_angle = 1.25f; return t; }
    CCamera*      m_pCamera;
    CEngineState  m_state;
    CEngineState* GetState() { return &m_state; }
};
static CDisplayEngine* g_engine;
struct CThingPlayerCreature {
    CDisplayEngine* GetDisplayEngine() { return g_engine; }
    float GetCurrentRelativeMovementAngleXY();
};
float CThingPlayerCreature::GetCurrentRelativeMovementAngleXY()
{
    CDisplayEngine* engine = GetDisplayEngine();
    if (engine->GetState()->m_useTransform)
        return engine->GetCameraTransform().GetAngleXY();
    return engine->m_pCamera->GetAngleXY();
}
int main()
{
    int bad = 0;
    CCamera cam; CDisplayEngine e; e.m_pCamera = &cam; e.m_state.m_useTransform = false; g_engine = &e;
    CThingPlayerCreature c;
    if (c.GetCurrentRelativeMovementAngleXY() != 2.5f) bad = 1;
    e.m_state.m_useTransform = true;
    if (c.GetCurrentRelativeMovementAngleXY() != 1.25f) bad = 2;
    if (bad) { std::printf("006a9640_TEST FAIL\n"); return 1; }
    std::printf("006a9640_TEST PASS\n");
    return 0;
}