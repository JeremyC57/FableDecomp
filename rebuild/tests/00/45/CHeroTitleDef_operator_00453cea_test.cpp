// behaviour test for 0x00453CEA CHeroTitleDef::operator=: the base part is assigned first, every member is copied, and the result is *this
#include <cstdio>
static int g_base = 0;
struct Base { long PDefManager; long GlobalIndex; Base& operator=(const Base& o) { ++g_base; PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; return *this; } };
struct Def : Base {
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
    Def& Assign(const Def& o)                              // the function under test
    {
        Base::operator=(o);
        Title = o.Title;
        HeroTitleGuiTag = o.HeroTitleGuiTag;
        HeroTitleGreetToHeroTag = o.HeroTitleGreetToHeroTag;
        HeroTitleCommentAtHeroTag = o.HeroTitleCommentAtHeroTag;
        HeroTitleCommentToSelfTag = o.HeroTitleCommentToSelfTag;
        RequiredRenownLevel = o.RequiredRenownLevel;
        RequiredStrengthExpLevel = o.RequiredStrengthExpLevel;
        RequiredSkillExpLevel = o.RequiredSkillExpLevel;
        RequiredWillExpLevel = o.RequiredWillExpLevel;
        IsBuyable = o.IsBuyable;
        IsAutomatic = o.IsAutomatic;
        return *this;
    }
};
int main()
{
    int fail = 0;
    Def s, d;
    s.PDefManager = 11; s.GlobalIndex = 12; d.PDefManager = 0; d.GlobalIndex = 0;
    s.Title = 100L; d.Title = 0L;
    s.HeroTitleGuiTag = 101L; d.HeroTitleGuiTag = 0L;
    s.HeroTitleGreetToHeroTag = 102L; d.HeroTitleGreetToHeroTag = 0L;
    s.HeroTitleCommentAtHeroTag = 103L; d.HeroTitleCommentAtHeroTag = 0L;
    s.HeroTitleCommentToSelfTag = 104L; d.HeroTitleCommentToSelfTag = 0L;
    s.RequiredRenownLevel = 105L; d.RequiredRenownLevel = 0L;
    s.RequiredStrengthExpLevel = 106L; d.RequiredStrengthExpLevel = 0L;
    s.RequiredSkillExpLevel = 107L; d.RequiredSkillExpLevel = 0L;
    s.RequiredWillExpLevel = 108L; d.RequiredWillExpLevel = 0L;
    s.IsBuyable = true; d.IsBuyable = false;
    s.IsAutomatic = true; d.IsAutomatic = false;
    g_base = 0;
    if (&d.Assign(s) != &d) fail |= 1;
    if (g_base != 1 || d.PDefManager != 11 || d.GlobalIndex != 12) fail |= 2;
    if (d.Title != s.Title) fail |= 4;
    if (d.HeroTitleGuiTag != s.HeroTitleGuiTag) fail |= 4;
    if (d.HeroTitleGreetToHeroTag != s.HeroTitleGreetToHeroTag) fail |= 4;
    if (d.HeroTitleCommentAtHeroTag != s.HeroTitleCommentAtHeroTag) fail |= 4;
    if (d.HeroTitleCommentToSelfTag != s.HeroTitleCommentToSelfTag) fail |= 4;
    if (d.RequiredRenownLevel != s.RequiredRenownLevel) fail |= 4;
    if (d.RequiredStrengthExpLevel != s.RequiredStrengthExpLevel) fail |= 4;
    if (d.RequiredSkillExpLevel != s.RequiredSkillExpLevel) fail |= 4;
    if (d.RequiredWillExpLevel != s.RequiredWillExpLevel) fail |= 4;
    if (d.IsBuyable != s.IsBuyable) fail |= 4;
    if (d.IsAutomatic != s.IsAutomatic) fail |= 4;
    if (fail) { std::printf("00453cea_TEST FAIL %d\n", fail); return 1; }
    std::printf("00453cea_TEST PASS\n");
    return 0;
}