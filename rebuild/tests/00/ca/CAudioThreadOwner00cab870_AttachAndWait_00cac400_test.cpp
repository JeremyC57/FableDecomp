#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00cac400.cpp) ----
// CAudioThreadOwner_00cab870::AttachAndWait() -- attach a fresh auto-reset event as the owner's wake handle, block until it is signalled and fold the wait outcome into an HRESULT (S_OK, the last-error HRESULT on WAIT_FAILED, E_UNEXPECTED otherwise)
// Leaf: ?AttachAndWait@CAudioThreadOwner_00cab870@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAC400, 149 bytes (+11 int3 padding), plain `ret` (thiscall, no arguments).
// Label: the catalog's "NHitLocations::CPhysicalPrimitiveCylinder_TransferBinaryOutAsync" is an autoname (labels_forge_autoname_t5)
// built on the propagated label of its callee; the callee 0x00CAB870 is landed as CAudioThreadOwner_00cab870::Attach(thread)
// (a statically linked COM-style helper class named by address), and `this` is passed straight to it, so this is a member
// of the same class -- descriptive name, see NOTES label corrections. Bounds are right (149 bytes, three duplicated tails).
// Callees: CreateEventW / WaitForSingleObject / CloseHandle (KERNEL32 IAT 0x0143FDC8 / 0x0143FEAC / 0x0143FE30),
// CAudioThreadOwner_00cab870::Attach @ 0x00CAB870, LastErrorToHResult @ 0x00CB0BC0 (AmGetLastErrorToHResult; tail-jumped
// to when the event cannot be created).
// Shape: a `switch` on the wait result (case 0 laid out last, WAIT_FAILED second, default first); each arm duplicates
// the CloseHandle + `hr >= 0 ? 0 : hr` tail (`setge ; dec ; and`).
// test: the Win32 entry points are renamed so the stubs below do not collide with kernel32.lib
#define CreateEventW TestCreateEventW
#define WaitForSingleObject TestWaitForSingleObject
#define CloseHandle TestCloseHandle
extern "C" void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name);
extern "C" unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);
extern "C" int __stdcall CloseHandle(void* handle);
int LastErrorToHResult_00cab870();   // test: HRESULTs are 32-bit (LP64 host)        // 0x00CB0BC0

struct CAudioThreadOwner_00cab870 {
    int Attach(void* thread);             // 0x00CAB870
    int AttachAndWait();
};

int CAudioThreadOwner_00cab870::AttachAndWait()
{
    void* event = CreateEventW(0, 0, 0, 0);
    if (!event)
        return LastErrorToHResult_00cab870();
    int hr = Attach(event);
    if (hr < 0) {
        CloseHandle(event);
        return hr;
    }
    switch (WaitForSingleObject(event, 0xFFFFFFFF)) {
    case 0:
        hr = 0;
        break;
    case 0xFFFFFFFF:
        hr = LastErrorToHResult_00cab870();
        break;
    default:
        hr = (int)0x8000FFFF;
        break;
    }
    CloseHandle(event);
    return hr >= 0 ? 0 : hr;
}

// ---- stubs for the external callees ----

static int g_eventStorage = 0;
static bool g_createFails = false;
static int g_attachResult = 0;
static unsigned long g_waitResult = 0;
static int g_closes = 0, g_waits = 0;
static void* g_attached = 0;
static void* g_closed = 0;
static unsigned long g_timeout = 0;
extern "C" void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name)
{
    if (attributes || manualReset || initialState || name) return 0;
    return g_createFails ? 0 : &g_eventStorage;
}
extern "C" unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds) { ++g_waits; g_timeout = milliseconds; return handle == &g_eventStorage ? g_waitResult : 1; }
extern "C" int __stdcall CloseHandle(void* handle) { ++g_closes; g_closed = handle; return 1; }
int LastErrorToHResult_00cab870() { return (int)0x80070005; }
int CAudioThreadOwner_00cab870::Attach(void* thread) { g_attached = thread; return g_attachResult; }

static void run()
{

    CAudioThreadOwner_00cab870 owner;
    g_createFails = true;
    CHECK(owner.AttachAndWait() == (int)0x80070005);          // no event: the last-error HRESULT
    CHECK(g_closes == 0 && g_waits == 0 && g_attached == 0);
    g_createFails = false;
    g_attachResult = (int)0x80040293;
    CHECK(owner.AttachAndWait() == (int)0x80040293);          // attach failed: event closed, its HRESULT
    CHECK(g_attached == &g_eventStorage && g_closes == 1 && g_closed == &g_eventStorage && g_waits == 0);
    g_attachResult = 1;                                       // S_FALSE still waits
    g_waitResult = 0;
    CHECK(owner.AttachAndWait() == 0);
    CHECK(g_waits == 1 && g_timeout == 0xFFFFFFFF && g_closes == 2);
    g_attachResult = 0;
    g_waitResult = 0xFFFFFFFF;
    CHECK(owner.AttachAndWait() == (int)0x80070005);          // WAIT_FAILED
    CHECK(g_closes == 3);
    g_waitResult = 0x102;
    CHECK(owner.AttachAndWait() == (int)0x8000FFFF);          // anything else: E_UNEXPECTED
    CHECK(g_closes == 4 && g_waits == 3);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00cac400_TEST FAIL\n"); return 1; }
    std::printf("00cac400_TEST PASS\n");
    return 0;
}