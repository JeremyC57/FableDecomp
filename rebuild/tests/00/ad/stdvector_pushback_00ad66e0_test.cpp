// behaviour test for 0x00AD66E0: push_back copies each record in order, growing when full
#include <cstdio>
#include <vector>
#include <string>
struct E { std::string s; long a; bool b; };
int main()
{
    std::vector<E> v;
    for (int i = 0; i < 4; ++i) { E e; e.s = std::string(i + 1, 'x'); e.a = i; e.b = (i & 1) != 0; v.push_back(e); }
    int ok = v.size() == 4 && v[3].s == "xxxx" && v[2].a == 2 && v[1].b && !v[0].b;
    if (!ok) { std::printf("00ad66e0_TEST FAIL\n"); return 1; }
    std::printf("00ad66e0_TEST PASS\n");
    return 0;
}