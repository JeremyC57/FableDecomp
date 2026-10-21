#include <cstdio>
// Behaviour test for CMouseDX::SetExclusiveMode (0x00AB5710): the old device is released, the new one is created with the
// mode's cooperative level, SetDataFormat falls back to the older mouse format, and any failure returns false.
static int g_failAt = -1, g_step = 0, g_released = 0, g_cleared = 0, g_resets = 0;
static unsigned long g_level = 0;
static long Step(int id) { ++g_step; return id == g_failAt ? 1 : 0; }
struct Device {
    void Unacquire() { }
    void Release() { ++g_released; }
    long SetCooperativeLevel(unsigned long level) { g_level = level; return Step(1); }
    long SetProperty(unsigned long size) { return size == 0x100 ? Step(2) : 1; }
    long SetDataFormat(int which) { return Step(which == 2 ? 3 : 4); }
    long Acquire() { return Step(5); }
};
static Device g_device;
struct CMouseDX {
    Device* DMouse; bool Acquired; bool ExclusiveMode;
    long CreateDevice(Device** out) { *out = &g_device; return Step(0); }
    bool SetExclusiveMode(bool exclusive)
    {
        if (DMouse) { DMouse->Unacquire(); DMouse->Release(); DMouse = 0; }
        ++g_cleared;
        if (exclusive) {
            ExclusiveMode = true;
            if (CreateDevice(&DMouse) == 0) {
                if (DMouse->SetCooperativeLevel(5) == 0) {
                    if (DMouse->SetProperty(0x100) == 0
                        && (DMouse->SetDataFormat(2) == 0 || DMouse->SetDataFormat(1) == 0)
                        && DMouse->Acquire() == 0) {
                        Acquired = true;
                        return true;
                    }
                }
            }
        } else {
            if (CreateDevice(&DMouse) == 0) {
                if (DMouse->SetCooperativeLevel(10) == 0) {
                    if (DMouse->SetProperty(0x100) == 0
                        && (DMouse->SetDataFormat(2) == 0 || DMouse->SetDataFormat(1) == 0)
                        && DMouse->Acquire() == 0) {
                        Acquired = true;
                        ++g_resets;
                        ExclusiveMode = false;
                        return true;
                    }
                }
            }
        }
        return false;
    }
};
int main()
{
    CMouseDX m = { 0, false, false };
    int bad = 0;
    bad |= !m.SetExclusiveMode(true) || !m.ExclusiveMode || !m.Acquired || g_level != 5 || g_step != 5;
    g_step = 0; m.Acquired = false;
    bad |= !m.SetExclusiveMode(false) || m.ExclusiveMode || !m.Acquired || g_level != 10 || g_released != 1 || g_resets != 1;
    g_failAt = 3; g_step = 0;                 // first data format fails, fallback succeeds
    bad |= !m.SetExclusiveMode(true) || g_step != 6;
    g_failAt = 1; g_step = 0; m.Acquired = false;
    bad |= m.SetExclusiveMode(false) || m.Acquired || g_resets != 1 || g_step != 2;
    if (bad) { std::printf("00ab5710_TEST FAIL\n"); return 1; }
    std::printf("00ab5710_TEST PASS\n");
    return 0;
}