// CV_AmbushScamScript::WatchForQuestFinished() -- wait (one script frame at a time) until the quest is completed or finished; unless the thread is terminating or the quest finished, mark it completed
// Retail: Fable.exe @ 0x00D66E90, 73 bytes (+ int3 padding). Referenced once as a thread entry (0x00D66DA8).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame
// (slot 7, +0x1c) through Game at +0x40.
// Layout: the common quest-script flags (Ego_r.pdb - 0x14): QuestFinished +0x48, QuestComplete +0x49. The
// owning class is not identified (the catalog label is shared by several script bodies).
// Shape: the script WAIT macro `while (!(QuestComplete || QuestFinished)) { NewScriptFrame(); if (terminating)
// return; }` (rotated: QuestComplete is tested at the entry and at the bottom, QuestFinished at the aligned loop
// head), then the macro's trailing terminate test.
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

struct CV_AmbushScamScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    bool QuestComplete;                                  // +0x49
    void WatchForQuestFinished();
};

void CV_AmbushScamScript::WatchForQuestFinished()
{
    while (!(QuestComplete || QuestFinished)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!QuestFinished)
        QuestComplete = true;
}