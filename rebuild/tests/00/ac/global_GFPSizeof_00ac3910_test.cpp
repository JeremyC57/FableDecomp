// Behaviour test for 0x00AC3910 GFPSizeof<C3DMeshPartitionElement* const> -- pointer size plus, when non-null, the pointee's recursive Sizeof
#include <cstdio>
template <class T> inline bool GFPIsNull(T* p) { return p == 0; }
template <class T> inline unsigned long GFSizeof(T& object) { return object.Sizeof(); }
template <class T> unsigned long GFPSizeof(T& pointer)
{
    if (!GFPIsNull(pointer))
        return sizeof(T) + GFSizeof(*pointer);
    return sizeof(T);
}
class C3DMeshPartitionElement {
public:
    long StartTriangle;
    long TriangleCount;
    C3DMeshPartitionElement* SubElements[2];
    unsigned long Sizeof() const { return GFPSizeof(SubElements[0]) + GFPSizeof(SubElements[1]); }
};
int main()
{
    const unsigned long P = sizeof(C3DMeshPartitionElement*);
    C3DMeshPartitionElement leafA, leafB, root;
    leafA.SubElements[0] = leafA.SubElements[1] = 0;
    leafB.SubElements[0] = leafB.SubElements[1] = 0;
    root.SubElements[0] = &leafA; root.SubElements[1] = &leafB;
    C3DMeshPartitionElement* const nullp = 0;
    C3DMeshPartitionElement* const rootp = &root;
    C3DMeshPartitionElement* const leafp = &leafA;
    if (GFPSizeof(nullp) != P) { std::printf("00ac3910_TEST FAIL\n"); return 1; }
    if (GFPSizeof(leafp) != 3 * P) { std::printf("00ac3910_TEST FAIL\n"); return 1; }
    if (GFPSizeof(rootp) != 7 * P) { std::printf("00ac3910_TEST FAIL\n"); return 1; }
    std::printf("00ac3910_TEST PASS\n");
    return 0;
}