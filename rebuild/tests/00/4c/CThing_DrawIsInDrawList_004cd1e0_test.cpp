// Behaviour test for 004cd1e0 CThing::vfunc_30 (forwards to the virtual DrawIsDrawable)
#include <cstdio>
class CThing { public:
    bool drawable;
    virtual bool DrawIsDrawable() { return drawable; }
    virtual bool vfunc_30();
    virtual ~CThing() {}
};
bool CThing::vfunc_30()
{
    return DrawIsDrawable();
}
struct CNever : CThing { virtual bool DrawIsDrawable() { return false; } };
int main()
{
    CThing t; t.drawable = true;
    CNever n; n.drawable = true;
    if (!t.vfunc_30() || n.vfunc_30()) { std::printf("004cd1e0_TEST FAIL\n"); return 1; }
    t.drawable = false;
    if (t.vfunc_30()) { std::printf("004cd1e0_TEST FAIL\n"); return 1; }
    std::printf("004cd1e0_TEST PASS\n");
    return 0;
}