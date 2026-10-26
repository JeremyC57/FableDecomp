// Behaviour test for 0x00513B00 std::__uninitialized_fill_n<CRGBColour*,unsigned int,CRGBColour>: n placement copies
// of x (every channel), the returned pointer is first + n, n == 0 writes nothing and returns first.
#include <cstdio>
#include <new>

class CRGBColour { public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct false_type_model { false_type_model() { } };

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n_model(ForwardIter first, Size n, const T& x, const false_type_model&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (&*cur) T(x);
    return cur;
}

int main()
{
    CRGBColour buf[6];
    CRGBColour x; x.Set(10, 20, 30, 40);
    CRGBColour* end = uninitialized_fill_n_model(buf + 1, 4u, x, false_type_model());
    if (end != buf + 5) { std::printf("00513b00_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 5; ++i)
        if (buf[i].R != 10 || buf[i].G != 20 || buf[i].B != 30 || buf[i].A != 40) { std::printf("00513b00_TEST FAIL\n"); return 1; }
    if (buf[0].R != 0 || buf[5].A != 0) { std::printf("00513b00_TEST FAIL\n"); return 1; }
    if (uninitialized_fill_n_model(buf, 0u, x, false_type_model()) != buf || buf[0].G != 0) { std::printf("00513b00_TEST FAIL\n"); return 1; }
    std::printf("00513b00_TEST PASS\n");
    return 0;
}