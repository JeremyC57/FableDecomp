// Behaviour test for 0x0046916B std::vector<CRGBColour>::_M_insert_overflow (non-POD growth path)
#include <cstdio>
#include <cstdlib>
#include <new>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {}
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
};
static int g_constructs = 0;
static void Construct(CRGBColour* p, const CRGBColour& value) { ++g_constructs; if (p) new (p) CRGBColour(value); }
struct ColourVector { CRGBColour* _M_start; CRGBColour* _M_finish; CRGBColour* _M_end_of_storage; };
static CRGBColour* UninitCopy(CRGBColour* first, CRGBColour* last, CRGBColour* cur)
{
    for (; first != last; ++first, ++cur) Construct(cur, *first);
    return cur;
}
static void InsertOverflow(ColourVector* v, CRGBColour* position, const CRGBColour& x, unsigned int n, bool atEnd)
{
    const unsigned int oldSize = (unsigned int)(v->_M_finish - v->_M_start);
    const unsigned int length = oldSize + (oldSize < n ? n : oldSize);
    CRGBColour* newStart = length != 0 ? (CRGBColour*)std::malloc(length * sizeof(CRGBColour)) : 0;
    CRGBColour* newFinish = UninitCopy(v->_M_start, position, newStart);
    if (n == 1) { Construct(newFinish, x); ++newFinish; }
    else for (; n > 0; --n, ++newFinish) Construct(newFinish, x);
    if (!atEnd) newFinish = UninitCopy(position, v->_M_finish, newFinish);
    if (v->_M_start != 0) std::free(v->_M_start);
    v->_M_start = newStart; v->_M_finish = newFinish; v->_M_end_of_storage = newStart + length;
}
int main()
{
    ColourVector v;
    v._M_start = (CRGBColour*)std::malloc(3 * sizeof(CRGBColour));
    for (int i = 0; i < 3; ++i) new (v._M_start + i) CRGBColour((unsigned char)i, 0, 0, 255);
    v._M_finish = v._M_end_of_storage = v._M_start + 3;
    CRGBColour red(200, 1, 2, 3);
    InsertOverflow(&v, v._M_start + 1, red, 1, false);     // 0 red 1 2, capacity 6
    if (v._M_finish - v._M_start != 4 || v._M_end_of_storage - v._M_start != 6 || g_constructs != 4
        || v._M_start[0].R != 0 || v._M_start[1].R != 200 || v._M_start[1].A != 3 || v._M_start[2].R != 1 || v._M_start[3].R != 2) {
        std::printf("0046916b_TEST FAIL\n"); return 1;
    }
    InsertOverflow(&v, v._M_finish, red, 5, true);          // append five: capacity 4 + 5 = 9
    if (v._M_finish - v._M_start != 9 || v._M_end_of_storage - v._M_start != 9 || v._M_start[8].R != 200 || v._M_start[3].R != 2) {
        std::printf("0046916b_TEST FAIL\n"); return 1;
    }
    std::free(v._M_start);
    std::printf("0046916b_TEST PASS\n");
    return 0;
}