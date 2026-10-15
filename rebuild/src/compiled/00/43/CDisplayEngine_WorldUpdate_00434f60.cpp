// CDisplayEngine::WorldUpdate() -- count down the initial-fade readiness (stopping the progress display and clearing its two strings when it reaches zero), keep the about-to-run state while it is pending, and publish the engine's primitive draw-distance factor minus one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434F60, length 104 bytes.
// Callees: StopProgress(bool, bool) @ 0x009E9F20 (`__fastcall`, cl/dl), CWideString::operator=(const wchar_t*) @ 0x0099B800 (on the
// global strings @ 0x013B89D8 / 0x013B89DC, with the empty literal @ 0x0122D70C), CDisplayEngine::SetAboutToRun() @ 0x00434CD0,
// CIEngine::GetPrimitiveDrawDistanceFactor (slot 19, `call [eax+4Ch]`). Globals: the float @ 0x013B8774; 1.0f @ 0x0122DED8.
// Layout: engine/CDisplayEngine.h (PDB, fits the bytes): Engine3D +0x2c, InitialFadeReadynessCount +0xe8.
// Shape: `--count == 0` inside the `> 0` test (the decremented value stored then tested), the count re-read for the second test;
// the engine pointer is loaded and null-tested after esi is restored, the float result minus 1.0f stored to the global.
#include "engine/CDisplayEngine.h"

struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual float GetPrimitiveDrawDistanceFactor();   // slot 19 (+0x4c)
};

struct CWideString {
    unsigned short* m_pData;
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800
};

void __fastcall StopProgress(bool stopNow, bool keepScreen);   // 0x009E9F20

extern CWideString g_progressTitle;                // 0x013B89D8
extern CWideString g_progressText;                 // 0x013B89DC
extern float g_primitiveDrawDistanceScale;         // 0x013B8774

struct CDisplayEngine_Methods : CDisplayEngine {
    void SetAboutToRun();                          // 0x00434CD0
    void WorldUpdate();
};

void CDisplayEngine_Methods::WorldUpdate()
{
    if (InitialFadeReadynessCount > 0) {
        if (--InitialFadeReadynessCount == 0) {
            StopProgress(true, false);
            g_progressTitle = (const unsigned short*)L"";
            g_progressText = (const unsigned short*)L"";
        }
    }
    if (InitialFadeReadynessCount > 0)
        SetAboutToRun();
    if (Engine3D != 0)
        g_primitiveDrawDistanceScale = Engine3D->GetPrimitiveDrawDistanceFactor() - 1.0f;
}