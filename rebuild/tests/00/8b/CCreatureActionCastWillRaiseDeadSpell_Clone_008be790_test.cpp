// behaviour test for 0x008BE790 CCreatureAction_CastWillRaiseDeadSpell::Clone() const (model; the base copy constructor is a stub)
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
class CCreatureAction_CastWillRaiseDeadSpell : public CCombatActionBase {
public:
    CCreatureAction_CastWillRaiseDeadSpell() { }
    virtual CCreatureActionBase* Clone() const;
    CIntelligentPointer<CThing>* PUndeadFollower;        // +0x114 (PDB +300)
    long Health;                                         // +0x118
    unsigned long AppearanceSeed;                        // +0x11C
};
CCreatureActionBase* CCreatureAction_CastWillRaiseDeadSpell::Clone() const { return new CCreatureAction_CastWillRaiseDeadSpell(*this); }
int main()
{
    CCreatureAction_CastWillRaiseDeadSpell a; std::memset(a._body, 0x5A, sizeof(a._body)); a.Health = 40;
    CCreatureAction_CastWillRaiseDeadSpell* b = (CCreatureAction_CastWillRaiseDeadSpell*)a.Clone();
    int ok = b != 0 && b != &a && g_copies == 1 && b->Health == a.Health && b->_body[3] == 0x5A;
    delete b;
    if (!ok) { std::printf("008be790_TEST FAIL\n"); return 1; }
    std::printf("008be790_TEST PASS\n");
    return 0;
}