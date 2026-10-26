// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CAtmosBankEntry>& value) -- write the element count, then every atmos-bank entry's ID and file-name def string
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046AFF4, 70 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: Ego_r.pdb ??$GFSerialiseVectorBinaryOut@VCAtmosBankEntry@@...; CAtmosBankEntry (8 bytes) = {unsigned long ID,
//   CDefString Filename}: the ID goes out through WriteULONG, the def string's table index through WriteSLONG. Save half of
//   CPersistContext::TransferBinary @ 0x0046AFC6; the load half is 0x0046B03A.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count and Filename), CDataOutputStream::WriteULONG @ 0x00993EA0 (ID).
// Shape: the generic template (landed 0x005C12A1) with the entry's traits inlined (`__forceinline`: the size TU would call a
//   plain inline member out of line and give the traits a frame word) -- the byte twin of 0x00469C6C but for the first call.
//   The stream rides in the `push ecx` slot and is reloaded before each call, the element pointer in ebx. Size TU, no frame
//   pointer: `#pragma optimize("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CAtmosBankEntry/CDefString are local (no engine header);
//   the vector is STLport's three words.
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteULONG(unsigned long value);                   // 0x00993EA0
    void WriteSLONG(long value);                             // 0x00993EB0
};
class CDefString { public:
    long TablePos;                                          // +0x00 def-string table index
    void TransferBinaryOut(CDataOutputStreamWriter& stream) const { stream.WriteSLONG(TablePos); }
};
class CAtmosBankEntry { public:
    unsigned long ID;                                       // +0x00
    CDefString    Filename;                                 // +0x04
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CAtmosBankEntry> {
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream, const CAtmosBankEntry* value) const
    {
        stream.WriteULONG(value->ID);
        value->Filename.TransferBinaryOut(stream);
    }
};
template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CAtmosBankEntry>(CDataOutputStreamWriter& stream, std::vector<CAtmosBankEntry>& value);