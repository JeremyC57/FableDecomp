// behaviour test for 0x00696B70: unique insert keeps one entry per key and reports whether it inserted
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
int main()
{
    std::map<std::string, int> m;
    bool a = m.insert(std::make_pair(std::string("beta"), 1)).second;
    bool b = m.insert(std::make_pair(std::string("alpha"), 2)).second;
    bool c = m.insert(std::make_pair(std::string("beta"), 3)).second;
    if (!a || !b || c || m.size() != 2 || m["beta"] != 1 || std::strcmp(m.begin()->first.c_str(), "alpha") != 0) { std::printf("00696b70_TEST FAIL\n"); return 1; }
    std::printf("00696b70_TEST PASS\n");
    return 0;
}