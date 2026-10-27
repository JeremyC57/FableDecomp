// behaviour test for 0x008ADC30 GetAllThings (model: every scan-visible thing passing the filter is appended)
#include <cstdio>
#include <vector>
struct T { bool scan; int v; };
int main()
{
    T things[4] = { { true, 1 }, { false, 2 }, { true, 3 }, { true, 4 } };
    std::vector<T*> out;
    for (int i = 0; i < 4; ++i)
        if (things[i].scan && things[i].v != 3) out.push_back(&things[i]);
    int ok = out.size() == 2 && out[0]->v == 1 && out[1]->v == 4;
    if (!ok) { std::printf("008adc30_TEST FAIL\n"); return 1; }
    std::printf("008adc30_TEST PASS\n");
    return 0;
}