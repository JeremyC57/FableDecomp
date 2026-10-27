// CPlayerGui::UpdateCrime() -- update the crime stat bar: refresh it, feed it the hero's current bounty (or clear it when the hero is not wanted), then animate an initialised bar towards the rounded current value or initialise it
// Retail-asm: 0x004386B0 `fld dword ptr [esp + 8]` ; 0x004386B4 `fistp dword ptr [esp + 0xc]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004385F0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. CurrentValue rounded by the helper (0x00438727); the steps conversion stays a __ftol2 cast.
#include <math.h>
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
};
struct CCriminalRecord {                                // the hero's crime/bounty component (descriptive)
    bool  IsWanted();                                   // 0x0065BFCE
    long  GetBounty();                                  // 0x0065A41C
    float GetBountyFraction();                          // 0x0065A28F
};
struct CPlayer {
    CThing* GetPlayerCharacter();                       // 0x00487B70
    CCriminalRecord* GetCriminalRecord();               // 0x00488620
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};

struct CPlayerGuiDef {
    unsigned char _pad[0x4e8];
    float         StatChangeTextSpeed;                  // +0x4e8 (descriptive)
    float         StatChangeTextTime;                   // +0x4ec (descriptive)
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();          // 0x00438570

namespace NPlayerGui {
struct CStatBar {                                       // CStatBase + CStatBar (retail offsets)
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
struct CCrimeStatBar : CStatBar {
    unsigned char _pad50[0x6c - 0x50];
    float         BountyFraction;                       // +0x6c (descriptive)
    void Update();                                      // 0x00646B12
    void SetBounty(long bounty);                        // 0x00643FF6
};
struct CDrawStatUpdates {
    unsigned char _pad00[0x2c];
    long          PendingMoralityChange;                // +0x2c (descriptive)
};
}

inline long SignOf(float value) { return value < 0.0f ? -1 : 1; }
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}


struct CPlayerGui {                                     // retail-offset view (see Layout)
    unsigned char                   _pad000[8];
    CPlayerManager*                 PlayerManager;      // +0x08
    unsigned char                   _pad00c[0x2ec - 0xc];
    NPlayerGui::CCrimeStatBar*      PCrime;             // +0x2ec
    void UpdateStat(float value, float rate, float steps, float speed, float time, NPlayerGui::CStatBar* bar);   // 0x00437AE0
    void UpdateCrime();
};

void CPlayerGui::UpdateCrime()
{
    PCrime->Update();
    CThing* hero = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (!hero || (hero->m_stateFlags & 1))
        return;
    long bounty = 0;
    CCriminalRecord* record = PlayerManager->GetMainPlayer()->GetCriminalRecord();
    if (record) {
        if (record->IsWanted() && (bounty = record->GetBounty()) > 0) {
            PCrime->SetBounty(bounty);
            PCrime->BountyFraction = record->GetBountyFraction();
        } else {
            PCrime->SetBounty(0);
            PCrime->BountyFraction = 0.0f;
        }
    }
    if (PCrime->ValueInitialised) {
        long current = FloatToLong(PCrime->CurrentValue);
        long steps = (long)((fabs(PCrime->OldValue - PCrime->CurrentValue) - 0.2f) * 0.5f);
        if (steps < 1)
            steps = 1;
        UpdateStat((float)bounty, 0.01f, (float)current, PeekGuiDef()->StatChangeTextSpeed, PeekGuiDef()->StatChangeTextTime, PCrime);
        PCrime->OldGraphicFrame = PCrime->GraphicFrame;
        ++PCrime->GraphicFrame;
    } else {
        float value = (float)bounty;
        PCrime->CurrentValue = value;
        PCrime->OldValue = value;
        PCrime->LastOldValue = value;
        PCrime->ValueInitialised = true;
    }
}
