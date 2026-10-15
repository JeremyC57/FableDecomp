// CV_ExposeMayorScript::PhaseOne() -- wait (one script frame at a time) until the path stage leaves 0, then the macro's trailing terminate check
// Retail: Fable.exe @ 0x00E7A580, 52 bytes (+ int3 padding). Called once (0x00E7A4BA).
// Callees: CGameScriptInterface::NewScriptFrame (slot 7, +0x1c) through Game at +0x40, CScriptBase::IsActiveThreadTerminating
// @ 0x00CB7940 (in the loop, and the trailing one as a tail jump).
// Layout: Ego_r.pdb CV_ExposeMayorScript - 0x14: Path +0x48, StageNumber +0x4c.
// Shape: the WAIT macro `while (StageNumber == 0) { NewScriptFrame(); if (terminating) return; }` (rotated, aligned
// loop head) followed by `if (terminating) return;` whose unused result makes the last call a `jmp`.
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

struct CV_ExposeMayorScript : CQuestScriptBase {
    long Path;                                           // +0x48 (PATH_ID)
    long StageNumber;                                    // +0x4c (STAGE_IN_PATH)
    void PhaseOne();
};

void CV_ExposeMayorScript::PhaseOne()
{
    while (StageNumber == 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}