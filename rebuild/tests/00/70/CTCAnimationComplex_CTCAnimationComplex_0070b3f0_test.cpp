#include <cstdio>
#include <cstring>
#include <math.h>

struct CThing { int id; };
static int g_animators = 0;
struct CAnimateThing { CThing* m_owner; CAnimateThing(CThing& o) : m_owner(&o) { ++g_animators; } };
struct CTCBase { CThing* ParentThing; CTCBase(CThing& o) : ParentThing(&o) { } };
struct CTCAnimationComplex : CTCBase {
    CAnimateThing* PAnimation;
    CTCAnimationComplex(CThing& owner) : CTCBase(owner), PAnimation(new CAnimateThing(owner)) { }
    ~CTCAnimationComplex() { delete PAnimation; }
};
int main()
{
    CThing t; t.id = 4;
    CTCAnimationComplex c(t);
    if (c.ParentThing != &t || c.PAnimation == 0 || c.PAnimation->m_owner != &t || g_animators != 1) { std::printf("0070b3f0_TEST FAIL\n"); return 1; }
    std::printf("0070b3f0_TEST PASS\n");
    return 0;
}