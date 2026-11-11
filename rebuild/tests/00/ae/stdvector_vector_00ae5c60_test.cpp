// Behaviour test for the vector<16-byte element> copy constructor (0x00AE5C60): the copy owns fresh storage of exactly size()
// elements holding the same values; an empty source gives an empty vector with no storage.
#include <stdio.h>
#include <stdlib.h>
struct Element { long a, b, c, d; };
struct Vector {
    Element* start; Element* finish; Element* endOfStorage;
    Vector() : start(0), finish(0), endOfStorage(0) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    Vector(const Vector& x) : start(0), finish(0), endOfStorage(0)
    {
        unsigned int n = x.size();
        start = n != 0 ? (Element*)malloc(n * sizeof(Element)) : 0;
        finish = start;
        endOfStorage = start + n;
        Element* cur = start;
        for (const Element* p = x.start; p != x.finish; ++p, ++cur) *cur = *p;
        finish = cur;
    }
};
int main(void)
{
    Vector source;
    source.start = (Element*)malloc(5 * sizeof(Element)); source.finish = source.start + 3; source.endOfStorage = source.start + 5;
    for (int i = 0; i < 3; ++i) { source.start[i].a = i; source.start[i].b = i * 10; source.start[i].c = -i; source.start[i].d = 7; }
    Vector copy(source);
    bool ok = copy.size() == 3 && copy.start != source.start && copy.endOfStorage == copy.start + 3;   // capacity = size, not 5
    for (int i = 0; ok && i < 3; ++i) ok = copy.start[i].a == i && copy.start[i].b == i * 10 && copy.start[i].c == -i && copy.start[i].d == 7;
    Vector empty; Vector emptyCopy(empty);
    ok = ok && emptyCopy.start == 0 && emptyCopy.finish == 0 && emptyCopy.endOfStorage == 0;
    if (!ok) { printf("00ae5c60_TEST FAIL\n"); return 1; }
    printf("00ae5c60_TEST PASS\n");
    return 0;
}