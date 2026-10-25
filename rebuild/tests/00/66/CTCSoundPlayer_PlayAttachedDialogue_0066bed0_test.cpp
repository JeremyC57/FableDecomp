#include <cstdio>
#include <cstring>

// Host model of CTCSoundPlayer::PlayAttachedDialogue(const CCharString&, bool, bool, bool, bool): CRC the name (seed 0,
// chars and length; an unassigned string is "" / 0), map the CRC to the game-text tag through the text bank, then call
// the tag overload with the four flags unchanged.
struct CCharStringData { const char* PData; unsigned int StringLength; };
struct CCharString {
    CCharStringData* PStringData;
    unsigned int GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
};
static unsigned int Calc(unsigned int seed, const void* data, unsigned int length)   // stand-in CRC
{
    const unsigned char* p = (const unsigned char*)data;
    unsigned int h = seed ^ 0x811C9DC5u;
    for (unsigned int i = 0; i < length; ++i) h = (h ^ p[i]) * 16777619u;
    return h;
}
static unsigned int GetCRC(const CCharString& s) { return Calc(0, s.GetString(), s.GetLength()); }

struct CDataBank {
    unsigned int Crc[4]; unsigned int Tag[4]; int Count;
    unsigned int GetTagFromCRC(unsigned int crc) const
    {
        for (int i = 0; i < Count; ++i) if (Crc[i] == crc) return Tag[i];
        return 0;
    }
};
struct CMainGameComponent { CDataBank* PGameTextDataBank; };
static CMainGameComponent* g_pMainGameComponent;

struct CTCSoundPlayer {
    unsigned int LastTag; bool Flags[4]; int Calls;
    unsigned int PlayAttachedDialogue(unsigned int tag, bool a, bool b, bool c, bool d)
    {
        LastTag = tag; Flags[0] = a; Flags[1] = b; Flags[2] = c; Flags[3] = d; ++Calls;
        return tag + 1000;
    }
    unsigned int PlayAttachedDialogue(const CCharString& name, bool a, bool b, bool c, bool d)
    {
        const CDataBank* bank = g_pMainGameComponent->PGameTextDataBank;
        unsigned int tag = bank->GetTagFromCRC(GetCRC(name));
        return PlayAttachedDialogue(tag, a, b, c, d);
    }
};

int main()
{
    CCharStringData hello = { "TEXT_WIFE_HELLO", 15 };
    CCharStringData bye = { "TEXT_WIFE_BYE", 13 };
    CDataBank bank;
    bank.Count = 3;
    bank.Crc[0] = Calc(0, hello.PData, hello.StringLength); bank.Tag[0] = 41;
    bank.Crc[1] = Calc(0, bye.PData, bye.StringLength);     bank.Tag[1] = 42;
    bank.Crc[2] = Calc(0, "", 0);                            bank.Tag[2] = 43;
    CMainGameComponent game = { &bank };
    g_pMainGameComponent = &game;
    CTCSoundPlayer player; player.Calls = 0;
    CCharString name; name.PStringData = &hello;
    if (player.PlayAttachedDialogue(name, false, false, false, true) != 1041 || player.LastTag != 41) { std::printf("0066bed0_TEST FAIL\n"); return 1; }
    if (player.Flags[0] || player.Flags[1] || player.Flags[2] || !player.Flags[3]) { std::printf("0066bed0_TEST FAIL\n"); return 1; }
    name.PStringData = &bye;
    if (player.PlayAttachedDialogue(name, false, false, true, false) != 1042 || !player.Flags[2] || player.Flags[3]) { std::printf("0066bed0_TEST FAIL\n"); return 1; }
    name.PStringData = 0;                                   // unassigned: the CRC of ""
    if (player.PlayAttachedDialogue(name, true, false, false, false) != 1043 || !player.Flags[0]) { std::printf("0066bed0_TEST FAIL\n"); return 1; }
    if (player.Calls != 3) { std::printf("0066bed0_TEST FAIL\n"); return 1; }
    std::printf("0066bed0_TEST PASS\n");
    return 0;
}