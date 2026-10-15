// CPlayerGui::SetCrimeAsDisplaying() -- reopen the crime stat bar: clear its fully-opened frame and put a bar that is not closed back into the opening mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00437770, length 36 bytes.
// Callees: none.
// Layout: the five stat-bar counted pointers PMoney/PRenown/PMorality/PExperience/PCrime sit at retail
// +0x2cc/+0x2d4/+0x2dc/+0x2e4/+0x2ec (Ego_r.pdb +0x2e0.. - 0x14), and the four sibling bodies 0x004376B0..
// 0x00437740 do the same for the other bars; this one is PCrime (CCrimeStatBar). CStatBase (Ego_r.pdb):
// FrameFullyOpenedOn +0x24, GuiMode (EPlayerGuiMode) +0x28. The mode values are inferred from the landed
// CPlayerGui::UpdateTimer (2 = opening, 3 = closing); 0 is taken as closed. The engine header
// CPlayerGui.h has the PDB offsets, so the class is modelled locally.
// Shape: two reads of the counted pointer's Data through an inline operator->; `mov eax,edx` before the store
// is the re-addressed object of the conditional assignment.
enum EPlayerGuiMode { GUI_MODE_CLOSED = 0, GUI_MODE_OPEN = 1, GUI_MODE_OPENING = 2 };

struct CStatBase {
    void*          __vftable;                               // +0x00
    void*          GuiDef;                                  // +0x04
    bool           ValueInitialised;                        // +0x08
    float          CurrentValue;                            // +0x0c
    float          OldValue;                                // +0x10
    float          LastOldValue;                            // +0x14
    float          RecentMaxValue;                          // +0x18
    float          OldRecentMaxValue;                       // +0x1c
    unsigned long  FrameCurrentValueLastMoved;              // +0x20
    unsigned long  FrameFullyOpenedOn;                      // +0x24
    EPlayerGuiMode GuiMode;                                 // +0x28
};
struct CCrimeStatBar : CStatBase { };
struct CCPPointerInfo;

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

struct CPlayerGui {
    unsigned char                  _pad[0x2ec];
    CCountedPointer<CCrimeStatBar> PCrime;                  // +0x2ec
    void SetCrimeAsDisplaying();
};

void CPlayerGui::SetCrimeAsDisplaying()
{
    PCrime->FrameFullyOpenedOn = 0;
    if (PCrime->GuiMode != GUI_MODE_CLOSED)
        PCrime->GuiMode = GUI_MODE_OPENING;
}
