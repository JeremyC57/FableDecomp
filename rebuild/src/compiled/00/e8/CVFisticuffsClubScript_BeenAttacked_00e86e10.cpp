// CV_FisticuffsClubScript::BeenAttacked() -- outside the club, count one more attack warning, or once the def's warning limit is reached make everybody attack
// Retail: Fable.exe @ 0x00E86E10, 78 bytes (+ int3 padding). Called from three places (0x00E852E1, 0x00E85F7C, 0x00E88354).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (the script's terminate check before each action).
// Layout: Ego_r.pdb CV_FisticuffsClubScript - 0x14: HeroInFisticuffsClub +0x53, AttackWarnings +0x84, EverybodyAttack
// +0x8c; the warning limit is the long at +0x624 of the script def @ 0x0143E90C (descriptive name).
// Shape: nested ifs, each action preceded by `if (terminating) return;`; `++AttackWarnings` is `inc dword ptr [esi+84h]`.
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

struct CScriptDef {
    unsigned char _pad[0x624];
    long FisticuffsAttackWarningLimit;                   // +0x624
};
extern CScriptDef* g_pScriptDef;                         // 0x0143E90C

struct CV_FisticuffsClubScript : CQuestScriptBase {
    unsigned char _pad48[0x53 - 0x48];
    bool HeroInFisticuffsClub;                           // +0x53
    unsigned char _pad54[0x84 - 0x54];
    long AttackWarnings;                                 // +0x84
    long FightCount;                                     // +0x88
    bool EverybodyAttack;                                // +0x8c
    void BeenAttacked();
};

void CV_FisticuffsClubScript::BeenAttacked()
{
    if (!HeroInFisticuffsClub) {
        if (IsActiveThreadTerminating())
            return;
        if (AttackWarnings < g_pScriptDef->FisticuffsAttackWarningLimit) {
            if (IsActiveThreadTerminating())
                return;
            ++AttackWarnings;
        } else {
            if (IsActiveThreadTerminating())
                return;
            EverybodyAttack = true;
        }
    }
}