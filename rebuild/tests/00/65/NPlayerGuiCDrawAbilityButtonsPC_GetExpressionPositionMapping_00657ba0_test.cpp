#include <cstdio>
#include <vector>
// Behaviour model: the mapping getter clears the caller's vector and appends its three fixed entries.
static void get_mapping(std::vector<long>& mapping) { mapping.clear(); mapping.push_back(0); mapping.push_back(1); mapping.push_back(-1); }
int main()
{
    std::vector<long> m; m.push_back(9); m.push_back(9); m.push_back(9); m.push_back(9);
    get_mapping(m);
    if (m.size() != 3 || m[0] != 0 || m[1] != 1 || m[2] != -1) { std::printf("00657ba0_TEST FAIL\n"); return 1; }
    std::vector<long> e; get_mapping(e);
    if (e.size() != 3 || e[2] != -1) { std::printf("00657ba0_TEST FAIL\n"); return 1; }
    std::printf("00657ba0_TEST PASS\n");
    return 0;
}