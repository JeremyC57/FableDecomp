// CPlayerGui::UpdateCrime() -- update the crime stat bar: tick its change texts, feed it the fine the nearest village holds against the hero (and the normalised time left on those fines, or clear both when the hero is not a criminal there), then animate an initialised bar towards the fine or initialise it outright
// Leaf: ?UpdateCrime@CPlayerGui@@AAEXXZ
// Flags: none
// Retail-asm: 0x004386B4 `fistp dword ptr [esp+0Ch]` -- engine/GFFloatToLongNear.h (GFFloatToLongNear: the bar's CurrentValue
//   copied through a frame slot, `fld ; fistp dword`, the result never read). VC7.1 C++ `(long)x` calls __ftol2, `/QIfist`
//   stores `fistp qword`, `_mm_cvtss_si32` emits cvtss2si (NOTES.md attempts table).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004385F0, length 379 bytes, plain `ret`.
// Name: Ego_r.pdb ?UpdateCrime@CPlayerGui@@AAEXXZ (private).
// Callees: NPlayerGui::CStatBar::UpdateChangeTexts 0x00646B12, CPlayerManager::GetMainPlayer 0x00449970 (engine/CPlayerManager.h), CPlayer::GetPPlayerCharacterThing 0x00487B70,
//   CPlayer::GetPNearestTCVillage 0x00488620, CTCVillage::IsHeroCriminal 0x0065BFCE / GetFineAmountForCurrentCrimes 0x0065A41C /
//   GetNormalisedTimeLeftOnFines 0x0065A28F, NPlayerGui::CCrimeStatBar::SetFine 0x00643FF6 (all Ego_r.pdb names), __ftol2
//   0x00BFEA70, CPlayerGui::PeekGuiDef 0x00438570, CPlayerGui::UpdateStat 0x00437AE0 (as the landed UpdateMorality 0x00439F20).
// Layout: as UpdateMorality (retail-offset view): the crime bar +0x2EC; CCrimeStatBar NormalisedTimeLeft +0x6C (the inline
//   Ego_r SetNormalisedTimeLeft setter).
#include <math.h>
#include "engine/GFFloatToLongNear.h"
#include "engine/CThing.h"
class CTCVillage { public:
    bool  IsHeroCriminal();                             // 0x0065BFCE
    long  GetFineAmountForCurrentCrimes();              // 0x0065A41C
    float GetNormalisedTimeLeftOnFines();               // 0x0065A28F
};
class CPlayer { public:
    CThing* GetPPlayerCharacterThing() const;           // 0x00487B70 (Ego_r ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ)
    CTCVillage* GetPNearestTCVillage();                 // 0x00488620
};
#include "engine/CPlayerManager.h"
#include "engine/CCountedPointer.h"

class CPlayerGuiDef { public:
    unsigned char _pad[0x4e8];
    float         StatChangeTextSpeed;                  // +0x4e8 (descriptive)
    float         StatChangeTextTime;                   // +0x4ec (descriptive)
};

namespace NPlayerGui {
class CStatBase { public:                                      // Ego_r.pdb CStatBase (retail offsets; its two vectors are STLport 0xc)
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
    long          GuiMode;                              // +0x28 EPlayerGuiMode
    float         FadeInTimer;                          // +0x2c
    unsigned char PrimitiveHandles[0xc];                // +0x30 std::vector<CEnginePrimitiveHandle>
    unsigned char Effects[0xc];                         // +0x3c std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >
};
class CStatBar : public CStatBase { public:                    // Ego_r.pdb CStatBar (retail offsets)
    unsigned long GraphicFrame;                         // +0x48
    unsigned long OldGraphicFrame;                      // +0x4c
    unsigned char _pad50[0x68 - 0x50];                  // +0x50 GraphicIndex .. ChangeTexts
    void UpdateChangeTexts();                           // 0x00646B12
};
class CCrimeStatBar : public CStatBar { public:                // Ego_r.pdb CCrimeStatBar (retail offsets)
    long          CurrentFine;                          // +0x68
    float         NormalisedTimeLeft;                   // +0x6c
    void SetFine(long fine);                            // 0x00643FF6
    void SetNormalisedTimeLeft(float timeLeft) { NormalisedTimeLeft = timeLeft; }
};
}

class CPlayerGui { public:                                     // retail-offset view (see Layout)
    static const CPlayerGuiDef& __fastcall PeekGuiDef();          // 0x00438570
    unsigned char                   _pad000[8];
    CPlayerManager*                 PlayerManager;      // +0x08
    unsigned char                   _pad00c[0x2ec - 0xc];
    CCountedPointer<NPlayerGui::CCrimeStatBar> PCrime;  // +0x2ec
    NPlayerGui::CCrimeStatBar& GetCrime() { return *PCrime; }   // Ego_r.pdb ?GetCrime@CPlayerGui@@QAEAAVCCrimeStatBar@NPlayerGui@@XZ (inline)
private:
    void UpdateStat(float value, float rate, float steps, float speed, float time, NPlayerGui::CStatBase& bar) const;   // 0x00437AE0
    void UpdateCrime();
};

void CPlayerGui::UpdateCrime()
{
    PCrime->UpdateChangeTexts();
    CThing* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacterThing();
    if (!hero || hero->IsDead())
        return;
    long fine = 0;
    CTCVillage* village = PlayerManager->GetMainPlayer().GetPNearestTCVillage();
    if (village) {
        if (village->IsHeroCriminal() && (fine = village->GetFineAmountForCurrentCrimes()) > 0) {
            PCrime->SetFine(fine);
            GetCrime().SetNormalisedTimeLeft(village->GetNormalisedTimeLeftOnFines());
        } else {
            PCrime->SetFine(0);
            PCrime->SetNormalisedTimeLeft(0.0f);
        }
    }
    if (PCrime->ValueInitialised) {
        long current = GFFloatToLongNear(PCrime->CurrentValue);
        long steps = (long)((fabs(PCrime->OldValue - PCrime->CurrentValue) - 0.2f) * 0.5f);
        if (steps < 1)
            steps = 1;
        UpdateStat((float)fine, 0.01f, (float)steps, CPlayerGui::PeekGuiDef().StatChangeTextSpeed, CPlayerGui::PeekGuiDef().StatChangeTextTime, GetCrime());
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