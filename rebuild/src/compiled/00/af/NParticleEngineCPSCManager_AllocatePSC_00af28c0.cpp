// NParticleEngine::CPSCManager::AllocatePSC(const CCharString& name) -- look the component class name up in the registered-PSC vector map and return a fresh component from its registered allocator, or null for an unknown name
// Leaf: ?AllocatePSC@CPSCManager@NParticleEngine@@QAEPAVCPSCBase@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF28C0, 73 bytes (+7 int3 padding), __fastcall `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb name `?AllocatePSC@CPSCManager@NParticleEngine@@QAEPAVCPSCBase@2@ABVCCharString@@@Z`
//   (Ego_r 0x00AB5BB0, right before RegisterPSC); the body calls the registered AllocFunc (+4 of the 16-byte entry).
//   Catalog "CVectorMap_FindEntry" is descriptive.
// Callees: CVectorMap<CCharString, CRegisteredPSCInfo, CKeyPairCompareLess<...> >::LowerBound @ 0x00AF3000 (landed),
//   CBasicString<char>::operator< @ 0x00429950 (behind the inline CCharString::operator<), the entry's AllocFunc.
// Layout: CPSCManager is one CVectorMap (RegisteredPSCs, the landed ctor 0x00AF2970): STLport vector {first, last, end},
//   the empty Compare at +0xc, Dirty at +0xd; entry pair<CCharString, CRegisteredPSCInfo> (AllocFunc +4, ComponentType +8,
//   GUIName +0xc).
// Shape: the map's inline Find (`it = LowerBound(key); if (it == end() || Compare(key, *it)) return end();`) -- the end
//   pointer read once after the call and shared by both tests; the inline CCharString::operator< tests the same block,
//   a null left (the key), a null right, then calls the block compare. The block compare's landed body sits above as a
//   same-TU noinline function: VC7.1 then knows the call writes no memory and keeps the end pointer in esi for the
//   caller's `it != end` test (a declaration-only callee reloads [this+4]). The found path is the fall-through
//   (`if (it != end) return AllocFunc(); return 0;`).
namespace NParticleEngine { class CPSCBase; }

template <class T> class CBasicString {
public:
    T* m_pData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline function, so VC7.1 knows it writes no memory.
    __declspec(noinline) bool operator<(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return false;
            if (*a < *b)
                return true;
            if (*a > *b)
                return false;
            ++a;
            ++b;
        }
    }
#pragma auto_inline(on)
};
class CCharString {
public:
    CBasicString<char>* PData;
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

namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
struct CRegisteredPSCInfo {
    CPSCBase* (__cdecl* AllocFunc)();                    // +0x0
    EPSCType ComponentType;                              // +0x4
    CCharString GUIName;                                 // +0x8
};
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    C Compare;                                           // +0x0c
    bool Dirty;                                          // +0x0d
    value_type* LowerBound(const K& key);                // 0x00AF3000
    value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        if (it == Last || Compare(key, *it))
            return Last;
        return it;
    }
};

namespace NParticleEngine {
class CPSCManager {
public:
    CVectorMap<CCharString, CRegisteredPSCInfo, CKeyPairCompareLess<CCharString, CRegisteredPSCInfo> > RegisteredPSCs;   // +0x00
    CPSCBase* AllocatePSC(const CCharString& name);
};

CPSCBase* CPSCManager::AllocatePSC(const CCharString& name)
{
    std::pair<CCharString, CRegisteredPSCInfo>* it = RegisteredPSCs.Find(name);
    if (it != RegisteredPSCs.Last)
        return it->second.AllocFunc();
    return 0;
}
}