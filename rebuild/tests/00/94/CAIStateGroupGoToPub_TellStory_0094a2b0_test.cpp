// Behaviour test for 0094a2b0: body copied from src/0094a2b0.cpp (CAIStateGroup_GoToPub::TellStory), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
class CThing;
class CWorld;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual void v13() {}
    virtual long GetTimeToInitiate() { return 0; }
    virtual void v15() {} virtual void v16() {} virtual void v17() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
static int g_ror = 0;
unsigned long __fastcall GFROR13(unsigned long value) { ++g_ror; return (value >> 13) | (value << 19); }
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
class CCharStringData;
static int g_strCtors = 0, g_strDtors = 0;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length) : PStringData((CCharStringData*)text) { ++g_strCtors; CHECK(length == -1); }
    ~CCharString() { ++g_strDtors; }
};
struct CTCStoryTeller {
    unsigned char _pad[0x44];
    bool TellingStory;
};
static unsigned long g_seed = 0; static bool g_playResult = false; static int g_plays = 0; static float g_loopTime = -1; static bool g_tellingDuringPlay = false; static CTCStoryTeller* g_teller = 0; static char g_names[3][32];
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    unsigned char _pad14[0x10];
    CBaseIntelligentPointer PStoryListener;
    unsigned long* GetPWorldSeed() { return &g_seed; }
    bool PlayAnims(const CCharString& into, const CCharString& loop, const CCharString& outOf, float loopTime)
    {
        ++g_plays; g_loopTime = loopTime; g_tellingDuringPlay = g_teller->TellingStory;
        strcpy(g_names[0], (const char*)into.PStringData); strcpy(g_names[1], (const char*)loop.PStringData); strcpy(g_names[2], (const char*)outOf.PStringData);
        return g_playResult;
    }
    bool TellStory();
};
bool CAIStateGroup_GoToPub::TellStory()
{
    CTCStoryTeller* teller;
    PStoryListener.GetPItem()->GetInterface(&teller, 0xb8);
    teller->TellingStory = true;
    float loopTime = (float)WorldRandomRange(*GetPWorldSeed(), 10);
    bool result = PlayAnims(CCharString("TELLING_STORY_INTO", -1), CCharString("TELLING_STORY_LOOP", -1),
                            CCharString("TELLING_STORY_OUTOF", -1), loopTime);
    teller->TellingStory = false;
    return result;
}
static unsigned long Step(unsigned long s) { s = s * 0x24A1 + 0x24DF; return (s >> 13) | (s << 19); }
int main()
{
    static CThing listener; static CTCStoryTeller teller; memset(&listener, 0, sizeof(listener)); teller.TellingStory = false; g_teller = &teller;
    CInterfaceMap::SEntry entries[2] = { { 3, 0 }, { 0xb8, (CTCBase*)&teller } };
    listener.m_interfaces.first = entries; listener.m_interfaces.last = entries + 2; listener.m_presentBits[0xb8 >> 5] |= 1UL << (0xb8 & 31);
    CAIStateGroup_GoToPub g; g.PStoryListener.m_pItem = &listener;
    g_seed = 1234; unsigned long expected = Step(1234);
    CHECK(g.TellStory() == false);
    CHECK(g_plays == 1 && g_ror == 1 && g_seed == expected && g_loopTime == (float)(expected % 10)); CHECK(g_tellingDuringPlay && !teller.TellingStory);
    CHECK(strcmp(g_names[0], "TELLING_STORY_INTO") == 0 && strcmp(g_names[1], "TELLING_STORY_LOOP") == 0 && strcmp(g_names[2], "TELLING_STORY_OUTOF") == 0); CHECK(g_strCtors == 3 && g_strDtors == 3);
    g_playResult = true; CHECK(g.TellStory() == true); CHECK(g_plays == 2 && g_seed == Step(expected) && !teller.TellingStory);
    if (fails == 0) printf("0094a2b0_TEST PASS\n"); else printf("0094a2b0_TEST FAIL\n");
    return fails;
}