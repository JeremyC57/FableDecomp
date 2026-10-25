#include <cstdio>
// Behaviour test for 0x00913590 std::_Construct<CBezierCurve, CBezierCurve>: links against the source object's explicit
// instantiation; checks the memberwise copy of all four control points into a raw slot.
class C3DVector {
public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CBezierCurve {
public:
    C3DVector Point1;
    C3DVector Point2;
    C3DVector Point3;
    C3DVector Point4;
};
namespace std {
template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val);
}
static double g_storage[16];
int main()
{
    if (sizeof(CBezierCurve) != 0x30) { std::printf("00913590_TEST FAIL size\n"); return 1; }
    CBezierCurve src;
    C3DVector* p = &src.Point1;
    for (int i = 0; i < 4; ++i) { p[i].X = 1.0f + i; p[i].Y = 10.0f + i; p[i].Z = -100.0f - i; }
    for (int k = 0; k < 16; ++k) g_storage[k] = 0.0;
    CBezierCurve* slot = reinterpret_cast<CBezierCurve*>(g_storage);
    std::_Construct<CBezierCurve, CBezierCurve>(slot, src);
    const C3DVector* q = &slot->Point1;
    for (int j = 0; j < 4; ++j)
        if (q[j].X != p[j].X || q[j].Y != p[j].Y || q[j].Z != p[j].Z) { std::printf("00913590_TEST FAIL point %d\n", j); return 1; }
    if (g_storage[6] != 0.0) { std::printf("00913590_TEST FAIL overrun\n"); return 1; }
    std::printf("00913590_TEST PASS\n");
    return 0;
}
