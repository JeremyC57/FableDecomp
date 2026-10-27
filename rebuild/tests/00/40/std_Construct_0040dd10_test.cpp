// behaviour test for 0x0040dd10 std::_Construct<T,T>(T* p, const T& value): placement-copies a 16-byte record into p
#include <cstdio>
#include <new>
struct Rec { unsigned long W0, W1, W2, W3; };
template <class T1, class T2> void Construct(T1* p, const T2& value) { new (p) T1(value); }
int main()
{
    int fails = 0;
    Rec src; src.W0 = 1; src.W1 = 2; src.W2 = 3; src.W3 = 4;
    Rec dst[2]; dst[0].W0 = dst[0].W1 = dst[0].W2 = dst[0].W3 = 0; dst[1] = dst[0];
    Construct(&dst[1], src);
    if (dst[1].W0 != 1 || dst[1].W1 != 2 || dst[1].W2 != 3 || dst[1].W3 != 4) fails |= 1;
    if (dst[0].W0 != 0 || dst[0].W3 != 0) fails |= 2;
    if (fails) { std::printf("0040dd10_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040dd10_TEST PASS\n");
    return 0;
}