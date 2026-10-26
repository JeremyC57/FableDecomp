// CTCPhotojournalCapture::GlobalUpdate(long) -- advance the frame counter (capped at 20): on frame 18 take the quick screenshot and reopen the player GUI if it was open; count down TakeScreenshotAfterFrames and shoot when it reaches 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D4D0, 73 bytes (`ret 4`: the CTCBase::GlobalUpdate argument is unused).
// Callees: CTCPhotojournalCapture::TakeQuickScreenshot @ 0x0055D180 (twice), CPlayerGui::Open @ 0x004374B0 on the global 0x013B8790.
// Layout (PDB CTCPhotojournalCapture, 0x48; retail 0x44 -- the STLport vector at +0x2c is 12 bytes, so PMannequin +0x38,
//   WasPlayerGuiOpen +0x3c, TakeScreenshotAfterFrames +0x40; FrameCounter +0x14 unchanged). No engine header: local model.
// Shape: `if (FrameCounter < 20) { if (++FrameCounter == 18) ... }` (inc, compare, then the store); the count-down guarded by
//   `> -1` and the zero test re-reading the member at the join.
// Leaf: GlobalUpdate
class CPlayerGui { public:
    bool IsOpen() const;                    // 0x004374F0 (state +0x294 != 1; manifest "C3DMeshInfo::HasPhysicsMesh", propagated)
    void Open();                            // 0x004374B0 (state 2)
    void Close(bool immediately);           // 0x00438400 (state 1 when immediate, else 3)
};

extern CPlayerGui* g_pPlayerGui;            // 0x013B8790

class CWideString { public: void* m_pData; };
class CThing;

class CTCPhotojournalCapture { public:             // 0x44 bytes in retail (PDB 0x48: the STLport vector is 12 bytes)
    void*          __vftable;               // +0x00 (CTCBase)
    CThing*        ParentThing;             // +0x04
    bool           ToBeRemoved;             // +0x08
    bool           IsInGlobalUpdateTCs;     // +0x09
    CWideString    Description;             // +0x0c
    bool           DiscardScreenShot;       // +0x10
    long           FrameCounter;            // +0x14
    bool           FirstRenderFrame;        // +0x18
    bool           ScreenOpen;              // +0x19
    double         LastRenderTime;          // +0x20
    CWideString    FileName;                // +0x28
    void*          Components[3];           // +0x2c vector<NUISystem::CComponent*>
    CThing*        PMannequin;              // +0x38
    bool           WasPlayerGuiOpen;        // +0x3c
    long           TakeScreenshotAfterFrames; // +0x40
    void TakeQuickScreenshot();             // 0x0055D180
    void GlobalUpdate(long frame);
};

void CTCPhotojournalCapture::GlobalUpdate(long frame)
{
    if (FrameCounter < 20) {
        if (++FrameCounter == 18) {
            TakeQuickScreenshot();
            if (WasPlayerGuiOpen)
                g_pPlayerGui->Open();
        }
    }
    if (TakeScreenshotAfterFrames > -1)
        --TakeScreenshotAfterFrames;
    if (TakeScreenshotAfterFrames == 0)
        TakeQuickScreenshot();
}