// CCombatSequence_BattleCharge::IsValid() -- valid once more than ten seconds of logic frames have passed since the last use
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00908DA0, length 54 bytes (+ int3 padding).
// Callees: the frame counter through the world object @ 0x013B86A0 (`mov ecx,[g] ; call 0x004167C8`,
// an incremental-link thunk onto GFGetCurrentFrame @ 0x0049D870).
// The frame difference is spilled to the one-dword frame for `fild`, divided by the integer logic frame
// rate @ 0x01375550 (`fidiv`, value 15) and compared with the float constant 10.0 @ 0x01237F0C; the
// bool is `test ah,41h` (not below-or-equal) behind `if (..) return true; return false;` (a bare
// `return cond;` widens to eax). m_frameOfLastUse is +0x28 in retail (PDB +0x2c, the base's
// 16-byte vector is 12 bytes here).
struct CWorld_Retail {
    long GetCurrentFrame();                    // 0x004167C8 -> GFGetCurrentFrame 0x0049D870
};

extern CWorld_Retail* g_pWorld;                // 0x013B86A0
extern long g_logicFramesPerSecond;            // 0x01375550 (15)

struct CCombatSequence_BattleCharge_Retail {
    virtual ~CCombatSequence_BattleCharge_Retail();
    unsigned char _base[0x24];                 // +0x04 CCombatSequenceBase data (retail 0x28 bytes in all)
    long m_frameOfLastUse;                     // +0x28 (PDB +0x2c)
    virtual bool IsValid();
};

bool CCombatSequence_BattleCharge_Retail::IsValid()
{
    if ((float)(g_pWorld->GetCurrentFrame() - m_frameOfLastUse) / g_logicFramesPerSecond > 10.0f)
        return true;
    return false;
}
