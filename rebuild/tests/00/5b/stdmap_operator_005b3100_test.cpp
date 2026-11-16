// Behaviour test for 0x005B3100 map<CComponent*, pair<CWideString, long> >::operator[] (std::map is the reference: absent keys insert a default (empty text, 0) pair, present keys return the stored one)
#include <cstdio>
#include <map>
#include <string>
#include <utility>
struct Component { int id; };
typedef std::map<Component*, std::pair<std::wstring, long> > CountMap;
int main()
{
    Component a, b;
    CountMap m;
    std::pair<std::wstring, long>& pa = m[&a];
    if (m.size() != 1 || pa.second != 0 || !pa.first.empty()) { std::printf("005b3100_TEST FAIL\n"); return 1; }
    pa.second = 5; pa.first = L"x5";
    m[&b].second += 2;
    if (m.size() != 2 || m[&a].second != 5 || m[&a].first != L"x5" || m[&b].second != 2) { std::printf("005b3100_TEST FAIL\n"); return 1; }
    if (&m[&a] != &pa || m.size() != 2) { std::printf("005b3100_TEST FAIL\n"); return 1; }
    std::printf("005b3100_TEST PASS\n");
    return 0;
}