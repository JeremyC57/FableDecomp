// CVectorMap<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance, CKeyPairCompareLess<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> >::LowerBound(const NParticleEngine::EPSCType& key) -- binary search of the sorted 24-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@W4EPSCType@NParticleEngine@@VCComponentInstance@2@V?$CKeyPairCompareLess@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@@@@QAEPAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@ABW4EPSCType@NParticleEngine@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00af60c0, 83 bytes, thiscall `ret 4`; the STLport iterator is a plain entry pointer in eax.
// Identity (proven): NParticleEngine::CParticleSystem::Components (+0xC) is CVectorMap<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance,...> (Ego_r.pdb; 24-byte pair); callers CParticleSystem::GetComponentOfType 0x00AF4360 (Ego_r.pdb QAE, non-const) and GetParticleStats 0x00AF43B0 -- the non-const overload.
// Body: count = (last - first) / 24; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } -- signed key compare (`jge`), the key read once before the loop, `count -= half + 1`
//   as `or -1 ; sub ; add`.
// Shape (family L069): the landed CVectorMap<unsigned long, CSmallVector<...> >::LowerBound 0x006FAF80 with a signed key.
namespace NParticleEngine {
enum EPSCType { EPSCType_FIRST };
class CComponentInstance {                          // Ego_r.pdb class, 20 bytes (+0 unnamed, Component +4, Enabled +0xC, InstanceName +0x10)
public:
    unsigned char _opaque[20];
};
}
#include "engine/CArray.h"


template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};

template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C (empty comparator)
    bool               Dirty;                                 // +0x0D
    iterator LowerBound(const K& key);
};

template <class K, class V, class TCompare>
typename CVectorMap<K, V, TCompare>::iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key)
{
    iterator first = Data.begin();
    int count = Data.end() - Data.begin();
    while (count > 0)
    {
        int half = count / 2;
        iterator middle = first + half;
        if (Compare(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

template class CVectorMap<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance, CKeyPairCompareLess<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance > >;