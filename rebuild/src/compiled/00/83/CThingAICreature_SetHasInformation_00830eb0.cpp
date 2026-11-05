// CThingAICreature::SetHasInformation(bool hasInformation, bool wander, bool wave, bool continueAI) -- set the four information flag bits and refresh the player GUI when HasInformation changed
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830EB0, 104 bytes (`ret 10h`).
// Callees: CPlayerGui::UpdateMiniMapMarkers(bool) @ 0x004378C0 (label "_Cons_val<...>" propagated) on the player GUI singleton
//   pointer at 0x013B8790.
// Layout: Ego_r.pdb flag byte +0x1f8 is retail +0x1c8 (0x30 lower): bit 0 PerformingVillageTaskFlag, bit 1 AISuspended,
//   bit 2 HasInformation, bit 3 WanderWithInformation, bit 4 WaveWithInformation, bit 5 ContinueAIWithInformation,
//   bit 6 ImmuneToAttack, bit 7 EnableCreatureAutoPlacing.
// Shape: the change test is taken first (kept in the dead hasInformation slot: `setne [esp+0Ch]`), then the four
//   adjacent bit assignments fold into one read-modify-write (`and al,0C3h ; shl dl,2 ; or`).
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool mode);       // 0x004378C0
};

class CThingAICreature { public:
    unsigned char _pad[0x1c8];
    bool PerformingVillageTaskFlag : 1;         // +0x1c8 bit 0
    bool AISuspended : 1;
    bool HasInformation : 1;                    // bit 2
    bool WanderWithInformation : 1;             // bit 3
    bool WaveWithInformation : 1;               // bit 4
    bool ContinueAIWithInformation : 1;         // bit 5
    bool ImmuneToAttack : 1;
    bool EnableCreatureAutoPlacing : 1;
    void SetHasInformation(bool hasInformation, bool wander, bool wave, bool continueAI);
};

void CThingAICreature::SetHasInformation(bool hasInformation, bool wander, bool wave, bool continueAI)
{
    bool changed = HasInformation != hasInformation;
    HasInformation = hasInformation;
    WanderWithInformation = wander;
    WaveWithInformation = wave;
    ContinueAIWithInformation = continueAI;
    if (changed)
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
}