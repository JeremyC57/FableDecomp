// _global::ReplaceDoubleApostrophes(const basic_string&) -- returns a copy of the string with every "''" collapsed to "'"
// Leaf: ?ReplaceDoubleApostrophes@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055ECB0, 353 bytes (0x161 + int3 padding), __fastcall: ecx = the hidden
//   basic_string result, edx = the input string; returns the result pointer.
// Identity: Ego_r.pdb ?ReplaceDoubleApostrophes@?A0xf010c0d0@@YI?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV23@@Z
//   -- an anonymous-namespace free function of the LHNetworkLib crypto TU (with EncipherMorsel .. Explode).
// Callees: CCharString(const char*, long) 0x0099EBF0, CCharString() 0x0099E4B0, ~CCharString 0x0099EAE0,
//   CCharString::operator+=(const CCharString&) 0x0099F0A0, CCharString::operator=(const CCharString&) 0x0099EFB0,
//   strstr 0x00BFEB3A, basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&)
//   0x005672D0; empty C string 0x0129AAF4, "'" 0x0124C630, "''" 0x0124C62C.
// Shape: the text CCharString outlives an inner block holding the replacement ("'"), the search ("''") and the
//   accumulated result: the block's destructors run result, search, replacement before the return string is built
//   from text.c_str() (STLport's inline const char* constructor: three zeroed pointers, the strlen intrinsic, then the
//   out-of-line range initialiser with the forward_iterator_tag temporary). The loop cuts the text at each match,
//   appends the head and the replacement, and restarts strstr past the match (strlen of the search re-run each time).
#include <string.h>
#pragma intrinsic(strlen)

extern "C" char* __cdecl strstr(const char* text, const char* pattern);   // 0x00BFEB3A

struct CCharStringData { char* m_pString; };
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);                       // 0x0099EBF0
    ~CCharString();                                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                      // 0x0099EFB0
    CCharString& operator+=(const CCharString& other);                     // 0x0099F0A0
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->m_pString; }
};

namespace std {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _CharT> class char_traits {
public:
    static unsigned int length(const char* __s) { return strlen(__s); }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                         // +0x00
    _Tp* _M_finish;                                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                // +0x08
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _Traits traits_type;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;
    basic_string(const _CharT* __s, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + traits_type::length(__s), forward_iterator_tag());
    }
    const _CharT* c_str() const { return this->_M_start; }
    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);   // 0x005672D0
};

}

namespace {
std::basic_string<char> __fastcall ReplaceDoubleApostrophes(const std::basic_string<char>& in)
{
    CCharString text(in.c_str());
    {
        CCharString replacement("'");
        CCharString search("''");
        CCharString result;
        char* head = (char*)text.c_str();
        const char* pattern = search.c_str();
        char* found = strstr(head, pattern);
        while (found) {
            *found = 0;
            found += strlen(pattern);
            result += CCharString(head);
            result += replacement;
            head = found;
            found = strstr(found, pattern);
        }
        result += CCharString(head);
        text = result;
    }
    return std::basic_string<char>(text.c_str());
}
}

std::basic_string<char> (__fastcall *g_keepReplaceDoubleApostrophes)(const std::basic_string<char>&) =
    &ReplaceDoubleApostrophes;   // keeps the internal-linkage body emitted; not part of the retail function