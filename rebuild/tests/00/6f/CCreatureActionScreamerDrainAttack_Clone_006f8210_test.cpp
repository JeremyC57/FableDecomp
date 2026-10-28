#include <stdio.h>
#include <string.h>
class CCreatureActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CCreatureActionBase* Clone() const = 0;
    unsigned char _state[0xa0-sizeof(void*)];
};
class CCombatActionBase : public CCreatureActionBase {
public:
    unsigned char _combat_state[0x114-0xa0];
    CCombatActionBase();
    CCombatActionBase(const CCombatActionBase&);
};
class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase {
public:
    long Level; long DrainDefIndex;
    virtual CCreatureActionBase* Clone() const;
};
class CCreatureAction_ScreamerDrainAttack : public CCreatureAction_CastWillDrainLifeSpell {
public:
    virtual CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScreamerDrainAttack::Clone() const { return new CCreatureAction_ScreamerDrainAttack(*this); }
void CCreatureActionBase::v00() {}
void CCreatureActionBase::v01() {}
void CCreatureActionBase::v02() {}
void CCreatureActionBase::v03() {}
void CCreatureActionBase::v04() {}
void CCreatureActionBase::v05() {}
void CCreatureActionBase::v06() {}
void CCreatureActionBase::v07() {}
void CCreatureActionBase::v08() {}
void CCreatureActionBase::v09() {}
void CCreatureActionBase::v10() {}
static int copies;
CCombatActionBase::CCombatActionBase() { memset(_state,0,sizeof(_state));memset(_combat_state,0,sizeof(_combat_state)); }
CCombatActionBase::CCombatActionBase(const CCombatActionBase& x) {
    ++copies;memcpy(_state,x._state,sizeof(_state));memcpy(_combat_state,x._combat_state,sizeof(_combat_state));
}
CCreatureActionBase* CCreatureAction_CastWillDrainLifeSpell::Clone() const {return 0;}
int main() {
    CCreatureAction_ScreamerDrainAttack original;original._state[3]=71;original._combat_state[5]=19;original.Level=3;original.DrainDefIndex=41;
    const CCreatureActionBase* base=&original;CCreatureActionBase* result=base->Clone();CCreatureAction_ScreamerDrainAttack* clone=static_cast<CCreatureAction_ScreamerDrainAttack*>(result);
    int ok=clone && clone!=&original && copies==1 && clone->_state[3]==71 && clone->_combat_state[5]==19 && clone->Level==3 && clone->DrainDefIndex==41;
    if(clone){clone->_state[3]=1;ok=ok && original._state[3]==71;delete clone;}
    puts(ok ? "006f8210_TEST PASS" : "006f8210_TEST FAIL");return ok ? 0 : 1;
}