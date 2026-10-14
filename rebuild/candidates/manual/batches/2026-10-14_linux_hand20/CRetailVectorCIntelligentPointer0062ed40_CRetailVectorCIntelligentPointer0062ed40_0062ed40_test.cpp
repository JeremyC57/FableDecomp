#include <cstdio>
#include <cstdlib>
struct CRetailAllocator { CRetailAllocator() { } };
struct E { unsigned char _body[8]; };
static int g_copies = 0;
static void ConstructCopy(E* d, const E& s) { *d = s; ++g_copies; }
static E* UninitializedCopy(const E* f, const E* l, E* r) { for (; f != l; ++f, ++r) ConstructCopy(r, *f); return r; }
struct VBase { E* first; E* last; E* end_of_storage; VBase(unsigned long n, const CRetailAllocator&) { first = last = end_of_storage = 0; if (n) { first = (E*)std::malloc(n * sizeof(E)); last = first; end_of_storage = first + n; } } };
struct V : VBase {
    V(const V& other) : VBase(other.size(), other.get_allocator()) { last = UninitializedCopy(other.first, other.last, first); }
    V(E* f, E* l) : VBase(0, CRetailAllocator()) { first = f; last = l; end_of_storage = l; }
    unsigned long size() const { return (unsigned long)(last - first); }
    CRetailAllocator get_allocator() const { return CRetailAllocator(); }
};
int main()
{
    E src[3]; for (int i = 0; i < 3; ++i) for (int j = 0; j < 8; ++j) src[i]._body[j] = (unsigned char)(i * 7 + j);
    V a(src, src + 3); V b(a);
    if (b.size() != 3 || g_copies != 3 || b.first == a.first || b.end_of_storage != b.first + 3) { std::printf("0062ed40_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 8; ++j) if (b.first[i]._body[j] != src[i]._body[j]) { std::printf("0062ed40_TEST FAIL\n"); return 1; }
    V e(src, src); V f(e);
    if (f.size() != 0 || f.first != 0 || g_copies != 3) { std::printf("0062ed40_TEST FAIL\n"); return 1; }
    std::printf("0062ed40_TEST PASS\n");
    return 0;
}
