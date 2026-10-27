// behaviour test for 0x008CFB70 CCreatureAction_TentacleStrikeLoop::Clone() const (model; the base copy constructor is a stub)
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
class CCreatureAction_TentacleStrikeLoop : public CCombatActionBase {
public:
    CCreatureAction_TentacleStrikeLoop() { }
    virtual CCreatureActionBase* Clone() const;
    long StartTick;                                      // +0x114 (PDB +300)
    bool Interrupt;                                      // +0x118
    float StartHealth;                                   // +0x11C
};
CCreatureActionBase* CCreatureAction_TentacleStrikeLoop::Clone() const { return new CCreatureAction_TentacleStrikeLoop(*this); }
int main()
{
    CCreatureAction_TentacleStrikeLoop a; std::memset(a._body, 0x5A, sizeof(a._body)); a.StartTick = 99;
    CCreatureAction_TentacleStrikeLoop* b = (CCreatureAction_TentacleStrikeLoop*)a.Clone();
    int ok = b != 0 && b != &a && g_copies == 1 && b->StartTick == a.StartTick && b->_body[3] == 0x5A;
    delete b;
    if (!ok) { std::printf("008cfb70_TEST FAIL\n"); return 1; }
    std::printf("008cfb70_TEST PASS\n");
    return 0;
}