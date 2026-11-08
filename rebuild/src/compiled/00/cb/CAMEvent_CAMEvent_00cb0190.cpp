// CAMEvent::CAMEvent(BOOL fManualReset) -- DirectShow strmbase wxutil.cpp: create an unnamed, initially unsignalled Win32 event (CreateEventW @ IAT 0x0143FDC8) and keep its handle
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/wxutil.cpp) ??0CAMEvent@@QAE@H@Z -- 28 bytes verified against Fable.exe 0x00CB0190
// Leaf: ??0CAMEvent@@QAE@H@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB0190, 32 bytes, thiscall, `ret 4`.
// Identity: the DX9 SDK wxutil.cpp CAMEvent ctor (one BOOL argument in the DX9 SDK; UNICODE build: CreateEvent = CreateEventW); called
//   with 0 / 1 / 1 for m_RenderEvent / m_ThreadSignal / m_evComplete by CBaseRenderer::CBaseRenderer 0x00CA61D0; its destructor
//   0x00CB01B0 is landed as ??1CAMEvent@@QAE@XZ (0x00CAF580). The catalog label "CHeroExperienceDef::Init" was wrong.
// Shape: four pushes, `call [__imp]`, `mov [esi],eax`, `mov eax,esi`, `ret 4`.
// History: landed as CEvent_00cb0190 (named by address); respelled in naming wave 32 (family dshow).
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name);
typedef int BOOL;                                  // windef.h
#define FALSE 0

class CAMEvent {
protected:
    void* m_hEvent;                                // +0x00 HANDLE
public:
    CAMEvent(BOOL fManualReset = FALSE);
};

CAMEvent::CAMEvent(BOOL fManualReset)
{
    m_hEvent = CreateEventW(0, fManualReset, FALSE, 0);
}
