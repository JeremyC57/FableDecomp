// Grade: DIFFER(379v379) -- FloatToLong retail_asm helper (dead result), steps passed to UpdateStat, PDB fines API and the GetCrime() accessor (bar held in ebp across the fraction call as retail); residue: one-step scratch-register rotation in the UpdateStat argument loads and the initialise branch, shared with UpdateMoney 0x00439BB0
// Retail-asm: 0x004386B0 fld dword ptr [esp+8] ; fistp dword ptr [esp+0Ch] -- the engine FloatToLong helper (its result is unused; the asm block stays)
// CPlayerGui::UpdateCrime() -- update the crime stat bar: refresh it, feed it the hero's current bounty (or clear it when the hero is not wanted), then animate an initialised bar towards the rounded current value or initialise it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004385F0, length 379 bytes, plain `ret`.
// Callees: CCrimeStatBar::Update 0x00646B12, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPlayerCharacter
// 0x00487B70, CPlayer criminal-record accessor 0x00488620, its IsWanted 0x0065BFCE / GetBounty 0x0065A41C /
// GetBountyFraction 0x0065A28F (descriptive), CCrimeStatBar::SetBounty 0x00643FF6, _ftol 0x00BFEA70, PeekGuiDef
// 0x00438570, CPlayerGui::UpdateStat 0x00437AE0 (signature from the matched UpdateMorality 0x00439F20).
// Layout: as UpdateMorality (retail-offset view): PCrime +0x2EC.
#include <math.h>
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
};
class CTCVillage {                                      // the village the hero is nearest (fines)
public:
    bool  IsHeroCriminal();                             // 0x0065BFCE
    long  GetFineAmountForCurrentCrimes();              // 0x0065A41C
    float GetNormalisedTimeLeftOnFines();               // 0x0065A28F
};
struct CPlayer {
    CThing* GetPlayerCharacter();                       // 0x00487B70
    CTCVillage* GetPNearestTCVillage();                 // 0x00488620
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
    float         NormalisedTimeLeft;                   // +0x6c (PDB +0x7c)
    void SetNormalisedTimeLeft(float value) { NormalisedTimeLeft = value; }
    void Update();                                      // 0x00646B12
    void SetFine(long fine);                            // 0x00643FF6 (sets CurrentFine, PDB +0x78)
};
struct CDrawStatUpdates {
    unsigned char _pad00[0x2c];
    long          PendingMoralityChange;                // +0x2c (descriptive)
};
}

inline long SignOf(float value) { return value < 0.0f ? -1 : 1; }

struct CPlayerGui {                                     // retail-offset view (see Layout)
    unsigned char                   _pad000[8];
    CPlayerManager*                 PlayerManager;      // +0x08
    unsigned char                   _pad00c[0x2ec - 0xc];
    NPlayerGui::CCrimeStatBar*      PCrime;             // +0x2ec
    void UpdateStat(float value, float rate, float steps, float speed, float time, NPlayerGui::CStatBar* bar);   // 0x00437AE0
    void UpdateCrime();
    NPlayerGui::CCrimeStatBar& GetCrime() { return *PCrime; }
};

__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

void CPlayerGui::UpdateCrime()
{
    PCrime->Update();
    CThing* hero = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (!hero || (hero->m_stateFlags & 1))
        return;
    long fine = 0;
    CTCVillage* village = PlayerManager->GetMainPlayer()->GetPNearestTCVillage();
    if (village) {
        if (village->IsHeroCriminal() && (fine = village->GetFineAmountForCurrentCrimes()) > 0) {
            PCrime->SetFine(fine);
            GetCrime().SetNormalisedTimeLeft(village->GetNormalisedTimeLeftOnFines());
        } else {
            PCrime->SetFine(0);
            GetCrime().SetNormalisedTimeLeft(0.0f);
        }
    }
    if (PCrime->ValueInitialised) {
        long current = FloatToLong(PCrime->CurrentValue);
        long steps = (long)((fabs(PCrime->OldValue - PCrime->CurrentValue) - 0.2f) * 0.5f);
        if (steps < 1)
            steps = 1;
        UpdateStat((float)fine, 0.01f, (float)steps, PeekGuiDef()->StatChangeTextSpeed, PeekGuiDef()->StatChangeTextTime, PCrime);
        PCrime->OldGraphicFrame = PCrime->GraphicFrame;
        ++PCrime->GraphicFrame;
    } else {
        float value = (float)fine;
        PCrime->CurrentValue = value;
        PCrime->OldValue = value;
        PCrime->LastOldValue = value;
        PCrime->ValueInitialised = true;
    }
}
