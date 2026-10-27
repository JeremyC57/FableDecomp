// Behaviour test for 0x00A8ECE0 N3DPrimitive::CDeviceIndependent::GetTotalTriangleCount() const
#include <cstdio>
template <class T> class TArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
class C3DTriangle2 { public: unsigned short Index[3]; };
struct CTriangleList { TArray<C3DTriangle2> Triangles; TArray<unsigned short> StripIndices; };
struct CPtr { CTriangleList* Object; CTriangleList* operator->() const { return Object; } };
struct CDeviceIndependent {
    TArray<CPtr> TriangleLists;
    unsigned long GetTotalTriangleCount() const
    {
        unsigned long total = 0;
        for (unsigned long i = 0; i < TriangleLists.size(); ++i)
            total += TriangleLists[i]->Triangles.size();
        return total;
    }
};
int main()
{
    C3DTriangle2 tris[10];
    CTriangleList a, b;
    a.Triangles._M_start = tris; a.Triangles._M_finish = tris + 4;
    b.Triangles._M_start = tris + 4; b.Triangles._M_finish = tris + 10;
    CPtr ptrs[2]; ptrs[0].Object = &a; ptrs[1].Object = &b;
    CDeviceIndependent d;
    d.TriangleLists._M_start = ptrs; d.TriangleLists._M_finish = ptrs;
    if (d.GetTotalTriangleCount() != 0) { std::printf("00a8ece0_TEST FAIL\n"); return 1; }
    d.TriangleLists._M_finish = ptrs + 2;
    if (d.GetTotalTriangleCount() != 10) { std::printf("00a8ece0_TEST FAIL\n"); return 1; }
    std::printf("00a8ece0_TEST PASS\n");
    return 0;
}