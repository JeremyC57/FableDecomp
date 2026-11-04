// CTCRegionDisplay::GlobalUpdate(bool paused) -- fade the region-name banner: alpha ramps up over the first 0.2 s after the name changed and, when the banner times out, back down over the last 0.2 s of its display time
// Leaf: ?GlobalUpdate@CTCRegionDisplay@@UAEX_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006399B0, 218 bytes, `ret 4` (Ego_r.pdb ?GlobalUpdate@CTCRegionDisplay@@UAEX_N@Z, CTCBase slot 26;
//   the bool is unused).
// Callees: CWorld::GetFrame() 0x0049D870 (static __fastcall), __ftol2 0x00BFEA70; CMainGameComponent::GetConstantFPS() (inline: the
//   protected static ConstantFPS 0x01375550, `fild` in place); FableWin.pdb ?GetFrameNameChanged@CTCRegionDisplay@@QBEKXZ (inline,
//   unsigned: the frame delta converts through `fild ; jge ; fadd 2^32`). Float pool: 0.2f 0x0125A0E4, 255.0f 0x01230014.
// Layout: Ego_r.pdb CTCRegionDisplay (40 bytes): CurrentAreaName +0x0c, CurrentAreaNameExtra +0x10, FrameNameChanged +0x14,
//   CurrentAlpha +0x18, LastAlpha +0x1c, DisplayTime +0x20, TimeOut +0x24, InfoDisplay +0x25, UpdateWhilePaused +0x26.
// Shape: the fade-out tail is written `(long)((remaining / fadeFrames) * 255.0f)`: VC7.1 then keeps both `fdiv ; fmul ; __ftol2 ;
//   store ; ret 4` tails, where the unparenthesised spelling (the fade-in form) lets it cross-jump the second into the first.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

class CTCRegionDisplay : public CTCBase {                   // vtable 0x0125A0EC; no shared header
public:
    virtual void GlobalUpdate(bool paused);
    unsigned long GetFrameNameChanged() const { return FrameNameChanged; }
private:
    CWideString   CurrentAreaName;                          // +0x0c
    CWideString   CurrentAreaNameExtra;                     // +0x10
    long          FrameNameChanged;                         // +0x14
    long          CurrentAlpha;                             // +0x18
    long          LastAlpha;                                // +0x1c
    float         DisplayTime;                              // +0x20
    bool          TimeOut;                                  // +0x24
    bool          InfoDisplay;                              // +0x25
    bool          UpdateWhilePaused;                        // +0x26
};

void CTCRegionDisplay::GlobalUpdate(bool paused)
{
    if (!UpdateWhilePaused)
        return;
    LastAlpha = CurrentAlpha;
    float fps = (float)CMainGameComponent::GetConstantFPS();
    float displayFrames = fps * DisplayTime;
    float fadeFrames = fps * 0.2f;
    float elapsed = (float)(CWorld::GetFrame() - GetFrameNameChanged());
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
            CurrentAlpha = (long)((remaining / fadeFrames) * 255.0f);
    }
}