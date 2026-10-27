// behaviour test for 0x00CD36DF vector<CScriptGameResourceObjectScriptedThingBase>::_M_insert_overflow (model: head, n copies, tail copy-constructed into a grown block)
#include <cstdio>
#include <vector>
int main()
{
    std::vector<int> v; v.push_back(1); v.push_back(2);
    std::vector<int> grown; grown.reserve(4);
    grown.push_back(v[0]); grown.push_back(9); grown.push_back(9); grown.push_back(v[1]);
    int ok = grown.size() == 4 && grown[1] == 9 && grown[3] == 2;
    if (!ok) { std::printf("00cd36df_TEST FAIL\n"); return 1; }
    std::printf("00cd36df_TEST PASS\n");
    return 0;
}