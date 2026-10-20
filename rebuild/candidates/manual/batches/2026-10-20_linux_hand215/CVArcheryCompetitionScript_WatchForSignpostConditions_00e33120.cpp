// CV_ArcheryCompetitionScript::WatchForSignpostConditions() -- every script frame, pick the archery signpost's state from the contest phase and the hero's conduct
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E33120, length 193 bytes (208 with padding).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), and through the game script interface
// (+0x40): NewScriptFrame (slot 7, +0x1c).
// Layout (retail = Ego_r.pdb CV_ArcheryCompetitionScript - 0x14): QuestActivated +0x4e, AttackedByHero +0x4f,
// QuestFinished +0x51. The object at +0x44 (the quest-script base's second word, typed only as padding in the PDB)
// carries two longs the method reads/writes: a phase at +0x3c (0 and 2 handled) and the signpost state at +0x40
// (2 = closed, 1 = finished, 0 = open). The member names are descriptive.
// Shape: the signpost store precedes the loop; inside, `if (phase == 0) {...} else if (phase == 2) {...}`, every
// store guarded by its own terminating check (the script code's `if (terminating) return;` before each action).
struct CGameScriptInterface {                       // CGameScriptInterface, retail vtable @ 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
};

struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestOwnerState {                                   // the +0x44 object (descriptive model)
    unsigned char _pad00[0x3c];
    long          m_phase;                                  // +0x3c
    long          m_signpostState;                          // +0x40
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    CQuestOwnerState*     Owner;                            // +0x44
};

struct CV_ArcheryCompetitionScript : CQuestScriptBase {
    long          Score;                                    // +0x48
    bool          IsOwnerDead;                              // +0x4c
    bool          PrizeAlreadyGot;                          // +0x4d
    bool          QuestActivated;                           // +0x4e
    bool          AttackedByHero;                           // +0x4f
    bool          GiveAttackWarning;                        // +0x50
    bool          QuestFinished;                            // +0x51
    void WatchForSignpostConditions();
};

void CV_ArcheryCompetitionScript::WatchForSignpostConditions()
{
    Owner->m_signpostState = 2;
    while (!IsActiveThreadTerminating()) {
        long phase = Owner->m_phase;
        if (phase == 0) {
            if (IsActiveThreadTerminating())
                return;
            if (AttackedByHero) {
                if (IsActiveThreadTerminating())
                    return;
                Owner->m_signpostState = 2;
            } else if (QuestFinished) {
                if (IsActiveThreadTerminating())
                    return;
                Owner->m_signpostState = 1;
            }
        } else if (phase == 2) {
            if (IsActiveThreadTerminating())
                return;
            if (QuestActivated && !AttackedByHero) {
                if (IsActiveThreadTerminating())
                    return;
                Owner->m_signpostState = 0;
            }
        }
        GSI->NewScriptFrame();
    }
}
