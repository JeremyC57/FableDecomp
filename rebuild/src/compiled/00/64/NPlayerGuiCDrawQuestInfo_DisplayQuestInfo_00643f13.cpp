#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::DisplayQuestInfo(bool) -- open the quest box (mode 2, timers reset, mini-mode countdown from the GUI def) or start closing it (mode 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643F13, length 100 bytes.
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), __ftol2 @ 0x00BFEA70.
// Globals: the game frames-per-second long @ 0x01375550.
// Layout: PDB CDrawQuestInfo, retail 4 lower from GuiMode on: GuiMode +0x28 (EPlayerGuiMode: 0 hidden, 1 closing,
// 2 open, 3 opening), FadeInTimer +0x2c, MiniModeTimer +0x30, MiniMode +0x34, FramesUntilMiniMode +0x38. The GUI def
// float at +0x588 is the delay in seconds before the box shrinks to mini mode (retail offset; the PDB def has wider vectors).
// Shape: frame-pointer size TU; the zero for MiniMode shares ecx with the `display` test (`xor ecx,ecx ; cmp [ebp+8],cl`);
// the frame rate is read into a long local BEFORE the def call, and VC homes it in the dead `display` slot for the `fild`.
struct CPlayerGuiDef {
    unsigned char _pad[0x588];
    float         QuestInfoMiniModeDelaySeconds;   // +0x588
};

extern CPlayerGuiDef* __fastcall PeekGuiDef();     // 0x00438570
extern long g_framesPerSecond;                     // 0x01375550

namespace NPlayerGui {

struct CDrawQuestInfo {
    unsigned char _base[0x28];                     // CDrawBase (vptr, GUI pointers, Drawing, primitive handles)
    long          GuiMode;                         // +0x28 (EPlayerGuiMode)
    float         FadeInTimer;                     // +0x2c
    float         MiniModeTimer;                   // +0x30
    bool          MiniMode;                        // +0x34
    long          FramesUntilMiniMode;             // +0x38
    void DisplayQuestInfo(bool display);
};

void CDrawQuestInfo::DisplayQuestInfo(bool display)
{
    if (display) {
        if (GuiMode == 3 || GuiMode == 1) {
            GuiMode = 2;
            FadeInTimer = 0.0f;
            MiniMode = false;
            MiniModeTimer = 0.0f;
            long framesPerSecond = g_framesPerSecond;
            FramesUntilMiniMode = (long)(framesPerSecond * PeekGuiDef()->QuestInfoMiniModeDelaySeconds);
        }
    }
    else {
        if (GuiMode == 2 || GuiMode == 0) {
            GuiMode = 1;
            MiniMode = false;
        }
    }
}

}