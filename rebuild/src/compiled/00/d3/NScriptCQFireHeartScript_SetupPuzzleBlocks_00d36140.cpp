// NScript::CQ_FireHeartScript::SetupPuzzleBlocks() -- copy the current round's pad pattern into StartingPattern, then raise ResetBlocks and wait frame by frame until every pad block has reported its reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D36140, length 166 bytes, plain `ret`.
// Identity: catalog CQ_FireHeartScript::SetupPuzzleBlocks; the members match the class's Ego_r.pdb layout once every
// retail STLport container is 12 bytes (vector<bool> included: retail stores one byte per element).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slot 7 NewScriptFrame (+0x1C).
// Layout (retail): NumberOfPads +0x7C, ResetBlocks +0x8C, NumBlocksReset +0x90, EvilRounds +0x9C, GoodRounds +0xA0,
// StartingPattern +0xBC (vector<bool>), Pattern +0x134 (vector<CFireHeartPatternDef>, 0x34-byte elements: the
// def base 0x28 bytes, PadActive vector<bool> +0x28) -- Ego_r.pdb offsets less 0x14 (script base) and 4 per container.
// Shape: a signed counted `for` with a termination return at the top of each pass; the pattern index is
// `GoodRounds * 5 + EvilRounds` (`lea eax,[eax+eax*4] ; add ; imul 34h`); the block-reset wait is the rotated script
// idiom; ResetBlocks is cleared from the termination test's zero `al`.
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
};

namespace std {
template <class T> struct vector {                      // retail STLport vector, 12 bytes (vector<bool> is byte-based)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
}

class CFireHeartPatternDef { public:                           // retail 0x34 bytes
    unsigned char     _defBase[0x28];                   // +0x00 .. +0x28 (CDefClassBase part)
    std::vector<bool> PadActive;                        // +0x28
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_FireHeartScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char _pad048[0x34];                        // +0x48 .. +0x7c (mission flags, cutscene maps, prophet counts)
    long NumberOfPads;                                  // +0x7c
    long NumberOfSuns;                                  // +0x80
    long NumberOfVillagers;                             // +0x84
    long PuzzleRound;                                   // +0x88
    bool ResetBlocks;                                   // +0x8c
    long NumBlocksReset;                                // +0x90
    unsigned char _flags094[8];                         // +0x94 .. +0x9c (round flags)
    long EvilRounds;                                    // +0x9c
    long GoodRounds;                                    // +0xa0
    unsigned char _pad0a4[0x18];                        // +0xa4 .. +0xbc (PadRadius, Effect, pad effect names)
    std::vector<bool> StartingPattern;                  // +0xbc
    unsigned char _pad0c8[0x6c];                        // +0xc8 .. +0x134 (prophet flags, prophet/villager/follower vectors)
    std::vector<CFireHeartPatternDef> Pattern;          // +0x134
    void SetupPuzzleBlocks();
};

void CQ_FireHeartScript::SetupPuzzleBlocks()
{
    for (long i = 0; i < NumberOfPads; ++i) {
        if (IsActiveThreadTerminating())
            return;
        StartingPattern[i] = Pattern[GoodRounds * 5 + EvilRounds].PadActive[i];
    }
    if (IsActiveThreadTerminating())
        return;
    NumBlocksReset = 0;
    ResetBlocks = true;
    while (NumBlocksReset < NumberOfPads) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ResetBlocks = false;
}
}