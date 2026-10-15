#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCalls = 0;
struct CParentDefClassBase {
    virtual void v00() { }
    unsigned char _base[0x38];
    CParentDefClassBase() { ++g_baseCalls; }
};
template <class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector() : first(0), last(0), end(0) { } };
class CCharString;
struct CSpecialAbilitiesDrainLifeDef : CParentDefClassBase {
    CRetailVector<float> AmountOfHealthSucked;
    CRetailVector<long> NumEffects;
    CRetailVector<float> SpellRange;
    CRetailVector<CCharString> ShotName;
    float SpellDelaySecs;
    virtual void v00() { }
    CSpecialAbilitiesDrainLifeDef() : CParentDefClassBase() { }
};
int main()
{
    static unsigned char buf[sizeof(CSpecialAbilitiesDrainLifeDef)];
    std::memset(buf, 0xCD, sizeof(buf));
    CSpecialAbilitiesDrainLifeDef* d = new (buf) CSpecialAbilitiesDrainLifeDef();
    unsigned char spell[sizeof(float)];
    std::memcpy(spell, &d->SpellDelaySecs, sizeof(spell));
    bool ok = g_baseCalls == 1 && !d->AmountOfHealthSucked.first && !d->NumEffects.last && !d->SpellRange.end
        && !d->ShotName.first && !d->ShotName.end && spell[0] == 0xCD;   // the float is left alone
    if (!ok) { std::printf("004578de_TEST FAIL\n"); return 1; }
    std::printf("004578de_TEST PASS\n");
    return 0;
}
