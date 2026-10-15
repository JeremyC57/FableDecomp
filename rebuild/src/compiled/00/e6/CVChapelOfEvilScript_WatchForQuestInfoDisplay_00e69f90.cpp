// CV_ChapelOfEvilScript::WatchForQuestInfoDisplay() -- every script frame, check whether the quest-info display state disagrees with "inside the chapel and not attacked" (the retail branch body only re-checks thread termination)
// Leaf: WatchForQuestInfoDisplay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E69F90, length 81 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface slot 7
// NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// Identity: the manifest label CQ_BountyHuntScript::WatchForQuestSuccess is propagated (BSim); the only
// reference is CV_ChapelOfEvilScript::Main @ 0x00E693F0 (spawn at 0x00E697DE), and the flags fit Ego_r.pdb
// CV_ChapelOfEvilScript - 0x14: DisplayQuestInfo +0x55, InsideChapel +0x59, AttackedByHero +0x5a. The real
// method name is unknown; WatchForQuestInfoDisplay is descriptive.
// Shape: the condition is the ternary `DisplayQuestInfo ? (AttackedByHero || !InsideChapel) : (InsideChapel &&
// !AttackedByHero)` (both arms share the final `test al,al ; jne`); the taken body is only `if (terminating)
// return;` (no store survives in retail).
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

struct CV_ChapelOfEvilScript : CQuestScriptBase {
    unsigned char _pad48[0x0d];                             // +0x48
    bool DisplayQuestInfo;                                  // +0x55
    unsigned char _pad56[3];                                // +0x56
    bool InsideChapel;                                      // +0x59
    bool AttackedByHero;                                    // +0x5a
    void WatchForQuestInfoDisplay();
};

void CV_ChapelOfEvilScript::WatchForQuestInfoDisplay()
{
    while (!IsActiveThreadTerminating()) {
        if (DisplayQuestInfo ? (AttackedByHero || !InsideChapel) : (InsideChapel && !AttackedByHero)) {
            if (IsActiveThreadTerminating())
                return;
        }
        GSI->NewScriptFrame();
    }
}