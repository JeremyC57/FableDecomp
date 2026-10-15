// CQ_GuildTrainingWoodsWillScript::EndMission() -- wait one script frame at a time until the mission is over, then mark it succeeded unless the script thread is terminating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D68CD0, length 59 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface slot 7
// NewScriptFrame (+0x1c) through the quest's interface pointer at +0x40.
// Layout: the retail quest-script base is 0x48 bytes (CScriptBase 0x40 + interface pointer + one word); the
// Ego_r.pdb members sit 0x14 higher (debug containers), so PDB MissionSucceeded +0x5c / MissionOver +0x5e
// are retail +0x48 / +0x4a.
// Shape: `while (!MissionOver) { NewScriptFrame(); if (terminating) return; }` rotated into an entry test and
// a bottom test with the 6-byte `lea ebx,[ebx]` alignment pad; the final `if (!terminating) MissionSucceeded = true`.
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

struct CQ_GuildTrainingWoodsWillScript : CQuestScriptBase {
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool MissionOver;                                       // +0x4a
    void EndMission();
};

void CQ_GuildTrainingWoodsWillScript::EndMission()
{
    while (!MissionOver) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating())
        MissionSucceeded = true;
}