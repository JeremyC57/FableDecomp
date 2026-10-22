#include <cstdio>
// Behaviour test for 007b55c0: the null-tag constructor leaves the handle null whatever the tag.
struct CNullTag { };
struct CHandle { void* m_p; CHandle(const CNullTag&) : m_p(0) { } };
int main()
{
    int x = 0;
    CNullTag tag;
    CHandle h(tag);
    int ok = (h.m_p == 0);
    h.m_p = &x;
    CHandle h2 = CHandle(CNullTag());
    ok = ok && h2.m_p == 0 && h.m_p == &x;
    if (!ok) { std::printf("007b55c0_TEST FAIL\n"); return 1; }
    std::printf("007b55c0_TEST PASS\n");
    return 0;
}