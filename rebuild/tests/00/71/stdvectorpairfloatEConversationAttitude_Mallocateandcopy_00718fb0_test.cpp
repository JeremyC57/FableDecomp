// Behaviour test for 0x00718FB0 vector<pair<float,EConversationAttitude>>::_M_allocate_and_copy: n slots, [first,last) copied.
#include <cstdio>
#include <cstdlib>

struct SAttitudePair { float first; int second; };
static SAttitudePair* AllocateAndCopy(unsigned int n, const SAttitudePair* first, const SAttitudePair* last)
{
    SAttitudePair* result = n != 0 ? (SAttitudePair*)std::malloc(n * sizeof(SAttitudePair)) : 0;
    SAttitudePair* cur = result;
    for (; first != last; ++first, ++cur) if (cur) *cur = *first;
    return result;
}
int main()
{
    SAttitudePair src[3] = { { 0.5f, 1 }, { 1.25f, 2 }, { 3.0f, 0 } };
    SAttitudePair* p = AllocateAndCopy(3, src, src + 3);
    bool ok = p != 0 && p[0].first == 0.5f && p[1].second == 2 && p[2].first == 3.0f;
    std::free(p);
    ok = ok && AllocateAndCopy(0, src, src) == 0;
    if (!ok) { std::printf("00718fb0_TEST FAIL\n"); return 1; }
    std::printf("00718fb0_TEST PASS\n");
    return 0;
}