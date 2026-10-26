// Behaviour test for 0x0097B3F0 (see src/0097b3f0.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0097b3f0_TEST FAIL\n"); return 1; } } while (0)
class CSoundPair { public: int FirstSound; int LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
// Combat-action base model: records what the derived constructor handed it.
class CCombatActionBase { public:
    void* owner; void* thing; int actionCode; CSoundPair sound; char animName[48];
    CCombatActionBase(void* o, int code, const CSoundPair& s) : owner(o), thing(0), actionCode(code), sound(s) { animName[0] = 0; }
    CCombatActionBase(void* o, void* t, int code, const CSoundPair& s) : owner(o), thing(t), actionCode(code), sound(s) { animName[0] = 0; }
    void SetAnimationName(const char* n) { std::strcpy(animName, n); }
};
static CSoundPair GetSoundPair(void* owner, const char* name)
{
    CSoundPair p; p.FirstSound = owner ? 1 : 0; p.LastSound = name ? (int)std::strlen(name) : -1; return p;
}

class C3DVector { public: unsigned int X, Y, Z; };
struct CJump : CCombatActionBase { unsigned int dx, dy, dz;
    CJump(void* owner, void* thing, const C3DVector& d) : CCombatActionBase(owner, thing, 150, GetSoundPair(owner, "SWORD_SWING")), dx(d.X), dy(d.Y), dz(d.Z) {} };
int main()
{
    int o, t; C3DVector d = { 1, 2, 3 }; CJump a(&o, &t, d);
    CHECK(a.actionCode == 150 && a.thing == &t && a.dx == 1 && a.dy == 2 && a.dz == 3);
    std::printf("0097b3f0_TEST PASS\n");
    return 0;
}