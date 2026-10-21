// Behaviour test for retail 0x00c5ead0: candidate source embedded verbatim (imports turned into local spies), then a main.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __stdcall
#define __cdecl
#define __fastcall
#endif
#define __declspec(x)
#define CloseHandle Spy_CloseHandle
#define SetEvent Spy_SetEvent
#define WaitForSingleObject Spy_WaitForSingleObject
// CWound_00c5eca0::Kill() -- stop the kernel-streaming pin (connection state PAUSE then STOP), signal and join the worker thread, close every handle and free the buffers
// Leaf: Kill
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5EAD0, 241 bytes (+ int3 padding), plain `ret`.
// Identity: slot 5 of the RTTI-less vtable 0x012B914C (word at 0x012B9160 = 0x00C5EAD0); the landed destructor
//   0x00C5ECA0 (CWound_00c5eca0, "CWound" is a propagated class label) calls it first through that slot. The class is the
//   audio module's kernel-streaming output device; names are descriptive.
// Callees: CKsObject::SetProperty 0x00C5E050 (landed: KSPROPERTY set via DeviceIoControl) on the pin object at +0x0C with
//   KSPROPSETID_Connection @ 0x012B90D8, KSPROPERTY_CONNECTION_STATE (0) and a 4-byte KSSTATE; KERNEL32 SetEvent
//   [0x0143FDE0], WaitForSingleObject [0x0143FEAC], CloseHandle [0x0143FE30] (cached in ebx for the whole body); the
//   module's free wrapper 0x00C3C370 (__cdecl).
// Layout (as the landed destructor 0x00C5ECA0 reads it): pin CKsObject +0x0C (handle +0x10, -1 sentinel), block buffer
//   +0x14, event-handle array +0x18, sample buffer +0x1C, event count +0x20, queued count +0x24, stop event +0x2C
//   (handle +0x30), worker thread +0x34 (handle +0x38); null-sentinel handle holders close with an inline Close().
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);                                  // IAT 0x0143FE30
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                                      // IAT 0x0143FDE0
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long ms); // IAT 0x0143FEAC
void __cdecl FreeAudioBlock(void* block);                                                                  // 0x00C3C370

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern const GUID KSPROPSETID_Connection;                                                                  // 0x012B90D8

enum KSSTATE { KSSTATE_STOP = 0, KSSTATE_ACQUIRE = 1, KSSTATE_PAUSE = 2, KSSTATE_RUN = 3 };

struct CKsObject {                                  // vtable 0x012B90D4 / 0x012B9148
    virtual void v00();
    void* m_handle;                                 // +4 (-1 when closed)
    bool SetProperty(const GUID& propertySet, unsigned long id, void* value, unsigned long size);   // 0x00C5E050
    void Close() { if (m_handle != (void*)-1) { CloseHandle(m_handle); m_handle = (void*)-1; } }
};

struct CWin32Handle {                               // vtables 0x012B90E8 / 0x012B90EC: null sentinel
    virtual void v00();
    void* m_handle;                                 // +4
    void Close() { if (m_handle != 0) { CloseHandle(m_handle); m_handle = 0; } }
};

struct CWound_00c5eca0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Kill();                            // slot 5
    CKsObject m_unused04;                           // +0x04
    CKsObject m_pin;                                // +0x0c
    void* m_pBlocks;                                // +0x14
    void** m_pEvents;                               // +0x18
    void* m_pSamples;                               // +0x1c
    unsigned long m_eventCount;                     // +0x20
    long m_queued;                                  // +0x24
    long _28;                                       // +0x28
    CWin32Handle m_stopEvent;                       // +0x2c
    CWin32Handle m_thread;                          // +0x34
};

