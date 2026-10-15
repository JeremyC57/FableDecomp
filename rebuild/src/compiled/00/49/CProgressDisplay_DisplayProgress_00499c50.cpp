// CProgressDisplay::DisplayProgress(const CCharString& textTag, float progress, bool a, bool b) -- when progress display is live, re-render the progress screen at most every 0.05 s (always on the first update) and remember the last text tag shown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00499C50, length 127 bytes, `ret 10h`.
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, double on the FPU), CProgressDisplay::RenderProgress @ 0x00499880,
// CCharString::operator!= @ 0x0099E900 (the manifest's less<CCharString>::operator() is propagated),
// CCharString::operator= @ 0x0099EFB0. Globals: bytes @ 0x01375445 (display enabled) and @ 0x013B89D3 (suppressed);
// the 0.0001f epsilon variable @ 0x0129BA3C (a global, not a literal: `fld dword`) and the 0.05 literal @ 0x012388A8.
// Layout: Ego_r.pdb CProgressDisplay with retail 12-byte vectors (five before: -0x14): LastTextDisplayed +0x50,
// LastUpdateTime (double) +0x60, Initialised +0x79.
// Shape: one guarded block; the first-update test `g_fEpsilon > LastUpdateTime` loads the float global and compares
// the double member (a 0.0001f literal is promoted to a double constant and swaps the operands).
struct CCharString {
    void* PStringData;
    bool operator!=(const CCharString& other) const;       // 0x0099E900
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

double __fastcall GFGetTime();                             // 0x009E1BC0

extern bool g_bProgressDisplayEnabled;                     // 0x01375445
extern bool g_bProgressDisplaySuppressed;                  // 0x013B89D3
extern float g_fEpsilon;                                   // 0x0129BA3C (0.0001f, read from memory: not folded)

struct CProgressDisplay {
    unsigned char _pad00[0x50];                            // +0x00 textures, text tags, wide text, current tag
    CCharString   LastTextDisplayed;                       // +0x50
    long          GuildSealFrameIndex;                     // +0x54
    long          StageCount;                              // +0x58
    double        LastUpdateTime;                          // +0x60
    double        LastScreenUpdate;                        // +0x68
    double        LastProgressUpdate;                      // +0x70
    bool          DebugDisplay;                            // +0x78
    bool          Initialised;                             // +0x79
    void RenderProgress(const CCharString& textTag, float progress, bool a, bool b);   // 0x00499880
    void DisplayProgress(const CCharString& textTag, float progress, bool a, bool b);
};

void CProgressDisplay::DisplayProgress(const CCharString& textTag, float progress, bool a, bool b)
{
    if (g_bProgressDisplayEnabled && !g_bProgressDisplaySuppressed && Initialised) {
        if (g_fEpsilon > LastUpdateTime || GFGetTime() - LastUpdateTime > 0.05) {
            RenderProgress(textTag, progress, a, b);
            LastUpdateTime = GFGetTime();
        }
        if (textTag != LastTextDisplayed)
            LastTextDisplayed = textTag;
    }
}