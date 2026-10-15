// Behaviour test for 0x004163B7 (CMainGameComponent::SetExclusiveMode): mode switch then PostChangeResolution(width, height, depth).
#include <cstdio>
static bool g_exclusive; static int g_notified;
struct Surface { long w, h; unsigned int depth; long Width() const { return w; } unsigned int GetHeight() const { return (unsigned int)h; } unsigned int Depth() const { return depth; } };
static Surface g_surface = { 1024, 768, 32 };
struct Component {
    unsigned int pw, ph, pd;
    void PostChangeResolution(unsigned int w, unsigned int h, unsigned int d) { pw = w; ph = h; pd = d; }
    void SetExclusiveMode(bool exclusive)
    {
        ++g_notified;
        g_exclusive = exclusive;
        Surface* surface = &g_surface;
        unsigned int width = (unsigned int)surface->Width();
        PostChangeResolution(width, surface->GetHeight(), g_surface.Depth());
    }
};
int main()
{
    Component c; c.SetExclusiveMode(true);
    int ok = g_exclusive && g_notified == 1 && c.pw == 1024 && c.ph == 768 && c.pd == 32;
    if (!ok) { std::printf("004163b7_TEST FAIL\n"); return 1; }
    std::printf("004163b7_TEST PASS\n");
    return 0;
}