// behaviour test for COpinionReactionManagerDef::operator= (0x0067CE50): base, name, sub-def map (vector + sorted flag), then every member in order; the POD block and both cursors copied
#include <cstdio>
static char g_log[32]; static int g_n = 0;
static void Log(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }
class CDefClassBase { public: int m_body[9]; CDefClassBase& operator=(const CDefClassBase& o) { Log('B'); for (int i = 0; i < 9; ++i) m_body[i] = o.m_body[i]; return *this; } };
template<char K> struct CCopy { int a, b, c; CCopy& operator=(const CCopy& o) { Log(K); a = o.a; b = o.b; c = o.c; return *this; } };
struct CKeyPairCompareLess {};
struct CVectorMap { CCopy<'s'> m_vector; CKeyPairCompareLess m_compare; bool m_sorted; };
class CDefString { public: long TablePos; };
class CParentDefClassBase : public CDefClassBase { public: CDefString InstantiationName; CVectorMap SubDefInfoMap; };
struct CList { void* m_head; CList& operator=(const CList& o) { Log('l'); m_head = o.m_head; return *this; } };
struct CTraitsArray { unsigned long m_words[0x9e]; };
class COpinionReactionManagerDef : public CParentDefClassBase { public:
    CCopy<'v'> AttitudeBlur; CCopy<'1'> AttitudeCondition; CCopy<'2'> TargetingCondition; CCopy<'3'> PreReactionDelay; CCopy<'4'> ToleranceToBeingHit;
    CCopy<'b'> BlockFurtherReactions, AllowSpeechOnNonPureAISpeaker, AllowWhileCarrying, AllowWhileFollowingPlayer;
    CList Matches; CTraitsArray FrequencyTraitsArray; void* FrequencyTraits; void* FrequencyTraitsHigh;
    COpinionReactionManagerDef& operator=(const COpinionReactionManagerDef& other)
    {
        CParentDefClassBase::operator=(other);
        AttitudeBlur = other.AttitudeBlur; AttitudeCondition = other.AttitudeCondition; TargetingCondition = other.TargetingCondition;
        PreReactionDelay = other.PreReactionDelay; ToleranceToBeingHit = other.ToleranceToBeingHit; BlockFurtherReactions = other.BlockFurtherReactions;
        AllowSpeechOnNonPureAISpeaker = other.AllowSpeechOnNonPureAISpeaker; AllowWhileCarrying = other.AllowWhileCarrying;
        AllowWhileFollowingPlayer = other.AllowWhileFollowingPlayer; Matches = other.Matches; FrequencyTraitsArray = other.FrequencyTraitsArray;
        FrequencyTraits = other.FrequencyTraits; FrequencyTraitsHigh = other.FrequencyTraitsHigh;
        return *this;
    }
};
int main()
{
    static COpinionReactionManagerDef a, b;
    int x = 1, y = 2;
    a.InstantiationName.TablePos = 77; a.SubDefInfoMap.m_sorted = true; a.SubDefInfoMap.m_vector.b = 3; a.AllowWhileCarrying.c = 9;
    a.Matches.m_head = &x; a.FrequencyTraitsArray.m_words[0x9d] = 0xabc; a.FrequencyTraits = (void*)&x; a.FrequencyTraitsHigh = (void*)&y;
    COpinionReactionManagerDef* r = &(b = a);
    const char* want = "Bsv1234bbbbl";
    int bad = r != &b;
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) bad = 1;
    if (b.InstantiationName.TablePos != 77 || !b.SubDefInfoMap.m_sorted || b.SubDefInfoMap.m_vector.b != 3 || b.AllowWhileCarrying.c != 9) bad = 1;
    if (b.Matches.m_head != &x || b.FrequencyTraitsArray.m_words[0x9d] != 0xabc || b.FrequencyTraits != (void*)&x || b.FrequencyTraitsHigh != (void*)&y) bad = 1;
    if (bad) { std::printf("0067ce50_TEST FAIL\n"); return 1; }
    std::printf("0067ce50_TEST PASS\n");
    return 0;
}