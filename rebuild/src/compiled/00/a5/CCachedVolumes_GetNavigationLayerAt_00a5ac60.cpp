// CCachedVolumes::GetNavigationLayerAt(const C3DVector& position) const -- the navigation layer (map key) of the first cached containment volume holding the position, 0 when none does
// Leaf: ?GetNavigationLayerAt@CCachedVolumes@@QBEJABVC3DVector@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
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
#include "engine/CVectorMap.h"

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