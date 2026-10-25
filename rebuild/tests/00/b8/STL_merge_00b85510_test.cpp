// Behaviour test for 0x00B85510 std::merge of two sorted CEngineSubPrimitiveRenderer::CSortElement runs by the unsigned sort value.
#include <cstdio>

struct CEngineSubPrimitiveBase { int id; };
struct CSortElement { unsigned long SortValue; CEngineSubPrimitiveBase* SubPrimitive; };
struct CSortLess {
    bool operator()(const CSortElement& a, const CSortElement& b) const { return a.SortValue < b.SortValue; }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
static CSortElement* __copy(CSortElement* first, CSortElement* last, CSortElement* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
static CSortElement* copy(CSortElement* first, CSortElement* last, CSortElement* result)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
static CSortElement* merge(CSortElement* first1, CSortElement* last1, CSortElement* first2, CSortElement* last2, CSortElement* result, CSortLess comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) { *result = *first2; ++first2; }
        else { *result = *first1; ++first1; }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

int main()
{
    CEngineSubPrimitiveBase prims[9];
    unsigned long v1[4] = { 1, 5, 5, 0x90000000UL };   // the high value checks the unsigned compare
    unsigned long v2[5] = { 0, 5, 6, 7, 0x80000000UL };
    CSortElement a[4], b[5], out[9];
    for (int k = 0; k < 4; ++k) { prims[k].id = k; a[k].SortValue = v1[k]; a[k].SubPrimitive = &prims[k]; }
    for (int k = 0; k < 5; ++k) { prims[4 + k].id = 4 + k; b[k].SortValue = v2[k]; b[k].SubPrimitive = &prims[4 + k]; }
    CSortLess comp;
    CSortElement* end = merge(a, a + 4, b, b + 5, out, comp);
    if (end != out + 9) { std::printf("00b85510_TEST FAIL\n"); return 1; }
    // ties keep the first run first: 0(b0) 1(a0) 5(a1) 5(a2) 5(b1) 6(b2) 7(b3) 0x80000000(b4) 0x90000000(a3)
    int order[9] = { 4, 0, 1, 2, 5, 6, 7, 8, 3 };
    for (int j = 0; j < 9; ++j)
        if (out[j].SubPrimitive != &prims[order[j]]) { std::printf("00b85510_TEST FAIL\n"); return 1; }
    // one empty run: the other is copied unchanged
    CSortElement out2[4];
    if (merge(a, a, a, a + 4, out2, comp) != out2 + 4 || out2[3].SubPrimitive != &prims[3]) { std::printf("00b85510_TEST FAIL\n"); return 1; }
    std::printf("00b85510_TEST PASS\n");
    return 0;
}