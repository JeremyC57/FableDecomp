// Behaviour test for 0x00A62980 CCriticalSectionLock::Lock (NF415: linked): links the row object (the method is declared here, not
// defined). An unlocked lock enters its critical section through KERNEL32!EnterCriticalSection (the import the row calls) and sets the
// flag; a locked one does not enter again.
#include <cstdio>
#include "engine/CCriticalSectionLock.h"
#include "engine/Win32Types.h"
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION* section);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION* section);
struct CCriticalSectionLock_Methods : CCriticalSectionLock {
    void Lock();                                        // the row (linked)
};

int main()
{
    static CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    CCriticalSectionLock_Methods lock; lock.CriticalSection = &cs; lock.Locked = 0;
    lock.Lock();
    bool ok = lock.Locked == 1 && cs.RecursionCount == 1 && cs.OwningThread != 0;
    lock.Lock();                                        // already locked: no second enter
    ok = ok && lock.Locked == 1 && cs.RecursionCount == 1;
    if (cs.RecursionCount > 0) LeaveCriticalSection(&cs);
    DeleteCriticalSection(&cs);
    std::printf(ok ? "LOCK_OK\n" : "LOCK_FAIL\n");
    return ok ? 0 : 1;
}
