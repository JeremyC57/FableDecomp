// CThingAICreature::SetHasInformation(bool hasInformation, bool wander, bool wave, bool continueAI) -- set the four information flag bits and refresh the player GUI when HasInformation changed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830EB0, 104 bytes (`ret 10h`).
// Callees: CPlayerGui::Refresh(long) @ 0x004378C0 (label "_Cons_val<...>" propagated) on the player GUI singleton
//   pointer at 0x013B8790.
// Layout: Ego_r.pdb flag byte +0x1f8 is retail +0x1c8 (0x30 lower): bit 0 PerformingVillageTaskFlag, bit 1 AISuspended,
//   bit 2 HasInformation, bit 3 WanderWithInformation, bit 4 WaveWithInformation, bit 5 ContinueAIWithInformation,
//   bit 6 ImmuneToAttack, bit 7 EnableCreatureAutoPlacing.
// Shape: the change test is taken first (kept in the dead hasInformation slot: `setne [esp+0Ch]`), then the four
//   adjacent bit assignments fold into one read-modify-write (`and al,0C3h ; shl dl,2 ; or`).
class CPlayerGui { public:
    void Refresh(long mode);                    // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

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
        g_pPlayerGui->Refresh(0);
}