// Behaviour test for 0x005657C0: __uninitialized_fill_n of a 16-byte activity record (3 strings + 2 bytes)
#include <cstdio>
#include <new>

static int g_copies = 0;
struct Str {
    const char* p;
    Str() : p(0) { }
    Str(const char* s) : p(s) { }
    Str(const Str& o) : p(o.p) { ++g_copies; }
};
struct Activity {
    Str Name;
    Str Description;
    Str Unit;
    unsigned char NumDecPlaces;
    bool LowerIsBetter;
};

static Activity* FillN(Activity* first, unsigned int n, const Activity& x)
{
    Activity* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) Activity(x);
    return cur;
}

int main()
{
    Activity x; x.Name = Str("n"); x.Description = Str("d"); x.Unit = Str("u"); x.NumDecPlaces = 3; x.LowerIsBetter = true;
    static unsigned char raw[sizeof(Activity) * 5];
    Activity* a = (Activity*)raw;
    Activity* e = FillN(a, 4, x);
    if (e != a + 4 || g_copies != 12) { std::printf("005657c0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (a[i].Name.p != x.Name.p || a[i].Unit.p != x.Unit.p || a[i].NumDecPlaces != 3 || !a[i].LowerIsBetter) {
            std::printf("005657c0_TEST FAIL\n"); return 1;
        }
    if (FillN(a, 0, x) != a || g_copies != 12) { std::printf("005657c0_TEST FAIL\n"); return 1; }
    std::printf("005657c0_TEST PASS\n");
    return 0;
}