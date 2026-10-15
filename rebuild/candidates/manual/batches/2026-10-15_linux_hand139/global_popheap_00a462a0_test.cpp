// Behaviour test for 0x00A462A0 __pop_heap: *result = *first, then __adjust_heap(first, 0, last - first, value, comp); value destroyed once.
#include <cstdio>
static int g_strDtors = 0;
struct CCharString {
    const char* s;
    CCharString() : s(0) {}
    CCharString(const CCharString& o) : s(o.s) {}
    ~CCharString() { ++g_strDtors; }
    CCharString& operator=(const CCharString& o) { s = o.s; return *this; }
};
struct C3DVector { float X, Y, Z; C3DVector() : X(0), Y(0), Z(0) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct Elem { CCharString BoneName; unsigned int BoneNameCRC; C3DVector Scaling; };
struct Comp { int tag; };
static Elem* g_first = 0; static int g_hole = -1, g_len = -1, g_compTag = 0;
static Elem g_value;
static void adjust_heap(Elem* first, int hole, int len, Elem value, Comp comp)
{ g_first = first; g_hole = hole; g_len = len; g_value = value; g_compTag = comp.tag; }
static void pop_heap(Elem* first, Elem* last, Elem* result, Elem value, Comp comp, int*)
{
    *result = *first;
    adjust_heap(first, 0, int(last - first), value, comp);
}
int main()
{
    static const char* names[4] = { "a", "b", "c", "d" };
    Elem a[4];
    for (int i = 0; i < 4; ++i) { a[i].BoneName.s = names[i]; a[i].BoneNameCRC = 100 + i; a[i].Scaling.X = (float)i; }
    Elem res;
    Comp comp; comp.tag = 5;
    int before = g_strDtors;
    pop_heap(a, a + 3, &res, a[2], comp, 0);
    if (res.BoneName.s != names[0] || res.BoneNameCRC != 100 || res.Scaling.X != 0.0f || g_value.BoneName.s != names[2] || g_value.BoneNameCRC != 102 || g_value.Scaling.X != 2.0f || g_first != a || g_hole != 0 || g_len != 3 || g_compTag != 5 || g_strDtors - before < 1) {
        std::printf("00a462a0_TEST FAIL\n"); return 1;
    }
    std::printf("00a462a0_TEST PASS\n");
    return 0;
}
