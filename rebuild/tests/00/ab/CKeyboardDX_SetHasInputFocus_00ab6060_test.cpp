// Behaviour test for 0x00ab6060 (CKeyboardDX::SetHasInputFocus): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CDevice {
    long stateResult[2]; int stateCalls; int acquires; int unacquires; long unacquireResult; long acquireResult; long pending; int dataCalls;
    long GetDeviceState(unsigned int size, void* data) { std::memset(data, 0x80, size); return stateResult[stateCalls++ > 0 ? 1 : 0]; }
    long Acquire() { ++acquires; return acquireResult; }
    long Unacquire() { ++unacquires; return unacquireResult; }
    long GetDeviceData(unsigned int, void*, unsigned int* count, unsigned int) { ++dataCalls; unsigned int n = pending > 4 ? 4 : (unsigned int)pending; pending -= n; *count = n; return 0; }
};
struct CKeyboardDX {
    CDevice* DKeyboard; bool Acquired;
    void SetHasInputFocus(bool focus)
    {
        unsigned int count; unsigned char data[256 * 20];
        if (!focus) {
            if (Acquired == true) { long r = DKeyboard->Unacquire(); if (r == 0 || r == 1) Acquired = false; }
        } else {
            long r = DKeyboard->Acquire(); bool was = Acquired; Acquired = r >= 0;
            if (!was) { count = 0x100; long read; do { if (count == 0) break; read = DKeyboard->GetDeviceData(20, data, &count, 0); } while (read >= 0); }
        }
    }
};
static void Run()
{
    CDevice d; std::memset(&d, 0, sizeof(d)); CKeyboardDX k; k.DKeyboard = &d; k.Acquired = false;
    d.pending = 10; k.SetHasInputFocus(true); CHECK(k.Acquired && d.acquires == 1 && d.pending == 0 && d.dataCalls == 4);
    d.dataCalls = 0; d.pending = 3; k.SetHasInputFocus(true); CHECK(k.Acquired && d.dataCalls == 0);
    d.unacquireResult = 2; k.SetHasInputFocus(false); CHECK(k.Acquired && d.unacquires == 1);
    d.unacquireResult = 1; k.SetHasInputFocus(false); CHECK(!k.Acquired && d.unacquires == 2);
    k.SetHasInputFocus(false); CHECK(d.unacquires == 2);
    d.acquireResult = -1; k.SetHasInputFocus(true); CHECK(!k.Acquired);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00ab6060_TEST FAIL\n"); return 1; }
    std::printf("00ab6060_TEST PASS\n");
    return 0;
}