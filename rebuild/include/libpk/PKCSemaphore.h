// PKCSemaphore -- libpk (Lionhead audio library; no PDB or RTTI covers it): a Win32 semaphore handle (4 bytes).
// Identity: the class name is the trace file string ".\PKCSemaphore.cpp" of the landed definers (libpk names its classes after
//   their files): constructor 0x00C29FC0 (CreateSemaphoreA(0, initialCount, maximumCount, 0), traced on failure), destructor
//   0x00C29FB0 (CloseHandle), Wait 0x00C2A090 (WaitForSingleObject INFINITE, traced unless WAIT_OBJECT_0) and Release 0x00C2A150
//   (ReleaseSemaphore(h, 1, 0), traced on failure). Layout and access as those definers declare them.
// Vendor naming V2 (plan phase 1, 2026-11-04): one shared model instead of ~75 local copies (`class` key as the definers; the
//   `struct` copies bound PAU spellings).
#ifndef FABLE_LIBPK_PKCSEMAPHORE_H
#define FABLE_LIBPK_PKCSEMAPHORE_H

class PKCSemaphore {
public:
    PKCSemaphore(long initialCount, long maximumCount);   // 0x00C29FC0
    ~PKCSemaphore();                        // 0x00C29FB0
    void Wait();                            // 0x00C2A090
    void Release();                         // 0x00C2A150
private:
    void* m_hSemaphore;                     // +0x00 the Win32 semaphore
};

#endif
