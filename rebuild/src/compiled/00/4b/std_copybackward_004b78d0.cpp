// std::__copy_backward(pair<CCharString,CPersistInfo>* first, pair<CCharString,CPersistInfo>* last, pair<CCharString,CPersistInfo>* result, const random_access_iterator_tag&, int*) -- STLport random-access copy_backward over the quest-persistence entries: counted backward member-wise assignment, returns the new result start
// Instantiation: std::__copy_backward<pair<CCharString,CPersistInfo>*,pair<CCharString,CPersistInfo>*,int>
// Leaf: ??$__copy_backward@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B78D0, 90 bytes, /Gr __fastcall (first ecx, last edx; result, the tag
//   reference and the distance-type pointer on the stack), `ret 0Ch`.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x004B78D0 SGI __copy_backward (catalogued
//   _Move_backward)"); its caller is the landed quest-persistence std::__linear_insert 0x004BAB30, whose reloc binding
//   already names this Leaf -> 004b78d0; the quest-persistence sort chain over pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>
//   (12 bytes; Ego_r.pdb CRuntimeQuestPersistence::PersistScripts / MasterDataPersistScripts are
//   CVectorMap<CCharString, CPersistInfo, CKeyPairCompareLess<CCharString, CPersistInfo> >). The landed placeholder name
//   CopyBackward_PairCharStringQuestPersistInfo_004b78d0 had the right body.
// Callees: CCharString::operator= @ 0x0099EFB0 (three per entry: the pair's implicit assignment).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last;` (count in ebx).
class CCharString {
public:
    void* PStringData;                                      // +0x0 the shared string block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template CPersistEntry* __fastcall __copy_backward(CPersistEntry*, CPersistEntry*, CPersistEntry*, const random_access_iterator_tag&, int*);
}
