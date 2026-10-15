// CV_TempleOfLightScript::WatchForQuestInfoDisplay() -- every script frame, check whether the quest-info display state disagrees with "inside the temple and not attacked" (the retail branch body only re-checks thread termination)
// Leaf: WatchForQuestInfoDisplay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDBD10, length 81 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface slot 7
// NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// Identity: the manifest label CQ_BountyHuntScript::WatchForQuestSuccess is propagated (BSim); the only
// reference is CV_TempleOfLightScript::Main @ 0x00EDB190 (spawn at 0x00EDB512), and the flags fit Ego_r.pdb
// CV_TempleOfLightScript - 0x14: DisplayQuestInfo +0x56, InsideTemple +0x5a, AttackedByHero +0x5b. Twin of
// the chapel body 0x00E69F90 one byte higher; the method name is descriptive.
// Shape: as 0x00E69F90 -- the ternary condition and a body that only re-checks thread termination.
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

struct CV_TempleOfLightScript : CQuestScriptBase {
    unsigned char _pad48[0x0e];                             // +0x48
    bool DisplayQuestInfo;                                  // +0x56
    unsigned char _pad57[3];                                // +0x57
    bool InsideTemple;                                      // +0x5a
    bool AttackedByHero;                                    // +0x5b
    void WatchForQuestInfoDisplay();
};

void CV_TempleOfLightScript::WatchForQuestInfoDisplay()
{
    while (!IsActiveThreadTerminating()) {
        if (DisplayQuestInfo ? (AttackedByHero || !InsideTemple) : (InsideTemple && !AttackedByHero)) {
            if (IsActiveThreadTerminating())
                return;
        }
        GSI->NewScriptFrame();
    }
}