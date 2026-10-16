// CTCRegionDisplay::FinishShowingRegionName() -- start the fade-out of the region name: unless it is already timing out past the fade point, back-date FrameNameChanged so the remaining display time equals the fade time, then flag TimeOut
// Leaf: FinishShowingRegionName | landing session, 3 tries: `elapsed > d` gives `test ah,5 ; jp`, `!(elapsed <= d)` gives `test ah,1 ; je`, `!(d >= elapsed)` gives `jnp` with the fild moved; retail `test ah,41h ; jne` not reproduced, parked
// Grade: DIFFER(115v115) -- compare operand load order only: retail `fild elapsed ; fld display ; fsub fade ; fcompp ; test ah,41h ; jne` (skip when display-fade <= elapsed), ours loads the difference first; next: write the test as `elapsed < displayFrames - fadeFrames` (4 attempts spent on the compare direction)
// Retail: Fable.exe 0x00639930, 115 bytes (plain `ret`).
// Callees: GFGetCurrentFrame() @ 0x0049D870 (`__fastcall`, manifest "GFGetPreMainMemoryUsed" is propagated) twice,
// __ftol2 @ 0x00BFEA70. Globals: the integer frame rate @ 0x01375550 (`fild`, shared by both products) and the
// fade-out time float @ 0x0125A0E4.
// Layout = Ego_r.pdb CTCRegionDisplay (0x28, as landed 0x00639B70): FrameNameChanged +0x14, DisplayTime +0x20,
// TimeOut +0x24.
// Shape: FrameNameChanged is read into edi before the products; the products are float locals.
extern long g_framesPerSecond;                              // 0x01375550
extern const float g_regionNameFadeTime;                    // 0x0125A0E4
long __fastcall GFGetCurrentFrame();                        // 0x0049D870

struct CWideString { void* m_pData; };
struct CTCRegionDisplay {
    virtual void v00();
    void*       ParentThing;                                // +0x04 (CTCBase)
    bool        ToBeRemoved;                                // +0x08
    bool        IsInGlobalUpdateTCs;                        // +0x09
    CWideString CurrentAreaName;                            // +0x0c
    CWideString CurrentAreaNameExtra;                       // +0x10
    long        FrameNameChanged;                           // +0x14
    long        CurrentAlpha;                               // +0x18
    long        LastAlpha;                                  // +0x1c
    float       DisplayTime;                                // +0x20
    bool        TimeOut;                                    // +0x24
    bool        InfoDisplay;                                // +0x25
    bool        UpdateWhilePaused;                          // +0x26
    void FinishShowingRegionName();
};

void CTCRegionDisplay::FinishShowingRegionName()
{
    long changed = FrameNameChanged;
    float displayFrames = g_framesPerSecond * DisplayTime;
    float fadeFrames = g_framesPerSecond * g_regionNameFadeTime;
    long elapsed = GFGetCurrentFrame() - changed;
    if (!TimeOut || displayFrames - fadeFrames < elapsed)
        FrameNameChanged = (long)(GFGetCurrentFrame() - (displayFrames - fadeFrames));
    TimeOut = true;
}
