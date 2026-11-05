// Behaviour test for 0x00f21590: NScript::CQ_ArenaScript::GetEndRoundQuestion -- question tag per gold multiplier
// Host model of CCharString / TRet<T> and the reconstructed body; checks every case, the default and the string lifetimes.
#include <cstdio>
#include <cstring>
#include <string>
static int g_fail = 0;
static int g_live = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
class CCharString {
public:
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; ++g_live; }
    CCharString(const CCharString& o) : S(o.S) { ++g_live; }
    ~CCharString() { --g_live; }
};
template <class T> class TRet {
public:
    T Val;
    TRet(const T& val) : Val(val) { }
    ~TRet() { }
    T Return() { return Val; }
};
class CQ_ArenaScript {
public:
    long GoldMultiplier;
    CCharString GetEndRoundQuestion()
    {
        TRet<CCharString> _Fail_Val("An error");
        switch (GoldMultiplier)
        {
        case 0: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_1";
        case 1: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_2";
        case 2: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_3";
        case 3: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_4";
        case 4: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_5";
        case 5: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_6";
        case 6: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_7";
        case 7: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_8";
        }
        return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION";
    }
};
static void run()
{
    CQ_ArenaScript s;
    char want[64];
    for (long m = 0; m < 8; ++m) {
        s.GoldMultiplier = m;
        {
            CCharString q = s.GetEndRoundQuestion();
            std::sprintf(want, "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_%ld", m + 1);
            CHECK(q.S == want);
            CHECK(g_live == 1);
        }
        CHECK(g_live == 0);
    }
    s.GoldMultiplier = 8;
    { CCharString q = s.GetEndRoundQuestion(); CHECK(q.S == "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION"); }
    s.GoldMultiplier = -1;
    { CCharString q = s.GetEndRoundQuestion(); CHECK(q.S == "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION"); }
    CHECK(g_live == 0);
}
int main()
{
    run();
    if (g_fail) { std::printf("00f21590_TEST FAIL\n"); return 1; }
    std::printf("00f21590_TEST PASS\n");
    return 0;
}