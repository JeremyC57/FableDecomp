#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
static char g_log[256];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct C3DMeshFile { int dummy; };
struct C3DTriangle2;
static const void* g_createdFrom; static const C3DMeshFile* g_file;
static C3DTriangle2* g_faces; static int g_faceCount;
struct CMovableResourceMemoryPool;
struct C3DTriangle2 { unsigned short Index[3]; };   // 6 bytes (the size() divide is by 6)
struct C3DVector { float X, Y, Z; };
struct C2DLineF;
struct C3DHelperPoint; struct C3DDummyObject; struct C3DMeshVolume;

template <class T> struct CAllocator {                 // STLport allocator<T>: empty
    CAllocator() {}
};
template <class T> struct CAllocProxy : CAllocator<T> {  // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), _M_data(p) {}
};
template <class T> struct CVectorBase {                // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) {}
};
template <class T> struct CArray : CVectorBase<T> {    // CArray<T>: an STLport vector, retail 12 bytes
    CArray() : CVectorBase<T>(CAllocator<T>()) {}
    unsigned long size() const { return this->_M_finish - this->_M_start; }
};
template <class K, class V> struct CVectorMap {         // retail 0x10 bytes
    CArray<void*>  Vector;
    unsigned char  Compare;
    bool           Dirty;
    CVectorMap() : Dirty(false) {}
};
struct C3DBoundingSphere {
    long Centre[3];
    long Radius;
    C3DBoundingSphere() { Centre[0] = 0; Centre[1] = 0; Centre[2] = 0; Radius = 0; }
};
struct C3DBoundingBox { C3DVector Min, Max; };
struct CCircle { float X, Y, Radius; };

class CIPhysicsFacesSegmentBase;
extern CIPhysicsFacesSegmentBase g_segment;
class CIPhysicsFacesSegmentBase {
public:
    static CIPhysicsFacesSegmentBase* FASTCALL Create(const CArray<C3DTriangle2>& faces, const CArray<C3DVector>& points) { LOG("create"); g_createdFrom = &faces; return &g_segment; }
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* data) { Data = data; LOG("reset"); }
};

class CIVCountedBase {                                  // intrusive counted base: vptr + the +4 word
public:
    virtual ~CIVCountedBase() {}
    long BaseCount;                                     // +0x04
    CIVCountedBase() : BaseCount(1) {}
};

class CPhysicsMesh : public CIVCountedBase {            // vtable 0x0129DC68
public:
    unsigned long                                ReferenceCount;               // +0x08
    bool                                         NavigationLineListInfo;       // +0x0c
    CVectorMap<long, C3DHelperPoint>             HelperPoints;                 // +0x10
    CVectorMap<long, C3DDummyObject>             DummyObjects;                 // +0x20
    CArray<C3DTriangle2>                         TotalFaceIndices;             // +0x30
    CArray<C3DVector>                            TotalPoints;                  // +0x3c
    CArray<CArray<C2DLineF> >                    NavigationLines;              // +0x48
    CVectorMap<long, C3DMeshVolume>              NavigationContainmentVolumes; // +0x54
    CVectorMap<long, C3DMeshVolume>              ContainmentVolumes;           // +0x64
    C3DBoundingSphere                            BoundingSphere;               // +0x74
    C3DBoundingBox                               BoundingBox;                  // +0x84
    CCircle                                      BoundingCircle;               // +0x9c
    C3DVector                                    CentreOfGravity;              // +0xa8
    CCountedPointer<const CIPhysicsFacesSegmentBase> RootSegment;              // +0xb4
    CMovableResourceMemoryPool*                  MemoryPool;                   // +0xbc
    CPhysicsMesh(const C3DMeshFile& file, CMovableResourceMemoryPool* pool);
    virtual ~CPhysicsMesh() {}
    void GetMeshDataFromFileData(const C3DMeshFile& file) { g_file = &file; LOG("mesh"); TotalFaceIndices._M_start = g_faces; TotalFaceIndices._M_finish = g_faces + g_faceCount; }
    void GetHelperDataFromFileData(const C3DMeshFile& file) { if (&file == g_file) LOG("helper"); }
    void CalculateNavigationLineInfo() { LOG("nav"); }
    void CalculateBoundingInfo() { LOG("bound"); }
    void CalculateCentreOfGravity() { LOG("cog"); }
    void GetContainmentDataFromFileData(const C3DMeshFile& file) { if (&file == g_file) LOG("contain"); }
    void CalculateSegmentation()
    {
        if (TotalFaceIndices.size() > 60)
            RootSegment.Reset(CIPhysicsFacesSegmentBase::Create(TotalFaceIndices, TotalPoints));
    }
};

CPhysicsMesh::CPhysicsMesh(const C3DMeshFile& file, CMovableResourceMemoryPool* pool)
    : ReferenceCount(1)
{
    NavigationLineListInfo = false;
    MemoryPool = pool;
    GetMeshDataFromFileData(file);
    GetHelperDataFromFileData(file);
    CalculateNavigationLineInfo();
    CalculateBoundingInfo();
    CalculateSegmentation();
    CalculateCentreOfGravity();
    GetContainmentDataFromFileData(file);
}

CIPhysicsFacesSegmentBase g_segment;
static C3DTriangle2 s_faces[100];
int main()
{
    C3DMeshFile file; int pool;
    g_faces = s_faces; g_faceCount = 10;
    CPhysicsMesh small(file, (CMovableResourceMemoryPool*)&pool);
    if (std::strcmp(g_log, "mesh;helper;nav;bound;cog;contain;") || small.RootSegment.Data != 0 || small.ReferenceCount != 1 || small.BaseCount != 1 ||
        small.NavigationLineListInfo != false || small.MemoryPool != (CMovableResourceMemoryPool*)&pool || small.HelperPoints.Dirty != false ||
        small.TotalPoints._M_start != 0 || small.BoundingSphere.Radius != 0 || small.ContainmentVolumes.Vector._M_finish != 0) { std::printf("00a6da40_TEST FAIL\n"); return 1; }
    g_log[0] = 0; g_faceCount = 61;
    CPhysicsMesh big(file, 0);
    if (std::strcmp(g_log, "mesh;helper;nav;bound;create;reset;cog;contain;") || big.RootSegment.Data != &g_segment || g_createdFrom != &big.TotalFaceIndices) { std::printf("00a6da40_TEST FAIL\n"); return 1; }
    g_log[0] = 0; g_faceCount = 60;
    CPhysicsMesh edge(file, 0);
    if (std::strcmp(g_log, "mesh;helper;nav;bound;cog;contain;")) { std::printf("00a6da40_TEST FAIL\n"); return 1; }
    std::printf("00a6da40_TEST PASS\n");
    return 0;
}