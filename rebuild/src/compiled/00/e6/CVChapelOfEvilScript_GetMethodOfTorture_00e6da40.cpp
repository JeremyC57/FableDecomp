// CV_ChapelOfEvilScript::GetMethodOfTorture() -- pick the next torture: the first seven in order (TortureCount), afterwards a random one of the seven
// Retail: Fable.exe @ 0x00E6DA40, 63 bytes (+ int3 padding). Called once (0x00E6C52C).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (the terminate check before each action), rand @ 0x00BFEB16.
// Layout: Ego_r.pdb CV_ChapelOfEvilScript - 0x14: TortureCount +0x70, TortureType +0x8c (ChapelOfEvilTortureType).
// Shape: if/else with the terminate check first in each arm; `TortureType = TortureCount++` stores the old value and
// then the incremented register; `rand() % 7` is a real `idiv` by 7 (signed remainder).
extern "C" int __cdecl rand();                           // 0x00BFEB16

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

struct CV_ChapelOfEvilScript : CQuestScriptBase {
    unsigned char _pad48[0x70 - 0x48];
    long TortureCount;                                   // +0x70
    unsigned char _pad74[0x8c - 0x74];
    long TortureType;                                    // +0x8c
    void GetMethodOfTorture();
};

void CV_ChapelOfEvilScript::GetMethodOfTorture()
{
    if (TortureCount < 7) {
        if (IsActiveThreadTerminating())
            return;
        TortureType = TortureCount++;
    } else {
        if (IsActiveThreadTerminating())
            return;
        TortureType = rand() % 7;
    }
}