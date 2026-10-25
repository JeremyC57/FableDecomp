// PKCSemaphore::~PKCSemaphore() -- close the Win32 semaphore handle at +0 (CloseHandle)
// Leaf: ??1PKCSemaphore@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29FB0, 10 bytes (`mov eax,[ecx] ; push eax ; call [CloseHandle] ; ret`),
//   thiscall.
// Identity (sweep1, 2026-10-25): the destructor of the landed PKCSemaphore (constructor 0x00C29FC0, the next function,
//   creates the handle with CreateSemaphoreA into +0; Wait 0x00C2A090 / Release 0x00C2A150 use it; class name from the
//   file string ".\PKCSemaphore.cpp"): the KERNEL32 import at IAT 0x0143FE30 is CloseHandle. Its call sites are the
//   destructors and unwind funclets of the semaphore-holding libpk objects, e.g. ~PKCDefaultFileManager 0x00C1A230
//   (its +0x78 member, call 0x00C1A32F).
// Label correction: the landed row was CClass_00c29fb0::~CClass_00c29fb0 over an address-named import (catalog
//   "~CClass_00c29fb0"; manifest "~CPersistTraits<CReactionFrequencyTraitsArray>", propagated).
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // KERNEL32 import 0x0143FE30

class PKCSemaphore {
public:
    PKCSemaphore(long initialCount, long maximumCount);   // 0x00C29FC0
    ~PKCSemaphore();
    void Wait();                            // 0x00C2A090
    void Release();                         // 0x00C2A150
private:
    void* m_hSemaphore;                     // +0x00 the Win32 semaphore
};

PKCSemaphore::~PKCSemaphore()
{
    CloseHandle(m_hSemaphore);
}
