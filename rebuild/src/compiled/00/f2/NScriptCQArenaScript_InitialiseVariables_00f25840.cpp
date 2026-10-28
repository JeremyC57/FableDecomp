// CQ_ArenaScript::InitialiseVariables() -- copy the script def's ArenaRounds vector into Rounds, then fill the 4x5 crowd-loop sound-tag table
// Leaf: ?InitialiseVariables@CQ_ArenaScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25840, length 305 bytes, plain `ret`.
// Callees: vector<CArenaRoundDef>::operator= @ 0x00F25980 (out-of-line STLport assignment, same TU as the
// caller in retail), CCharString::operator=(const char*) @ 0x0099EFE0 (twenty times, one per table cell).
// Data: the script def @ 0x0143E90C; its ArenaRounds vector sits at retail +0x1044 (Ego_r.pdb +0x1104, minus
// 4 per preceding STLport container: retail vectors are 12 bytes, the PDB's 16).
// Layout (from the landed CQ_ArenaScript ctor 0x00CFA660, Ego_r.pdb - 0x14): CrowdLoopTags (CCharString[4][5])
// +0x48, Rounds +0x98.
// Shape: twenty `lea ecx,[esi+N] ; push literal ; call` assignments in row-major order; equal literals pool
// to one .rdata string (HECKLE_SMALL/CHANT/CHEER/AWWW repeat).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    const CCharString& operator=(const char* text);        // 0x0099EFE0
};

class CArenaRoundDef;

struct CArenaRoundVector {                           // STLport vector<CArenaRoundDef> (12 bytes in retail)
    CArenaRoundDef* _M_start;
    CArenaRoundDef* _M_finish;
    CArenaRoundDef* _M_end_of_storage;
    CArenaRoundVector& operator=(const CArenaRoundVector& other);   // 0x00F25980
};

class CScriptDef { public:
    unsigned char _pad0[0x1044];
    CArenaRoundVector ArenaRounds;                   // +0x1044
};
extern CScriptDef* pGlobalScriptDef;                     // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

namespace NScript {
class CQ_ArenaScript {
public:
    unsigned char _base[0x48];                       // CScriptBase + interface/owner pair
    CCharString CrowdLoopTags[4][5];                 // +0x48
    CArenaRoundVector Rounds;                        // +0x98
    void InitialiseVariables();
};
} // namespace NScript
using NScript::CQ_ArenaScript;

void NScript::CQ_ArenaScript::InitialiseVariables()
{
    Rounds = pGlobalScriptDef->ArenaRounds;
    CrowdLoopTags[0][0] = "ARENA_HECKLE_SMALL_LOOP";
    CrowdLoopTags[0][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[0][2] = "ARENA_APPLAUSE_SMALL_LOOP";
    CrowdLoopTags[0][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[0][4] = "ARENA_AWWW";
    CrowdLoopTags[1][0] = "ARENA_HECKLE_SMALL_LOOP";
    CrowdLoopTags[1][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[1][2] = "ARENA_APPLAUSE_MEDIUM_LOOP";
    CrowdLoopTags[1][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[1][4] = "ARENA_AWWW";
    CrowdLoopTags[2][0] = "ARENA_HECKLE_MEDIUM_LOOP";
    CrowdLoopTags[2][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[2][2] = "ARENA_APPLAUSE_BIG_LOOP";
    CrowdLoopTags[2][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[2][4] = "ARENA_AWWW";
    CrowdLoopTags[3][0] = "ARENA_HECKLE_BIG_LOOP";
    CrowdLoopTags[3][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[3][2] = "ARENA_APPLAUSE_BIG_LOOP";
    CrowdLoopTags[3][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[3][4] = "ARENA_AWWW";
}