// CWideString::MakeOwnCopyOfData() -- copy-on-write for wide strings: when the shared block has more than one owner, clone its text into a fresh block, drop our reference to the old one and adopt the clone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B560, length 128 bytes.
// Label correction: the manifest's "CCharString::MakeOwnCopyOfData" is the narrow twin (landed 0x0099EAF0, same
// shape); this body copies 16-bit text into the 16-byte CWideStringData block and calls CWideString::UnassignString.
// Callees: operator new @ 0x00BFEA1A (0x10 bytes), basic_string<wchar_t>::_M_range_initialize(first, last,
// const forward_iterator_tag&) @ 0x0099C670, CWideString::UnassignString @ 0x0099B4D0 (landed).
// Layout: CWideStringData = STLport basic_string<wchar_t> {_M_start, _M_finish, _M_end_of_storage}, NoRefs +0xc.
// Shape: the landed narrow MakeOwnCopyOfData with the wide block constructor inlined (as CWideString::AllocStringData
// 0x0099B3C0: _String_base over the allocator proxy, the open-coded wcslen, the out-of-line range initialise); with no
// argument slot to reuse, the iterator tag temporary takes the high byte of the `push ecx` frame word
// (`lea ecx,[esp+0Fh]`); the text pointer is read before the allocation (edi) and the clone is published after
// UnassignString even when the allocation failed.
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
};
class CWideStringData { public:                      // 16 bytes
    CBasicStringW Data;                       // +0x00
    long          NoRefs;                     // +0x0c
    __forceinline CWideStringData(const unsigned short* text) : Data(text), NoRefs(1) { }
};

class CWideString {
public:
    CWideStringData* PStringData;             // +0x00
    void UnassignString();                    // 0x0099B4D0
    void MakeOwnCopyOfData();
};

void CWideString::MakeOwnCopyOfData()
{
    CWideStringData* data = PStringData;
    if (data != 0 && data->NoRefs > 1) {
        const unsigned short* text = data->Data._M_start;
        CWideStringData* copy = new CWideStringData(text);
        UnassignString();
        PStringData = copy;
    }
}