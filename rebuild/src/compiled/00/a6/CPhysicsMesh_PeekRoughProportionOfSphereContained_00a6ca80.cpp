// CPhysicsMesh::PeekRoughProportionOfSphereContained(const CCharString& name, const C3DVector& position, float radius) const -- sum the rough contained proportion of the sphere over every containment volume registered under the name, clamped to [0, 1]
// Leaf: ?PeekRoughProportionOfSphereContained@CPhysicsMesh@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6CA80, 149 bytes (+11 int3), thiscall `ret 0Ch`. Ego_r.pdb
//   `?PeekRoughProportionOfSphereContained@CPhysicsMesh@@QBEMABVCCharString@@ABVC3DVector@@M@Z`.
// Callees: CVectorMap<CCharString,C3DMeshVolume,CKeyPairCompareLess<..> >::UpperBound 0x00A6FCB0 and ::LowerBound
//   0x00A6FC40 (landed, entry pointer in eax), C3DMeshVolume::PeekRoughProportionOfSphereContained(const C3DVector&,
//   float) const 0x00A2E700 (`mov ecx,[ecx+4] ; jmp` into CContainmentVolume::PeekRoughProportionOfSphereContained
//   0x00A75340; the catalog's `_Cons_val<...>` label is propagated). Float pool: 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Label: the manifest's "C3DMeshVolume_GetProportionInContainment" is descriptive; the body walks CPhysicsMesh::
//   ContainmentVolumes (+0x64), so it is the PDB's CPhysicsMesh::PeekRoughProportionOfSphereContained.
// Layout: as 0x00A6CA20 (ContainmentVolumes +0x64, 16-byte pair<CCharString, C3DMeshVolume> entries).
// Shape: `last = UpperBound(name); for (it = LowerBound(name); it != last; ++it) total += it->second.Peek(...)`
//   with the float total in the frame word the prologue's `push ecx` reserves (zeroed before the first call); the
//   clamp is two early returns `if (total < 0) return 0; if (total > 1) return 1; return total;` (`test ah,5 ; jp`
//   for the ordered <, `test ah,41h ; jne` for >).
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
    float PeekRoughProportionOfSphereContained(const C3DVector& position, float radius) const;   // 0x00A2E700
};

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const;
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
    const_iterator LowerBound(const K& key) const;      // 0x00A6FC40
    const_iterator UpperBound(const K& key) const;      // 0x00A6FCB0
};

typedef CVectorMap<CCharString, C3DMeshVolume, CKeyPairCompareLess<CCharString, C3DMeshVolume> > TVolumeMap;

class CPhysicsMesh {
public:
    unsigned char _pad00[0x64];
    TVolumeMap    ContainmentVolumes;                   // +0x64
    float PeekRoughProportionOfSphereContained(const CCharString& name, const C3DVector& position, float radius) const;
};

float CPhysicsMesh::PeekRoughProportionOfSphereContained(const CCharString& name, const C3DVector& position, float radius) const
{
    float total = 0.0f;
    TVolumeMap::const_iterator last = ContainmentVolumes.UpperBound(name);
    for (TVolumeMap::const_iterator it = ContainmentVolumes.LowerBound(name); it != last; ++it)
        total += it->second.PeekRoughProportionOfSphereContained(position, radius);
    if (total < 0.0f)
        return 0.0f;
    if (total > 1.0f)
        return 1.0f;
    return total;
}