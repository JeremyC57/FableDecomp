// CTCRegionDisplay::SetCurrentAreaName(const CWideString& name, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused) -- show a new region name (or clear it), with -1 meaning the default five-second display and 0 meaning no time-out
// Retail: Fable.exe (TLC, VC7.1) @ 0x00639A90, 224 bytes (`ret 14h`; PDB ?SetCurrentAreaName@CTCRegionDisplay@@QAEXABVCWideString@@M_N11@Z).
// Callees: CWideString::operator!=(const wchar_t*) const @ 0x0099B150, CWideString::operator=(const CWideString&) @ 0x0099B7D0,
//   CWideString::operator=(const wchar_t*) @ 0x0099B800 (with L"" @ 0x0122D70C), CTCBase::GetWorld() @ 0x00686A70,
//   CWorld::GetWorldFrame() @ 0x0049D870 (static; called through the GetWorld() result, which keeps the GetWorld call).
// Float pool: -1.0f @ 0x0122DEE0, 0.0f @ 0x0122DEDC (5.0f folds into an immediate store to the parameter slot).
// Layout: Ego_r.pdb CTCRegionDisplay (0x28, as the landed constructor 0x006398B0): CurrentAreaName +0x0c, CurrentAreaNameExtra +0x10,
//   FrameNameChanged +0x14, CurrentAlpha +0x18, LastAlpha +0x1c, DisplayTime +0x20, TimeOut/InfoDisplay/UpdateWhilePaused +0x24..26.
// Shape: `CurrentAlpha = LastAlpha = 255` stores LastAlpha first; `TimeOut = displayTime != 0.0f` is the full-register 0/1 with the
//   three byte stores duplicated into both arms.
#include <stddef.h>
class CThing;
class CWorld { public:
    static long GetWorldFrame();                            // 0x0049D870
};
extern long g_framesPerSecond;                              // 0x01375550
class CWideString { public:
    void* m_pData;
    bool operator!=(const wchar_t* text) const;             // 0x0099B150
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator=(const wchar_t* text);      // 0x0099B800
};
class CTCBase { public:                                            // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    unsigned char _pad0a[2];
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCRegionDisplay : public CTCBase { public:                         // vtable 0x0125A0EC
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

void CTCRegionDisplay::SetCurrentAreaName(const CWideString& name, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused)
{
    if (name != L"") {
        CurrentAreaName = name;
        CurrentAreaNameExtra = L"";
        FrameNameChanged = GetWorld().GetWorldFrame();
        LastAlpha = 0;
    } else {
        CurrentAreaName = L"";
        CurrentAreaNameExtra = L"";
        CurrentAlpha = 0;
    }
    if (displayTime == -1.0f)
        displayTime = 5.0f;
    if (showNow)
        CurrentAlpha = LastAlpha = 255;
    DisplayTime = displayTime;
    TimeOut = displayTime != 0.0f;
    InfoDisplay = infoDisplay;
    UpdateWhilePaused = updateWhilePaused;
}