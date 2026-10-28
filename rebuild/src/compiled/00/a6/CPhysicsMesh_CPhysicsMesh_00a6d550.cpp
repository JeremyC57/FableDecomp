// CPhysicsMesh::~CPhysicsMesh() -- destroys the mesh's members in reverse order (root segment counted pointer, the two containment-volume maps, the navigation lines, the point / face arrays, the dummy-object and helper-point maps), then the CIVCountedPointeeBase part
// Leaf: ??1CPhysicsMesh@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6D550, 273 bytes, thiscall, `ret`.
// Identity (proven): RTTI vtable 0x0129DC68 (.?AVCPhysicsMesh@@) slot 0 is the deleting destructor 0x00A73CC0, whose first
//   call is this row; Ego_r.pdb ??1CPhysicsMesh@@UAE@XZ (virtual, public). The first store is CPhysicsMesh's vftable, the
//   last CIVCountedPointeeBase's (0x012354D0). The catalog's "C3DMeshVolume_Destructor" is descriptive.
// Layout (retail, STLport sizes; Ego_r.pdb member names and order): base CIVCountedPointeeBase {vptr, IVRefCount} +0,
//   ReferenceCount +0x08, NavigationLineListInfo +0x0C, HelperPoints +0x10 / DummyObjects +0x20 (CVectorMap 0x10 over
//   pair<CCharString, C3DHelperPoint> 0x14 / pair<CCharString, C3DDummyObject> 0x38), TotalFaceIndices +0x30,
//   TotalPoints +0x3C, NavigationLines +0x48 (CArray 0x0C), NavigationContainmentVolumes +0x54, ContainmentVolumes +0x64
//   (CVectorMap 0x10), BoundingSphere +0x74, BoundingBox +0x84, BoundingCircle +0x9C, CentreOfGravity +0xA8,
//   RootSegment +0xB4 (CCountedPointer<const CIPhysicsFacesSegmentBase>), MemoryPool +0xBC; 0xC0 bytes.
// Callees: operator delete 0x00BFE9BC, free 0x00BFEA14, ~CCharString 0x0099EAE0, std::__destroy_aux over
//   pair<CCharString, C3DMeshVolume> 0x00A70EE0 and pair<long, C3DMeshVolume> 0x00A70E80 (/Gr __fastcall, landed),
//   CArray<CArray<C2DLineF> >::~CArray 0x00A73AE0 (Ego_r.pdb ??1?$CArray@V?$CArray@VC2DLineF@@@@@@QAE@XZ; landed under the
//   invented CRetailVector_SBufferEntry12 -- NAME_FIXES).
// Shape: an empty user-written virtual destructor (VC resets the vptr first); the CCharString-keyed maps' element loops
//   are inlined (__forceinline __destroy_aux), the C3DMeshVolume ones and the nested CArray destructor are out of line
//   (explicit specialisation declarations of the real retail symbols).
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;                                     // +0x00
    ~CCharString();                                        // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float M[12]; };
class C3DHelperPoint { public: C3DVector Position; long BoneIndex; };          // 0x10
class C3DDummyObject { public: CMatrix3x4 Transform; long BoneIndex; };        // 0x34
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class C2DLineF { public: float P[4]; };
class CContainmentVolume;
class CIPhysicsFacesSegmentBase;
class CMovableResourceMemoryPool;
class C3DBoundingSphere { public: C3DVector Centre; float Radius; };           // 0x10
class C3DBoundingBox { public: C3DVector Min; C3DVector Max; };                // 0x18
class CCircle { public: float X, Y, Radius; };                                 // 0x0C

class CCPPointerInfo {
public:
    unsigned long RefCount;                                // +0
    void (__fastcall* DeleteFunc)(void* data);             // +4
    void* Data;                                            // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0
    CCPPointerInfo* Info;                                  // +4
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
class C3DMeshVolume {                                      // 0x0C, non-trivial destructor (out of line in retail)
public:
    CCharString Name;                                      // +0
    CCountedPointer<CContainmentVolume> ConvexVolume;      // +4
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <> void __fastcall __destroy_aux<pair<CCharString, C3DMeshVolume>*>(pair<CCharString, C3DMeshVolume>*,
                                                                            pair<CCharString, C3DMeshVolume>*,
                                                                            const __false_type&);   // 0x00A70EE0
template <> void __fastcall __destroy_aux<pair<long, C3DMeshVolume>*>(pair<long, C3DMeshVolume>*,
                                                                    pair<long, C3DMeshVolume>*,
                                                                    const __false_type&);           // 0x00A70E80
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T> {
public:
    ~CArray() { }
};
template <> CArray<CArray<C2DLineF> >::~CArray();          // 0x00A73AE0 (out of line in retail)

template <class T> class CCompareString { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                         // +0x00
    TCompare                 Compare;                      // +0x0C
    bool                     Dirty;                        // +0x0D
};

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;                              // +0x04
};

class CPhysicsMesh : public CIVCountedPointeeBase {
public:
    virtual ~CPhysicsMesh();
    unsigned long ReferenceCount;                                                               // +0x08
    bool NavigationLineListInfo;                                                                // +0x0C
    CVectorMap<CCharString, C3DHelperPoint, CCompareString<C3DHelperPoint> > HelperPoints;      // +0x10
    CVectorMap<CCharString, C3DDummyObject, CCompareString<C3DDummyObject> > DummyObjects;      // +0x20
    CArray<C3DTriangle2> TotalFaceIndices;                                                      // +0x30
    CArray<C3DVector> TotalPoints;                                                              // +0x3C
    CArray<CArray<C2DLineF> > NavigationLines;                                                  // +0x48
    CVectorMap<long, C3DMeshVolume, CKeyPairCompareLess<long, C3DMeshVolume> > NavigationContainmentVolumes;   // +0x54
    CVectorMap<CCharString, C3DMeshVolume, CKeyPairCompareLess<CCharString, C3DMeshVolume> > ContainmentVolumes; // +0x64
    C3DBoundingSphere BoundingSphere;                                                           // +0x74
    C3DBoundingBox BoundingBox;                                                                 // +0x84
    CCircle BoundingCircle;                                                                     // +0x9C
    C3DVector CentreOfGravity;                                                                  // +0xA8
    CCountedPointer<const CIPhysicsFacesSegmentBase> RootSegment;                               // +0xB4
    CMovableResourceMemoryPool* MemoryPool;                                                     // +0xBC
};

CPhysicsMesh::~CPhysicsMesh()
{
}