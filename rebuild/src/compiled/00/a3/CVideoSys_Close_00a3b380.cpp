// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test hand-transcribed with renamed types; #pragma pack(1) only to reach offsets)
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