#include <cstdio>
static int g_dtor = 0;
struct E {
    float bb[6]; long start, count; E* sub[2]; float bs[4];
    E() : start(5), count(6) { sub[0] = 0; sub[1] = 0; }
    ~E() { delete sub[0]; delete sub[1]; start = 0; count = 0; ++g_dtor; }
};
int main()
{
    E* root = new E; root->sub[0] = new E; root->sub[1] = new E; root->sub[1]->sub[0] = new E;
    delete root;
    if (g_dtor != 4) { std::printf("00ac37b0_TEST FAIL\n"); return 1; }
    E local; local.~E(); if (local.start != 0 || local.count != 0) { std::printf("00ac37b0_TEST FAIL\n"); return 1; }
    local.start = 0; local.count = 0;
    std::printf("00ac37b0_TEST PASS\n");
    return 0;
}
