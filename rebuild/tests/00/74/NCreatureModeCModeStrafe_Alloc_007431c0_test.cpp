// Behaviour test for NCreatureMode::CModeStrafe::Alloc (0x007431C0): returns a new strafe mode built for the thing with the flag false.
#include <cstdio>
class CThing { public: int Id; };
class CModeBase { public: virtual ~CModeBase() {} };
class CModeStrafe : public CModeBase { public:
    CThing* Owner; bool Flag;
    CModeStrafe(CThing& t, bool f) : Owner(&t), Flag(f) {}
    static CModeBase* Alloc(CThing& thing) { return new CModeStrafe(thing, false); }
};
int main()
{
    CThing t; t.Id = 3;
    CModeBase* m = CModeStrafe::Alloc(t);
    CModeStrafe* s = (CModeStrafe*)m;
    if (!m || s->Owner != &t || s->Flag) { std::printf("007431c0_TEST FAIL\n"); return 1; }
    delete m;
    std::printf("007431c0_TEST PASS\n");
    return 0;
}