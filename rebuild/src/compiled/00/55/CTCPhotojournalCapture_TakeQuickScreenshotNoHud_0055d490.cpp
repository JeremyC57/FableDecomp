// CTCPhotojournalCapture::TakeQuickScreenshotNoHud() -- once the capture has waited 18 frames, remember whether the player GUI was open, restart the frame counter and close the GUI so the next shot has no HUD
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D490, 45 bytes.
// Callees: CPlayerGui (global 0x013B8790) IsOpen @ 0x004374F0 and Close(false) @ 0x00438400 (manifest "CPlayerGui::Close").
// Layout (PDB CTCPhotojournalCapture, 0x48; retail 0x44 -- the STLport vector at +0x2c is 12 bytes, so PMannequin +0x38,
//   WasPlayerGuiOpen +0x3c, TakeScreenshotAfterFrames +0x40; FrameCounter +0x14 unchanged). No engine header: local model.
// Shape: `FrameCounter > 18` (`cmp ; jle`), the global re-read for each call, the counter zeroed between the two calls.
// Leaf: TakeQuickScreenshotNoHud
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
    void TakeQuickScreenshotNoHud();
};

void CTCPhotojournalCapture::TakeQuickScreenshotNoHud()
{
    if (FrameCounter > 18) {
        WasPlayerGuiOpen = g_pPlayerGui->IsOpen();
        FrameCounter = 0;
        g_pPlayerGui->Close(false);
    }
}