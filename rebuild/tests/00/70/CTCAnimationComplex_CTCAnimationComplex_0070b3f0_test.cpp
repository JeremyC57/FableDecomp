#include <cstdio>
#include <cstring>
#include <math.h>

class CThing { public: int id; };
static int g_animators = 0;
class CAnimateThing { public: CThing* m_owner; CAnimateThing(CThing& o) : m_owner(&o) { ++g_animators; } };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing& o) : ParentThing(&o) { } };
class CTCAnimationComplex : public CTCBase { public:
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