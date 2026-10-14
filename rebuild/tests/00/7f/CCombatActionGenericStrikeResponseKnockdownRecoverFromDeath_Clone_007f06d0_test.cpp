#include <cstdio>
#include <cstring>
// Model of CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath::Clone @ 0x007F06D0
static int g_copies = 0;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4]; CCombatActionBase() { std::memset(_body, 0x11, sizeof(_body)); } CCombatActionBase(const CCombatActionBase& o) { ++g_copies; std::memcpy(_body, o._body, sizeof(_body)); } };
struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase { bool CanBeHit; bool Flip; virtual void v00() {} };
struct CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath : CCombatAction_GenericStrikeResponseKnockdownRecoverBase {
    virtual void v00() {}
    CCombatActionBase* Clone() const { return new CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath(*this); }
};
int main()
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath a; a.CanBeHit = true; a.Flip = false;
    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath* c = (CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath*)a.Clone();
    bool ok = c != 0 && c != &a && g_copies == 1 && c->CanBeHit == true && c->Flip == false && std::memcmp(c->_body, a._body, sizeof(a._body)) == 0
        && (sizeof(void*) != 4 || sizeof(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath) == 0x118) && *(void**)c == *(void**)&a;
    delete c;
    if (!ok) { std::printf("007f06d0_TEST FAIL\n"); return 1; }
    std::printf("007f06d0_TEST PASS\n"); return 0;
}