// Behaviour test for 0x007649D0 std::__find_if over CCountedPointer<NShape::CShape> with NShape::CIsShapeTrack
// (unrolled by four, by-value predicate argument).
#include <cstdio>

static int g_Copies = 0;
static int g_Calls = 0;

struct Shape { int type; int GetType() const { ++g_Calls; return type; } };

struct CountedPointer {
    Shape* Data;
    int*   refs;
    CountedPointer(Shape* s, int* r) : Data(s), refs(r) { }
    CountedPointer(const CountedPointer& o) : Data(o.Data), refs(o.refs) { ++*refs; ++g_Copies; }
    ~CountedPointer() { --*refs; }
};

struct IsShapeTrack {
    bool operator()(CountedPointer shape) const { return shape.Data->GetType() == 1; }
};

static const CountedPointer* find_if_unrolled(const CountedPointer* first, const CountedPointer* last, IsShapeTrack pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}

int main()
{
    int ok = 1;
    int refs = 0;
    Shape s[11];
    for (int i = 0; i < 11; ++i) s[i].type = 2;
    CountedPointer v[11] = {
        CountedPointer(&s[0], &refs), CountedPointer(&s[1], &refs), CountedPointer(&s[2], &refs),
        CountedPointer(&s[3], &refs), CountedPointer(&s[4], &refs), CountedPointer(&s[5], &refs),
        CountedPointer(&s[6], &refs), CountedPointer(&s[7], &refs), CountedPointer(&s[8], &refs),
        CountedPointer(&s[9], &refs), CountedPointer(&s[10], &refs) };
    refs = 0;
    if (find_if_unrolled(v, v + 11, IsShapeTrack()) != v + 11 || g_Calls != 11) ok = 0;   // none: all 11 tested
    s[9].type = 1;                                          // in the remainder switch (case 3 entry)
    g_Calls = 0;
    if (find_if_unrolled(v, v + 11, IsShapeTrack()) != v + 9 || g_Calls != 10) ok = 0;
    s[2].type = 1;                                          // inside the first trip
    g_Calls = 0;
    if (find_if_unrolled(v, v + 11, IsShapeTrack()) != v + 2 || g_Calls != 3) ok = 0;
    if (find_if_unrolled(v, v, IsShapeTrack()) != v) ok = 0;                                // empty range
    if (refs != 0 || g_Copies == 0) ok = 0;                 // every by-value copy was released again
    if (!ok) { std::printf("007649d0_TEST FAIL\n"); return 1; }
    std::printf("007649d0_TEST PASS\n");
    return 0;
}