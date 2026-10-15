// CPlayer::SetAggressiveMode(bool on) -- when the global aggressive-mode switch is set, toggle AggressiveMode (retail +0x217) and rebuild the controls on an actual change
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487EB0, length 64 bytes.
// Identity: Ego_r.pdb `?SetAggressiveMode@CPlayer@@QAEX_N@Z`.
// Callees: CPlayer::SetUpControlsBasedOnAggressiveMode @ 0x00487DE0 (`call ; ret 4` in both arms),
// the global enable byte @ 0x013B85F3.
// Shape: AggressiveMode is PDB +0x21F, retail +0x217 (the retail PlayerModes list at +0x210 is
// 4 bytes); the two arms are `!AggressiveMode && on` (stores the immediate 1) and the else-if
// `AggressiveMode && !on` (stores `on`, i.e. dl), whose re-test of AggressiveMode survives
// (`test al,al ; je`).
extern unsigned char g_bAggressiveModeAvailable;   // 0x013B85F3

struct CPlayer {
    unsigned char _pad00[0x217];
    bool          AggressiveMode;                  // +0x217 (PDB +0x21f)
    void SetUpControlsBasedOnAggressiveMode();     // 0x00487DE0
    void SetAggressiveMode(bool on);
};

void CPlayer::SetAggressiveMode(bool on)
{
    if (g_bAggressiveModeAvailable) {
        if (!AggressiveMode && on) {
            AggressiveMode = true;
            SetUpControlsBasedOnAggressiveMode();
        } else if (AggressiveMode && !on) {
            AggressiveMode = on;
            SetUpControlsBasedOnAggressiveMode();
        }
    }
}
