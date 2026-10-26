// behaviour test for 0x004ed0c6 std::vector<CShopItemDef >::erase(first, last): the tail [last, finish) is assigned down over first,
// the leftover elements at the end are destroyed, finish moves back by (last - first) and first is returned
#include <cstdio>
static int gAssigns = 0;
static int gDestroys = 0;
class Elem48 {                                         // test-only element of the retail size 0x48
public:
    int Words[18];
    Elem48& operator=(const Elem48& other) { ++gAssigns; for (int i = 0; i < 18; ++i) Words[i] = other.Words[i]; return *this; }
    void Destroy() { ++gDestroys; Words[0] = -1; }
};
struct Vec48 {
    Elem48* Start;
    Elem48* Finish;
    Elem48* EndOfStorage;
};
static Elem48* Erase(Vec48& v, Elem48* first, Elem48* last)
{
    Elem48* i = first;
    for (Elem48* s = last; s != v.Finish; ++s, ++i) *i = *s;
    for (Elem48* d = i; d != v.Finish; ++d) d->Destroy();
    v.Finish = i;
    return first;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem48) != 0x48u) fails |= 1;
    Elem48 buf[6];
    for (int k = 0; k < 6; ++k) for (int i = 0; i < 18; ++i) buf[k].Words[i] = k * 100 + i;
    Vec48 v = { buf, buf + 6, buf + 6 };
    Elem48* r = Erase(v, buf + 1, buf + 3);
    if (r != buf + 1 || v.Finish != buf + 4 || gAssigns != 3 || gDestroys != 2) fails |= 2;
    if (buf[1].Words[0] != 300 || buf[3].Words[18 - 1] != 500 + 18 - 1 || buf[4].Words[0] != -1 || buf[5].Words[0] != -1) fails |= 4;
    r = Erase(v, buf + 2, buf + 2);
    if (r != buf + 2 || v.Finish != buf + 4 || gAssigns != 5 || gDestroys != 2) fails |= 8;
    if (fails) { std::printf("004ed0c6_TEST FAIL %d\n", fails); return 1; }
    std::printf("004ed0c6_TEST PASS\n");
    return 0;
}