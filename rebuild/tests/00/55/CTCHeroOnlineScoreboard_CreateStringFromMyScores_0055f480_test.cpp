// Behaviour test for CTCHeroOnlineScoreboard::CreateStringFromMyScores (0055f480): header, user, session, then per activity id, LIB/HIB and the score
#include <cstdio>
#include <cstring>

struct Str {
    char buf[512];
    int len;
    Str() : len(0) { buf[0] = 0; }
    Str(const char* s) : len(0) { buf[0] = 0; add(s); }
    Str& add(const char* s) { while (*s && len < 511) buf[len++] = *s++; buf[len] = 0; return *this; }
    Str& add(const Str& s) { return add(s.buf); }
    Str& addInt(int v) { char t[32]; std::sprintf(t, "%d", v); return add(t); }
    Str sub(int start, int count) const
    {
        Str r;
        if (start < 0) start = 0;
        for (int i = start; i < len && i < start + count; ++i) r.buf[r.len++] = buf[i];
        r.buf[r.len] = 0;
        return r;
    }
    bool eq(const char* s) const { return std::strcmp(buf, s) == 0; }
};

struct Activity { bool LowerIsBetter; };
struct Score { int activity; double best; };

static Str CreateStringFromMyScores(const char* user, const char* session, const Activity* info, const Score* scores, int count)
{
    Str result("Fable1.5\r\nVersion 1\r\n");
    result.add("UPLOAD_SCORES\r\n");
    result.add(user).add("\r\n").add(session).add("\r\n");
    for (int i = 0; i < count; ++i) {
        result.addInt(scores[i].activity).add("\r\n");
        result.add(info[scores[i].activity].LowerIsBetter ? "LIB" : "HIB").add("\r\n");
        char t[64];
        std::sprintf(t, "%.6f", scores[i].best);
        result.add(t).add("\r\n");
    }
    return result;
}

int main()
{
    Activity info[3] = { { false }, { true }, { false } };
    Score scores[2] = { { 1, 12.5 }, { 2, 3.0 } };
    Str s = CreateStringFromMyScores("hero", "S1", info, scores, 2);
    if (!s.eq("Fable1.5\r\nVersion 1\r\nUPLOAD_SCORES\r\nhero\r\nS1\r\n1\r\nLIB\r\n12.500000\r\n2\r\nHIB\r\n3.000000\r\n")) {
        std::printf("0055f480_TEST FAIL\n");
        return 1;
    }
    std::printf("0055f480_TEST PASS\n");
    return 0;
}