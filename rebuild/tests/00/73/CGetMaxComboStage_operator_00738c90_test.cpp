// Behaviour test for 0x00738C90 CGetMaxComboStage::operator()(const pair<unsigned long, CAnimationEntry>&).
#include <stdio.h>
namespace std2 {
template <class T1, class T2> struct pair { T1 first; T2 second; };
}
struct CComboDef { long pad[2]; long Stage; long ComboType; };
class CAnimationEntry {
public:
    const CComboDef* def;
    void GetComboDef(const CComboDef** out) const { *out = def; }
};
class CGetMaxComboStage {
public:
    long MaxStage;
    long CurrentID;
    void operator()(const std2::pair<unsigned long, CAnimationEntry>& entry);
};
void CGetMaxComboStage::operator()(const std2::pair<unsigned long, CAnimationEntry>& entry)
{
    const CComboDef* def;
    entry.second.GetComboDef(&def);
    if (def->ComboType == CurrentID && def->Stage > MaxStage)
        MaxStage = def->Stage;
}
int main()
{
    int fail = 0;
    CComboDef defs[5] = { { {0,0}, 2, 7 }, { {0,0}, 5, 3 }, { {0,0}, 4, 7 }, { {0,0}, 1, 7 }, { {0,0}, -5, 7 } };
    std2::pair<unsigned long, CAnimationEntry> entries[5];
    for (int i = 0; i < 5; ++i) { entries[i].first = i; entries[i].second.def = &defs[i]; }
    CGetMaxComboStage f; f.MaxStage = -777; f.CurrentID = 7;
    for (int i = 0; i < 5; ++i) f(entries[i]);
    if (f.MaxStage != 4 || f.CurrentID != 7) fail |= 1;      // stage 5 belongs to combo 3
    CGetMaxComboStage g; g.MaxStage = -777; g.CurrentID = 9;
    for (int i = 0; i < 5; ++i) g(entries[i]);
    if (g.MaxStage != -777) fail |= 2;
    if (fail) { printf("00738c90_TEST FAIL %d\n", fail); return 1; }
    printf("00738c90_TEST PASS\n");
    return 0;
}