// behaviour test for 0x0047616e std::vector<CQuickAccessItemButtonGuiDef >::erase(first, last): the tail [last, finish) is assigned down over first,
// the leftover elements at the end are destroyed, finish moves back by (last - first) and first is returned
#include <cstdio>
static int gAssigns = 0;
static int gDestroys = 0;
class Elem50 {                                         // test-only element of the retail size 0x50
public:
    int Words[20];
    Elem50& operator=(const Elem50& other) { ++gAssigns; for (int i = 0; i < 20; ++i) Words[i] = other.Words[i]; return *this; }
    void Destroy() { ++gDestroys; Words[0] = -1; }
};
struct Vec50 {
    Elem50* Start;
    Elem50* Finish;
    Elem50* EndOfStorage;
};
static Elem50* Erase(Vec50& v, Elem50* first, Elem50* last)
{
    Elem50* i = first;
    for (Elem50* s = last; s != v.Finish; ++s, ++i) *i = *s;
    for (Elem50* d = i; d != v.Finish; ++d) d->Destroy();
    v.Finish = i;
    return first;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem50) != 0x50u) fails |= 1;
    Elem50 buf[6];
    for (int k = 0; k < 6; ++k) for (int i = 0; i < 20; ++i) buf[k].Words[i] = k * 100 + i;
    Vec50 v = { buf, buf + 6, buf + 6 };
    Elem50* r = Erase(v, buf + 1, buf + 3);
    if (r != buf + 1 || v.Finish != buf + 4 || gAssigns != 3 || gDestroys != 2) fails |= 2;
    if (buf[1].Words[0] != 300 || buf[3].Words[20 - 1] != 500 + 20 - 1 || buf[4].Words[0] != -1 || buf[5].Words[0] != -1) fails |= 4;
    r = Erase(v, buf + 2, buf + 2);
    if (r != buf + 2 || v.Finish != buf + 4 || gAssigns != 5 || gDestroys != 2) fails |= 8;
    if (fails) { std::printf("0047616e_TEST FAIL %d\n", fails); return 1; }
    std::printf("0047616e_TEST PASS\n");
    return 0;
}