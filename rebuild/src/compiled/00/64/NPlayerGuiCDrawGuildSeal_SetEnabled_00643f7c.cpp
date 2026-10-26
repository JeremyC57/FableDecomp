#pragma optimize("s", on)
// NPlayerGui::CDrawGuildSeal::SetEnabled(bool) -- enable/disable the seal, restart the flash (alpha 0, cycle count from the GUI definition) and clear the reminder timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643F7C, length 34 bytes.
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`; its member at +0x68 is the guild-seal flash cycle count). PDB CDrawGuildSeal: Enabled +0x2c, FlashAlpha +0x2e,
// FlashCycles +0x30, ReminderTime +0x50 -- retail 4 lower (+0x28, +0x2a, +0x2c, +0x4c: the CDrawBase vector is 12 bytes in the shipped build).
// Shape: size TU; the float zero goes through `fldz`/`fstp` and the scheduler emits it between the call and the FlashCycles store.
class CPlayerGuiDef { public:
    unsigned char _pad[0x68];
    long m_guildSealFlashCycles;                   // +0x68
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};          // 0x00438570

namespace NPlayerGui {

class CDrawGuildSeal { public:
    unsigned char _pad[0x28];
    bool          Enabled;                         // +0x28
    bool          ReadyToLevelUp;                  // +0x29
    unsigned char FlashAlpha;                      // +0x2a
    unsigned char _pad2b;
    long          FlashCycles;                     // +0x2c
    unsigned char _pad30[0x1c];                    // +0x30 FadeInOut
    float         ReminderTime;                    // +0x4c
    void SetEnabled(bool enabled);
};

void CDrawGuildSeal::SetEnabled(bool enabled)
{
    Enabled = enabled;
    FlashAlpha = 0;
    FlashCycles = CPlayerGui::PeekGuiDef().m_guildSealFlashCycles;
    ReminderTime = 0.0f;
}

}