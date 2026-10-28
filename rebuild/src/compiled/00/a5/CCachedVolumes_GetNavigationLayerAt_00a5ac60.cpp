// CCachedVolumes::GetNavigationLayerAt(const C3DVector& position) const -- the navigation layer (map key) of the first cached containment volume holding the position, 0 when none does
// Leaf: ?GetNavigationLayerAt@CCachedVolumes@@QBEJABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5AC60, 62 bytes, `ret 4`. Name: Ego_r.pdb ?GetNavigationLayerAt@CCachedVolumes@@QBEJABVC3DVector@@@Z;
//   the catalog label agrees.
// Layout: Ego_r.pdb CCachedVolumes {CVectorMap<long, CCountedPointer<CContainmentVolume> > NavigationContainmentVolumes +0}
//   (CVectorMap: CArray Data +0 = STLport vector start/finish/eos; 12-byte pair {long, CCountedPointer {Data, Info}}).
// Callee: CContainmentVolume::IsContainingPoint(const C3DVector&) const 0x00A75430 (landed).
// Shape: the key is read into a local before the volume test (retail loads it into edi ahead of the call); end() re-read
//   every pass through the inline CVectorMap accessor.
class C3DVector { public: float X, Y, Z; };
class CContainmentVolume {
public:
    bool IsContainingPoint(const C3DVector& point) const;   // 0x00A75430
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    T* operator->() const { return Data; }
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
};
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}
template <class T> class CArray : public std::vector<T> { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                            // +0x00
    TCompare           Compare;                         // +0x0C
    bool               Dirty;                           // +0x0D
    const_iterator begin() const { return Data.begin(); }
    const_iterator end() const { return Data.end(); }
};

class CCachedVolumes {
public:
    CVectorMap<long, CCountedPointer<CContainmentVolume>, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > >
        NavigationContainmentVolumes;                   // +0x00
    long GetNavigationLayerAt(const C3DVector& position) const;
};

long CCachedVolumes::GetNavigationLayerAt(const C3DVector& position) const
{
    for (CVectorMap<long, CCountedPointer<CContainmentVolume>, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > >::const_iterator
             it = NavigationContainmentVolumes.begin(); it != NavigationContainmentVolumes.end(); ++it) {
        long layer = it->first;
        if (it->second->IsContainingPoint(position))
            return layer;
    }
    return 0;
}