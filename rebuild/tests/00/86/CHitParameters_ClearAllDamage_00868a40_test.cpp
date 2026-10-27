// behaviour test for 0x00868A40: clearing the member container leaves it empty and reusable
#include <cstdio>
#include <map>
struct Owner { std::map<int, float> M; void Clear() { M.clear(); } };
int main()
{
    Owner o; o.M[1] = 2.0f; o.M[5] = 1.0f; o.Clear(); bool a = o.M.empty();
    o.Clear(); o.M[3] = 4.0f;
    if (!a || o.M.size() != 1) { std::printf("00868a40_TEST FAIL\n"); return 1; }
    std::printf("00868a40_TEST PASS\n");
    return 0;
}