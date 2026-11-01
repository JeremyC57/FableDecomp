// Behaviour test for 0x008F5740 CCreatureAction_DragonFlight::Clone() const: a new object of the same dynamic type, the base copied through its copy
// constructor and FaceTarget / TurnRate copied.
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
class CCreatureAction_DragonFlight : public CCombatActionBase {
public:
    CCreatureAction_DragonFlight() : FaceTarget(false), TurnRate(0) {}
    virtual CCreatureActionBase* Clone() const;
    virtual int Kind() const { return 7; }
    bool FaceTarget;
    float TurnRate;
};
CCreatureActionBase* CCreatureAction_DragonFlight::Clone() const
{
    return new CCreatureAction_DragonFlight(*this);
}
int main()
{
    CCreatureAction_DragonFlight a;
    a.FaceTarget = true;
    a.TurnRate = 0.75f;
    CCreatureActionBase* c = a.Clone();
    CCreatureAction_DragonFlight* d = (CCreatureAction_DragonFlight*)c;
    if (c == 0 || c->Kind() != 7 || g_baseCopies != 1 || d->FaceTarget != true || d->TurnRate != 0.75f) {
        std::printf("008f5740_TEST FAIL\n"); return 1;
    }
    delete c;
    std::printf("008f5740_TEST PASS\n");
    return 0;
}