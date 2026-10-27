#include <cstdio>
#include <map>
#include <string>
// Behaviour model: inserting distinct keys keeps them ordered and counted; each node holds a copy of its value.
int main()
{
    std::map<std::string, std::string> m;
    m.insert(std::make_pair(std::string("m"), std::string("1")));
    m.insert(std::make_pair(std::string("a"), std::string("2")));
    m.insert(std::make_pair(std::string("z"), std::string("3")));
    if (m.size() != 3 || m.begin()->first != "a" || m.rbegin()->second != "3") { std::printf("00a2dc90_TEST FAIL\n"); return 1; }
    std::printf("00a2dc90_TEST PASS\n");
    return 0;
}