#include <cstdio>
#include <stddef.h>
#include <new>
static int g_base = 0;
#pragma pack(push, 1)
struct CParentDefClassBase_Calls { virtual void v00() { } unsigned char _body[0x35]; CParentDefClassBase_Calls() { ++g_base; } };
#pragma pack(pop)
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage; CRetailVector() : first(0), last(0), end_of_storage(0) { } };
struct CSpecialAbilitiesMultiArrowDef : CParentDefClassBase_Calls {
    long NumberOfShots[4]; long NumberInVolley[4]; float AmmoEffectCreationDelaySecs; CDefString CastSound; float BowHighlightWidth;
    CRetailVector<long> BowHighlightColor; CRetailVector<float> MultiArrowPerArrowDamageMultiplier; virtual void v00() { }
    CSpecialAbilitiesMultiArrowDef() : CParentDefClassBase_Calls(), CastSound(), BowHighlightColor(), MultiArrowPerArrowDamageMultiplier() { }
};
int main()
{
    unsigned char raw[sizeof(CSpecialAbilitiesMultiArrowDef)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CSpecialAbilitiesMultiArrowDef* d = new (raw) CSpecialAbilitiesMultiArrowDef;
    if (g_base != 1 || d->CastSound.TablePos != -1 || d->BowHighlightColor.first || d->BowHighlightColor.last || d->BowHighlightColor.end_of_storage
        || d->MultiArrowPerArrowDamageMultiplier.first || d->MultiArrowPerArrowDamageMultiplier.last || d->MultiArrowPerArrowDamageMultiplier.end_of_storage
        || offsetof(CSpecialAbilitiesMultiArrowDef, NumberOfShots) != 0x3c || offsetof(CSpecialAbilitiesMultiArrowDef, CastSound) != 0x60
        || offsetof(CSpecialAbilitiesMultiArrowDef, BowHighlightColor) != 0x68 || offsetof(CSpecialAbilitiesMultiArrowDef, MultiArrowPerArrowDamageMultiplier) != 0x74 || sizeof(*d) != 0x80)
    { std::printf("0045725b_TEST FAIL\n"); return 1; }
    std::printf("0045725b_TEST PASS\n"); return 0;
}