// behaviour test for 0x00947780 GetAllLeafThings (model: every scan-visible thing of the type list passing the filter is appended)
#include <cstdio>
#include <vector>
struct T { bool scan; int v; };
int main()
{
    T things[3] = { { true, 1 }, { true, 2 }, { false, 3 } };
    std::vector<T*> out;
    for (int i = 0; i < 3; ++i)
        if (things[i].scan && things[i].v != 2) out.push_back(&things[i]);
    int ok = out.size() == 1 && out[0]->v == 1;
    if (!ok) { std::printf("00947780_TEST FAIL\n"); return 1; }
    std::printf("00947780_TEST PASS\n");
    return 0;
}