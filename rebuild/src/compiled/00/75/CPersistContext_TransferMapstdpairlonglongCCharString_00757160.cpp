// CPersistContext::TransferMap<std::pair<long, long>, CCharString>(const char* name, std::map<CCharString, std::pair<long, long> >& map) -- transfer the map with a value-initialised pair<long, long> default element
// Leaf: ??$TransferMap@U?$pair@JJ@std@@VCCharString@@@CPersistContext@@QAEXPBDAAV?$map@VCCharString@@U?$pair@JJ@std@@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00757160, length 37 bytes, thiscall `ret 8` (unreferenced in Fable.exe).
// Identity (sweep3): the only callee is the 3-argument TransferMap 0x00756D00, which CTCHeroRememberCrimes::OnSerialise
//   0x007556B0 calls on RegionNamesToCrimeDurationAndFine (Ego_r: map<CCharString, pair<long, long> > at +0x0C) -- one
//   instantiation per address, and 0x00756D00 parses CCharString keys and two-word values. Ego_r/FableWin name sets hold
//   exactly this pair of overloads: ??$TransferMap@U?$pair@JJ@std@@VCCharString@@@CPersistContext@@QAEXPBDAAV?$map@...@std@@@Z
//   (this one, no default argument) and the same with ABU?$pair@JJ@2@ (0x00756D00). PDB TransferMap<V, K> takes map<K, V>.
//   The landed name/types (TransferMap over a CObjectAugmentationParticleSetMap, from the propagated 0x00756D00 label
//   TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType>) were stand-ins.
// Callees: CPersistContext::TransferMap<std::pair<long, long>, CCharString>(const char*, map&, const pair<long, long>&) @ 0x00756D00.
// Shape: STLport 4.6 `pair() : first(_T1()), second(_T2())` -- the 8-byte frame temporary zeroed through ONE register
//   (`xor eax,eax` stored twice) before the pushes, its address pushed with the map and name slots; `this` stays in ecx;
//   `add esp,8 ; ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
class CCharString;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map;
}
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CMemoryDataInputStream;
class CDataOutputStream;

// CPersistContext (0x2c bytes, PDB layout Ego_r.pdb; no engine header, HAND_OWNED in engine/INDEX.tsv)
class CPersistContext {
public:
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18 EMode: 0 text load, 1 text save, 2 binary load, 3 binary save
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28

    template <class _Tp, class _Key> void TransferMap(const char* name, std::map<_Key, _Tp>& map, const _Tp& defaultValue);   // 0x00756D00 for <pair<long, long>, CCharString>
    template <class _Tp, class _Key> void TransferMap(const char* name, std::map<_Key, _Tp>& map)
    {
        TransferMap(name, map, _Tp());
    }
};

template void CPersistContext::TransferMap<std::pair<long, long>, CCharString>(const char*, std::map<CCharString, std::pair<long, long> >&);
