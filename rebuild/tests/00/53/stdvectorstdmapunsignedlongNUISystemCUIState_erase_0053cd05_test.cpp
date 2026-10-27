// behaviour test for 0x0053CD05: erase(pos) removes exactly that element and returns pos
#include <cstdio>
#include <vector>
#include <string>
int main()
{
    std::vector<std::string> v; v.push_back("a"); v.push_back("b"); v.push_back("c");
    std::vector<std::string>::iterator r = v.erase(v.begin() + 1);
    bool a = v.size() == 2 && v[0] == "a" && v[1] == "c" && r == v.begin() + 1;
    v.erase(v.begin() + 1);
    if (!a || v.size() != 1 || v[0] != "a") { std::printf("0053cd05_TEST FAIL\n"); return 1; }
    std::printf("0053cd05_TEST PASS\n");
    return 0;
}