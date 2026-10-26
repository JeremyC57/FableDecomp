#include <cstdio>
static unsigned long g_resizeN = 0, g_reserve[3] = { 99, 99, 99 }; static void* g_resizeVal = (void*)1;
class CMeshDataBankEntry; class C3DMeshInfo; class CPhysicsMeshInfo; class C3DAnimationInfo;
template<class T> class CArray { public: void resize(unsigned long n, const T& v) { g_resizeN = n; g_resizeVal = (void*)v; } void reserve(unsigned long); };
template<> void CArray<C3DMeshInfo>::reserve(unsigned long n) { g_reserve[0] = n; }
template<> void CArray<CPhysicsMeshInfo>::reserve(unsigned long n) { g_reserve[1] = n; }
template<> void CArray<C3DAnimationInfo>::reserve(unsigned long n) { g_reserve[2] = n; }
struct CBankEntryTypeSize { long Type; unsigned long Size; };
template<class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return *(_M_start + i); }
};
inline bool IsMeshType(long type) { return type == 1 || type == 2 || type == 4 || type == 5; }
inline bool IsAnimationType(long type) { return type == 6 || type == 10 || type == 7 || type == 8 || type == 9; }
class CMeshDataBank { public:
    bool ReadOnly;
    CArray<CMeshDataBankEntry*> MeshesAndAnimations; CArray<C3DMeshInfo> MeshInfoBuffer; CArray<CPhysicsMeshInfo> PhysicsMeshInfoBuffer; CArray<C3DAnimationInfo> AnimationInfoBuffer;
    void NotifyBankSize(unsigned long noEntries, const vector<CBankEntryTypeSize>& entries);
};
void CMeshDataBank::NotifyBankSize(unsigned long noEntries, const vector<CBankEntryTypeSize>& entries)
{
    MeshesAndAnimations.resize(noEntries, 0);
    if (ReadOnly) {
        unsigned long meshSize = 0;
        unsigned long physicsMeshSize = 0;
        unsigned long animationSize = 0;
        for (unsigned long i = 0; i < entries.size(); ++i) {
            long type = entries[i].Type;
            unsigned long size = entries[i].Size;
            if (IsMeshType(type))
                meshSize += size;
            else if (type == 3)
                physicsMeshSize += size;
            else if (IsAnimationType(type))
                animationSize += size;
        }
        MeshInfoBuffer.reserve(meshSize);
        PhysicsMeshInfoBuffer.reserve(physicsMeshSize);
        AnimationInfoBuffer.reserve(animationSize);
    }
}
int main()
{
    CBankEntryTypeSize e[12] = { {1,10}, {2,20}, {3,30}, {4,40}, {5,50}, {6,60}, {7,70}, {8,80}, {9,90}, {10,100}, {11,1000}, {0,2000} };
    vector<CBankEntryTypeSize> v; v._M_start = e; v._M_finish = e + 12; v._M_end_of_storage = e + 12;
    CMeshDataBank b; b.ReadOnly = false;
    b.NotifyBankSize(7, v);
    if (g_resizeN != 7 || g_resizeVal != 0 || g_reserve[0] != 99) { std::printf("00a26660_TEST FAIL\n"); return 1; }
    b.ReadOnly = true; b.NotifyBankSize(12, v);
    if (g_reserve[0] != 120 || g_reserve[1] != 30 || g_reserve[2] != 400) { std::printf("00a26660_TEST FAIL\n"); return 1; }
    v._M_finish = e; b.NotifyBankSize(0, v);
    if (g_reserve[0] != 0 || g_reserve[1] != 0 || g_reserve[2] != 0) { std::printf("00a26660_TEST FAIL\n"); return 1; }
    std::printf("00a26660_TEST PASS\n");
    return 0;
}