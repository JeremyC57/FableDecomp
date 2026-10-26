// test for 004382e0 SetButtonAlpha
#include <stdio.h>
#include <string.h>
struct vector_float {                                  // STLport vector<float> (12 bytes)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    float& operator[](long n) { return *(_M_start + n); }
};
class CPlayerGui { public:
    unsigned char _pad000[0x260];
    vector_float  AbilityButtonsAlpha;                   // +0x260
    vector_float  QuickAccessButtonsAlpha;               // +0x26c
    void SetButtonAlpha(long button, bool special, float alpha);
};
void CPlayerGui::SetButtonAlpha(long button, bool special, float alpha)
{
    if (special)
        AbilityButtonsAlpha[button] = alpha;
    else
        QuickAccessButtonsAlpha[button] = alpha;
}

int main()
{
    static CPlayerGui g; float a[4] = { 0, 0, 0, 0 }, q[4] = { 0, 0, 0, 0 };
    g.AbilityButtonsAlpha._M_start = a; g.QuickAccessButtonsAlpha._M_start = q;
    g.SetButtonAlpha(2, true, 0.5f); g.SetButtonAlpha(1, false, 0.25f);
    int ok = a[2] == 0.5f && q[1] == 0.25f && a[1] == 0 && q[2] == 0;
    printf(ok ? "004382e0_TEST PASS\n" : "004382e0_TEST FAIL\n");
    return 0;
}