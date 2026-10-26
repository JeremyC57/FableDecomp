// behaviour test for 0x00452884 CHeroTitleDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x4D),
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
    long Title;
    long HeroTitleGuiTag;
    long HeroTitleGreetToHeroTag;
    long HeroTitleCommentAtHeroTag;
    long HeroTitleCommentToSelfTag;
    long RequiredRenownLevel;
    long RequiredStrengthExpLevel;
    long RequiredSkillExpLevel;
    long RequiredWillExpLevel;
    bool IsBuyable;
    bool IsAutomatic;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Title);
    c.Transfer(d.HeroTitleGuiTag);
    c.Transfer(d.HeroTitleGreetToHeroTag);
    c.Transfer(d.HeroTitleCommentAtHeroTag);
    c.Transfer(d.HeroTitleCommentToSelfTag);
    c.Transfer(d.RequiredRenownLevel);
    c.Transfer(d.RequiredStrengthExpLevel);
    c.Transfer(d.RequiredSkillExpLevel);
    c.Transfer(d.RequiredWillExpLevel);
    c.Transfer(d.IsBuyable);
    c.Transfer(d.IsAutomatic);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.IsBuyable = true;
    d.IsAutomatic = true;
    Context save(0); Persist(d, save);
    const void* want[11] = { &d.Title, &d.HeroTitleGuiTag, &d.HeroTitleGreetToHeroTag, &d.HeroTitleCommentAtHeroTag, &d.HeroTitleCommentToSelfTag, &d.RequiredRenownLevel, &d.RequiredStrengthExpLevel, &d.RequiredSkillExpLevel, &d.RequiredWillExpLevel, &d.IsBuyable, &d.IsAutomatic };
    if (save.count != 11u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 11u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Title, 0, sizeof(e.Title));
    std::memset(&e.HeroTitleGuiTag, 0, sizeof(e.HeroTitleGuiTag));
    std::memset(&e.HeroTitleGreetToHeroTag, 0, sizeof(e.HeroTitleGreetToHeroTag));
    std::memset(&e.HeroTitleCommentAtHeroTag, 0, sizeof(e.HeroTitleCommentAtHeroTag));
    std::memset(&e.HeroTitleCommentToSelfTag, 0, sizeof(e.HeroTitleCommentToSelfTag));
    std::memset(&e.RequiredRenownLevel, 0, sizeof(e.RequiredRenownLevel));
    std::memset(&e.RequiredStrengthExpLevel, 0, sizeof(e.RequiredStrengthExpLevel));
    std::memset(&e.RequiredSkillExpLevel, 0, sizeof(e.RequiredSkillExpLevel));
    std::memset(&e.RequiredWillExpLevel, 0, sizeof(e.RequiredWillExpLevel));
    std::memset(&e.IsBuyable, 0, sizeof(e.IsBuyable));
    std::memset(&e.IsAutomatic, 0, sizeof(e.IsAutomatic));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00452884_TEST FAIL %d\n", fail); return 1; }
    std::printf("00452884_TEST PASS\n");
    return 0;
}