// Behaviour test for std::copy_backward<CHeader**, CHeader**> @ 0x00C29290 (the scalar memmove form).
#include <cstdio>
#include <cstring>

struct CHeader { int id; };

namespace model {
template<class BidIt1, class BidIt2>
BidIt2 copy_backward(BidIt1 first, BidIt1 last, BidIt2 dest)
{
    int off = (int)(last - first);
    return (BidIt2)std::memmove(&*dest - off, &*first, off * sizeof(*first));
}
}

int main()
{
    CHeader h[5];
    CHeader* p[6];
    for (int i = 0; i < 5; ++i) { h[i].id = i; p[i] = &h[i]; }
    p[5] = 0;
    CHeader** r = model::copy_backward(p, p + 5, p + 6);   // shift every entry up one slot
    bool ok = (r == p + 1) && p[0] == &h[0];
    for (int i = 1; i < 6; ++i) if (p[i] != &h[i - 1]) ok = false;
    if (model::copy_backward(p, p, p + 2) != p + 2) ok = false;
    if (!ok) { std::printf("00c29290_TEST FAIL\n"); return 1; }
    std::printf("00c29290_TEST PASS\n");
    return 0;
}