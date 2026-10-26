// behaviour test for 0x004DF658 COracleMinigameDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x48, +0x44, +0x4C, +0x50, +0x54, +0x58),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    float WaitBetweenRoundsSeconds;
    float PauseBetweenOutputSeconds;
    float PauseBetweenOutputsLevelMultiplier;
    long MaxLevel;
    unsigned long TintRed;
    unsigned long TintGreen;
    unsigned long TintBlue;
    unsigned long TextLose;
    unsigned long TextWin;
    unsigned long TextWatch;
    unsigned long TextRepeat;
    unsigned long TextOfferGame;
    unsigned long TextWonRound;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.WaitBetweenRoundsSeconds);
    c.Transfer(d.PauseBetweenOutputSeconds);
    c.Transfer(d.PauseBetweenOutputsLevelMultiplier);
    c.Transfer(d.MaxLevel);
    c.Transfer(d.TintRed);
    c.Transfer(d.TintGreen);
    c.Transfer(d.TintBlue);
    c.Transfer(d.TextWin);
    c.Transfer(d.TextLose);
    c.Transfer(d.TextWatch);
    c.Transfer(d.TextRepeat);
    c.Transfer(d.TextOfferGame);
    c.Transfer(d.TextWonRound);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[13] = { &d.WaitBetweenRoundsSeconds, &d.PauseBetweenOutputSeconds, &d.PauseBetweenOutputsLevelMultiplier, &d.MaxLevel, &d.TintRed, &d.TintGreen, &d.TintBlue, &d.TextWin, &d.TextLose, &d.TextWatch, &d.TextRepeat, &d.TextOfferGame, &d.TextWonRound };
    if (save.count != 13u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 13u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.WaitBetweenRoundsSeconds, 0, sizeof(e.WaitBetweenRoundsSeconds));
    std::memset(&e.PauseBetweenOutputSeconds, 0, sizeof(e.PauseBetweenOutputSeconds));
    std::memset(&e.PauseBetweenOutputsLevelMultiplier, 0, sizeof(e.PauseBetweenOutputsLevelMultiplier));
    std::memset(&e.MaxLevel, 0, sizeof(e.MaxLevel));
    std::memset(&e.TintRed, 0, sizeof(e.TintRed));
    std::memset(&e.TintGreen, 0, sizeof(e.TintGreen));
    std::memset(&e.TintBlue, 0, sizeof(e.TintBlue));
    std::memset(&e.TextWin, 0, sizeof(e.TextWin));
    std::memset(&e.TextLose, 0, sizeof(e.TextLose));
    std::memset(&e.TextWatch, 0, sizeof(e.TextWatch));
    std::memset(&e.TextRepeat, 0, sizeof(e.TextRepeat));
    std::memset(&e.TextOfferGame, 0, sizeof(e.TextOfferGame));
    std::memset(&e.TextWonRound, 0, sizeof(e.TextWonRound));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004df658_TEST FAIL %d\n", fail); return 1; }
    std::printf("004df658_TEST PASS\n");
    return 0;
}