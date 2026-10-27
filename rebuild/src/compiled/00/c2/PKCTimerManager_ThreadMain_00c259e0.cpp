// PKCTimerManager::ThreadMain() -- the timer thread's loop: until the shutdown flag (+4) is raised, tick the timer target (+8, its slot 0) and sleep one period of 1000 / rate (+0xC) milliseconds
// Leaf: ?ThreadMain@PKCTimerManager@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C259E0, 55 bytes, thiscall, `ret`.
// Identity: slot 0 of the one-slot vtable 0x012B3FA0, whose only store is the constructor 0x00C25A20 (landed as
//   CVtable_012b3fa0) that PKCTimerManager's constructor 0x00C25B10 calls on `this` (the ".\PKCTimerManager.cpp"
//   warning strings) before it creates the worker thread over `this` (+0x14, @ 0x00C355E0); the landed
//   PKCTimerManager::InitiateShutdown 0x00C25A50 raises the same +4 flag this loop polls. The method name is
//   descriptive (libpk has no PDB or RTTI).
// Callees: _ftol2 @ 0x00BFEA70 (the float-to-long conversion), the target's slot 0, KERNEL32 Sleep (import 0x0143FE1C,
//   held in ebx across the loop).
// Shape: the period is computed once into a named local before the loop; `while (!flag)` compiles to a test before the
//   loop and a second test at its bottom; the flag is re-read after each call.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // 0x0143FE1C

struct PKCTimerTarget {
    virtual void Tick();                        // slot 0
};

struct PKCTimerManager {                        // vtable 0x012B3FA0 (constructor 0x00C25A20)
    virtual void ThreadMain();                  // slot 0
    bool            m_bShutdown;                // +0x04
    PKCTimerTarget* m_pTarget;                  // +0x08
    float           m_rate;                     // +0x0c ticks per second
    long            m_10;                       // +0x10
};

void PKCTimerManager::ThreadMain()
{
    long period = (long)(1000.0f / m_rate);
    while (!m_bShutdown) {
        m_pTarget->Tick();
        Sleep(period);
    }
}