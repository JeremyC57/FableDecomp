// CPhysicsMesh::HasContainmentVolume(const CCharString& name) const -- true when the containment-volume map holds an entry keyed by the name
// Leaf: ?HasContainmentVolume@CPhysicsMesh@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6CBC0, 75 bytes (+5 int3), thiscall `ret 4`. Ego_r.pdb
//   `?HasContainmentVolume@CPhysicsMesh@@QBE_NABVCCharString@@@Z`.
// Callees: CVectorMap<CCharString,C3DMeshVolume,CKeyPairCompareLess<..> >::LowerBound 0x00A6FC40 (landed, entry
//   pointer in eax), CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<).
// Label: the manifest's "C3DMeshVolume_HasVolume" is descriptive; the body searches CPhysicsMesh::ContainmentVolumes
//   (+0x64), so it is the PDB's CPhysicsMesh::HasContainmentVolume.
// Layout: as 0x00A6CA20 (ContainmentVolumes +0x64: STLport vector start +0x64, finish +0x68; 16-byte entries).
// Shape: the inline CVectorMap::Find -- `it = LowerBound(key); last = end(); if (it == last || Compare(key, *it))
//   return last; return it;` with end() read once into esi after the call, and the key compare the inline
//   CCharString::operator< of the landed LowerBound 0x00A6FC40 (equal data -> false, null left -> true, null right ->
//   false, else the out-of-line data compare) -- then `Find(name) != end()` with the found iterator named, so the
//   map's finish is re-read after the search.
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;    // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;                          // +0x00
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
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
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
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
    const_iterator end() const { return _M_finish; }
    const_iterator LowerBound(const K& key) const;      // 0x00A6FC40
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        const_iterator last = end();
        if (it == last || Compare(key, *it))
            return last;
        return it;
    }
};

typedef CVectorMap<CCharString, C3DMeshVolume, CKeyPairCompareLess<CCharString, C3DMeshVolume> > TVolumeMap;

class CPhysicsMesh {
public:
    unsigned char _pad00[0x64];
    TVolumeMap    ContainmentVolumes;                   // +0x64
    bool HasContainmentVolume(const CCharString& name) const;
};

bool CPhysicsMesh::HasContainmentVolume(const CCharString& name) const
{
    TVolumeMap::const_iterator it = ContainmentVolumes.Find(name);
    return it != ContainmentVolumes.end();
}