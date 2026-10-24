// Behaviour test for 0x00613DD0 std::__uninitialized_copy(const CUIMiniMapMarker*, const CUIMiniMapMarker*, CUIMiniMapMarker*,
// __false_type): placement copy of mini-map markers -- region, the UI def-name string (copy ctor 0x0099EC30, modelled as a
// counting copy), position, name length, graphic index, angle; returns the end of the copies.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_stringCopies = 0;

struct CCharString {
    const char* m_pStringData;
    explicit CCharString(const char* s) : m_pStringData(s) { }
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; }
};

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

struct CTCInventoryMap {
    struct CUIMiniMapMarker {
        long        Region;
        CCharString UIDefName;
        C2DVector   Position;
        long        DefNameLength;
        long        GraphicIndex;
        float       Angle;
        CUIMiniMapMarker(long r, const char* name, float x, float y, long len, long gfx, float angle)
            : Region(r), UIDefName(name), Position(x, y), DefNameLength(len), GraphicIndex(gfx), Angle(angle) { }
    };
};

namespace stl {
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class InputIter, class ForwardIter>
ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
}

typedef CTCInventoryMap::CUIMiniMapMarker M;

int main()
{
    int ok = 1;
    M src[2] = { M(3, "MARKER_QUEST", 1.5f, -2.0f, 12, 4, 0.25f), M(7, "MARKER_HOUSE", 10.0f, 20.0f, 12, 9, 3.0f) };
    M* dst = (M*)std::malloc(3 * sizeof(M));
    M* end = stl::__uninitialized_copy((const M*)src, (const M*)src + 2, dst, stl::__false_type());
    if (end != dst + 2 || g_stringCopies != 2) ok = 0;
    for (int i = 0; i < 2; ++i) {
        const M& a = src[i];
        const M& b = dst[i];
        if (a.Region != b.Region || a.UIDefName.m_pStringData != b.UIDefName.m_pStringData || a.Position.X != b.Position.X ||
            a.Position.Y != b.Position.Y || a.DefNameLength != b.DefNameLength || a.GraphicIndex != b.GraphicIndex ||
            a.Angle != b.Angle) ok = 0;
    }
    if (stl::__uninitialized_copy((const M*)src, (const M*)src, dst + 2, stl::__false_type()) != dst + 2 ||
        g_stringCopies != 2) ok = 0;
    std::free(dst);
    if (!ok) { std::printf("00613dd0_TEST FAIL\n"); return 1; }
    std::printf("00613dd0_TEST PASS\n");
    return 0;
}