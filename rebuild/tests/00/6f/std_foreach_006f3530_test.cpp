#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
struct SAttackerSlot { long m_words[7]; };
class CRotateThing {
public:
    long m_words[8];
    CRotateThing() { for (int i = 0; i < 8; ++i) m_words[i] = 0; }
    void operator()(const SAttackerSlot& r) { for (int i = 0; i < 7; ++i) m_words[i] += r.m_words[i]; ++m_words[7]; }
};
int main()
{
    SAttackerSlot s[2];
    for (int i = 0; i < 2; ++i) for (int j = 0; j < 7; ++j) s[i].m_words[j] = j + i;
    CRotateThing f = std::for_each(s, s + 2, CRotateThing());
    if (f.m_words[0] != 1 || f.m_words[6] != 13 || f.m_words[7] != 2) { std::printf("006f3530_TEST FAIL\n"); return 1; }
    std::printf("006f3530_TEST PASS\n");
    return 0;
}