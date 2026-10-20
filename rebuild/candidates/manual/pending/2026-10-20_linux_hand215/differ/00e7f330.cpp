// CV_FishingCompetitionScript::Init() -- reset the fishing competition: clear the flags and counters, pick one of three fisher names as the winner, randomise the biggest competitor fish from the fishing def and size the three-entry SizeComment vector (all -1)
// Grade: DIFFER(218v218) -- scheduling only: retail puts `mov [esi-8],ebx` (BiggestFish) after `faddp` and the resize fill temporary store after the fstp; ours schedules both one slot earlier (same instructions)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7F330, length 218 bytes (224 with padding).
// Callees: rand @ 0x00BFEB16, GFIntToCharString(long) @ 0x0099F830 and operator+(const char*, const CCharString&)
// @ 0x0099F690 (both `__fastcall` returning a CCharString through the hidden pointer in ecx),
// CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, vector<long>::resize(n, const long&) @ 0x004428F0
// (shared with vector<unsigned int>). Globals: the master data @ 0x0143E90C, whose +0x5d0 points at the fishing
// def (its float at +4 scaled by 0.25 is the competitor's base fish size; descriptive names). Literal
// "TEXT_QST_B15_FISHER_NAME_" @ 0x012EA64C.
// Layout (retail = Ego_r.pdb CV_FishingCompetitionScript - 0x14): BountyHuntActive +0x48 .. CompetitorsJustGotAttacked
// +0x50, AttackCounter +0x54, HitCount +0x5c, DoneIntro +0x60, RodHintGiven +0x61, PrizeNumber +0x64, Winner +0x68,
// BiggestFish +0x6c, BiggestCompetitorFish +0x70, SizeComment (12-byte STLport vector<long>) +0x74.
// Shape: stores in source order with one zero register; the def pointer is read into a local before the second
// rand(), whose remainder is a named local (homed in the dead string slot for the fild); the resize fill value is a
// `long` temporary bound to the const reference; the three element stores re-read the vector's start pointer.
extern "C" int __cdecl rand();                              // 0x00BFEB16
struct CCharString {
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

struct CFishingDef {
    unsigned char _pad0[4];
    float         m_competitorFishBase;                     // +0x04
};
struct CMasterData {
    unsigned char _pad000[0x5d0];
    CFishingDef*  m_pFishingDef;                            // +0x5d0
};
extern CMasterData* g_pMasterData;                          // 0x0143E90C

struct CLongVector {                                        // STLport vector<long>, 12 bytes in retail
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    void resize(unsigned long n, const long& value);       // 0x004428F0
};

struct CV_FishingCompetitionScript {
    unsigned char _base[0x48];                              // CScriptBase + interface/data pair
    bool        BountyHuntActive;                           // +0x48
    bool        OrganiserRanAway;                           // +0x49
    bool        OrganiserKilled;                            // +0x4a
    bool        CompetitionRunning;                         // +0x4b
    bool        CompetitionWon;                             // +0x4c
    bool        RodOfChampsGiven;                           // +0x4d
    bool        CommentIRanAway;                            // +0x4e
    bool        CompetitorsAttacked;                        // +0x4f
    bool        CompetitorsJustGotAttacked;                 // +0x50
    long        AttackCounter;                              // +0x54
    long        AttackDayCount;                             // +0x58
    long        HitCount;                                   // +0x5c
    bool        DoneIntro;                                  // +0x60
    bool        RodHintGiven;                               // +0x61
    long        PrizeNumber;                                // +0x64
    CCharString Winner;                                     // +0x68
    float       BiggestFish;                                // +0x6c
    float       BiggestCompetitorFish;                      // +0x70
    CLongVector SizeComment;                                // +0x74
    void Init();
};

void CV_FishingCompetitionScript::Init()
{
    CompetitorsAttacked = false;
    CompetitorsJustGotAttacked = false;
    OrganiserKilled = false;
    OrganiserRanAway = false;
    CompetitionRunning = true;
    BountyHuntActive = false;
    CompetitionWon = false;
    RodOfChampsGiven = false;
    CommentIRanAway = false;
    AttackCounter = 0;
    HitCount = 0;
    Winner = "TEXT_QST_B15_FISHER_NAME_" + GFIntToCharString(rand() % 3);
    CFishingDef* def = g_pMasterData->m_pFishingDef;
    long random = rand() % 50;
    BiggestCompetitorFish = (float)random * 0.1f + def->m_competitorFishBase * 0.25f;
    DoneIntro = false;
    RodHintGiven = false;
    BiggestFish = 0.0f;
    PrizeNumber = -1;
    SizeComment.resize(3, 0);
    SizeComment._M_start[0] = -1;
    SizeComment._M_start[1] = -1;
    SizeComment._M_start[2] = -1;
}
