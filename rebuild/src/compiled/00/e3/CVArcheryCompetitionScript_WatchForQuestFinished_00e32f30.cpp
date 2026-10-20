// CV_ArcheryCompetitionScript::WatchForQuestFinished() -- wait for the archery owner's death, then for the contest to be idle or over, then mark the quest finished and let two script frames pass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E32F30, length 164 bytes (176 with padding).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed; the last check is a tail jump), and through
// the game script interface (+0x40): NewScriptFrame (slot 7, +0x1c).
// Layout (retail = Ego_r.pdb CV_ArcheryCompetitionScript - 0x14): IsOwnerDead +0x4c, QuestFinished +0x51,
// ContestState +0xa0 (0 and 2 are the states that end the wait).
// Shape: the outer `while (!terminating) { if (IsOwnerDead) { ...; return; } NewScriptFrame(); }` loop keeps the flag
// test at its head (a `while (!IsOwnerDead)` wait is rotated and duplicates the test); inside, the usual wait
// `while (!cond) { NewScriptFrame(); if (terminating) return; } if (terminating) return;`, then two frame/terminate
// pairs and a last terminating check whose result is unused (the tail jump).
struct CGameScriptInterface {                       // CGameScriptInterface, retail vtable @ 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
};

struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CV_ArcheryCompetitionScript : CQuestScriptBase {
    long          Score;                                    // +0x48
    bool          IsOwnerDead;                              // +0x4c
    bool          PrizeAlreadyGot;                          // +0x4d
    bool          QuestActivated;                           // +0x4e
    bool          AttackedByHero;                           // +0x4f
    bool          GiveAttackWarning;                        // +0x50
    bool          QuestFinished;                            // +0x51
    unsigned char _pad52[0x4e];                             // +0x52 .. +0xa0 (counters, info ids, timers, speech)
    long          ContestState;                             // +0xa0
    void WatchForQuestFinished();
};

void CV_ArcheryCompetitionScript::WatchForQuestFinished()
{
    while (!IsActiveThreadTerminating()) {
        if (IsOwnerDead) {
            if (IsActiveThreadTerminating())
                return;
            while (ContestState != 0 && ContestState != 2) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            QuestFinished = true;
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            IsActiveThreadTerminating();
            return;
        }
        GSI->NewScriptFrame();
    }
}