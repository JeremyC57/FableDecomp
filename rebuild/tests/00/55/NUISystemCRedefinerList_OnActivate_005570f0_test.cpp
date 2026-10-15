// Behaviour test for NUISystem::CRedefinerList::OnActivate @ 0x005570F0: first activation refreshes and hides the component.
#include <cstdio>

struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };
struct CComponent { C2DVector m_pos; virtual C2DVector GetPosition() const { return m_pos; } virtual ~CComponent() {} };
struct CListRoot { virtual ~CListRoot() {} };
struct CGuiControl { virtual ~CGuiControl() {} };
struct CScrollableList : CListRoot, CGuiControl {
    int m_baseActivations, m_hides; CComponent* m_hidden; float m_start, m_end; C2DVector m_at; unsigned char m_alpha;
    void HideComponent(CComponent* c, float startTime, float endTime, C2DVector position, unsigned char alpha)
    { ++m_hides; m_hidden = c; m_start = startTime; m_end = endTime; m_at = position; m_alpha = alpha; }
    void OnActivate() { ++m_baseActivations; }
};
struct CRedefinerList : CScrollableList {
    bool m_bActive; int m_refreshes; CComponent* m_pComponent;
    void Refresh() { ++m_refreshes; }
    void OnActivate()
    {
        if (!m_bActive) {
            m_bActive = true;
            Refresh();
            if (m_pComponent)
                HideComponent(m_pComponent, 0.0f, 0.0f, m_pComponent->GetPosition(), 0x60);
        }
        CScrollableList::OnActivate();
    }
};

int main()
{
    CComponent c; c.m_pos = C2DVector(3.0f, 4.0f);
    CRedefinerList l; l.m_bActive = false; l.m_refreshes = 0; l.m_pComponent = &c; l.m_baseActivations = 0; l.m_hides = 0;
    l.OnActivate();
    if (!l.m_bActive || l.m_refreshes != 1 || l.m_hides != 1 || l.m_hidden != &c || l.m_at.X != 3.0f || l.m_at.Y != 4.0f ||
        l.m_alpha != 0x60 || l.m_start != 0.0f || l.m_baseActivations != 1) { std::printf("005570f0_TEST FAIL\n"); return 1; }
    l.OnActivate();                                   // already active: only the base runs
    if (l.m_refreshes != 1 || l.m_hides != 1 || l.m_baseActivations != 2) { std::printf("005570f0_TEST FAIL\n"); return 1; }
    CRedefinerList n; n.m_bActive = false; n.m_refreshes = 0; n.m_pComponent = 0; n.m_baseActivations = 0; n.m_hides = 0;
    n.OnActivate();                                   // no component: refresh but no hide
    if (!n.m_bActive || n.m_refreshes != 1 || n.m_hides != 0 || n.m_baseActivations != 1) { std::printf("005570f0_TEST FAIL\n"); return 1; }
    std::printf("005570f0_TEST PASS\n");
    return 0;
}