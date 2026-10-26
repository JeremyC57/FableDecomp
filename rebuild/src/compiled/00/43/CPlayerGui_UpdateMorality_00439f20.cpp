// CPlayerGui::UpdateMorality() -- feed the hero's pending morality change into the morality stat bar: animate an initialised bar towards the new value (step count from how far it still has to move) or initialise it outright, then hide the bar when the change flips the sign of the bar's value, and consume the change once the bar is in mode 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00439F20, length 352 bytes, plain `ret`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPlayerCharacter 0x00487B70, _ftol 0x00BFEA70, PeekGuiDef
// 0x00438570 (`__fastcall`, the GUI def), CPlayerGui::UpdateStat 0x00437AE0.
// Data: 0.2f 0x012305A0, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 0.01f (immediate).
// Layout (retail offsets, local view; engine/CPlayerGui.h is the PDB layout, retail sits 8 / 0x14 lower as in the
// landed NPlayerGui::CDrawCrime::Draw 0x006442AE): PlayerManager +0x08, the stat-update illustrator +0x1A8 (pending
// morality change +0x2C), PMorality +0x2DC. CStatBase (Ego_r.pdb): ValueInitialised +8, CurrentValue +0xC, OldValue
// +0x10, LastOldValue +0x14, GuiMode +0x28; CStatBar GraphicFrame +0x48 / OldGraphicFrame +0x4C (retail, landed
// DrawCrime). The GUI def's two stat-change parameters at retail +0x4E8 / +0x4EC are named descriptively.
// Shape: the change converted to a float local that stays on the x87 stack across both arms; the two signs are an
// inline `x < 0 ? -1 : 1`.
#include <math.h>
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
};
class CPlayer { public:
    CThing* GetPlayerCharacter();                       // 0x00487B70
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                           // 0x00449970
};

class CPlayerGuiDef { public:
    unsigned char _pad[0x4e8];
    float         StatChangeTextSpeed;                  // +0x4e8 (descriptive)
    float         StatChangeTextTime;                   // +0x4ec (descriptive)
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();          // 0x00438570

namespace NPlayerGui {
class CStatBar { public:                                       // CStatBase + CStatBar (retail offsets)
    void*         __vftable;
    CPlayerGuiDef* GuiDef;                              // +0x04
    bool          ValueInitialised;                     // +0x08
    float         CurrentValue;                         // +0x0c
    float         OldValue;                             // +0x10
    float         LastOldValue;                         // +0x14
    float         RecentMaxValue;                       // +0x18
    float         OldRecentMaxValue;                    // +0x1c
    unsigned long FrameCurrentValueLastMoved;           // +0x20
    unsigned long FrameFullyOpenedOn;                   // +0x24
    long          GuiMode;                              // +0x28
    unsigned char _pad2c[0x48 - 0x2c];
    unsigned long GraphicFrame;                         // +0x48
    unsigned long OldGraphicFrame;                      // +0x4c
};
class CDrawStatUpdates { public:
    unsigned char _pad00[0x2c];
    long          PendingMoralityChange;                // +0x2c (descriptive)
};
}

inline long SignOf(float value) { return value < 0.0f ? -1 : 1; }

class CPlayerGui { public:                                     // retail-offset view (see Layout)
    unsigned char                   _pad000[8];
    CPlayerManager*                 PlayerManager;      // +0x08
    unsigned char                   _pad00c[0x1a8 - 0xc];
    NPlayerGui::CDrawStatUpdates*   PDrawStatUpdates;   // +0x1a8
    unsigned char                   _pad1ac[0x2dc - 0x1ac];
    NPlayerGui::CStatBar*           PMorality;          // +0x2dc
    void UpdateStat(float value, float rate, float steps, float speed, float time, NPlayerGui::CStatBar* bar);   // 0x00437AE0
    void UpdateMorality();
};

void CPlayerGui::UpdateMorality()
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (!hero || (hero->m_stateFlags & 1))
        return;
    long change = PDrawStatUpdates->PendingMoralityChange;
    float value;
    if (PMorality->ValueInitialised) {
        long steps = (long)((fabs(PMorality->OldValue - PMorality->CurrentValue) - 0.2f) * 0.5f);
        if (steps < 1)
            steps = 1;
        value = (float)change;
        UpdateStat(value, 0.01f, (float)steps, PeekGuiDef()->StatChangeTextSpeed, PeekGuiDef()->StatChangeTextTime, PMorality);
        PMorality->OldGraphicFrame = PMorality->GraphicFrame;
        ++PMorality->GraphicFrame;
    } else {
        value = (float)change;
        PMorality->CurrentValue = value;
        PMorality->OldValue = value;
        PMorality->LastOldValue = value;
        PMorality->ValueInitialised = true;
    }
    if (change != 0) {
        if (SignOf(value) != SignOf(PMorality->OldValue))
            PMorality->ValueInitialised = false;
        if (PMorality->GuiMode == 1) {
            PDrawStatUpdates->PendingMoralityChange = 0;
            PMorality->ValueInitialised = false;
        }
    }
}