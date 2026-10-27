// std::remove_if<pair<CCharString, CSymbolInfo>*, CPred_HasGroupName>(first, last, pred) -- drops the symbols of one group
// Retail: Fable.exe 0x009E7380, 110 bytes, __fastcall (/Gr TU: first in ecx, last in edx, the by-value predicate on the
// stack, `ret 4`). STLport 4.6 remove_if: the inline find_if copies the predicate (CCharString copy ctor 0x0099EC30)
// and calls the out-of-line random-access __find_if 0x009E6A70 (pred copy + tag reference), destroys its copy
// (0x0099EAE0); if nothing matched it returns last, else remove_copy_if(++next, last, first, pred) (0x009E6990); the
// by-value predicate is destroyed on exit.
// Identity: the only caller 0x009E5360 (CSymbolMap::RemoveSymbolsOfGroupType region) passes SLONGMap's [begin, end)
// and a CCharString predicate; 0x009E6990 copies 16-byte pair<CCharString, CSymbolInfo> elements whose
// CSymbolInfo::GroupName (+4) differs from the predicate's name. Ego_r.pdb names this instantiation
// ??$remove_if@...@VCPred_HasGroupName@@@std@@ (with Dinkumware iterators; retail's STLport vector iterator is a raw
// pointer). CPred_HasGroupName (Ego_r.pdb): Name CCharString +0, ctor from CCharString, operator()(const pair&).
// Leaf: ??$remove_if@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@VCPred_HasGroupName@@@std@@
class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};

class CSymbolMap {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;                          // +0x00
        CCharString EntryName;                          // +0x04
        long        Val;                                // +0x08
    };
};

namespace std {
    template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
    struct input_iterator_tag { input_iterator_tag() { } };
    struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
    struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
}

class CPred_HasGroupName {
public:
    bool operator()(const std::pair<CCharString, CSymbolMap::CSymbolInfo>& entry);
    CCharString Name;                                   // +0x00
};

namespace std {
    // Same-TU stand-in body of the out-of-line random-access __find_if (0x009E6A70; STLport 4.6 _algobase.c, unrolled
    // by four): with the body visible VC7.1 sees that the tag reference is never read and passes the address of the
    // live predicate argument slot for it, as retail does (declaration-only gives the tag its own frame byte).
    // Not graded here (the Leaf selects remove_if).
#pragma auto_inline(off)
    template <class _RandomAccessIter, class _Predicate>
    __declspec(noinline) _RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                _Predicate __pred, const random_access_iterator_tag&)
    {
        int __trip_count = (int)(__last - __first) >> 2;
        for ( ; __trip_count > 0 ; --__trip_count) {
            if (__pred(*__first)) return __first;
            ++__first;
            if (__pred(*__first)) return __first;
            ++__first;
            if (__pred(*__first)) return __first;
            ++__first;
            if (__pred(*__first)) return __first;
            ++__first;
        }
        switch (__last - __first) {
        case 3:
            if (__pred(*__first)) return __first;
            ++__first;
        case 2:
            if (__pred(*__first)) return __first;
            ++__first;
        case 1:
            if (__pred(*__first)) return __first;
        case 0:
        default:
            return __last;
        }
    }
#pragma auto_inline(on)

    template <class _InputIter, class _OutputIter, class _Predicate>
    _OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result,
                                          _Predicate __pred);                                         // 0x009E6990

    template <class _Tp>
    inline _Tp* __fastcall find_if(_Tp* __first, _Tp* __last, CPred_HasGroupName __pred)
    {
        return __find_if(__first, __last, __pred, random_access_iterator_tag());
    }

    template <class _ForwardIter, class _Predicate>
    _ForwardIter __fastcall remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
    {
        __first = find_if(__first, __last, __pred);
        if (__first == __last)
            return __first;
        else {
            _ForwardIter __next = __first;
            return remove_copy_if(++__next, __last, __first, __pred);
        }
    }

    template pair<CCharString, CSymbolMap::CSymbolInfo>* __fastcall
    remove_if<pair<CCharString, CSymbolMap::CSymbolInfo>*, CPred_HasGroupName>(
        pair<CCharString, CSymbolMap::CSymbolInfo>*, pair<CCharString, CSymbolMap::CSymbolInfo>*, CPred_HasGroupName);
}