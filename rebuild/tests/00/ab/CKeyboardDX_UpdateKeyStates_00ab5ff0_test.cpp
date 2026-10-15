// Behaviour test for 0x00ab5ff0 (CKeyboardDX::UpdateKeyStates): the reconstructed logic run over a host model of its callees.
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
    CDevice* DKeyboard; unsigned char KeyStates[0x100]; bool Acquired;
    bool UpdateKeyStates()
    {
        std::memset(KeyStates, 0, sizeof(KeyStates));
        if (Acquired == true) {
            long result = DKeyboard->GetDeviceState(0x100, KeyStates);
            if (result == (long)0x8007001E) { DKeyboard->Acquire(); result = DKeyboard->GetDeviceState(0x100, KeyStates); }
            return result == 0 ? true : false;
        }
        return false;
    }
};
static void Run()
{
    CDevice d; std::memset(&d, 0, sizeof(d)); CKeyboardDX k; k.DKeyboard = &d; k.Acquired = false;
    std::memset(k.KeyStates, 1, sizeof(k.KeyStates));
    CHECK(!k.UpdateKeyStates() && k.KeyStates[0] == 0 && k.KeyStates[255] == 0 && d.stateCalls == 0);
    k.Acquired = true; CHECK(k.UpdateKeyStates() && k.KeyStates[10] == 0x80 && d.acquires == 0);
    d.stateCalls = 0; d.stateResult[0] = (long)0x8007001E; d.stateResult[1] = 0; CHECK(k.UpdateKeyStates() && d.acquires == 1 && d.stateCalls == 2);
    d.stateCalls = 0; d.stateResult[1] = -5; CHECK(!k.UpdateKeyStates());
}

int main()
{
    Run();
    if (g_fail) { std::printf("00ab5ff0_TEST FAIL\n"); return 1; }
    std::printf("00ab5ff0_TEST PASS\n");
    return 0;
}