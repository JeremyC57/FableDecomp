// CTCRegionDisplay::GlobalUpdate(bool paused) -- fade the region-name banner: alpha ramps up over the first 0.2 s after the name changed and, when the banner times out, back down over the last 0.2 s of its display time
// Grade: DIFFER(193v218) -- the lane tail-merges the two `fdiv ; fmul 255 ; __ftol2 ; store ; ret 4` alpha tails (jnp into the first one) where retail keeps both copies
// Retail: Fable.exe (TLC, VC7.1) @ 0x006399B0, 218 bytes (`ret 4`; PDB ?GlobalUpdate@CTCRegionDisplay@@UAEX_N@Z, the bool unused).
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static), __ftol2 @ 0x00BFEA70. Globals: g_framesPerSecond @ 0x01375550.
// Float pool: 0.2f @ 0x0125A0E4, 255.0f @ 0x01230014, 2^32 @ 0x0122DCB4 (the unsigned conversion of the frame delta).
// Shape: GetFrameNameChanged() returns unsigned long (PDB QBEKXZ), so the elapsed frames convert through `fild ; jge ; fadd 2^32`.
//   `!(elapsed < displayFrames)` is the `test ah,5 ; jnp` skip. Tried: early returns, if/else chain (both merge the tails).
#include <stddef.h>
struct CThing;
struct CWorld {
    static long GetWorldFrame();                            // 0x0049D870
};
extern long g_framesPerSecond;                              // 0x01375550
struct CWideString {
    void* m_pData;
    bool operator!=(const wchar_t* text) const;             // 0x0099B150
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator=(const wchar_t* text);      // 0x0099B800
};
struct CTCBase {                                            // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    unsigned char _pad0a[2];
    CWorld* GetWorld();                                     // 0x00686A70
};
struct CTCRegionDisplay : CTCBase {                         // vtable 0x0125A0EC
    CWideString   CurrentAreaName;                          // +0x0c
    CWideString   CurrentAreaNameExtra;                     // +0x10
    long          FrameNameChanged;                         // +0x14
    long          CurrentAlpha;                             // +0x18
    long          LastAlpha;                                // +0x1c
    float         DisplayTime;                              // +0x20
    bool          TimeOut;                                  // +0x24
    bool          InfoDisplay;                              // +0x25
    bool          UpdateWhilePaused;                        // +0x26
    virtual void GlobalUpdate(bool paused);
    unsigned long GetFrameNameChanged() const { return FrameNameChanged; }
    void SetCurrentAreaName(const CWideString& name, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused);
};

void CTCRegionDisplay::GlobalUpdate(bool paused)
{
    if (!UpdateWhilePaused)
        return;
    LastAlpha = CurrentAlpha;
    float fps = (float)g_framesPerSecond;
    float displayFrames = fps * DisplayTime;
    float fadeFrames = fps * 0.2f;
    float elapsed = (float)(CWorld::GetWorldFrame() - GetFrameNameChanged());
    if (!(elapsed < displayFrames) && TimeOut) {
        CurrentAlpha = 0;
        return;
    }
    if (elapsed < fadeFrames) {
        CurrentAlpha = (long)(elapsed / fadeFrames * 255.0f);
        return;
    }
    if (TimeOut) {
        float remaining = displayFrames - elapsed;
        if (remaining < fadeFrames)
            CurrentAlpha = (long)(remaining / fadeFrames * 255.0f);
    }
}
