// behaviour test for 0x00478C79 vector<CAbilityTrackLevelDef>::_M_fill_insert (model: in-place insert of n copies)
#include <cstdio>
#include <vector>
int main()
{
    std::vector<int> v; v.reserve(10);
    v.push_back(1); v.push_back(2); v.push_back(3);
    v.insert(v.begin() + 1, 2, 7);
    int ok = v.size() == 5 && v[0] == 1 && v[1] == 7 && v[2] == 7 && v[3] == 2 && v[4] == 3;
    v.insert(v.end(), 8, 5);
    ok = ok && v.size() == 13 && v[12] == 5;
    if (!ok) { std::printf("00478c79_TEST FAIL\n"); return 1; }
    std::printf("00478c79_TEST PASS\n");
    return 0;
}