// Behaviour test for 0x009D87F0 MicroThread::SwitchToMicroThread: the thread becomes active, its saved image is copied
// under the stack top, the sleep switches into it (here: the simulated thread grows its stack by 4 bytes), the grown image
// is saved back and the result is !Dead.
#include <cstdio>
#include <cstring>
struct ByteVector {
    unsigned char buf[64]; unsigned int n;
    unsigned char* begin() { return buf; }
    unsigned int size() const { return n; }
    void resize(unsigned int m) { if (m > n) std::memset(buf + n, 0, m - n); n = m; }
};
struct Info { bool Dead; ByteVector Stack; };
static unsigned char g_stack[128];
static Info* ActiveThread;
static unsigned char* SavedStackPointer;
static unsigned char* StackTop = g_stack + 128;
static void RestoreStack(ByteVector& s) { SavedStackPointer = StackTop - s.size(); std::memmove(SavedStackPointer, s.begin(), s.size()); }
static void PreserveStack(ByteVector& s) { unsigned int size = StackTop - SavedStackPointer; s.resize(size); std::memmove(s.begin(), SavedStackPointer, size); }
static void MicroThreadSleep()
{
    // the thread runs: it checks its image is in place, pushes 4 bytes, and may die
    SavedStackPointer -= 4; SavedStackPointer[0] = 0xAA; SavedStackPointer[3] = 0xBB;
    if (ActiveThread->Stack.size() == 6) ActiveThread->Dead = true;
}
static bool SwitchToMicroThread(Info* info)
{
    ActiveThread = info;
    RestoreStack(info->Stack);
    MicroThreadSleep();
    PreserveStack(info->Stack);
    return !info->Dead;
}
int main()
{
    Info t; t.Dead = false; t.Stack.n = 2; t.Stack.buf[0] = 1; t.Stack.buf[1] = 2;
    bool alive = SwitchToMicroThread(&t);
    if (!alive || ActiveThread != &t || t.Stack.size() != 6 || t.Stack.buf[0] != 0xAA || t.Stack.buf[3] != 0xBB
        || t.Stack.buf[4] != 1 || t.Stack.buf[5] != 2) { std::printf("009d87f0_TEST FAIL\n"); return 1; }
    alive = SwitchToMicroThread(&t);             // size 6 on entry: the simulated thread dies
    if (alive || t.Stack.size() != 10 || t.Stack.buf[8] != 1) { std::printf("009d87f0_TEST FAIL\n"); return 1; }
    std::printf("009d87f0_TEST PASS\n");
    return 0;
}