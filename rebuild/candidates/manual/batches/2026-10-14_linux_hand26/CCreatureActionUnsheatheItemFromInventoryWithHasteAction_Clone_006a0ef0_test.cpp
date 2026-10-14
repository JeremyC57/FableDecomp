#include <cstdio>
static int g_copies = 0;
struct Base { virtual ~Base() {} unsigned char body[0xb4]; Base() {} Base(const Base& o) { ++g_copies; } };
struct Act : Base { void* m_haste; Act(void* h) : m_haste(h) {} Base* Clone() { return new Act(*this); } };
int main()
{
    int haste; Act a(&haste); Act* b = (Act*)a.Clone();
    if (!b || g_copies != 1 || b->m_haste != &haste) { std::printf("006a0ef0_TEST FAIL\n"); return 1; }
    delete b;
    std::printf("006a0ef0_TEST PASS\n");
    return 0;
}
