// CPhysicsMesh::CPhysicsMesh(const C3DMeshFile& file, CMovableResourceMemoryPool* pool) -- build a physics mesh from a mesh file: empty helper/dummy maps, face/point/navigation arrays and containment maps, cleared bounding sphere and segment tree, one reference; then read the mesh, helper and containment data and derive the navigation, bounding, segmentation and centre-of-gravity information
// Leaf: ??0CPhysicsMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6DA40, 255 bytes, `ret 8`. PDB: ??0CPhysicsMesh@@QAE@ABVC3DMeshFile@@PAVCMovableResourceMemoryPool@@@Z.
//   Called from 0x00A25D90. Vtable 0x0129DC68 (CPhysicsMesh).
// Callees (PDB names, catalogue labels agree): GetMeshDataFromFileData 0x00A6CC10, GetHelperDataFromFileData 0x00A6D0E0,
//   CalculateNavigationLineInfo 0x00A74360, CalculateBoundingInfo 0x00A6C640, CIPhysicsFacesSegmentBase::Create(const
//   CArray<C3DTriangle2>&, const CArray<C3DVector>&) 0x00A86CD0 (`__fastcall`), CCountedPointer<const
//   CIPhysicsFacesSegmentBase>::Reset 0x00A6E9A0, CalculateCentreOfGravity 0x00A6C890, GetContainmentDataFromFileData 0x00A6D670.
// Layout: Ego_r.pdb CPhysicsMesh (0xDC) with retail 12-byte CArrays and 0x10-byte CVectorMaps (retail 0xC0): base word +4,
//   ReferenceCount +8, NavigationLineListInfo +0x0C, HelperPoints +0x10, DummyObjects +0x20, TotalFaceIndices +0x30,
//   TotalPoints +0x3C, NavigationLines +0x48, NavigationContainmentVolumes +0x54, ContainmentVolumes +0x64,
//   BoundingSphere +0x74, BoundingBox +0x84, BoundingCircle +0x9C, CentreOfGravity +0xA8, RootSegment +0xB4, MemoryPool +0xBC.
// Shape: CArrays are STLport vectors (`_Vector_base` + `_STLP_alloc_proxy` from an `allocator()` temporary: that places
//   `push edi` and the two `lea` before the 0x61/0x64 stores like retail); C3DTriangle2 is 6 bytes (`imul` with no `sar`);
//   the private PDB CalculateSegmentation is inline (`if (TotalFaceIndices.size() > 60) RootSegment.Reset(Create(...))`).
struct C3DMeshFile;
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

class CIPhysicsFacesSegmentBase {
public:
    static CIPhysicsFacesSegmentBase* __fastcall Create(const CArray<C3DTriangle2>& faces, const CArray<C3DVector>& points);   // 0x00A86CD0
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* data);                                // 0x00A6E9A0 for T = const CIPhysicsFacesSegmentBase
};

class CIVCountedBase {                                  // intrusive counted base: vptr + the +4 word
public:
    virtual ~CIVCountedBase();
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
    virtual ~CPhysicsMesh();
    void GetMeshDataFromFileData(const C3DMeshFile& file);          // 0x00A6CC10
    void GetHelperDataFromFileData(const C3DMeshFile& file);        // 0x00A6D0E0
    void CalculateNavigationLineInfo();                             // 0x00A74360
    void CalculateBoundingInfo();                                   // 0x00A6C640
    void CalculateCentreOfGravity();                                // 0x00A6C890
    void GetContainmentDataFromFileData(const C3DMeshFile& file);   // 0x00A6D670
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