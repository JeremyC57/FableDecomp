// Behaviour test for retail 0x00a3b380: the candidate source below is embedded verbatim (from src/00a3b380.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CVideoSys::Close() -- release the DirectShow interfaces, then close the render event and delete the critical section
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3B380, 162 bytes. Each interface is released through IUnknown::Release (COM slot 2,
//   __stdcall) and nulled; KERNEL32 imports CloseHandle (IAT 0x0143FE30) and DeleteCriticalSection (IAT 0x0143FDE4).
// Layout: PDB (Ego_r) CVideoSys names/types: the nine interface pointers +0x00..+0x20 match; the TLC object is 8 bytes
//   shorter before the critical section (CriticalSection +0x64, RenderEvent +0x7c; PDB +0x6c/+0x84), the bytes between
//   (PlayMode .. pTextBank) are opaque here. No packing is needed. Local model: no engine/ header for CVideoSys, and the
//   lane has no Windows SDK headers, so the COM and KERNEL32 declarations are written out.
struct _GUID;
struct IUnknown {
    virtual long __stdcall QueryInterface(const _GUID& iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();          // slot 2 (+0x08)
};
struct IGraphBuilder : IUnknown {};
struct IMediaControl : IUnknown {};
struct IMediaPosition : IUnknown {};
struct IMediaSeeking : IUnknown {};
struct IMediaEvent : IUnknown {};
struct IBasicAudio : IUnknown {};
struct IBaseFilter : IUnknown {};
struct IFileSourceFilter : IUnknown {};
struct CTextureRenderer;
struct _RTL_CRITICAL_SECTION {
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section);
struct CVideoSys {
    IGraphBuilder*      GBuilder;                       // +0x00
    IMediaControl*      MControl;                       // +0x04
    IMediaPosition*     MPosition;                      // +0x08
    IMediaSeeking*      MSeeking;                       // +0x0c
    IMediaEvent*        MEvent;                         // +0x10
    IBasicAudio*        BasicAudio;                     // +0x14
    IBaseFilter*        PAsfReader;                     // +0x18
    IFileSourceFilter*  PFileSource;                    // +0x1c
    CTextureRenderer*   TexRenderer;                    // +0x20
    unsigned char       PlayMode_to_pTextBank[0x40];    // +0x24..+0x63
    _RTL_CRITICAL_SECTION CriticalSection;              // +0x64
    void*               RenderEvent;                    // +0x7c
    void Close();
};
void CVideoSys::Close()
{
    if (MControl) { MControl->Release(); MControl = 0; }
    if (MPosition) { MPosition->Release(); MPosition = 0; }
    if (MSeeking) { MSeeking->Release(); MSeeking = 0; }
    if (BasicAudio) { BasicAudio->Release(); BasicAudio = 0; }
    if (PFileSource) { PFileSource->Release(); PFileSource = 0; }
    if (PAsfReader) { PAsfReader->Release(); PAsfReader = 0; }
    if (MEvent) { MEvent->Release(); MEvent = 0; }
    if (GBuilder) { GBuilder->Release(); GBuilder = 0; }
    if (RenderEvent) {
        CloseHandle(RenderEvent);
        RenderEvent = 0;
        DeleteCriticalSection(&CriticalSection);
    }
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void* attrs, int manual, int initial, const char* name);
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION* section);
static int g_order[16]; static int g_n;
long __stdcall IUnknown::QueryInterface(const _GUID&, void**) { return -1; }   /* base slots: never reached */
unsigned long __stdcall IUnknown::AddRef() { return 0; }
unsigned long __stdcall IUnknown::Release() { g_order[g_n++] = -1; return 0; }
struct TCom : IGraphBuilder {      /* one spy class serves every interface slot (all share the IUnknown vtable shape) */
    int id; unsigned long refs;
    virtual long __stdcall QueryInterface(const _GUID&, void**) { return -1; }
    virtual unsigned long __stdcall AddRef() { return ++refs; }
    virtual unsigned long __stdcall Release() { g_order[g_n++] = id; return --refs; }
};
int main() {
    static TCom c[8]; for (int i = 0; i < 8; ++i) { c[i].id = i; c[i].refs = 1; }
    CVideoSys v; memset(&v, 0, sizeof v);
    int ok = offsetof(CVideoSys, CriticalSection) == 0x64 && offsetof(CVideoSys, RenderEvent) == 0x7c && sizeof(_RTL_CRITICAL_SECTION) == 24;
    v.GBuilder = &c[0]; v.MControl = (IMediaControl*)(IUnknown*)&c[1]; v.MPosition = (IMediaPosition*)(IUnknown*)&c[2];
    v.MSeeking = (IMediaSeeking*)(IUnknown*)&c[3]; v.MEvent = (IMediaEvent*)(IUnknown*)&c[4]; v.BasicAudio = (IBasicAudio*)(IUnknown*)&c[5];
    v.PAsfReader = (IBaseFilter*)(IUnknown*)&c[6]; v.PFileSource = (IFileSourceFilter*)(IUnknown*)&c[7];
    v.TexRenderer = (CTextureRenderer*)0x99;
    InitializeCriticalSection(&v.CriticalSection);
    void* ev = CreateEventA(0, 1, 0, 0); v.RenderEvent = ev;
    v.Close();
    static const int expect[8] = { 1, 2, 3, 5, 7, 6, 4, 0 };   /* MControl, MPosition, MSeeking, BasicAudio, PFileSource, PAsfReader, MEvent, GBuilder */
    ok = ok && ev != 0 && g_n == 8;
    for (int i = 0; ok && i < 8; ++i) ok = g_order[i] == expect[i] && c[i].refs == 0;
    ok = ok && !v.GBuilder && !v.MControl && !v.MPosition && !v.MSeeking && !v.MEvent && !v.BasicAudio && !v.PAsfReader && !v.PFileSource;
    ok = ok && v.TexRenderer == (CTextureRenderer*)0x99 && v.RenderEvent == 0 && CloseHandle(ev) == 0;   /* already closed */
    g_n = 0; v.Close(); ok = ok && g_n == 0;                                                            /* idempotent */
    printf(ok ? "00a3b380_TEST PASS\n" : "00a3b380_TEST FAIL\n"); return ok ? 0 : 1;
}