// CV_BeardyBaldyScript::WatchForQuestFinished() -- wait (one script frame at a time) until the quest is finished or completed; unless the thread is terminating, set the flag at +0x4a
// Retail: Fable.exe @ 0x00EA3490, 66 bytes (+ int3 padding). Referenced once as a thread entry (0x00EA3389).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame
// (slot 7, +0x1c) through Game at +0x40.
// Layout: the common quest-script flags (Ego_r.pdb - 0x14): QuestFinished +0x48, QuestComplete +0x49,
// QuestActivated +0x4a. Same macro as 0x00E50100 with two conditions.
// Shape: the WAIT macro `while (!(QuestFinished || QuestComplete)) { NewScriptFrame(); if (terminating) return; }`,
// the trailing terminate test, then the unconditional flag store.
class CGameScriptInterface { public:                            // retail vtable @ 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                 // slot 7 (+0x1c)
};

class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04..+0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

struct CV_BeardyBaldyScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    bool QuestComplete;                                  // +0x49
    bool QuestActivated;                                 // +0x4a
    void WatchForQuestFinished();
};

void CV_BeardyBaldyScript::WatchForQuestFinished()
{
    while (!(QuestFinished || QuestComplete)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    QuestActivated = true;
}