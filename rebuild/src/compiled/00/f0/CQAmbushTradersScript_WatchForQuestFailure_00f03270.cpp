// CQ_AmbushTradersScript::WatchForQuestFailure() -- every script frame, fail the quest once the hero, the bandit contact or the spy is dead or the spy has escaped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F03270, length 80 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface slot 7
// NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// Layout (PDB offsets - 0x14): QuestFailed +0x48, ContactDead +0x55, HeroDead +0x57, SpyDead +0x58,
// SpyEscaped +0x60. The spawning Main at 0x00F024C0 is labelled CQ_HangingTreeGoodScript::Main in the
// manifest, but those offsets only make sense for the ambush script.
// Shape: `while (!terminating) { if (a || b || c || d) { if (terminating) return; QuestFailed = true; }
// NewScriptFrame(); }` -- the loop condition is tested once on entry and again at the bottom; the
// short-circuit chain tests the flags in source order (+0x57, +0x55, +0x58, +0x60).
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

struct CQ_AmbushTradersScript : CQuestScriptBase {
    bool QuestFailed;                                       // +0x48
    unsigned char _pad49[0x0c];                             // +0x49
    bool ContactDead;                                       // +0x55
    bool ContactAttackedByHero;                             // +0x56
    bool HeroDead;                                          // +0x57
    bool SpyDead;                                           // +0x58
    unsigned char _pad59[7];                                // +0x59
    bool SpyEscaped;                                        // +0x60
    void WatchForQuestFailure();
};

void CQ_AmbushTradersScript::WatchForQuestFailure()
{
    while (!IsActiveThreadTerminating()) {
        if (HeroDead || ContactDead || SpyDead || SpyEscaped) {
            if (IsActiveThreadTerminating())
                return;
            QuestFailed = true;
        }
        GSI->NewScriptFrame();
    }
}