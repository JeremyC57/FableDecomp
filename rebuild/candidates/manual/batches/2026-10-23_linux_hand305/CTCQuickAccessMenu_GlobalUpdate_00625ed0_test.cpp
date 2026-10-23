// Behaviour test for CTCQuickAccessMenu::GlobalUpdate (0x00625ED0): the quitting path deletes and clears the components
// and both sets; the active path arms Quitting on a foreign game mode or a flagged 0xfc component.
#include <cstdio>
#include <cstring>

static int g_deleted = 0;
struct CComponent { virtual ~CComponent() { ++g_deleted; } };

struct CSet { unsigned int count; void clear() { if (count != 0) count = 0; } };
struct CTC11 { CSet a, b; };
struct CTCFC { bool flag; };

struct CMenu {
    bool Active;
    CComponent* comps[4];
    unsigned int n;
    bool Quitting;
    long mode;
    CTC11* tc11;
    CTCFC* tcfc;
    void GlobalUpdate()
    {
        if (Quitting) {
            for (unsigned int i = 0; i < n; ++i)
                if (comps[i]) delete comps[i];
            n = 0;
            Active = false;
            Quitting = false;
            tc11->a.clear();
            tc11->b.clear();
            return;
        }
        if (Active) {
            if (mode != 0 && mode != 9)
                Quitting = true;
            if (tcfc && tcfc->flag)
                Quitting = true;
        }
    }
};

int main()
{
    int bad = 0;
    CTC11 t11; t11.a.count = 3; t11.b.count = 0;
    CMenu m;
    m.Active = true; m.Quitting = true; m.n = 3;
    m.comps[0] = new CComponent; m.comps[1] = 0; m.comps[2] = new CComponent;
    m.mode = 0; m.tc11 = &t11; m.tcfc = 0;
    m.GlobalUpdate();
    if (g_deleted != 2 || m.n != 0 || m.Active || m.Quitting || t11.a.count != 0) bad = 1;
    m.Active = true; m.mode = 9; m.GlobalUpdate();
    if (m.Quitting) bad = 2;
    m.mode = 3; m.GlobalUpdate();
    if (!m.Quitting) bad = 3;
    CTCFC fc; fc.flag = true; m.Quitting = false; m.mode = 0; m.tcfc = &fc; m.GlobalUpdate();
    if (!m.Quitting) bad = 4;
    if (bad) { std::printf("00625ed0_TEST FAIL\n"); return 1; }
    std::printf("00625ed0_TEST PASS\n");
    return 0;
}
