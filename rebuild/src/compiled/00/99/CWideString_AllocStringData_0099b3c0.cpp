// CWideString::AllocStringData(const wchar_t* text, long length) -- allocate a shared 16-byte wide-string block (an STLport basic_string<wchar_t> plus the owner count) for `text`: the whole NUL-terminated text when length is -1, else `length` characters; the new block has one owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B3C0, length 121 bytes (ret 8).
// Label correction: the manifest's "CCharString::AllocStringData" is the narrow twin (0x0099EA60); this body works on
// 16-bit characters and the 16-byte CWideStringData block, so it is CWideString's (the landed CWideString literal
// constructor 0x0099B6B0 calls AllocStringData(text, -1)).
// Callees: operator new @ 0x00BFEA1A (0x10 bytes), basic_string<wchar_t>::_M_range_initialize(first, last,
// const forward_iterator_tag&) @ 0x0099C670 (`ret 0Ch`: allocates last-first+1 characters, copies, terminates).
// Layout: CWideStringData = STLport basic_string<wchar_t> {_M_start +0, _M_finish +4, _M_end_of_storage +8}, NoRefs
// +0xc (Ego_r.pdb's 32-byte block is the debug basic_string).
// Shape: one operator new hoisted above the length test (each arm keeps its own null test returning 0); both arms
// inline the STLport constructors -- _String_base(const allocator&) with _M_start(0), _M_finish(0) and the
// end-of-storage pointer in an _STLP_alloc_proxy over the default allocator temporary (the plain three-pointer model
// schedules the tag's `lea` above the zero stores), then `_M_range_initialize(s, s + n)`, whose const-ref iterator
// tag temporary is homed in the dead `text` argument slot (`lea edx,[esp+0Ch]`); traits::length is the open-coded
// word scan. The two arms tail-merge into one call and the `NoRefs = 1` store; the `?:` form gives retail's block
// order (the if/return form places the (text, length) arm last and jumps back).
struct forward_iterator_tag {
    forward_iterator_tag() { }
};
struct allocator_w {                          // the empty STLport allocator<wchar_t>
    allocator_w() { }
};
struct _STLP_alloc_proxy_w {                  // end-of-storage pointer over the empty allocator
    unsigned short* _M_data;
    _STLP_alloc_proxy_w(const allocator_w&, unsigned short* p) : _M_data(p) { }
};
struct _String_base_w {
    unsigned short*     _M_start;             // +0x00
    unsigned short*     _M_finish;            // +0x04
    _STLP_alloc_proxy_w _M_end_of_storage;    // +0x08
    _String_base_w(const allocator_w& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
struct CBasicStringW : _String_base_w {       // STLport basic_string<wchar_t>, 12 bytes
    static unsigned long length(const unsigned short* s)
    {
        unsigned long i = 0;
        while (s[i] != 0)
            ++i;
        return i;
    }
    void _M_range_initialize(const unsigned short* first, const unsigned short* last,
                             const forward_iterator_tag& tag);   // 0x0099C670
    __forceinline CBasicStringW(const unsigned short* s, const allocator_w& a = allocator_w()) : _String_base_w(a)
    {
        _M_range_initialize(s, s + length(s), forward_iterator_tag());
    }
    __forceinline CBasicStringW(const unsigned short* s, unsigned long n, const allocator_w& a = allocator_w()) : _String_base_w(a)
    {
        _M_range_initialize(s, s + n, forward_iterator_tag());
    }
};
class CWideStringData { public:                      // 16 bytes
    CBasicStringW Data;                       // +0x00
    long          NoRefs;                     // +0x0c
    __forceinline CWideStringData(const unsigned short* text) : Data(text), NoRefs(1) { }
    __forceinline CWideStringData(const unsigned short* text, unsigned long n) : Data(text, n), NoRefs(1) { }
};

class CWideString {
public:
    CWideStringData* PStringData;             // +0x00
    CWideStringData* AllocStringData(const unsigned short* text, long length);
};

CWideStringData* CWideString::AllocStringData(const unsigned short* text, long length)
{
    return length == -1 ? new CWideStringData(text) : new CWideStringData(text, length);
}