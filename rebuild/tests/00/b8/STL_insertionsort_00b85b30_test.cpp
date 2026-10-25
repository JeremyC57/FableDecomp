// Behaviour test for 0x00B85B30 std::__insertion_sort over CEngineSubPrimitiveRenderer::CSortElement (unsigned key).
#include <cstdio>

struct CEngineSubPrimitiveBase { int id; };
struct CSortElement { unsigned long SortValue; CEngineSubPrimitiveBase* SubPrimitive; };
struct CSortLess {
    void* m_context;
    bool operator()(const CSortElement& a, const CSortElement& b) const { return a.SortValue < b.SortValue; }
};

static int g_linearCalls = 0;

static void __linear_insert(CSortElement* first, CSortElement* last, CSortElement value, CSortLess comp)
{
    ++g_linearCalls;
    if (comp(value, *first)) {
        for (CSortElement* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        CSortElement* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}

static void __insertion_sort(CSortElement* first, CSortElement* last, CSortLess comp)
{
    if (first == last)
        return;
    for (CSortElement* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    CEngineSubPrimitiveBase prims[5] = { {0}, {1}, {2}, {3}, {4} };
    CSortElement a[5] = { {0x80000000ul, &prims[0]}, {2, &prims[1]}, {7, &prims[2]}, {1, &prims[3]}, {2, &prims[4]} };
    CSortLess comp = { 0 };
    __insertion_sort(a, a + 5, comp);
    unsigned long keys[5] = { 1, 2, 2, 7, 0x80000000ul };
    for (int k = 0; k < 5; ++k)
        if (a[k].SortValue != keys[k]) { std::printf("00b85b30_TEST FAIL\n"); return 1; }
    if (a[1].SubPrimitive->id != 1 || a[2].SubPrimitive->id != 4 || a[4].SubPrimitive->id != 0 || g_linearCalls != 4) {
        std::printf("00b85b30_TEST FAIL\n"); return 1;
    }
    std::printf("00b85b30_TEST PASS\n");
    return 0;
}