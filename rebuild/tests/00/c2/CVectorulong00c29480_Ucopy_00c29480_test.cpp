// Behaviour test for 00c29480: Dinkumware vector<4-byte scalar>::_Ucopy -- memmove [first, last) to ptr and return ptr + count
#include <cstdio>
#include <cstring>

class CVector4 {
public:
    long          Alval;
    unsigned int* Myfirst;
    unsigned int* Mylast;
    unsigned int* Myend;
    unsigned int* Ucopy(unsigned int* first, unsigned int* last, unsigned int* ptr)
    {
        size_t count = (size_t)(last - first);
        return (unsigned int*)std::memmove(ptr, first, count * sizeof(*first)) + count;
    }
};

int main()
{
    unsigned int buf[10];
    for (int i = 0; i < 10; ++i) buf[i] = 100u + i;
    CVector4 v;
    v.Alval = 0; v.Myfirst = buf; v.Mylast = buf + 6; v.Myend = buf + 10;
    unsigned int dst[6] = { 0, 0, 0, 0, 0, 0 };
    unsigned int* r = v.Ucopy(buf, buf + 6, dst);
    bool ok = r == dst + 6;
    for (int i = 0; i < 6; ++i) ok = ok && dst[i] == 100u + i;
    r = v.Ucopy(buf, buf + 5, buf + 2);                 // overlapping move to the right
    ok = ok && r == buf + 7 && buf[2] == 100u && buf[6] == 104u && buf[0] == 100u && buf[1] == 101u;
    ok = ok && v.Ucopy(buf, buf, dst) == dst;
    if (!ok) { std::printf("00c29480_TEST FAIL\n"); return 1; }
    std::printf("00c29480_TEST PASS\n");
    return 0;
}