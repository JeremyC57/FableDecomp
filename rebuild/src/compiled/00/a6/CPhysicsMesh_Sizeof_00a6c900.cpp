// CPhysicsMesh::Sizeof() const -- memory footprint of a physics mesh: a fixed part plus every container's storage and the root BSP segment's own Sizeof
// Grade: MATCH (retry 9) -- the root segment is a CCountedPointer member tested through an inline IsValid() and called
//   through its inline operator->: the Data load then goes straight into ecx (the dead `this`) right after the 56-byte
//   multiply and serves as the virtual call's `this`; a raw pointer member (`if (RootSegment) RootSegment->Sizeof()`)
//   loads it into eax after the lea chain and copies to ecx, a named local reshuffles the sum
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6C900, 192 bytes, `ret`.
// Callees: the root segment's virtual Sizeof (CIPhysicsFacesSegmentBase slot 1, +4).
// Shape: retail containers are STLport-style 12-byte vectors (CVectorMap = vector + 4-byte comparator), so the Ego_r.pdb
//   offsets shift down: HelperPoints +0x10, DummyObjects +0x20, TotalFaceIndices +0x30, TotalPoints +0x3C,
//   NavigationLines +0x48, NavigationContainmentVolumes +0x54, ContainmentVolumes +0x64, RootSegment +0xB4. The two
//   name-keyed maps count their used entries (size(): 20-byte helper and 56-byte dummy pairs), the arrays and volume maps
//   their reserved storage (capacity(): 6-byte triangles, 12-byte points and line arrays, 16-byte volume pairs). VC folds
//   the fixed 252 bytes and the constant multipliers into one `lea` chain (3*(n + 2*(m + k) + 42)*2 ...). The terms are listed in the order that reproduces retail's
//   evaluation (dummy objects before helper points, ContainmentVolumes before the navigation volumes). The root
//   segment pointer (the CCountedPointer's data) is null-tested before its virtual call.
class C3DTriangle2 { public: short Index[3]; };                       // 6 bytes
class C3DVector { public: float X, Y, Z; };
struct CArrayOfLines { void* a; void* b; void* c; };           // CArray<C2DLineF>, 12 bytes
struct CHelperPointPair { unsigned char _d[20]; };             // pair<CCharString, C3DHelperPoint>
struct CDummyObjectPair { unsigned char _d[56]; };             // pair<CCharString, C3DDummyObject>
struct CMeshVolumePair { unsigned char _d[16]; };              // pair<key, C3DMeshVolume>

template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    long capacity() const { return _M_end_of_storage - _M_start; }
};
template <class T> class CVectorMap { public:
    CArray<T> Data;
    long      Compare;                                         // empty comparator, padded
};

class CIPhysicsFacesSegmentBase { public:
    virtual void v00();
    virtual long Sizeof() const;                               // slot 1 (+4)
};

template <class T> class CCountedPointer { public:
    T* Data;
    T* operator->() const { return Data; }
    bool IsValid() const { return Data != 0; }
};
class CPhysicsMesh {
public:
    virtual void v00();
    long                          m_04;                        // +0x04
    unsigned long                 ReferenceCount;              // +0x08
    bool                          NavigationLineListInfo;      // +0x0C
    CVectorMap<CHelperPointPair>  HelperPoints;                // +0x10
    CVectorMap<CDummyObjectPair>  DummyObjects;                // +0x20
    CArray<C3DTriangle2>          TotalFaceIndices;            // +0x30
    CArray<C3DVector>             TotalPoints;                 // +0x3C
    CArray<CArrayOfLines>         NavigationLines;             // +0x48
    CVectorMap<CMeshVolumePair>   NavigationContainmentVolumes;// +0x54
    CVectorMap<CMeshVolumePair>   ContainmentVolumes;          // +0x64
    unsigned char                 _pad74[0xB4 - 0x74];         // bounding volumes, centre of gravity
    CCountedPointer<const CIPhysicsFacesSegmentBase> RootSegment;   // +0xB4
    long Sizeof() const;
};

long CPhysicsMesh::Sizeof() const
{
    long size = 252
              + DummyObjects.Data.size() * sizeof(CDummyObjectPair)
              + HelperPoints.Data.size() * sizeof(CHelperPointPair)
              + TotalFaceIndices.capacity() * sizeof(C3DTriangle2)
              + TotalPoints.capacity() * sizeof(C3DVector)
              + NavigationLines.capacity() * sizeof(CArrayOfLines)
              + ContainmentVolumes.Data.capacity() * sizeof(CMeshVolumePair)
              + NavigationContainmentVolumes.Data.capacity() * sizeof(CMeshVolumePair);
    if (RootSegment.IsValid())
        size += RootSegment->Sizeof();
    return size;
}