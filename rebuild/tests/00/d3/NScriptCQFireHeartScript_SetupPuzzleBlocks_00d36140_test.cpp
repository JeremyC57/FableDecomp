// Behaviour test for 00d36140: the model below is a copy of src/00d36140.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
class CGameScriptInterface { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
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
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

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

// ---- test doubles and driver ----
static int g_frames = 0, g_termAt = 1000, g_termCalls = 0, g_resetSeen = 0;
static NScript::CQ_FireHeartScript* g_s = 0;
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_s->ResetBlocks) { ++g_resetSeen; ++g_s->NumBlocksReset; } }
bool NScript::CScriptBase::IsActiveThreadTerminating() { ++g_termCalls; return g_termCalls > g_termAt; }

int main()
{
    static CGameScriptInterface gsi;
    static CFireHeartPatternDef defs[12];
    static bool bits[12][5];
    for (int p = 0; p < 12; ++p) {
        for (int k = 0; k < 5; ++k) bits[p][k] = ((p * 7 + k * 3) % 4) == 0;
        defs[p].PadActive._M_start = bits[p]; defs[p].PadActive._M_finish = bits[p] + 5; defs[p].PadActive._M_end_of_storage = bits[p] + 5;
    }
    bool start[5];
    bool ok = true;
    NScript::CQ_FireHeartScript s;
    s.GSI = &gsi; s.PMasterData = 0; g_s = &s;
    s.Pattern._M_start = defs; s.Pattern._M_finish = defs + 12; s.Pattern._M_end_of_storage = defs + 12;
    s.StartingPattern._M_start = start; s.StartingPattern._M_finish = start + 5; s.StartingPattern._M_end_of_storage = start + 5;

    // full run: pattern 1*5+2 = 7 copied, reset raised, three frames until five blocks report (NumBlocksReset starts at 0)
    std::memset(start, 0xcd, sizeof(start));
    s.NumberOfPads = 5; s.GoodRounds = 1; s.EvilRounds = 2; s.NumBlocksReset = 77; s.ResetBlocks = false;
    g_frames = 0; g_termAt = 1000; g_termCalls = 0; g_resetSeen = 0;
    s.SetupPuzzleBlocks();
    ok = ok && std::memcmp(start, bits[7], 5) == 0 && g_frames == 5 && g_resetSeen == 5 && s.NumBlocksReset == 5 && !s.ResetBlocks;

    // terminated during the copy: only the first two pads copied, no reset
    std::memset(start, 0, sizeof(start));
    s.GoodRounds = 2; s.EvilRounds = 0; s.NumBlocksReset = 77; s.ResetBlocks = false;
    g_frames = 0; g_termAt = 2; g_termCalls = 0;
    s.SetupPuzzleBlocks();
    ok = ok && std::memcmp(start, bits[10], 2) == 0 && start[2] == 0 && start[3] == 0 && g_frames == 0 && s.NumBlocksReset == 77;

    // terminated while waiting: ResetBlocks stays raised
    s.NumberOfPads = 3; s.GoodRounds = 0; s.EvilRounds = 4; s.NumBlocksReset = 0;
    g_frames = 0; g_termAt = 5; g_termCalls = 0;
    s.SetupPuzzleBlocks();
    ok = ok && std::memcmp(start, bits[4], 3) == 0 && s.ResetBlocks && g_frames == 2 && s.NumBlocksReset == 2;

    if (!ok) { std::printf("00d36140_TEST FAIL\n"); return 1; }
    std::printf("00d36140_TEST PASS\n");
    return 0;
}