void CWound_00c5eca0::Kill()
{
    if (m_pin.m_handle != (void*)-1) {
        long state = KSSTATE_PAUSE;
        m_pin.SetProperty(KSPROPSETID_Connection, 0, &state, sizeof(state));
        state = KSSTATE_STOP;
        m_pin.SetProperty(KSPROPSETID_Connection, 0, &state, sizeof(state));
        SetEvent(m_stopEvent.m_handle);
        if (m_thread.m_handle != 0)
            WaitForSingleObject(m_thread.m_handle, 0xFFFFFFFF);
        m_thread.Close();
        m_stopEvent.Close();
        m_pin.Close();
    }
    m_queued = 0;
    if (m_pEvents != 0) {
        for (unsigned long i = 0; i < m_eventCount; ++i)
            CloseHandle(m_pEvents[i]);
        FreeAudioBlock(m_pEvents);
        m_pEvents = 0;
    }
    if (m_pBlocks != 0) {
        FreeAudioBlock(m_pBlocks);
        m_pBlocks = 0;
    }
    if (m_pSamples != 0) {
        FreeAudioBlock(m_pSamples);
        m_pSamples = 0;
    }
}

// ===== harness =====
static char g_log[256]; static int g_len;
static void logc(char c) { if (g_len < 255) g_log[g_len++] = c; }
static void* g_closed[16]; static int g_nclosed; static long g_states[4]; static int g_nstates;
static void* g_freed[8]; static int g_nfreed;
extern "C" int __stdcall CloseHandle(void* h) { g_closed[g_nclosed++ & 15] = h; logc('c'); return 1; }
extern "C" int __stdcall SetEvent(void* e) { logc('e'); return e != 0; }
extern "C" unsigned long __stdcall WaitForSingleObject(void* h, unsigned long ms) { logc(ms == 0xFFFFFFFF ? 'w' : 'W'); return 0; }
void __cdecl FreeAudioBlock(void* b) { g_freed[g_nfreed++ & 7] = b; logc('f'); }
const GUID KSPROPSETID_Connection = { 0x1D58C920, 0xAC9B, 0x11CF, { 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00 } };
bool CKsObject::SetProperty(const GUID& set, unsigned long id, void* value, unsigned long size)
{ logc('p'); if (&set == &KSPROPSETID_Connection && id == 0 && size == sizeof(long)) g_states[g_nstates++ & 3] = *(long*)value; return true; }
void CKsObject::v00() {}
void CWin32Handle::v00() {}
void CWound_00c5eca0::v00() {} void CWound_00c5eca0::v01() {} void CWound_00c5eca0::v02() {}
void CWound_00c5eca0::v03() {} void CWound_00c5eca0::v04() {}
int main()
{
    int pin, stop, thread, e0, e1, e2, blocks, samples; void* events[3] = { &e0, &e1, &e2 };
    CWound_00c5eca0 d;
    d.m_pin.m_handle = &pin; d.m_stopEvent.m_handle = &stop; d.m_thread.m_handle = &thread;
    d.m_pEvents = events; d.m_eventCount = 3; d.m_pBlocks = &blocks; d.m_pSamples = &samples; d.m_queued = 7;
    CWound_00c5eca0* p = &d; p->Kill();                 /* through the vtable */
    int ok = strncmp(g_log, "ppewccccccfff", 13) == 0 && g_len == 13 && g_nstates == 2 && g_states[0] == 2 && g_states[1] == 0
        && g_closed[0] == &thread && g_closed[1] == &stop && g_closed[2] == &pin && g_closed[3] == &e0 && g_closed[5] == &e2
        && g_freed[0] == events && g_freed[1] == &blocks && g_freed[2] == &samples
        && d.m_pin.m_handle == (void*)-1 && d.m_stopEvent.m_handle == 0 && d.m_thread.m_handle == 0
        && d.m_pEvents == 0 && d.m_pBlocks == 0 && d.m_pSamples == 0 && d.m_queued == 0;
    /* second call: everything already closed -> only the queued count is cleared */
    g_len = 0; d.m_queued = 5; p->Kill();
    ok = ok && g_len == 0 && d.m_queued == 0;
    if (!ok) { printf("00c5ead0_TEST FAIL %d %.*s\n", g_len, g_len, g_log); return 1; }
    printf("00c5ead0_TEST PASS\n");
    return 0;
}