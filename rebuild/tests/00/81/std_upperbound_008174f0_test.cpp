// Behaviour test for 0x008174F0 __upper_bound over THitLocationIntersectionElement (model copy).
#include <cstdio>
struct CHitLocation;
struct C3DVector { float X, Y, Z; };
struct THitLocationIntersectionElement {
    long          key;
    CHitLocation* hit_location;
    C3DVector     intersection_pos;
    C3DVector     outward_normal;
};
struct THitLocationIntersectionElementPred {
    bool operator()(const THitLocationIntersectionElement& a, long b) const { return a.key < b; }
    bool operator()(long a, const THitLocationIntersectionElement& b) const { return a < b.key; }
};
template <class ForwardIter, class Tp, class Compare, class Distance>
ForwardIter model_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = last - first;
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        middle += half;
        if (comp(val, *middle)) len = half;
        else { first = middle; ++first; len = len - half - 1; }
    }
    return first;
}
int main()
{
    THitLocationIntersectionElement e[7];
    long keys[7] = { 1, 3, 3, 3, 5, 8, 8 };
    for (int i = 0; i < 7; ++i) { e[i].key = keys[i]; e[i].hit_location = 0; }
    THitLocationIntersectionElementPred p;
    long v3 = 3, v8 = 8, v0 = 0, v9 = 9, v4 = 4;
    const THitLocationIntersectionElement* b = e;
    const THitLocationIntersectionElement* end = e + 7;
    long exp[5] = { 4, 7, 0, 7, 4 };
    long got[5];
    got[0] = model_bound(b, end, v3, p, (int*)0) - b;
    got[1] = model_bound(b, end, v8, p, (int*)0) - b;
    got[2] = model_bound(b, end, v0, p, (int*)0) - b;
    got[3] = model_bound(b, end, v9, p, (int*)0) - b;
    got[4] = model_bound(b, end, v4, p, (int*)0) - b;
    for (int k = 0; k < 5; ++k)
        if (got[k] != exp[k]) { std::printf("008174f0_TEST FAIL\n"); return 1; }
    if (model_bound(b, b, v3, p, (int*)0) != b) { std::printf("008174f0_TEST FAIL\n"); return 1; }
    std::printf("008174f0_TEST PASS\n");
    return 0;
}