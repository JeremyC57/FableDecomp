// CSemaphore::CSemaphore(long initialCount, long maximumCount) -- wraps a Win32 semaphore: m_handle = CreateSemaphoreA(0, initialCount, maximumCount, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36120, length 32 bytes.
// Callees: CreateSemaphoreA (kernel32 import, IAT slot 0x0143FEA8).
// The manifest label ("CActionParam::CActionParam_InitSemaphore") names the inlining context
// (CActionParam's constructor @ 0x00C2A230 calls this on its member at +4 with (0, 1)); the body is
// the two-argument constructor of a 4-byte handle holder: the arguments are pushed around two null
// literals (right to left: lpName = 0, maximum, initial, lpSecurityAttributes = 0), the handle is
// stored at +0 and `this` is returned (ret 8).
extern "C" __declspec(dllimport) void* __stdcall CreateSemaphoreA(void* attributes, long initialCount, long maximumCount, const char* name);   // IAT 0x0143FEA8

struct CSemaphore {
    void* m_handle;                                              // +0x00
    CSemaphore(long initialCount, long maximumCount);
};

CSemaphore::CSemaphore(long initialCount, long maximumCount)
{
    m_handle = CreateSemaphoreA(0, initialCount, maximumCount, 0);
}
