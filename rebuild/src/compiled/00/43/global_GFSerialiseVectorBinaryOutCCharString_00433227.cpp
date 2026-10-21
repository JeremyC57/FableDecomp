// GFSerialiseVectorBinaryOut<CCharString>(CDataOutputStream& stream, std::vector<CCharString>& v) -- write a string vector to a binary save stream: count, then each string null-terminated
// Leaf: ??$GFSerialiseVectorBinaryOut@VCCharString@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x00433227, 76 bytes, __fastcall (stream ecx, vector edx), plain `ret`; callers 0x00433214
// (CPersistContext::TransferVectorBinary<CCharString> 0x004331F9) and 0x0049B90A; Ego_r.pdb
// ??$GFSerialiseVectorBinaryOut@VCCharString@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z.
// Body: a CPersistTraits<CCharString> local (its CCharString value default-built 0x0099E4B0, destroyed 0x0099EAE0 at the
// end); stream.WriteLong(size()) (0x00993EB0); then per element the traits' TransferBinaryOut, i.e.
// stream.WriteNullTerminatedString(v[i]) (0x00995AE0), with a `long` index against the saved count.
// Layout: STLport vector (12 bytes), CCharString one pointer.
// Shape: size TU with frame pointers (`push ebp / mov ebp,esp`, `leave`, unaligned entry; the stream spilled to [ebp-4]).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
};
class CDataOutputStream {
public:
    void WriteLong(long value);                          // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& s);   // 0x00995AE0
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
template <class T> class CPersistTraits;
template <> class CPersistTraits<CCharString> {
public:
    CCharString DefaultValue;
    void TransferBinaryOut(CDataOutputStream& stream, const CCharString& value) { stream.WriteNullTerminatedString(value); }
};
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, std::vector<T>& v)
{
    CPersistTraits<T> traits;
    const long count = v.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, v[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CCharString>(CDataOutputStream&, std::vector<CCharString>&);