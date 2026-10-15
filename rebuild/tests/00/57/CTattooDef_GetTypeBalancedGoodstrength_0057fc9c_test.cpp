#include <cstdio>
struct CAppearanceModifierScalingDef { unsigned char _base[0x3c]; float Attractiveness; float Scariness; float Goodstrength; };
template <class T> struct CRetailVector {
    T* _First; T* _Last; T* _End;
    unsigned long size() const { return (unsigned long)(_Last - _First); }
    const T& operator[](unsigned long i) const { return _First[i]; }
};
struct CTattooDef {
    float Attractiveness, Scariness, Goodstrength;
    float GetTypeBalancedGoodstrength(const CRetailVector<CAppearanceModifierScalingDef>& scalings) const
    {
        if (scalings.size() == 5)
            return Goodstrength * scalings[4].Goodstrength;
        return Goodstrength;
    }
};
int main()
{
    static CAppearanceModifierScalingDef defs[5];
    for (int i = 0; i < 5; ++i) { defs[i].Attractiveness = 10.0f; defs[i].Scariness = 20.0f; defs[i].Goodstrength = 30.0f; }
    defs[4].Goodstrength = 0.5f;
    CRetailVector<CAppearanceModifierScalingDef> v; v._First = defs; v._Last = defs + 5; v._End = v._Last;
    CTattooDef t; t.Attractiveness = 1.0f; t.Scariness = 6.0f; t.Goodstrength = 6.0f;
    float full = t.GetTypeBalancedGoodstrength(v);
    v._Last = defs + 4;
    float part = t.GetTypeBalancedGoodstrength(v);
    if (full != 3.0f || part != 6.0f) { std::printf("0057fc9c_TEST FAIL\n"); return 1; }
    std::printf("0057fc9c_TEST PASS\n");
    return 0;
}