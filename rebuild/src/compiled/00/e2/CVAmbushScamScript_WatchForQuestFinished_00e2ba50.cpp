// CV_AmbushScamScript::WatchForQuestFinished() -- every script frame, finish the quest once all ambushers are dead with the trickster gone, or the trickster was killed (before/after the ambush) or is done
// Retail: Fable.exe @ 0x00E2BA50, 95 bytes. Referenced once as a thread entry (0x00E2B680).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (loop test, then twice in the finish branch, the last
// one a tail jump), CGameScriptInterface::NewScriptFrame (slot 7, +0x1c) through Game at +0x40.
// Layout: Ego_r.pdb CV_AmbushScamScript - 0x14: QuestFinished +0x48, TricksterDeadOrLeft +0x55, TricksterComplete +0x56,
// TricksterKilledBeforeAmbush +0x57, TricksterKilledAfterAmbush +0x58, AllAmbushersDead +0x61.
// Shape: `while (!terminating) { if (cond) { if (terminating) return; QuestFinished = true; if (terminating) return;
// return; } NewScriptFrame(); }` -- the condition's `||` chain falls into one shared finish block placed after the
// loop's `ret`, whose last terminate test (result unused) becomes `jmp IsActiveThreadTerminating`.
struct CGameScriptInterface {                            // retail vtable @ 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                 // slot 7 (+0x1c)
};

struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04..+0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

struct CV_AmbushScamScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    unsigned char _flags49[0x55 - 0x49];
    bool TricksterDeadOrLeft;                            // +0x55
    bool TricksterComplete;                              // +0x56
    bool TricksterKilledBeforeAmbush;                    // +0x57
    bool TricksterKilledAfterAmbush;                     // +0x58
    unsigned char _flags59[0x61 - 0x59];
    bool AllAmbushersDead;                               // +0x61
    void WatchForQuestFinished();
};

void CV_AmbushScamScript::WatchForQuestFinished()
{
    while (!IsActiveThreadTerminating()) {
        if ((AllAmbushersDead && TricksterDeadOrLeft) || TricksterKilledBeforeAmbush || TricksterKilledAfterAmbush
            || TricksterComplete) {
            if (IsActiveThreadTerminating())
                return;
            QuestFinished = true;
            if (IsActiveThreadTerminating())
                return;
            return;
        }
        Game->NewScriptFrame();
    }
}