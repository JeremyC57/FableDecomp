// std::remove_copy_if(first, last, result, pred) -- (pair<CCharString, CSymbolMap::CSymbolInfo>*, pair<CCharString, CSymbolMap::CSymbolInfo>*, CPred_HasGroupName) copy every symbol whose group name differs from the predicate's to result, return the end of the copied run
// Leaf: ??$remove_copy_if@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@PAU12@VCPred_HasGroupName@@@std@@YIPAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@PAU10@00VCPred_HasGroupName@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6990, 161 bytes, /Gr __fastcall (first ecx, last edx; result and the by-value
//   predicate on the stack), `ret 8`; the callee destroys its predicate copy (~CCharString 0x0099EAE0) on exit.
// Identity (proven): the landed std::remove_if<pair<CCharString, CSymbolMap::CSymbolInfo>*, CPred_HasGroupName> 0x009E7380
//   tail-calls this address, bound as this exact symbol (STLport 4.6 remove_if -> remove_copy_if(++next, last, first, pred)).
// Body: STLport 4.6 `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return result;`
//   with CPred_HasGroupName::operator() (Ego_r.pdb: `entry.second.GroupName == Name`) and CCharString::operator== inlined
//   down to CBasicString<char>::operator== (same block -> equal; a null block -> unequal; lengths, then the protected
//   static __fastcall CBasicString<char>::Compare 0x00411570 == 0). The pair's implicit operator= runs
//   CCharString::operator= 0x0099EFB0 three times (first, GroupName, EntryName) and copies Val.
// Layout: pair<CCharString, CSymbolMap::CSymbolInfo> 16 bytes (string +0; GroupName +4, EntryName +8, Val +0xC);
//   CCharStringData begins with its CBasicString<char> (PData +0, StringLength +4).
template<class T> class CBasicString {
public:
    T*            PData;                            // +0x00
    unsigned long StringLength;                     // +0x04
    unsigned long DataLength;                       // +0x08
    bool operator==(const CBasicString& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) == 0;
    }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570
};
#pragma auto_inline(off)
template<class T> long __fastcall CBasicString<T>::Compare(const T* a, const T* b)
{                                                   // same-TU body (landed 0x00411570): VC7.1 sees it writes no memory
    for (;;) {
        T ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        T cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharStringData {
public:
    CBasicString<char> String;                      // +0x00
};

class CCharString {
public:
    CCharStringData* PStringData;                   // +0x00
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return PStringData->String == other.PStringData->String;
    }
};

class CSymbolMap {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;                      // +0x00
        CCharString EntryName;                      // +0x04
        long        Val;                            // +0x08
    };
};

namespace std {
    template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}

class CPred_HasGroupName {
public:
    bool operator()(const std::pair<CCharString, CSymbolMap::CSymbolInfo>& entry)
    {
        return entry.second.GroupName == Name;
    }
    CCharString Name;                               // +0x00
};

namespace std {
    template <class _InputIter, class _OutputIter, class _Predicate>
    _OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result,
                                          _Predicate __pred)
    {
        for ( ; __first != __last; ++__first)
            if (!__pred(*__first)) {
                *__result = *__first;
                ++__result;
            }
        return __result;
    }

    typedef pair<CCharString, CSymbolMap::CSymbolInfo> _Entry;
    template _Entry* __fastcall remove_copy_if<_Entry*, _Entry*, CPred_HasGroupName>(_Entry*, _Entry*, _Entry*, CPred_HasGroupName);
}