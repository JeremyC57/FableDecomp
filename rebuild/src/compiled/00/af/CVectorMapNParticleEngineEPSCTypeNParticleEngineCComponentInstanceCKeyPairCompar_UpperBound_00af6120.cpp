// CVectorMap<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance,CKeyPairCompareLess<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance> >::UpperBound(const EPSCType& key) -- binary search of the particle system's sorted (component type, component instance) entries for the first entry whose type is above the key
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF6120, 83 bytes, __fastcall `ret 4`, the entry pointer (STLport iterator) in eax.
// Identity: called with `lea ecx,[this+0Ch]` by CParticleSystem::GetComponentOfType 0x00AF4360 (right after the
//   LowerBound 0x00AF60C0 on the same map), CParticleSystem::GetParticleStats 0x00AF43B0 and AddComponent 0x00AF4530;
//   Ego_r.pdb CParticleSystem::Components (+0xC) is CVectorMap<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance,
//   CKeyPairCompareLess<...> >, whose 24-byte pair<EPSCType, CComponentInstance> is the stride the body divides by
//   (0x2AAAAAAB, sar 2); FableWin.pdb has ?UpperBound@?$CVectorMap@W4EPSCType@NParticleEngine... Renamed from the
//   stand-in _unattributed::UpperBound_CVectorMap_00af6120.
// Body: Lionhead's CVectorMap bound, as the landed CVectorMap<EBodyLocation,...>::UpperBound 0x00758570: count =
//   (last - first) / sizeof(entry); while (count > 0) { half = count / 2 (cdq ; sub ; sar -- a signed division, not the
//   SGI/STLport `__len >> 1`); step past the middle unless key < middle key (`count -= half + 1`, `or -1 ; sub ; add`),
//   else keep the lower half }. The key reference is read once before the loop.
// Layout: STLport vector {first, last, end_of_storage}; entry pair<key, value>, the 4-byte enum key at +0.
namespace NParticleEngine {

enum EPSCType { PSC_TYPE_FIRST = 0 };

class CComponentInstance {                         // 0x14 (Ego_r.pdb), vtable 0x0129F478
public:
    virtual ~CComponentInstance();
    unsigned char _body[0x14 - 4];                 // Component CCountedPointer<CPSCBase> +4, Enabled +0xc, InstanceName +0x10
};

}

namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    value_type* UpperBound(const K& key);
};

template <class K, class V, class C>
typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::UpperBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0)
    {
        int half = count / 2;
        value_type* middle = first + half;
        if (!comp(key, *middle))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

template class CVectorMap<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance, CKeyPairCompareLess<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> >;
