#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCalls = 0;
class CParentDefClassBase { public:
    virtual void v00() { }
    unsigned char _base[0x38];
    CParentDefClassBase() { ++g_baseCalls; std::memset(_base, 0x22, sizeof(_base)); }
};
template <class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector() : first(0), last(0), end(0) { } };
class CSpecialAbilitiesTurncoatSpellDef : public CParentDefClassBase { public:
    CRetailVector<float> StaminaCostPerSec, ResistanceDamagePerSec, TurncoatTime, TurncoatRange, TurncoatXPUpEveryNSecs;
    virtual void v00() { }
    CSpecialAbilitiesTurncoatSpellDef() : CParentDefClassBase() { }
};
int main()
{
    static unsigned char buf[sizeof(CSpecialAbilitiesTurncoatSpellDef)];
    std::memset(buf, 0xCD, sizeof(buf));
    CSpecialAbilitiesTurncoatSpellDef* d = new (buf) CSpecialAbilitiesTurncoatSpellDef();
    bool ok = g_baseCalls == 1 && d->_base[5] == 0x22
        && !d->StaminaCostPerSec.first && !d->ResistanceDamagePerSec.last && !d->TurncoatTime.end
        && !d->TurncoatRange.first && !d->TurncoatXPUpEveryNSecs.first && !d->TurncoatXPUpEveryNSecs.end;
    if (!ok) { std::printf("00457a83_TEST FAIL\n"); return 1; }
    std::printf("00457a83_TEST PASS\n");
    return 0;
}