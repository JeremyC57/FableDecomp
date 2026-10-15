// Behaviour test for 0x00556980 CRedefinerList::NotifyOfRedefinition: every non-null redefiner except the source clears the control, then the warning updates.
#include <cstdio>
struct Control { int id; };
static int g_cleared[4]; static float g_warning = 0;
struct Redefiner { int idx; void ClearDuplicateDefinitions(const Control& c) { g_cleared[idx] = c.id; } };
struct Entry { Redefiner* r; int data; };
struct List {
    Entry* start; Entry* finish;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void UpdateUndefinedWarning(float d) { g_warning = d; }
    void NotifyOfRedefinition(const Control& control, const Redefiner* source)
    {
        for (unsigned int i = 0; i < size(); ++i) {
            Redefiner* r = start[i].r;
            if (r && r != source) r->ClearDuplicateDefinitions(control);
        }
        UpdateUndefinedWarning(0.2f);
    }
};
int main()
{
    Redefiner a = { 0 }, b = { 1 }, c = { 2 };
    Entry e[4] = { { &a, 0 }, { 0, 0 }, { &b, 0 }, { &c, 0 } };
    List l; l.start = e; l.finish = e + 4;
    Control ctl = { 42 };
    l.NotifyOfRedefinition(ctl, &b);
    bool ok = g_cleared[0] == 42 && g_cleared[1] == 0 && g_cleared[2] == 42 && g_warning == 0.2f;
    if (!ok) { std::printf("00556980_TEST FAIL\n"); return 1; }
    std::printf("00556980_TEST PASS\n");
    return 0;
}