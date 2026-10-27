// Behaviour test for 0x009E6990 std::remove_copy_if (pair<CCharString, CSymbolInfo>*, CPred_HasGroupName): the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
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

static int g_copies = 0, g_dtors = 0, g_assigns = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
static CCharStringData* D(const char* text)
{
    CCharStringData* d = new CCharStringData; d->String.PData = (char*)text;
    d->String.StringLength = (unsigned long)strlen(text); d->String.DataLength = d->String.StringLength + 1; return d;
}
typedef tstd::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
int main()
{
    CCharStringData* quest = D("Quest"); CCharStringData* quest2 = D("Quest"); CCharStringData* other = D("Other");
    CCharStringData* quesx = D("Quesx");
    Entry* e = (Entry*)malloc(5 * sizeof(Entry)); memset(e, 0, 5 * sizeof(Entry));
    e[0].second.GroupName.PStringData = quest; e[0].second.Val = 0;
    e[1].second.GroupName.PStringData = other; e[1].second.Val = 1;
    e[2].second.GroupName.PStringData = quest2; e[2].second.Val = 2;      // equal text, different data block
    e[3].second.GroupName.PStringData = quesx; e[3].second.Val = 3;       // same length, different text
    e[4].second.GroupName.PStringData = 0; e[4].second.Val = 4;           // null name
    Entry* out = (Entry*)malloc(5 * sizeof(Entry)); memset(out, 0, 5 * sizeof(Entry));
    CPred_HasGroupName* pred = (CPred_HasGroupName*)malloc(sizeof(CPred_HasGroupName)); pred->Name.PStringData = quest;
    Entry* end = tstd::remove_copy_if(e, e + 5, out, *pred);
    int ok = end == out + 3 && out[0].second.Val == 1 && out[1].second.Val == 3 && out[2].second.Val == 4;
    if (ok) printf("009e6990_TEST PASS\n");
    return ok ? 0 : 1;
}