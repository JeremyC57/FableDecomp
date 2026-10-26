// behaviour test for 0x00472095 COpinionDeedReactionDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x48, +0x4C),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob4 { unsigned char b[4]; };
struct Blob12 { unsigned char b[12]; };
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
    Blob12 Attitudes;
    Blob4 Animation;
    unsigned long DialogueTag;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Attitudes);
    c.Transfer(d.Animation);
    c.Transfer(d.DialogueTag);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[3] = { &d.Attitudes, &d.Animation, &d.DialogueTag };
    if (save.count != 3u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 3u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Attitudes, 0, sizeof(e.Attitudes));
    std::memset(&e.Animation, 0, sizeof(e.Animation));
    std::memset(&e.DialogueTag, 0, sizeof(e.DialogueTag));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00472095_TEST FAIL %d\n", fail); return 1; }
    std::printf("00472095_TEST PASS\n");
    return 0;
}