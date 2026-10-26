// behaviour test for 0x005c12fa std::__copy<NInventory::CItem*, NInventory::CItem*, int>: last - first elements of 0x14 bytes are assigned
// forward, one operator= each, and the end of the result range is returned; an empty range assigns nothing
#include <cstdio>
static int gAssigns = 0;
class Elem14 {                                         // test-only element of the retail size 0x14
public:
    int Words[5];
    Elem14& operator=(const Elem14& other) { ++gAssigns; for (int i = 0; i < 5; ++i) Words[i] = other.Words[i]; return *this; }
};
static Elem14* Copy(const Elem14* first, const Elem14* last, Elem14* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem14) != 0x14u) fails |= 1;
    Elem14 a[5], b[5];
    for (int k = 0; k < 5; ++k) for (int i = 0; i < 5; ++i) { a[k].Words[i] = k * 10 + i; b[k].Words[i] = -1; }
    Elem14* r = Copy(a + 1, a + 4, b);
    if (r != b + 3 || gAssigns != 3) fails |= 2;
    if (b[0].Words[0] != 10 || b[2].Words[5 - 1] != 30 + 5 - 1 || b[3].Words[0] != -1) fails |= 4;
    r = Copy(a, a, b + 4);
    if (r != b + 4 || gAssigns != 3) fails |= 8;
    if (fails) { std::printf("005c12fa_TEST FAIL %d\n", fails); return 1; }
    std::printf("005c12fa_TEST PASS\n");
    return 0;
}