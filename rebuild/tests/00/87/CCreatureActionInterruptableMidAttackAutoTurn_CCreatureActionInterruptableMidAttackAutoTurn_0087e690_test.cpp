// Behaviour test for 0x0087E690 (see src/0087e690.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0087e690_TEST FAIL\n"); return 1; } } while (0)
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
struct CPtr { void* item; CPtr(void* i) : item(i) {} CPtr(const CPtr& o) : item(o.item) {} };
struct CAutoTurn : CCombatActionBase { C3DVector RequiredFacing; CPtr PWeapon; bool PerformedStrikeEnd;
    CAutoTurn(void* w) : CCombatActionBase(0, 1, CSoundPair()), PWeapon(w), PerformedStrikeEnd(true) { RequiredFacing.X = 1; RequiredFacing.Y = 2; RequiredFacing.Z = 3; }
    CAutoTurn(const CAutoTurn& o) : CCombatActionBase(o), RequiredFacing(o.RequiredFacing), PWeapon(o.PWeapon), PerformedStrikeEnd(o.PerformedStrikeEnd) {} };
int main()
{
    int w; CAutoTurn src(&w); CAutoTurn a(src);
    CHECK(a.RequiredFacing.X == 1 && a.RequiredFacing.Y == 2 && a.RequiredFacing.Z == 3 && a.PWeapon.item == &w && a.PerformedStrikeEnd && a.actionCode == 1);
    std::printf("0087e690_TEST PASS\n");
    return 0;
}