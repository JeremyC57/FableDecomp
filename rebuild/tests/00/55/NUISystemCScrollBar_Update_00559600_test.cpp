// Behaviour test for NUISystem::CScrollBar::Update (0x00559600): a finished drag without a held click snaps back once and raises the layer; the base update always runs.
#include <cstdio>
struct C2DVector { float X, Y; };
struct CDraggable {
    char Layer;
    bool WasLeftClicked;
    C2DVector PositionBeforeDrag;
    bool WasDragged;
    C2DVector m_position;
    float m_lastTime;
    int m_baseUpdates;
    int m_changes;
    virtual ~CDraggable() {}
    virtual void ChangePosition(const C2DVector& position, float time, bool relative)
    {
        m_position = position; m_lastTime = time; (void)relative; ++m_changes;
    }
    virtual void Update(float deltaTime) { ++m_baseUpdates; (void)deltaTime; }
};
struct CScrollBar : CDraggable {
    virtual void Update(float deltaTime)
    {
        if (WasDragged && !WasLeftClicked) {
            WasDragged = false;
            ChangePosition(PositionBeforeDrag, 0.0f, false);
            ++Layer;
        }
        CDraggable::Update(deltaTime);
    }
};
int main()
{
    CScrollBar b;
    b.Layer = 2; b.WasLeftClicked = false; b.WasDragged = true; b.m_baseUpdates = 0; b.m_changes = 0;
    b.PositionBeforeDrag.X = 4.0f; b.PositionBeforeDrag.Y = 8.0f; b.m_lastTime = -1.0f;
    b.Update(0.5f);
    if (b.WasDragged || b.Layer != 3 || b.m_changes != 1 || b.m_position.Y != 8.0f || b.m_lastTime != 0.0f || b.m_baseUpdates != 1) { std::printf("00559600_TEST FAIL\n"); return 1; }
    b.Update(0.5f);
    if (b.Layer != 3 || b.m_changes != 1 || b.m_baseUpdates != 2) { std::printf("00559600_TEST FAIL\n"); return 1; }
    b.WasDragged = true; b.WasLeftClicked = true;
    b.Update(0.5f);
    if (!b.WasDragged || b.m_changes != 1 || b.m_baseUpdates != 3) { std::printf("00559600_TEST FAIL\n"); return 1; }
    std::printf("00559600_TEST PASS\n");
    return 0;
}