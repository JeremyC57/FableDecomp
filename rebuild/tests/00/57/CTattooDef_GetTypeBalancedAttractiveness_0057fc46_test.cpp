// test for 0057fc46 GetTypeBalancedAttractiveness
#include <stdio.h>
#include <string.h>
#include "engine/CTattooDef.h"
class CAppearanceModifierScalingDef {
public:
    unsigned char _base[0x3c];                           // CDefClassBase and the leading members
    float         Attractiveness;                        // +0x3c
    float         Scariness;                             // +0x40
    float         Goodstrength;                          // +0x44
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
}
struct CTattooDef_Methods : CTattooDef {
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>& scalings) const;
};
float CTattooDef_Methods::GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>& scalings) const
{
    if (scalings.size() == 5)
        return scalings[4].Attractiveness * Attractiveness;
    return Attractiveness;
}

int main()
{
    CAppearanceModifierScalingDef s[5]; memset(s, 0, sizeof s);
    s[4].Attractiveness = 3.0f; s[0].Attractiveness = 100.0f;
    std::vector<CAppearanceModifierScalingDef> v5; v5._M_start = s; v5._M_finish = s + 5; v5._M_end_of_storage = s + 5;
    std::vector<CAppearanceModifierScalingDef> v4; v4._M_start = s; v4._M_finish = s + 4; v4._M_end_of_storage = s + 5;
    CTattooDef_Methods t; memset(&t, 0, sizeof t); t.Attractiveness = 2.5f;
    int ok = sizeof(CAppearanceModifierScalingDef) == 0x48 && offsetof(CTattooDef, Attractiveness) == 0x54
          && t.GetTypeBalancedAttractiveness(v5) == 7.5f && t.GetTypeBalancedAttractiveness(v4) == 2.5f;
    printf(ok ? "0057fc46_TEST PASS\n" : "0057fc46_TEST FAIL\n");
    return 0;
}