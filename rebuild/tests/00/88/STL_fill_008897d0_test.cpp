#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of _STL::fill<CBlendedEnvironmentTheme*> @ 0x008897D0: assign val to every element of [first, last) through
// the class's operator=.
static int g_assignments = 0;
class CBlendedEnvironmentTheme {
public:
    int Value;
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other)   // 0x006B5DF0
    {
        ++g_assignments;
        Value = other.Value;
        return *this;
    }
};
template <class _ForwardIter, class _Tp>
void __fastcall Fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
int main()
{
    bool ok = true;
    CBlendedEnvironmentTheme themes[4];
    for (int i = 0; i < 4; ++i) themes[i].Value = i;
    CBlendedEnvironmentTheme val;
    val.Value = 77;
    Fill(themes + 1, themes + 3, val);
    if (themes[0].Value != 0 || themes[1].Value != 77 || themes[2].Value != 77 || themes[3].Value != 3) ok = false;
    if (g_assignments != 2) ok = false;
    Fill(themes, themes, val);
    if (g_assignments != 2) ok = false;
    if (!ok) { std::printf("008897d0_TEST FAIL\n"); return 1; }
    std::printf("008897d0_TEST PASS\n");
    return 0;
}