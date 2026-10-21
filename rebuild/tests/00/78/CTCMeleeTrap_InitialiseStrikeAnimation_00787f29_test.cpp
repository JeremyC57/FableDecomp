#include <cstdio>
#include <cstring>
// Behaviour test for CTCMeleeTrap::InitialiseStrikeAnimation (0x00787F29): same body as src/, with the animation
// manager's event-name query, the world bank query and the vector storage stubbed and recorded.
long Compare_00411570(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
template<class T> class CBasicString {
public:
    T*            PData;
    unsigned long StringLength;
    bool operator==(const CBasicString& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare_00411570(PData, other.PData) == 0;
    }
};
struct CCharStringData : CBasicString<char> { };
struct CCharString {
    CCharStringData* PStringData;
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    bool StartsWith(const CCharString& prefix) const
    {
        return strncmp(GetPString(), prefix.GetPString(), prefix.Length()) == 0;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return *PStringData == *other.PStringData;
        return false;
    }
};
static CCharStringData MakeData(const char* s)
{
    CCharStringData d; d.PData = (char*)s; d.StringLength = (unsigned long)std::strlen(s); return d;
}
static CCharStringData d_start = MakeData("STRIKE_START_"), d_end = MakeData("STRIKE_END_"),
    d_ev = MakeData("COMBT_STRIKE_EVENT"), d_stop = MakeData("COMBT_STOP"), d_ev2 = MakeData("COMBT_STRIKE_EVENT_2"),
    d_stop2 = MakeData("COMBT_STOP_2");
CCharString g_StrikeStart = { &d_start }, g_StrikeEnd = { &d_end }, g_CombatStrikeEvent = { &d_ev },
    g_CombatStop = { &d_stop }, g_CombatStrikeEvent2 = { &d_ev2 }, g_CombatStop2 = { &d_stop2 };

static int g_vectorDtors = 0;
struct CStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CStringVector() { ++g_vectorDtors; }
};
static CCharString* g_fillStart = 0;
static int g_fillCount = 0;
static long g_queriedNames = -1, g_queriedBank = -1;
struct CAnimationManager {
    void GetEventNames(long animation, CStringVector& names)
    {
        g_queriedNames = animation;
        names._M_start = g_fillStart; names._M_finish = g_fillStart + g_fillCount; names._M_end_of_storage = names._M_finish;
    }
};
static CAnimationManager g_manager;
CAnimationManager* GetAnimationManager() { return &g_manager; }
struct CAnimationBank {
    long Query(long animation) const { g_queriedBank = animation; return 0; }
};
struct CWorld { CAnimationBank* PAnimationBank; };
static CAnimationBank g_bank;
static CWorld g_world = { &g_bank };
struct CAnimationEntry { long Animation; };

struct CTCMeleeTrap {
    bool StrikeAnimation;
    CWorld* GetWorld() const { return &g_world; }
    void InitialiseStrikeAnimation(const CAnimationEntry& entry);
};

void CTCMeleeTrap::InitialiseStrikeAnimation(const CAnimationEntry& entry)
{
    long animation = entry.Animation;
    CAnimationManager* manager = GetAnimationManager();
    CStringVector names;
    manager->GetEventNames(animation, names);
    StrikeAnimation = false;
    CWorld* world = GetWorld();
    world->PAnimationBank->Query(entry.Animation);
    for (CCharString* it = names._M_start; it != names._M_finish; ++it) {
        if (it->StartsWith(g_StrikeStart) || *it == g_CombatStrikeEvent || *it == g_CombatStrikeEvent2)
            StrikeAnimation = true;
        else if (it->StartsWith(g_StrikeEnd) || *it == g_CombatStop || *it == g_CombatStop2) {
        }
    }
}

int main()
{
    int fails = 0;
    CCharStringData a = MakeData("FOOTSTEP"), b = MakeData("STRIKE_START_01"), c = MakeData("COMBT_STRIKE_EVENT_2"),
                    e = MakeData("STRIKE_END_02"), f = MakeData("COMBT_STRIKE_EVENT");
    CTCMeleeTrap trap;
    CAnimationEntry entry = { 42 };
    // No names: cleared, both queries made with the entry's animation.
    trap.StrikeAnimation = true; g_fillCount = 0;
    trap.InitialiseStrikeAnimation(entry);
    if (trap.StrikeAnimation || g_queriedNames != 42 || g_queriedBank != 42 || g_vectorDtors != 1) ++fails;
    // Only non-strike names (a null string, a plain name, an end marker): stays clear.
    CCharString list1[3] = { { 0 }, { &a }, { &e } };
    g_fillStart = list1; g_fillCount = 3; trap.StrikeAnimation = true;
    trap.InitialiseStrikeAnimation(entry);
    if (trap.StrikeAnimation) ++fails;
    // Prefix match.
    CCharString list2[2] = { { &a }, { &b } };
    g_fillStart = list2; g_fillCount = 2;
    trap.InitialiseStrikeAnimation(entry);
    if (!trap.StrikeAnimation) ++fails;
    // Exact match on the second strike event (separate block, equal text).
    CCharString list3[1] = { { &c } };
    g_fillStart = list3; g_fillCount = 1;
    trap.InitialiseStrikeAnimation(entry);
    if (!trap.StrikeAnimation) ++fails;
    // Exact match on the shared global block itself, and on equal text.
    CCharString list4[1] = { { &d_ev } };
    g_fillStart = list4; trap.InitialiseStrikeAnimation(entry);
    if (!trap.StrikeAnimation) ++fails;
    CCharString list5[1] = { { &f } };
    g_fillStart = list5; trap.InitialiseStrikeAnimation(entry);
    if (!trap.StrikeAnimation) ++fails;
    if (g_vectorDtors != 6) ++fails;
    if (fails) { std::printf("00787f29_TEST FAIL %d\n", fails); return 1; }
    std::printf("00787f29_TEST PASS\n");
    return 0;
}