// behaviour test for 0x00A43F20 vector<CLipSyncDataBankEntry >::resize(n, x): shrinking destroys exactly the surplus tail and keeps the head, growing appends n - size() copies of x, and an equal size changes nothing
#include <cstdio>
#include <vector>
static int gLive = 0;
struct Elem {                                            // a counted handle like the 8-byte element
    int value;
    Elem(int v = 0) : value(v) { ++gLive; }
    Elem(const Elem& o) : value(o.value) { ++gLive; }
    ~Elem() { --gLive; }
    Elem& operator=(const Elem& o) { value = o.value; return *this; }
};
static void Resize(std::vector<Elem>& v, unsigned int n, const Elem& x)   // the function under test
{
    if (n < v.size())
        v.erase(v.begin() + n, v.end());
    else
        v.insert(v.end(), n - v.size(), x);
}
int main()
{
    int fail = 0;
    {
        std::vector<Elem> v;
        for (int i = 0; i < 5; ++i) v.push_back(Elem(i));
        Elem x(42);
        Resize(v, 2, x);
        if (v.size() != 2 || v[0].value != 0 || v[1].value != 1) fail |= 1;
        if (gLive != 3) fail |= 2;
        Resize(v, 4, x);
        if (v.size() != 4 || v[2].value != 42 || v[3].value != 42 || v[1].value != 1) fail |= 4;
        Resize(v, 4, Elem(7));
        if (v.size() != 4 || v[3].value != 42) fail |= 8;
    }
    if (gLive != 0) fail |= 16;
    if (fail) { std::printf("00a43f20_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a43f20_TEST PASS\n");
    return 0;
}