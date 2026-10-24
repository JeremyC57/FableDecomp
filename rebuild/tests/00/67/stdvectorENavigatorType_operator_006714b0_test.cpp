// Behaviour test for 0x006714B0 std::vector<ENavigatorType>::operator= (model copy)
#include <cstdio>
#include <cstdlib>
enum ENav { N0, N1, N2, N3, N4 };
struct Vec {
    ENav* start; ENav* finish; ENav* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    void clear_storage() { std::free(start); }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            unsigned int xlen = x.size();
            if (xlen > capacity()) {
                ENav* tmp = (ENav*)std::malloc(xlen * sizeof(ENav));
                for (unsigned int k = 0; k < xlen; ++k) tmp[k] = x.start[k];
                clear_storage();
                start = tmp; eos = start + xlen;
            } else if (size() >= xlen) {
                for (int n = (int)xlen; n > 0; --n) start[xlen - n] = x.start[xlen - n];
            } else {
                unsigned int s = size();
                for (unsigned int k = 0; k < s; ++k) start[k] = x.start[k];
                for (unsigned int k = s; k < xlen; ++k) start[k] = x.start[k];
            }
            finish = start + xlen;
        }
        return *this;
    }
};
static Vec Make(unsigned int n, unsigned int cap, ENav base)
{
    Vec v; v.start = (ENav*)std::malloc((cap ? cap : 1) * sizeof(ENav)); v.finish = v.start + n; v.eos = v.start + cap;
    for (unsigned int k = 0; k < n; ++k) v.start[k] = (ENav)((base + k) % 5);
    return v;
}
static bool Same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int k = 0; k < a.size(); ++k) if (a.start[k] != b.start[k]) return false;
    return true;
}
int main()
{
    Vec x = Make(4, 4, N1);
    Vec small = Make(1, 1, N0); small = x;          // grows
    Vec big = Make(5, 6, N3); big = x;              // shrinks in place
    Vec mid = Make(2, 8, N4); mid = x;              // copies 2, constructs 2
    ENav* before = mid.start;
    Vec& self = (mid = mid);
    bool ok = Same(small, x) && small.capacity() == 4 && Same(big, x) && big.capacity() == 6 && Same(mid, x) && mid.start == before && &self == &mid;
    std::printf(ok ? "006714b0_TEST PASS\n" : "006714b0_TEST FAIL\n");
    return ok ? 0 : 1;
}