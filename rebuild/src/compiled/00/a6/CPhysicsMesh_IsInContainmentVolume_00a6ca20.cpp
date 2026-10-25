// CPhysicsMesh::IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const -- true when any containment volume registered under the name holds part of the sphere
// Leaf: ?IsInContainmentVolume@CPhysicsMesh@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6CA20, 87 bytes (+9 int3), thiscall `ret 0Ch`. Ego_r.pdb
//   `?IsInContainmentVolume@CPhysicsMesh@@QBE_NABVCCharString@@ABVC3DVector@@M@Z`.
// Callees: CVectorMap<CCharString,C3DMeshVolume,CKeyPairCompareLess<..> >::UpperBound 0x00A6FCB0 and ::LowerBound
//   0x00A6FC40 (both landed; __fastcall, the STLport iterator = entry pointer in eax), C3DMeshVolume::
//   IsContainingPartOfSphere(const C3DVector&, float) const 0x00A2E730 (`mov ecx,[ecx+4] ; jmp` into
//   CContainmentVolume::IsContainingPartOfSphere 0x00A754A0; the catalog's `_Cons_val<...>` label is propagated).
// Label: the manifest's "C3DMeshVolume_IsPointInContainment" is descriptive; the body walks CPhysicsMesh::
//   ContainmentVolumes (+0x64), so it is the PDB's CPhysicsMesh::IsInContainmentVolume.
// Layout (Ego_r.pdb CPhysicsMesh with the retail 16-byte CVectorMaps / 12-byte CArrays): HelperPoints +0x10,
//   DummyObjects +0x20, TotalFaceIndices +0x30, TotalPoints +0x3C, NavigationLines +0x48 (landed 0x00A73D70),
//   NavigationContainmentVolumes +0x54, ContainmentVolumes +0x64; entry pair<CCharString, C3DMeshVolume> = 16 bytes
//   (C3DMeshVolume: Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4).
// Shape: one key walked as `last = UpperBound(name); for (it = LowerBound(name); it != last; ++it)` (UpperBound is
//   called first, the map address kept in esi for both calls); a hit returns true straight from the loop.
class CCharString {
public:
    void* PData;                                        // +0x00
};

class C3DVector {
public:
    float X, Y, Z;
};

class CContainmentVolume;

template <class T> class CCountedPointer {
public:
    T*    Data;                                         // +0x00
    void* Info;                                         // +0x04
};

class C3DMeshVolume {                                   // 12 bytes
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
    bool IsContainingPartOfSphere(const C3DVector& position, float radius) const;   // 0x00A2E730
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
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
    const_iterator LowerBound(const K& key) const;      // 0x00A6FC40
    const_iterator UpperBound(const K& key) const;      // 0x00A6FCB0
};

typedef CVectorMap<CCharString, C3DMeshVolume, CKeyPairCompareLess<CCharString, C3DMeshVolume> > TVolumeMap;

class CPhysicsMesh {
public:
    unsigned char _pad00[0x64];
    TVolumeMap    ContainmentVolumes;                   // +0x64
    bool IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const;
};

bool CPhysicsMesh::IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const
{
    TVolumeMap::const_iterator last = ContainmentVolumes.UpperBound(name);
    for (TVolumeMap::const_iterator it = ContainmentVolumes.LowerBound(name); it != last; ++it)
        if (it->second.IsContainingPartOfSphere(position, radius))
            return true;
    return false;
}