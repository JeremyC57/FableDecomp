// GFSerialiseVectorBinaryOut(CDataOutputStream& out, std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory>& v) -- write the element count, then every element as a long
// Leaf: ??$GFSerialiseVectorBinaryOut@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@@YIXAAVCDataOutputStream@@AAV?$vector@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$allocator@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00614750, 55 bytes, `__fastcall` (stream in ecx, vector in edx), plain `ret`.
// Identity (proven): its only caller is the landed CPersistContext::TransferVector<CHeroLogBook::CHeroLogBookEntry::ECategory> 0x00619890 (the binary-out arm; agent954 K077 named the binary-in arm 0x00619620 the same way).
// Callees: CDataOutputStream::WriteSLONG(long) 0x00993EB0 (count, then each element).
// Shape (family M013, representative 0x0040FD40): `long count = v.size(); out.WriteSLONG(count); for (long i = 0; i < count;
//   ++i) out.WriteSLONG(v[i]);` -- STLport operator[] as `*(begin() + n)` re-reads _M_start every pass (`mov eax,[ebx]`);
//   the enum traits' TransferBinaryOut is the inline WriteSLONG of the value.
class CDataOutputStream {
public:
    void WriteSLONG(long value);                          // 0x00993EB0
};
class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { CATEGORY_NONE = 0 };
    };
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* begin() { return _M_start; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
};
}
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& out, std::vector<T>& v)
{
    long count = v.size();
    out.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        out.WriteSLONG(v[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CHeroLogBook::CHeroLogBookEntry::ECategory>(CDataOutputStream& out, std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory>& v);