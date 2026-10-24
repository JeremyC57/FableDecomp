#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
struct CComboDef {
    unsigned char _pad[8];
    long Stage;
    long ComboType;
};
class CAnimationEntry {
public:
    const CComboDef* m_pDef;
    void GetComboDef(const CComboDef** out) const { *out = m_pDef; }
};
class CGetMaxComboStage {
public:
    long m_maxStage;
    long m_comboType;
    CGetMaxComboStage(long initialStage, long comboType) : m_maxStage(initialStage), m_comboType(comboType) {}
    void operator()(const std::pair<unsigned long, CAnimationEntry>& entry)
    {
        const CComboDef* def;
        entry.second.GetComboDef(&def);
        if (def->ComboType == m_comboType && def->Stage > m_maxStage)
            m_maxStage = def->Stage;
    }
};
int main()
{
    CComboDef d[4];
    long stages[4] = { 2, 9, 4, 1 }; long types[4] = { 1, 2, 1, 1 };
    std::pair<unsigned long, CAnimationEntry> e[4];
    for (int i = 0; i < 4; ++i) { d[i].Stage = stages[i]; d[i].ComboType = types[i]; e[i].first = i; e[i].second.m_pDef = &d[i]; }
    CGetMaxComboStage f = std::for_each(e, e + 4, CGetMaxComboStage(-777, 1));
    if (f.m_maxStage != 4 || f.m_comboType != 1) { std::printf("00738f30_TEST FAIL\n"); return 1; }
    f = std::for_each(e, e + 4, CGetMaxComboStage(-777, 3));
    if (f.m_maxStage != -777) { std::printf("00738f30_TEST FAIL\n"); return 1; }
    std::printf("00738f30_TEST PASS\n");
    return 0;
}