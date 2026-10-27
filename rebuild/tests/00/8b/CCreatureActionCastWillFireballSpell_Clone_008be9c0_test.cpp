// behaviour test for 0x008BE9C0 CCreatureAction_CastWillFireballSpell::Clone() const (model; the base copy constructor is a stub)
#include <cstdio>
#include <cstring>
class CThing;
template <class T> class CIntelligentPointer;
static int g_copies = 0;
class CCreatureActionBase { public: virtual ~CCreatureActionBase() { } virtual CCreatureActionBase* Clone() const = 0; };
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase() { }
    CCombatActionBase(const CCombatActionBase& other) { ++g_copies; std::memcpy(_body, other._body, sizeof(_body)); }
    unsigned char _body[16];
};
class CCreatureAction_CastWillFireballSpell : public CCombatActionBase {
public:
    CCreatureAction_CastWillFireballSpell() { }
    virtual CCreatureActionBase* Clone() const;
    bool ImmediateCast;                                  // +0x114 (PDB +300)
    bool OneAnimCast;                                    // +0x115
    long LevelToCast;                                    // +0x118
};
CCreatureActionBase* CCreatureAction_CastWillFireballSpell::Clone() const { return new CCreatureAction_CastWillFireballSpell(*this); }
int main()
{
    CCreatureAction_CastWillFireballSpell a; std::memset(a._body, 0x5A, sizeof(a._body)); a.LevelToCast = 3;
    CCreatureAction_CastWillFireballSpell* b = (CCreatureAction_CastWillFireballSpell*)a.Clone();
    int ok = b != 0 && b != &a && g_copies == 1 && b->LevelToCast == a.LevelToCast && b->_body[3] == 0x5A;
    delete b;
    if (!ok) { std::printf("008be9c0_TEST FAIL\n"); return 1; }
    std::printf("008be9c0_TEST PASS\n");
    return 0;
}