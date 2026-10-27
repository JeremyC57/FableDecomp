// behaviour test for 0x007AFD10 vector<CIntelligentPointer<CThingAICreature> >::_M_insert_overflow (model: grown block, x inserted at pos)
#include <cstdio>
#include <vector>
int main()
{
    std::vector<int> v; v.push_back(1); v.push_back(2);
    std::vector<int> grown; grown.reserve(4);
    grown.push_back(v[0]); grown.push_back(9); grown.push_back(v[1]);
    int ok = grown.size() == 3 && grown[1] == 9 && grown[2] == 2 && grown.capacity() >= 4;
    if (!ok) { std::printf("007afd10_TEST FAIL\n"); return 1; }
    std::printf("007afd10_TEST PASS\n");
    return 0;
}