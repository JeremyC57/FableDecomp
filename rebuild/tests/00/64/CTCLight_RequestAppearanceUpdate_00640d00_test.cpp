#include <cstdio>
struct CLightDef { bool m_bNeedsAppearanceUpdate; };
struct CThing { int calls; long last; void ForwardToGraphicAppearance(long r) { ++calls; last = r; } };
struct CTCLight {
    CThing* m_pThing; const CLightDef* PLightDef; bool Active;
    void RequestAppearanceUpdate() { if (Active && PLightDef->m_bNeedsAppearanceUpdate) m_pThing->ForwardToGraphicAppearance(4); }
};
int main()
{
    CThing t; t.calls = 0; t.last = 0; CLightDef d; d.m_bNeedsAppearanceUpdate = true;
    CTCLight l; l.m_pThing = &t; l.PLightDef = &d; l.Active = false;
    l.RequestAppearanceUpdate();
    if (t.calls != 0) { std::printf("00640d00_TEST FAIL\n"); return 1; }
    l.Active = true; l.RequestAppearanceUpdate();
    if (t.calls != 1 || t.last != 4) { std::printf("00640d00_TEST FAIL\n"); return 1; }
    d.m_bNeedsAppearanceUpdate = false; l.RequestAppearanceUpdate();
    if (t.calls != 1) { std::printf("00640d00_TEST FAIL\n"); return 1; }
    std::printf("00640d00_TEST PASS\n");
    return 0;
}