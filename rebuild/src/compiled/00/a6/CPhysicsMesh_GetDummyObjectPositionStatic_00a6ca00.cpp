// CPhysicsMesh::GetDummyObjectPositionStatic(const CCharString& name, long index) const -- the index-th dummy object registered under the name (mesh-space transform and bone)
// Leaf: ?GetDummyObjectPositionStatic@CPhysicsMesh@@QBEABVC3DDummyObject@@ABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6CA00, 27 bytes, thiscall `ret 8`, the reference in eax.
// Identity: Ego_r.pdb `?GetDummyObjectPositionStatic@CPhysicsMesh@@QBEABVC3DDummyObject@@ABVCCharString@@J@Z`; in the donor
//   it follows GetNoDummyObjects and sits beside GetHelperPointPositionStatic, the landed retail twin 0x00A6C9C0 (same
//   shape on HelperPoints). Caller 0x00A1184E (CPhysicsMeshObject code).
// Callee: CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >::LowerBound 0x00A6F2F0.
// Layout: Ego_r.pdb CPhysicsMesh with the retail 16-byte CVectorMaps: HelperPoints +0x10, DummyObjects +0x20; an entry is
//   pair<CCharString, C3DDummyObject> (56 bytes), so `second` sits at entry +4 (`imul ecx,ecx,38h ; lea eax,[eax+ecx+4]`).
class CCharString {
public:
    void* PData;                                        // +0x00
};
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject {                                  // 52 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& entry, const CCharString& key) const;
};

template <class K, class V, class C> class CVectorMap {   // retail 16 bytes: STLport vector + Compare + Dirty
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* _M_start;                               // +0x00
    value_type* _M_finish;                              // +0x04
    value_type* _M_end_of_storage;                      // +0x08
    C           Compare;                                // +0x0C
    bool        Dirty;                                  // +0x0D
    const_iterator LowerBound(const K& key) const;      // 0x00A6F2F0
};

class C3DHelperPoint;
typedef CVectorMap<CCharString, C3DHelperPoint, CCompareString<C3DHelperPoint> > THelperPointMap;
typedef CVectorMap<CCharString, C3DDummyObject, CCompareString<C3DDummyObject> > TDummyObjectMap;

class CPhysicsMesh {
public:
    unsigned char   _pad00[0x10];
    unsigned char   HelperPoints[0x10];                 // +0x10 THelperPointMap
    TDummyObjectMap DummyObjects;                       // +0x20
    const C3DDummyObject& GetDummyObjectPositionStatic(const CCharString& name, long index) const;
};

const C3DDummyObject& CPhysicsMesh::GetDummyObjectPositionStatic(const CCharString& name, long index) const
{
    return DummyObjects.LowerBound(name)[index].second;
}