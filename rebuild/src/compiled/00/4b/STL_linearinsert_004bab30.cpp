// std::__linear_insert(pair<CCharString,CPersistInfo>* first, pair<CCharString,CPersistInfo>* last, pair<CCharString,CPersistInfo> val, CKeyPairCompareLess<CCharString,CPersistInfo> comp) -- one insertion-sort step of the quest-persistence script map: an entry whose quest name sorts before the first shifts the run up one slot (copy_backward) and lands at the front, otherwise the unguarded linear insert walks it down from `last`
// Instantiation: std::__linear_insert<pair<CCharString,CPersistInfo>*,pair<CCharString,CPersistInfo>,CKeyPairCompareLess<CCharString,CPersistInfo> >
// Leaf: ??$__linear_insert@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BAB30, 192 bytes, __fastcall (first ecx, last edx; the 12-byte by-value
//   entry and the 4-byte comparator slot on the stack), `ret 10h`; the callee destroys its by-value entry.
// Identity: called by the pair<CCharString, CPersistInfo> __insertion_sort @ 0x004BAFB0 (sort @ 0x004BBB40 <-
//   CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo, ..>::Sort @ 0x004BBBC0). The catalog
//   name `std::Set_Insert_OrReplace` is wrong.
// Callees: CBasicString<char>::operator< @ 0x00429950 (inside the inlined CCharString::operator<), the SGI
//   __copy_backward(first, last, result, const random_access_iterator_tag&, int*) @ 0x004B78D0 and
//   __unguarded_linear_insert @ 0x004B9480 (both out of line), CCharString::operator= @ 0x0099EFB0, the copy constructor
//   @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Shape: STLport 4.6 `__linear_insert` with copy_backward and `__copy_backward_ptrs(.., __false_type)` inlined: the
//   random_access_iterator_tag temporary is homed in the dead comparator slot (`lea eax,[esp+1Ch]`); STLport's tags are
//   empty structs WITH bases (non-POD, so `random_access_iterator_tag()` stores nothing), `__false_type` is a plain POD.
//   `*first = val` is three operator= calls, the else arm copy-constructs val into the argument area; val's implicit
//   destructor ends both arms. /Gr TU: the templates are `__fastcall`. The landed 0x00429950 body sits above under
//   `auto_inline(off)` as the extra rule asks.
#pragma auto_inline(off)
template <class T> class CBasicString {
public:
    T* m_pData;                                             // +0x0
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    // 0x00429950 (landed): the real body as a same-TU noinline function (STLport-over-CCharString rule).
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};
#pragma auto_inline(on)

class CCharString {
public:
    CBasicString<char>* PStringData;                        // +0x0 the shared string block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
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
struct __false_type { };

template <class _Tp1, class _Tp2> struct _OKToMemCpy {
    typedef __false_type _Type;
    static _Type _Answer() { return _Type(); }
};
template <class _Tp1, class _Tp2>
inline _OKToMemCpy<_Tp1, _Tp2> _IsOKToMemCpy(_Tp1*, _Tp2*) { return _OKToMemCpy<_Tp1, _Tp2>(); }

template <class _Tp> inline random_access_iterator_tag __iterator_category(const _Tp*) { return random_access_iterator_tag(); }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);       // 0x004B78D0

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, __iterator_category(__first), __distance_type(__first));
}

template <class _BI1, class _BI2>
inline _BI2 __fastcall copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, _IsOKToMemCpy(__first, __result)._Answer());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);  // 0x004B9480

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __fastcall __linear_insert(CPersistEntry*, CPersistEntry*, CPersistEntry, CPersistLess);
}