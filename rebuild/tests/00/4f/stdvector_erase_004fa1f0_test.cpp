// behaviour test for 0x004fa1f0 std::vector<CCardDef >::erase(first, last): the tail [last, finish) is assigned down over first,
// the leftover elements at the end are destroyed, finish moves back by (last - first) and first is returned
#include <cstdio>
static int gAssigns = 0;
static int gDestroys = 0;
class Elem44 {                                         // test-only element of the retail size 0x44
public:
    int Words[17];
    Elem44& operator=(const Elem44& other) { ++gAssigns; for (int i = 0; i < 17; ++i) Words[i] = other.Words[i]; return *this; }
    void Destroy() { ++gDestroys; Words[0] = -1; }
};
struct Vec44 {
    Elem44* Start;
    Elem44* Finish;
    Elem44* EndOfStorage;
};
static Elem44* Erase(Vec44& v, Elem44* first, Elem44* last)
{
    Elem44* i = first;
    for (Elem44* s = last; s != v.Finish; ++s, ++i) *i = *s;
    for (Elem44* d = i; d != v.Finish; ++d) d->Destroy();
    v.Finish = i;
    return first;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem44) != 0x44u) fails |= 1;
    Elem44 buf[6];
    for (int k = 0; k < 6; ++k) for (int i = 0; i < 17; ++i) buf[k].Words[i] = k * 100 + i;
    Vec44 v = { buf, buf + 6, buf + 6 };
    Elem44* r = Erase(v, buf + 1, buf + 3);
    if (r != buf + 1 || v.Finish != buf + 4 || gAssigns != 3 || gDestroys != 2) fails |= 2;
    if (buf[1].Words[0] != 300 || buf[3].Words[17 - 1] != 500 + 17 - 1 || buf[4].Words[0] != -1 || buf[5].Words[0] != -1) fails |= 4;
    r = Erase(v, buf + 2, buf + 2);
    if (r != buf + 2 || v.Finish != buf + 4 || gAssigns != 5 || gDestroys != 2) fails |= 8;
    if (fails) { std::printf("004fa1f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("004fa1f0_TEST PASS\n");
    return 0;
}