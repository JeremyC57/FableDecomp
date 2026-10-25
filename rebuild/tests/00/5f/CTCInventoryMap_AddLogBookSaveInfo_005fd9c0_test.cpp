// Behaviour test for 0x005FD9C0: CTCInventoryMap::AddLogBookSaveInfo(const CWideString& name, const CWideString& abbreviatedName, const CWideString& text, CHeroLogBook::CHeroLogBookEntry::ECategory category) -- append one log-book entry to the four parallel save-info vectors (name, abbreviated name, text, category)
// A copy of the source model and logic (compiled separately from src/005fd9c0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);                                           // 0x0099B720
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { ECategory_Force32 = 0x7fffffff };
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};

}

class CTCInventoryMap {
public:
    unsigned char _pad_0x000[0x150];
    std::vector<CWideString>                               LogBookNameTag;            // +0x150
    std::vector<CWideString>                               LogBookAbbreviatedNameTag; // +0x15C
    std::vector<CWideString>                               LogBookTextTag;            // +0x168
    std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory> LogBookCategory;          // +0x174
    void AddLogBookSaveInfo(const CWideString& name, const CWideString& abbreviatedName, const CWideString& text,
                            CHeroLogBook::CHeroLogBookEntry::ECategory category);
};

void CTCInventoryMap::AddLogBookSaveInfo(const CWideString& name, const CWideString& abbreviatedName,
                                         const CWideString& text, CHeroLogBook::CHeroLogBookEntry::ECategory category)
{
    LogBookNameTag.push_back(name);
    LogBookAbbreviatedNameTag.push_back(abbreviatedName);
    LogBookTextTag.push_back(text);
    LogBookCategory.push_back(category);
}

// ---- test stand-ins ----
static int g_stringCopies = 0;
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_stringCopies; }

namespace std {
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool)
{                                                         // STLport growth: old + max(old, n), copy head, n copies, copy tail
    size_type __old_size = (size_type)(this->_M_finish - this->_M_start);
    size_type __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    pointer __new_start = (pointer)std::malloc(__len * sizeof(_Tp));
    pointer __new_finish = __new_start;
    for (pointer p = this->_M_start; p != __position; ++p, ++__new_finish) _Construct(__new_finish, *p);
    for (size_type k = 0; k < __fill_len; ++k, ++__new_finish) _Construct(__new_finish, __x);
    for (pointer p = __position; p != this->_M_finish; ++p, ++__new_finish) _Construct(__new_finish, *p);
    std::free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
}

template <class T> struct VecView : public std::vector<T> {
    void Clear() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    void Free() { std::free(this->_M_start); Clear(); }
    unsigned int Size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int Capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    T& At(unsigned int i) { return this->_M_start[i]; }
};
template <class T> VecView<T>& View(std::vector<T>& v) { return static_cast<VecView<T>&>(v); }

static int Fail() { std::printf("005fd9c0_TEST FAIL\n"); return 1; }
typedef CHeroLogBook::CHeroLogBookEntry::ECategory ECategory;

int main()
{
    static CTCInventoryMap map;
    View(map.LogBookNameTag).Clear(); View(map.LogBookAbbreviatedNameTag).Clear();
    View(map.LogBookTextTag).Clear(); View(map.LogBookCategory).Clear();
    CWideStringData* tags[6];
    for (int i = 0; i < 6; ++i) tags[i] = (CWideStringData*)(&map._pad_0x000[i * 8]);
    for (int k = 0; k < 5; ++k) {
        CWideString name(*(CWideString*)&tags[k]);
        CWideString abbreviated(*(CWideString*)&tags[(k + 1) % 6]);
        CWideString text(*(CWideString*)&tags[(k + 2) % 6]);
        map.AddLogBookSaveInfo(name, abbreviated, text, (ECategory)(k * 3));
    }
    if (View(map.LogBookNameTag).Size() != 5 || View(map.LogBookAbbreviatedNameTag).Size() != 5) return Fail();
    if (View(map.LogBookTextTag).Size() != 5 || View(map.LogBookCategory).Size() != 5) return Fail();
    if (View(map.LogBookCategory).Capacity() < 5) return Fail();
    for (int k = 0; k < 5; ++k) {
        if (View(map.LogBookNameTag).At(k).PStringData != tags[k]) return Fail();
        if (View(map.LogBookAbbreviatedNameTag).At(k).PStringData != tags[(k + 1) % 6]) return Fail();
        if (View(map.LogBookTextTag).At(k).PStringData != tags[(k + 2) % 6]) return Fail();
        if (View(map.LogBookCategory).At(k) != (ECategory)(k * 3)) return Fail();
    }
    if (g_stringCopies < 30) return Fail();                  // 15 locals + 15 pushes (+ regrowth copies)
    View(map.LogBookNameTag).Free(); View(map.LogBookAbbreviatedNameTag).Free();
    View(map.LogBookTextTag).Free(); View(map.LogBookCategory).Free();
    std::printf("005fd9c0_TEST PASS\n");
    return 0;
}