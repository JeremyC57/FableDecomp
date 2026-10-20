// Behaviour test for CTCHeroOnlineScoreboard::UpdateScoreForActivity (0x0055F3B0): a first score is stored; later
// scores replace it only when better (lower with lowerIsBetter, else higher); the message temporaries balance.
#include <cstdio>

static int g_live = 0;
struct CCharString {
    int n;
    CCharString(const char*, long) : n(1) { ++g_live; }
    CCharString(const CCharString& o) : n(o.n) { ++g_live; }
    ~CCharString() { --g_live; }
};
CCharString GFIntToCharString(long) { return CCharString("0", -1); }
CCharString operator+(const CCharString&, const CCharString&) { return CCharString("x", -1); }
enum EOnlineScoreboardActivity { ACT_0, ACT_1, ACT_2 };
struct SScoreNode { EOnlineScoreboardActivity key; float value; };
struct CScoreMap {
    struct iterator { SScoreNode* _M_node; iterator() {} };
    SScoreNode* _M_header; unsigned long _M_node_count; unsigned char _M_key_compare;
    SScoreNode nodes[8]; SScoreNode header;
    void Init() { _M_header = &header; _M_node_count = 0; }
    iterator find(const EOnlineScoreboardActivity& key)
    { iterator it; it._M_node = _M_header; for (unsigned long i = 0; i < _M_node_count; ++i) if (nodes[i].key == key) it._M_node = &nodes[i]; return it; }
    float& operator[](const EOnlineScoreboardActivity& key)
    { iterator it = find(key); if (it._M_node != _M_header) return it._M_node->value; nodes[_M_node_count].key = key; nodes[_M_node_count].value = 0.0f; return nodes[_M_node_count++].value; }
};
struct CTCHeroOnlineScoreboard {
    CScoreMap LocalScoreboard;
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool lowerIsBetter);
};
void CTCHeroOnlineScoreboard::UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool lowerIsBetter)
{
    {
        CScoreMap::iterator it = LocalScoreboard.find(activity);
        if (it._M_node == LocalScoreboard._M_header) {
            LocalScoreboard[activity] = score;
            return;
        }
    }
    if (lowerIsBetter) {
        if (!(score < LocalScoreboard[activity]))
            return;
    } else if (!(score > LocalScoreboard[activity])) {
        return;
    }
    CCharString message = CCharString("New best score in activity ", -1) + GFIntToCharString(activity);
    LocalScoreboard[activity] = score;
}
int main()
{
    CTCHeroOnlineScoreboard sb; sb.LocalScoreboard.Init();
    sb.UpdateScoreForActivity(ACT_1, 10.0f, false);
    if (sb.LocalScoreboard._M_node_count != 1 || sb.LocalScoreboard[ACT_1] != 10.0f) { std::printf("0055f3b0_TEST FAIL\n"); return 1; }
    sb.UpdateScoreForActivity(ACT_1, 5.0f, false);
    if (sb.LocalScoreboard[ACT_1] != 10.0f) { std::printf("0055f3b0_TEST FAIL\n"); return 1; }
    sb.UpdateScoreForActivity(ACT_1, 12.0f, false);
    if (sb.LocalScoreboard[ACT_1] != 12.0f || g_live != 0) { std::printf("0055f3b0_TEST FAIL\n"); return 1; }
    sb.UpdateScoreForActivity(ACT_2, 30.0f, true);
    sb.UpdateScoreForActivity(ACT_2, 40.0f, true);
    if (sb.LocalScoreboard[ACT_2] != 30.0f) { std::printf("0055f3b0_TEST FAIL\n"); return 1; }
    sb.UpdateScoreForActivity(ACT_2, 20.0f, true);
    if (sb.LocalScoreboard[ACT_2] != 20.0f || sb.LocalScoreboard._M_node_count != 2 || g_live != 0) { std::printf("0055f3b0_TEST FAIL\n"); return 1; }
    std::printf("0055f3b0_TEST PASS\n");
    return 0;
}
