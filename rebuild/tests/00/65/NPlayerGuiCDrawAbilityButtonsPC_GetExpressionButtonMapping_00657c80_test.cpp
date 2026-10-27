#include <cstdio>
#include <vector>
// Behaviour model: the mapping getter clears the caller's vector and appends its three fixed entries.
static void get_mapping(std::vector<long>& mapping) { mapping.clear(); mapping.push_back(2); mapping.push_back(3); mapping.push_back(1); }
int main()
{
    std::vector<long> m; m.push_back(9); m.push_back(9); m.push_back(9); m.push_back(9);
    get_mapping(m);
    if (m.size() != 3 || m[0] != 2 || m[1] != 3 || m[2] != 1) { std::printf("00657c80_TEST FAIL\n"); return 1; }
    std::vector<long> e; get_mapping(e);
    if (e.size() != 3 || e[2] != 1) { std::printf("00657c80_TEST FAIL\n"); return 1; }
    std::printf("00657c80_TEST PASS\n");
    return 0;
}