#include <cstdio>
#include <cstring>

struct CCharString {
    char text[64];
    CCharString(const char* t, long = -1) { std::strcpy(text, t); }
    ~CCharString() {}
};

struct CWorkAtSchool {
    long lastTime; char seq[3][64];
    bool PlayLoop(long time, const CCharString& into, const CCharString& loop, const CCharString& outOf)
    {
        lastTime = time; std::strcpy(seq[0], into.text); std::strcpy(seq[1], loop.text); std::strcpy(seq[2], outOf.text);
        return time > 0;
    }
    bool Teach(long time)
    {
        return PlayLoop(time, "ST_TEACH_INTO", "ST_TEACH_LOOP", "ST_TEACH_OUTOF");
    }
};

int main()
{
    CWorkAtSchool g;
    if (!g.Teach(30) || g.lastTime != 30 || std::strcmp(g.seq[0], "ST_TEACH_INTO") || std::strcmp(g.seq[1], "ST_TEACH_LOOP")
        || std::strcmp(g.seq[2], "ST_TEACH_OUTOF")) { std::printf("009428a0_TEST FAIL\n"); return 1; }
    if (g.Teach(0)) { std::printf("009428a0_TEST FAIL\n"); return 1; }
    std::printf("009428a0_TEST PASS\n");
    return 0;
}