#include <cstdio>
struct CLightDef { bool m_bStartActive; };
struct CTCLight {
    const CLightDef* PLightDef; bool Initialised; int calls; bool active;
    void SetActive(bool a) { ++calls; active = a; }
    void ApplyDefActiveState() { if (PLightDef && !Initialised) SetActive(PLightDef->m_bStartActive); }
};
int main()
{
    CLightDef d; d.m_bStartActive = true;
    CTCLight l; l.PLightDef = 0; l.Initialised = false; l.calls = 0; l.active = false;
    l.ApplyDefActiveState();
    if (l.calls != 0) { std::printf("00641480_TEST FAIL\n"); return 1; }
    l.PLightDef = &d; l.ApplyDefActiveState();
    if (l.calls != 1 || !l.active) { std::printf("00641480_TEST FAIL\n"); return 1; }
    l.Initialised = true; d.m_bStartActive = false; l.ApplyDefActiveState();
    if (l.calls != 1 || !l.active) { std::printf("00641480_TEST FAIL\n"); return 1; }
    std::printf("00641480_TEST PASS\n");
    return 0;
}
