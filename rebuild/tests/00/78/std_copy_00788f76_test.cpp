// behaviour test for 0x00788f76 std::__copy<std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, int>: last - first elements of 0xC bytes are assigned
// forward, one operator= each, and the end of the result range is returned; an empty range assigns nothing
#include <cstdio>
static int gAssigns = 0;
class ElemC {                                         // test-only element of the retail size 0xC
public:
    int Words[3];
    ElemC& operator=(const ElemC& other) { ++gAssigns; for (int i = 0; i < 3; ++i) Words[i] = other.Words[i]; return *this; }
};
static ElemC* Copy(const ElemC* first, const ElemC* last, ElemC* result)
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
    if ((unsigned int)sizeof(ElemC) != 0xCu) fails |= 1;
    ElemC a[5], b[5];
    for (int k = 0; k < 5; ++k) for (int i = 0; i < 3; ++i) { a[k].Words[i] = k * 10 + i; b[k].Words[i] = -1; }
    ElemC* r = Copy(a + 1, a + 4, b);
    if (r != b + 3 || gAssigns != 3) fails |= 2;
    if (b[0].Words[0] != 10 || b[2].Words[3 - 1] != 30 + 3 - 1 || b[3].Words[0] != -1) fails |= 4;
    r = Copy(a, a, b + 4);
    if (r != b + 4 || gAssigns != 3) fails |= 8;
    if (fails) { std::printf("00788f76_TEST FAIL %d\n", fails); return 1; }
    std::printf("00788f76_TEST PASS\n");
    return 0;
}