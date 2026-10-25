// CWideStringData::CWideStringData(const unsigned short* text, long length) -- the shared wide-string block: an STLport basic_string holding length characters of text, owned once
// Leaf: ??0CWideStringData@@QAE@PBGJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099CC40, 51 bytes, thiscall, `ret 8`.
// Identity: FableWin.pdb ??0CWideStringData@@QAE@PB_WJ@Z (retail VC7.1 spells wchar_t `unsigned short`, mangled G);
//   the landed CWideString::AllocStringData 0x0099B3C0 inlines it into its (text, length) arm (same zero stores, range
//   call and `NoRefs = 1`); this out-of-line COMDAT has no direct caller (xref: 0 calls / 0 references).
// Callees: basic_string<unsigned short>::_M_range_initialize(const unsigned short*, const unsigned short*,
//   const forward_iterator_tag&) 0x0099C670 (landed; `ret 0xC`).
// Layout: CWideStringData (16 bytes) = Data (STLport basic_string<unsigned short>, 12 bytes) +0, NoRefs +0xC
//   (Ego_r.pdb's 32-byte block carries the debug basic_string).
// Shape: `CWideStringData(const wchar_t* text, long length) : Data(text, length), NoRefs(1) { }` -- the inline STLport
//   (pointer, count) constructor (0x0099CA40's body: three zero stores, the empty iterator tag homed in the `text`
//   argument slot, text reloaded after the push, end = text + length*2), then the owner count after the call.
// Label correction: catalogued as `vector<bool>::vector<bool>` (propagated); the body is CWideStringData's
//   (text, length) constructor.
namespace _STL {

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _CharT> class char_traits { };

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
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
    _String_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;

    basic_string(const _CharT* __s, size_type __n, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + __n);
    }

    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);   // 0x0099C670
    void _M_range_initialize(const _CharT* __f, const _CharT* __l)
    {
        _M_range_initialize(__f, __l, random_access_iterator_tag());
    }
};

}

class CWideStringData {
public:
    _STL::basic_string<unsigned short> Data;                        // +0x00
    long                               NoRefs;                      // +0x0C

    CWideStringData(const unsigned short* text, long length);
};

CWideStringData::CWideStringData(const unsigned short* text, long length) : Data(text, length), NoRefs(1)
{
}