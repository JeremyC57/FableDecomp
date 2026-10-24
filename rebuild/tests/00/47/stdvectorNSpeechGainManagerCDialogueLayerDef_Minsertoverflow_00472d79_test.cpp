// Behaviour test for 0x00472D79 vector<CDialogueLayerDef>::_M_insert_overflow: new capacity size + max(size, n), prefix,
// n copies of x (the n == 1 path too), suffix unless atEnd, old block released, finish/end set
#include <cstdio>
#include <cstdlib>
#include <new>
struct Layer { int id; int data[15]; static int copies; Layer(int i) : id(i) {} Layer(const Layer& o) : id(o.id) { ++copies; } };
int Layer::copies = 0;
struct Vec {
    Layer* start; Layer* finish; Layer* end; int clears;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void Clear() { ++clears; std::free(start); }
    void InsertOverflow(Layer* position, const Layer& x, unsigned int fillLength, bool atEnd)
    {
        unsigned int oldSize = size();
        unsigned int length = oldSize + (oldSize < fillLength ? fillLength : oldSize);
        Layer* newStart = length ? (Layer*)std::malloc(length * sizeof(Layer)) : 0;
        Layer* cur = newStart;
        for (Layer* p = start; p != position; ++p, ++cur) if (cur) new (cur) Layer(*p);
        if (fillLength == 1) { if (cur) new (cur) Layer(x); ++cur; }
        else for (unsigned int n = fillLength; n > 0; --n, ++cur) if (cur) new (cur) Layer(x);
        if (!atEnd) for (Layer* p = position; p != finish; ++p, ++cur) if (cur) new (cur) Layer(*p);
        Clear();
        start = newStart; finish = cur; end = newStart + length;
    }
};
int main()
{
    Vec v; v.clears = 0;
    v.start = (Layer*)std::malloc(3 * sizeof(Layer));
    for (int i = 0; i < 3; ++i) new (v.start + i) Layer(i + 1);
    v.finish = v.end = v.start + 3;
    Layer x(9);
    v.InsertOverflow(v.start + 1, x, 1, false);                 // 1 9 2 3, capacity 6
    bool ok = v.size() == 4 && v.end - v.start == 6 && v.start[0].id == 1 && v.start[1].id == 9 && v.start[3].id == 3 && v.clears == 1;
    Layer y(7);
    v.InsertOverflow(v.finish, y, 5, true);                     // append five 7s, capacity 4 + 5
    ok = ok && v.size() == 9 && v.end - v.start == 9 && v.start[4].id == 7 && v.start[8].id == 7 && v.start[3].id == 3 && v.clears == 2;
    ok = ok && Layer::copies == 4 + 9;
    std::free(v.start);
    if (!ok) { std::printf("00472d79_TEST FAIL\n"); return 1; }
    std::printf("00472d79_TEST PASS\n");
    return 0;
}