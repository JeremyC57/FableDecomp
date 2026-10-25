// behaviour test for 0x004611f5 std::vector<CObjectFamilyEntry >::operator=: self-assignment is a no-op; a longer source
// reallocates exactly its size; a shorter or equal one assigns in place (capacity kept); a longer one within capacity
// assigns size() elements and constructs the rest
#include <cstdio>
#include <cstdlib>
struct Elem { int a, b; };
struct Vec { Elem* start; Elem* finish; Elem* eos; };
static int gAllocs = 0, gFrees = 0;
static Vec& Assign(Vec& self, const Vec& x)
{
    if (&x != &self) {
        unsigned int xlen = (unsigned int)(x.finish - x.start);
        unsigned int cap = (unsigned int)(self.eos - self.start), size = (unsigned int)(self.finish - self.start);
        if (xlen > cap) {
            Elem* tmp = xlen ? (++gAllocs, (Elem*)std::malloc(xlen * sizeof(Elem))) : 0;
            for (unsigned int i = 0; i < xlen; ++i) tmp[i] = x.start[i];
            if (self.start) { ++gFrees; std::free(self.start); }
            self.start = tmp; self.eos = tmp + xlen;
        } else if (size >= xlen) {
            for (unsigned int i = 0; i < xlen; ++i) self.start[i] = x.start[i];
        } else {
            for (unsigned int i = 0; i < size; ++i) self.start[i] = x.start[i];
            for (unsigned int i = size; i < xlen; ++i) self.start[i] = x.start[i];
        }
        self.finish = self.start + xlen;
    }
    return self;
}
int main()
{
    int fails = 0;
    Elem src[4] = { { 1, 2 }, { 3, 4 }, { 5, 6 }, { 7, 8 } };
    Vec x; x.start = src; x.finish = src + 4; x.eos = src + 4;
    Vec v; v.start = v.finish = v.eos = 0;
    if (&Assign(v, v) != &v || v.start) fails |= 1;
    Assign(v, x);
    if (gAllocs != 1 || v.finish - v.start != 4 || v.eos != v.finish || v.start[3].b != 8) fails |= 2;
    Vec y; y.start = src; y.finish = src + 2; y.eos = src + 2;
    Assign(v, y);
    if (gAllocs != 1 || v.finish - v.start != 2 || v.eos - v.start != 4 || v.start[1].a != 3) fails |= 4;
    Assign(v, x);
    if (gAllocs != 1 || v.finish - v.start != 4 || v.start[2].a != 5 || v.start[3].b != 8) fails |= 8;
    std::free(v.start);
    if (fails) { std::printf("004611f5_TEST FAIL %d\n", fails); return 1; }
    std::printf("004611f5_TEST PASS\n");
    return 0;
}