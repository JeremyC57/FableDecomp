// CEvent_00cb0190::CEvent_00cb0190(int manualReset) -- create an unnamed, initially unsignalled Win32 event (CreateEventW @ IAT 0x0143FDC8; the argument is the manual-reset flag) and keep its handle at +0; returns `this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0190, length 32 bytes.
// The chunk label "CHeroExperienceDef::Init" is propagated (a DX/CRT-side helper module); named by address. Shape: four pushes, `call [__imp]`, `mov [esi],eax`, `mov eax,esi`, `ret 4`.
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name);
struct CEvent_00cb0190 {
    void* m_hEvent;                                // +0x00
    CEvent_00cb0190(int manualReset);
};

CEvent_00cb0190::CEvent_00cb0190(int manualReset)
{
    m_hEvent = CreateEventW(0, manualReset, 0, 0);
}
