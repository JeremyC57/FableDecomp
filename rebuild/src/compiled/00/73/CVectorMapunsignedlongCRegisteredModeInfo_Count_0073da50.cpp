// CVectorMap<unsigned long,CRegisteredModeInfo>::Count(const unsigned long& key) const -- (the CTCCreatureModeManager::CRegisteredModeInfo map, CKeyPairCompareLess) number of registered entries with the key: UpperBound(key) - LowerBound(key)
// Leaf: ?Count@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073DA50, 38 bytes (+10 int3), __fastcall `ret 4`.
// Callees: UpperBound(key) @ 0x0073D250 (landed), LowerBound(key) @ 0x0073D210 (both const members returning the entry
//   pointer in eax).
// Label: the manifest's "VectorMap_GetDistance" is descriptive. Ego_r.pdb names this exact member:
//   `?Count@?$CVectorMap@KVCRegisteredModeInfo@CTCCreatureModeManager@@V?$CKeyPairCompareLess@...@@@@QBEJABK@Z`
//   (long Count(const unsigned long&) const); the landed UpperBound 0x0073D250 is the same instance.
// Shape: `return UpperBound(key) - LowerBound(key);` -- VC7.1 calls the left operand first (UpperBound, kept in ebx),
//   then LowerBound, and divides the byte difference by the 8-byte entry with `sar 3`.
// Layout: STLport vector {First, Last, EndOfStorage} at +0, the empty Compare at +0x0C, Dirty at +0x0D (Ego_r.pdb gives
//   the Dinkumware 16-byte vector, so its Compare/Dirty sit at +0x10/+0x11 there).
namespace _STL {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
class CModeBase; class CThing;
class CTCCreatureModeManager {
public:
    struct CRegisteredModeInfo {
        CModeBase* (__cdecl* AllocFunc)(CThing*);
    };
};
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const _STL::pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    C Compare;                                           // +0x0c
    bool Dirty;                                          // +0x0d
    const_iterator LowerBound(const K& key) const;       // 0x0073D210
    const_iterator UpperBound(const K& key) const;       // 0x0073D250
    long Count(const K& key) const;
};
template <class K, class V, class C>
long CVectorMap<K, V, C>::Count(const K& key) const
{
    return UpperBound(key) - LowerBound(key);
}
template class CVectorMap<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo, CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> >;