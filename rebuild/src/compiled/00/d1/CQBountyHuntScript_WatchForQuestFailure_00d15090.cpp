// CQ_BountyHuntScript::WatchForQuestFailure() -- every script frame, fail the quest once a villager is killed before the intro or a hostage is killed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D15090, length 88 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed; also the tail `jmp`), CGameScriptInterface
// slot 7 NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// Layout (Ego_r.pdb CQ_BountyHuntScript - 0x14): QuestFailed +0x48, VillagerKilled +0x4d, VillagerIntroDone
// +0x4e, Hostage1Killed +0x54, Hostage2Killed +0x55.
// Shape: the failure branch leaves the loop: `if (terminating) return; QuestFailed = true;` and a final
// IsActiveThreadTerminating() whose discarded result VC7.1 turns into the tail `jmp`; the branch body is
// placed after the loop's `ret`.
struct CGameScriptInterface_Calls {                      // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
};

struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface_Calls* GSI;                              // +0x40
    unsigned char _pad44[4];                                // +0x44
};

struct CQ_BountyHuntScript : CQuestScriptBase {
    bool QuestFailed;                                       // +0x48
    unsigned char _pad49[4];                                // +0x49
    bool VillagerKilled;                                    // +0x4d
    bool VillagerIntroDone;                                 // +0x4e
    unsigned char _pad4f[5];                                // +0x4f
    bool Hostage1Killed;                                    // +0x54
    bool Hostage2Killed;                                    // +0x55
    void WatchForQuestFailure();
};

void CQ_BountyHuntScript::WatchForQuestFailure()
{
    while (!IsActiveThreadTerminating()) {
        if ((VillagerKilled && !VillagerIntroDone) || Hostage1Killed || Hostage2Killed) {
            if (IsActiveThreadTerminating())
                return;
            QuestFailed = true;
            IsActiveThreadTerminating();
            return;
        }
        GSI->NewScriptFrame();
    }
}