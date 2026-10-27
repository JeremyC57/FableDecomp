// Behaviour test for 0x00A8ED30 N3DPrimitive::CDeviceIndependent::GetTotalIndexCount() const
// Model: a strip list contributes its strip index count, a plain list three indices per triangle.
#include <cstdio>
template <class T> class TArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
class C3DTriangle2 { public: unsigned short Index[3]; };
struct CTriangleList { TArray<C3DTriangle2> Triangles; TArray<unsigned short> StripIndices; };
struct CPtr { CTriangleList* Object; };
struct CDeviceIndependent {
    TArray<CPtr> TriangleLists;
    unsigned long GetTotalIndexCount() const
    {
        unsigned long total = 0;
        for (unsigned long i = 0; i < TriangleLists.size(); ++i) {
            const CTriangleList* list = TriangleLists[i].Object;
            if (list->StripIndices.size() != 0)
                total += list->StripIndices.size();
            else
                total += list->Triangles.size() * 3;
        }
        return total;
    }
};
int main()
{
    C3DTriangle2 tris[10];
    unsigned short strip[7];
    CTriangleList a, b;
    a.Triangles._M_start = tris; a.Triangles._M_finish = tris + 4;
    a.StripIndices._M_start = strip; a.StripIndices._M_finish = strip;          // no strip: 4 * 3
    b.Triangles._M_start = tris; b.Triangles._M_finish = tris + 5;
    b.StripIndices._M_start = strip; b.StripIndices._M_finish = strip + 7;      // strip: 7
    CPtr ptrs[2]; ptrs[0].Object = &a; ptrs[1].Object = &b;
    CDeviceIndependent d;
    d.TriangleLists._M_start = ptrs; d.TriangleLists._M_finish = ptrs + 2;
    if (d.GetTotalIndexCount() != 19) { std::printf("00a8ed30_TEST FAIL\n"); return 1; }
    d.TriangleLists._M_finish = ptrs;
    if (d.GetTotalIndexCount() != 0) { std::printf("00a8ed30_TEST FAIL\n"); return 1; }
    std::printf("00a8ed30_TEST PASS\n");
    return 0;
}