// NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>::GetUpdateParamDesc(CParticleUpdateParamDesc<unsigned long,unsigned long>& desc, const unsigned long& key) const -- looks up the update-parameter descriptor registered for a (non-zero) key; copies it out and returns true, else clears desc.Callback and returns false
// Leaf: ?GetUpdateParamDesc@?$CParticleUpdateParameters@KK@NParticleEngine@@QBE_NAAV?$CParticleUpdateParamDesc@KK@2@ABK@Z
// Plan phase 5 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local same-TU LowerBound body removed (the header defines the same-TU noinline body).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B15B50, length 86 bytes, `ret 8`.
// Signature from Ego_r.pdb: ?GetUpdateParamDesc@?$CParticleUpdateParameters@KK@NParticleEngine@@QBE_NAAV?$CParticleUpdate
//   ParamDesc@KK@2@ABK@Z.
// Callees: CVectorMap<K, CParticleUpdateParamDesc<K,K>, CParticleUpdateCCompareParamDesc<K>>::LowerBound(const K&) const
//   @ 0x00AF2C50 (out of line: the STLport-style binary search over the 16-byte pairs, iterator = element pointer).
// Layout (Ego_r.pdb; retail vectors 12 bytes): CParticleUpdateParameters = vptr, VectorParams +4, UpdateParams +0x14
//   (CVectorMap: Data +0, Compare +0x0C, Dirty +0x0D); element = pair<K, CParticleUpdateParamDesc> (key, vptr, Callback,
//   Data).
// Shape: `if (key) { it = UpdateParams.Find(key); if (it != UpdateParams.end()) { desc = it->second; return true; } }
//   desc.Callback = 0; return false;` -- CVectorMap::Find inlined (`LowerBound`, then `it == end || key < it->first` ->
//   end, unsigned `jae`); the key is loaded once into ebx and reused after the LowerBound call, which VC7.1 only does when
//   the TU holds LowerBound's body (same-TU noinline stand-in that only reads memory); the descriptor's implicit operator=
//   copies Callback and Data (not the vptr).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#include "engine/C3DVector.h"

#include "engine/CArray.h"


#include "engine/CVectorMap.h"

#pragma auto_inline(off)
// Same-TU body of the binary search (retail instantiates LowerBound in this TU; out-of-line copy @ 0x00AF2C50 for the descriptor map): it
// only reads memory, which lets VC7.1 keep the caller's key in ebx across the call as retail does.
#pragma auto_inline(on)

namespace NParticleEngine {

template <class A, class B> class CParticleUpdateParamCallback;

template <class A, class B> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4
public:
    CParticleUpdateParamCallback<A, B>* Callback;           // +0x04
    unsigned long                       Data;               // +0x08
};

template <class A, class B> class CParticleUpdateParameters {
public:
    template <class K> struct CParticleUpdateCCompareVector {
        bool operator()(const K& key, const std::pair<K, C3DVector>& entry) const { return key < entry.first; }
    };
    template <class K> struct CParticleUpdateCCompareParamDesc {
        bool operator()(const K& key, const std::pair<K, CParticleUpdateParamDesc<A, B> >& entry) const { return key < entry.first; }
    };
    typedef CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> >                         CVectorParamMap;
    typedef CVectorMap<A, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<A> > CUpdateParamMap;

    virtual ~CParticleUpdateParameters();
    CVectorParamMap VectorParams;                           // +0x04
    CUpdateParamMap UpdateParams;                           // +0x14 (retail; PDB +0x18)

    bool GetVectorParam(C3DVector& vector, const A& key) const;
    bool GetUpdateParamDesc(CParticleUpdateParamDesc<A, B>& desc, const A& key) const;
};

}   // namespace NParticleEngine

namespace NParticleEngine {

template <class A, class B>
bool CParticleUpdateParameters<A, B>::GetUpdateParamDesc(CParticleUpdateParamDesc<A, B>& desc, const A& key) const
{
    if (key) {
        typename CUpdateParamMap::const_iterator it = UpdateParams.Find(key);
        if (it != UpdateParams.end()) {
            desc = it->second;
            return true;
        }
    }
    desc.Callback = 0;
    return false;
}

template bool CParticleUpdateParameters<unsigned long, unsigned long>::GetUpdateParamDesc(
    CParticleUpdateParamDesc<unsigned long, unsigned long>& desc, const unsigned long& key) const;

}   // namespace NParticleEngine