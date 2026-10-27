// CSemaphore::Release() -- signal the wrapped Win32 semaphore once: ReleaseSemaphore(m_handle, 1, 0)
// Leaf: ?Release@CSemaphore@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36170, 14 bytes, thiscall, plain `ret`.
// Identity: the handle holder at +0 is the landed CSemaphore (ctor 0x00C36120 = CreateSemaphoreA(0, initial, maximum, 0));
//   this body releases one count through KERNEL32 ReleaseSemaphore (IAT 0x0143FEB0, bound __imp__ReleaseSemaphore@12).
//   The catalog label "SemaphoreHandle_Release" is descriptive; the method name is descriptive (libpk, no PDB).
// Shape: arguments pushed right to left (0, 1, handle), `call [__imp_]`.
extern "C" __declspec(dllimport) int __stdcall ReleaseSemaphore(void* semaphore, long releaseCount, long* previousCount);   // IAT 0x0143FEB0

struct CSemaphore {
    void* m_handle;                                              // +0x00
    void Release();
};

void CSemaphore::Release()
{
    ReleaseSemaphore(m_handle, 1, 0);
}