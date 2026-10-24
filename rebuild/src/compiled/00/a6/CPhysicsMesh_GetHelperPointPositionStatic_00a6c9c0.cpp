// CPhysicsMesh::GetHelperPointPositionStatic(const CCharString& name, long index) const -- the static (mesh-space) position of the index-th helper point registered under the name
// Leaf: ?GetHelperPointPositionStatic@CPhysicsMesh@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6C9C0, 49 bytes (+15 int3), thiscall `ret 0Ch` (hidden C3DVector
//   result, name, index). Ego_r.pdb `?GetHelperPointPositionStatic@CPhysicsMesh@@QBE?AVC3DVector@@ABVCCharString@@J@Z`.
// Callee: CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> >::LowerBound 0x00A6F290 (landed under the
//   propagated CNavLineMapValue name; 20-byte entries = pair<CCharString, C3DHelperPoint>; entry pointer in eax).
// Caller: 0x00A11778 (CPhysicsMeshObject code, which scales the returned vector by the object's transform).
// Label: the manifest's "CVectorMap_GetValueByIndex" is descriptive; the map is CPhysicsMesh::HelperPoints (+0x10) and
//   the value read is C3DHelperPoint::Position, so it is the PDB's CPhysicsMesh::GetHelperPointPositionStatic.
// Layout: Ego_r.pdb CPhysicsMesh with the retail 16-byte CVectorMap: HelperPoints +0x10; C3DHelperPoint = { C3DVector
//   Position +0, long BoneIndex +0xC } (16 bytes), so an entry is 20 bytes and Position sits at entry +4.
// Shape: `return HelperPoints.LowerBound(name)[index].second.Position;` -- `lea ecx,[ecx+ecx*4]` is the 20-byte stride,
//   the C3DVector copied into the hidden result word by word through its memberwise user copy constructor (stores
//   through eax, the source re-based in ecx after the first load); the implicit copy keeps the result pointer apart
//   (`push esi ; mov ecx,eax`, 51 v 49). C3DVector is 1-aligned in retail (pack(1)).
class CCharString {
public:
    void* PData;                                        // +0x00
};

#pragma pack(push, 1)
class C3DVector {                                       // 12 bytes, 1-aligned in retail (GOTCHAS: pack(1))
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

class C3DHelperPoint {                                  // 16 bytes
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};
}

template <class V> class CCompareString {
public:
    bool operator()(const _STL::pair<CCharString, V>& entry, const CCharString& key) const;
};

template <class K, class V, class C> class CVectorMap {   // retail 16 bytes: STLport vector + Compare + Dirty
public:
    typedef _STL::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* _M_start;                               // +0x00
    value_type* _M_finish;                              // +0x04
    value_type* _M_end_of_storage;                      // +0x08
    C           Compare;                                // +0x0C
    bool        Dirty;                                  // +0x0D
    const_iterator LowerBound(const K& key) const;      // 0x00A6F290
};

typedef CVectorMap<CCharString, C3DHelperPoint, CCompareString<C3DHelperPoint> > THelperPointMap;

class CPhysicsMesh {
public:
    unsigned char   _pad00[0x10];
    THelperPointMap HelperPoints;                       // +0x10
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;
};

C3DVector CPhysicsMesh::GetHelperPointPositionStatic(const CCharString& name, long index) const
{
    return HelperPoints.LowerBound(name)[index].second.Position;
}