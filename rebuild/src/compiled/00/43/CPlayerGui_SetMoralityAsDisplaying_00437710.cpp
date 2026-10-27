// CPlayerGui::SetMoralityAsDisplaying() -- reopen the morality stat bar: clear its fully-opened frame and put a bar that is not closed back into the opening mode
// Leaf: ?SetMoralityAsDisplaying@CPlayerGui@@QAEXXZ
// Retail: Fable.exe @ 0x00437710, 36 bytes, __fastcall, plain `ret`.
// Identity (proven by layout + PDB name set): one of five byte-identical-but-for-the-offset bodies 0x004376B0 / 0x004376E0
// / 0x00437710 / 0x00437740 / 0x00437770, one per CPlayerGui stat-bar counted pointer. Ego_r.pdb CPlayerGui holds PMoney
// +0x2E0, PRenown +0x2E8, PMorality +0x2F0, PExperience +0x2F8, PCrime +0x300 (retail - 0x14: +0x2CC .. +0x2EC, as the
// landed SetCrimeAsDisplaying 0x00437770 = +0x2EC proves), and the PDB name sets hold the matching
// ?Set{Money,Renown,Morality,Experience,Crime}AsDisplaying@CPlayerGui@@QAEXXZ; +0x2DC is PMorality.
// Layout: NPlayerGui::CStatBar (CStatBase, Ego_r.pdb): FrameFullyOpenedOn +0x24, GuiMode (EPlayerGuiMode) +0x28; mode 2 is
// opening (landed CPlayerGui::UpdateTimer), 0 taken as closed. engine/CPlayerGui.h carries the Ego_r offsets, so the
// class is a local partial view (NOTES.md "Header debt"), as in the landed 0x00437770.
// Shape: two reads of the counted pointer's Data through an inline operator->; `mov eax,edx` before the store is the
// re-addressed object of the conditional assignment.
enum EPlayerGuiMode { GUI_MODE_CLOSED = 0, GUI_MODE_OPEN = 1, GUI_MODE_OPENING = 2 };

namespace NPlayerGui {
class CStatBase {
public:
    virtual ~CStatBase();
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
class CStatBar : public CStatBase { };
}
class CCPPointerInfo;

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

class CPlayerGui { public:
    unsigned char                         Body[0x2dc];      // +0x00 .. (not modelled)
    CCountedPointer<NPlayerGui::CStatBar> PMorality;        // +0x2dc
    void SetMoralityAsDisplaying();
};

void CPlayerGui::SetMoralityAsDisplaying()
{
    PMorality->FrameFullyOpenedOn = 0;
    if (PMorality->GuiMode != GUI_MODE_CLOSED)
        PMorality->GuiMode = GUI_MODE_OPENING;
}