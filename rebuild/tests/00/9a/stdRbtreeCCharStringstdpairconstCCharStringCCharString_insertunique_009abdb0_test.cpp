#include <cstdio>
#include <map>
#include <string>
// Behaviour model of the hinted unique insert: an existing key is found, a new key lands in order wherever the hint is.
int main()
{
    std::map<std::string, int> m;
    m["b"] = 1; m["d"] = 2;
    std::map<std::string, int>::iterator it = m.insert(m.end(), std::make_pair(std::string("c"), 3));
    if (it->first != "c" || m.size() != 3) { std::printf("009abdb0_TEST FAIL\n"); return 1; }
    it = m.insert(m.begin(), std::make_pair(std::string("d"), 9));
    if (it->second != 2 || m.size() != 3) { std::printf("009abdb0_TEST FAIL\n"); return 1; }
    it = m.insert(m.find("d"), std::make_pair(std::string("a"), 4));
    if (m.begin()->first != "a" || m.size() != 4) { std::printf("009abdb0_TEST FAIL\n"); return 1; }
    std::printf("009abdb0_TEST PASS\n");
    return 0;
}