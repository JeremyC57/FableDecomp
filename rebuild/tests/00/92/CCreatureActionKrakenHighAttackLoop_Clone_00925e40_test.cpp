// Behaviour test for 0x00925E40 CCreatureAction_KrakenHighAttackLoop::Clone() const: a new object of the same dynamic type, the base copied through its copy
// constructor and Interrupt / LoopNum copied.
#include <cstdio>
static int g_baseCopies = 0;
class CCreatureActionBase {
public:
    CCreatureActionBase() {}
    CCreatureActionBase(const CCreatureActionBase&) { ++g_baseCopies; }
    virtual ~CCreatureActionBase() {}
    virtual CCreatureActionBase* Clone() const = 0;
    virtual int Kind() const { return 0; }
    unsigned char _body[0xa8 - sizeof(void*)];
};
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase() {}
    CCombatActionBase(const CCombatActionBase& other) : CCreatureActionBase(other) {}
    unsigned char _combat[0x114 - 0xa8];
};
class CCreatureAction_KrakenHighAttackLoop : public CCombatActionBase {
public:
    CCreatureAction_KrakenHighAttackLoop() : Interrupt(false), LoopNum(0) {}
    virtual CCreatureActionBase* Clone() const;
    virtual int Kind() const { return 7; }
    bool Interrupt;
    long LoopNum;
};
CCreatureActionBase* CCreatureAction_KrakenHighAttackLoop::Clone() const
{
    return new CCreatureAction_KrakenHighAttackLoop(*this);
}
int main()
{
    CCreatureAction_KrakenHighAttackLoop a;
    a.Interrupt = true;
    a.LoopNum = 5;
    CCreatureActionBase* c = a.Clone();
    CCreatureAction_KrakenHighAttackLoop* d = (CCreatureAction_KrakenHighAttackLoop*)c;
    if (c == 0 || c->Kind() != 7 || g_baseCopies != 1 || d->Interrupt != true || d->LoopNum != 5) {
        std::printf("00925e40_TEST FAIL\n"); return 1;
    }
    delete c;
    std::printf("00925e40_TEST PASS\n");
    return 0;
}