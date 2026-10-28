// Behaviour test for 0x0077ED20 __pop_heap<pair<CCharString, C3DVector>>: *result receives the old root, and the adjust step
// is called with hole 0, len = last - first and a copy of value (destroyed after the call).
#include <cstdio>
#include <cstring>
static int g_strings = 0;
class CCharString {
public:
    char m_text[16];
    CCharString(const char* s) { std::strcpy(m_text, s); ++g_strings; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_strings; }
    ~CCharString() { --g_strings; }
    const CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
};
class C3DVector { public:
    unsigned int X, Y, Z;
    C3DVector(unsigned int x, unsigned int y, unsigned int z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct SNamedPosition { CCharString first; C3DVector second; SNamedPosition(const char* n, unsigned int x) : first(n), second(x, x + 1, x + 2) {} };
struct CKeyPairCompareLess {};
static long g_hole = -1, g_len = -1; static unsigned int g_valueX = 0; static char g_valueName[16];
static void __adjust_heap(SNamedPosition*, long hole, long len, SNamedPosition value, CKeyPairCompareLess)
{ g_hole = hole; g_len = len; g_valueX = value.second.X; std::strcpy(g_valueName, value.first.m_text); }
static void __pop_heap(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result, SNamedPosition value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}
int main()
{
    SNamedPosition heap[3] = { SNamedPosition("root", 10), SNamedPosition("b", 20), SNamedPosition("c", 30) };
    SNamedPosition out("out", 0);
    int before = g_strings;
    __pop_heap(heap, heap + 2, &out, SNamedPosition("v", 77), CKeyPairCompareLess(), 0);
    if (std::strcmp(out.first.m_text, "root") || out.second.X != 10 || out.second.Z != 12 || g_hole != 0 || g_len != 2
        || g_valueX != 77 || std::strcmp(g_valueName, "v") || g_strings != before) { std::printf("0077ed20_TEST FAIL\n"); return 1; }
    std::printf("0077ed20_TEST PASS\n");
    return 0;
}