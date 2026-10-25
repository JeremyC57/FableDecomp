// CVectorMap<CCharString, CPersistInfo, CKeyPairCompareLess<CCharString, CPersistInfo> >::Sort() -- sort the quest-persistence script map by quest name and clear the Dirty flag
// Leaf: ?Sort@?$CVectorMap@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BBBC0, 25 bytes, thiscall, no arguments.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x004BBBC0 / 0x004BBBE0 CVectorMap::Sort /
//   SortIfDirty ... T is CPersistInfo") in the quest-persistence sort chain over pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>
//   (12 bytes; Ego_r.pdb CRuntimeQuestPersistence::PersistScripts / MasterDataPersistScripts are
//   CVectorMap<CCharString, CPersistInfo, CKeyPairCompareLess<CCharString, CPersistInfo> >). An unreferenced COMDAT (the two
//   "abs" hits 0x0079E9A5 / 0x007A0228 are the bytes `C0 BB 4B 00` inside `test eax,eax ; mov ebx,4Bh`). The catalog's
//   CGuiVarTransferBool::TransferToGui is propagated across byte-identical CVectorMap::Sort bodies (FINDINGS chunk 380).
// Layout: retail STLport vector {First, Last, EndOfStorage} at +0, the empty Compare at +0xC, bool Dirty at +0xD (as the
//   landed FableWin-named CVectorMap<long,long,CKeyPairCompareLess<long,long> >::Sort 0x005FA496, whose model this row
//   follows); the callee is the chain's std::sort 0x004BBB40 (__fastcall, the comparator byte by value).
// Shape: `xor eax,eax ; mov al,[esi+0Ch] ; push eax` is the by-value copy of the one-byte empty comparator.
class CCharString {
public:
    void* PStringData;                                      // +0x0 the shared string block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                    // Ego_r.pdb, 8 bytes
    public:
        CCharString QuestName;                              // +0x0
        CCharString PersistScript;                          // +0x4
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo CPersistInfo;
typedef std::pair<CCharString, CPersistInfo> CPersistEntry;
typedef CKeyPairCompareLess<CCharString, CPersistInfo> CPersistLess;

namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004BBB40
}

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                             // +0x00
    value_type* Last;                                              // +0x04
    value_type* EndOfStorage;                                      // +0x08
    C           Compare;                                           // +0x0C
    bool        Dirty;                                             // +0x0D
    void Sort();
    void SortIfDirty();
};

template <class K, class V, class C>
void CVectorMap<K, V, C>::Sort()
{
    std::sort(First, Last, Compare);
    Dirty = false;
}

template void CVectorMap<CCharString, CPersistInfo, CPersistLess>::Sort();
