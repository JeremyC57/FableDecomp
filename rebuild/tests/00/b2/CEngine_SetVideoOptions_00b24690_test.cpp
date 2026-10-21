// behaviour test for 00b24690 CEngine::SetVideoOptions
#include <cstdio>
struct Opts { int X, Y, Bpp, Refresh, AA; bool VSync; int Mesh, Tex, Fx; };
struct Display {
    int okAfter;      // call index that first succeeds fully (2) ; earlier ones fail (0)
    int calls; int lastX, lastY; bool immediate;
    int SetDisplayMode(int x, int y) { ++calls; lastX = x; lastY = y; return calls >= okAfter ? (calls == 1 ? 2 : 1) : 0; }
};
struct Engine {
    bool IsInitialised, EnablePostDeviceReset; Opts VideoOptions; int restoreResult; int applied;
    bool SetVideoOptions(const Opts& o, Display& d)
    {
        if (!IsInitialised) return false;
        EnablePostDeviceReset = false;
        VideoOptions = o;
        bool result = true;
        if (d.SetDisplayMode(o.X, o.Y) != 2) {
            result = false;
            if (d.SetDisplayMode(o.X, o.Y) == 0) {
                int x = o.X, y = o.Y;
                if (x > 1024) x = 1024;
                if (y > 768) y = 768;
                if (d.SetDisplayMode(x, y) == 0) { EnablePostDeviceReset = true; return false; }
            }
        }
        d.immediate = !o.VSync;
        ++applied;
        if (restoreResult != 0) result = false;
        EnablePostDeviceReset = true;
        return result;
    }
};
int main()
{
    int bad = 0;
    Opts o = { 1600, 1200, 32, 60, 0, true, 1, 2, 3 };
    Engine e = { false, false };
    Display d = { 1 };
    if (e.SetVideoOptions(o, d) || d.calls) bad |= 1;
    e.IsInitialised = true;
    d.calls = 0; d.okAfter = 1;
    if (!e.SetVideoOptions(o, d) || !e.EnablePostDeviceReset || d.immediate || e.applied != 1) bad |= 2;
    d.calls = 0; d.okAfter = 3;
    if (e.SetVideoOptions(o, d) || d.lastX != 1024 || d.lastY != 768 || e.applied != 2) bad |= 4;
    d.calls = 0; d.okAfter = 4;
    if (e.SetVideoOptions(o, d) || !e.EnablePostDeviceReset || e.applied != 2) bad |= 8;
    d.calls = 0; d.okAfter = 1; e.restoreResult = 1;
    if (e.SetVideoOptions(o, d) || e.applied != 3) bad |= 16;
    if (bad) { std::printf("00b24690_TEST FAIL\n"); return 1; }
    std::printf("00b24690_TEST PASS\n");
    return 0;
}