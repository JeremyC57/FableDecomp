// CV_FishingCompetitionScript::GenerateCompetitionEntry() -- one time in four, a competitor lands a fish of 0.1..0.9 above the current best competitor catch; if it still stays below the next prize threshold it becomes the best competitor catch and a random fisher (one of three) becomes the winner text tag
// Leaf: GenerateCompetitionEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7F730, length 156 bytes (padded to 160), `ret`.
// Callees: rand @ 0x00BFEB16 (x3), CV_FishingCompetitionScript::GetPrizeThreshold @ 0x00E7F7D0 (catalog
// "GetFloatTableEntryByIndex": the script-def float table @ [0x0143E90C]+0x5d0 indexed by PrizeNumber + 1, skipping -1;
// descriptive name), GFIntToCharString @ 0x0099F830 (__fastcall, by value), operator+(const char*, const CCharString&)
// @ 0x0099F690 (__fastcall, "TEXT_QST_B15_FISHER_NAME_" @ 0x012EA64C), CCharString::operator= @ 0x0099EFB0,
// ~CCharString @ 0x0099EAE0 (x2). Constant 0.1f @ 0x0122EE58.
// Layout (retail = Ego_r.pdb CV_FishingCompetitionScript - 0x14): PrizeNumber +0x64, Winner +0x68, BiggestFish +0x6c,
// BiggestCompetitorFish +0x70.
extern "C" int __cdecl rand();                     // 0x00BFEB16
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
CCharString __fastcall GFIntToCharString(long value);                         // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CV_FishingCompetitionScript {
    unsigned char _base[0x64];                     // CScriptBase + interface/data pair + flags/counters
    long        PrizeNumber;                       // +0x64
    CCharString Winner;                            // +0x68
    float       BiggestFish;                       // +0x6c
    float       BiggestCompetitorFish;             // +0x70
    float GetPrizeThreshold();                     // 0x00E7F7D0
    void GenerateCompetitionEntry();
};
void CV_FishingCompetitionScript::GenerateCompetitionEntry()
{
    if (rand() % 4 <= 0)
    {
        float weight = (rand() % 9 + 1) * 0.1f + BiggestCompetitorFish;
        if (GetPrizeThreshold() > weight)
        {
            BiggestCompetitorFish = weight;
            Winner = "TEXT_QST_B15_FISHER_NAME_" + GFIntToCharString(rand() % 3);
        }
    }
}
