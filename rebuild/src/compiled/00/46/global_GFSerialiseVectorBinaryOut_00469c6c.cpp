// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CTextureReplacementDef::CEntry>& value) -- write the element count, then every entry's original and replacement texture indices
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x00469C6C, 70 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CAtmosBankEntry> is propagated. This is the save half of CPersistContext::TransferBinary @ 0x00469C3E,
//   called from CTextureReplacementDef::Transfer (0x00469C2E); Ego_r.pdb has
//   ??$GFSerialiseVectorBinaryOut@VCEntry@CTextureReplacementDef@@@@YIXAAVCDataOutputStream@@AAV?$vector@...  and the 8-byte
//   CTextureReplacementDef::CEntry = {long OriginalTexture, long NewTexture}. Its load half is 0x00469CB2.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count, then both members of each element).
// Shape: the generic template (landed 0x005C12A1): `count = value.size(); WriteSLONG(count); for (i < count)
//   traits.TransferBinaryOut(stream, &value[i]);` with the entry's traits inlined to two WriteSLONG calls -- it must be
//   `__forceinline` (the size TU calls a plain inline member out of line and gives the traits a frame word). The traits is an
//   explicit specialisation without a DefaultVal; the stream rides in the `push ecx` slot and is reloaded before each call,
//   the element pointer in ebx. Size TU, no frame pointer: `#pragma optimize("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CTextureReplacementDef::CEntry is local (no engine header);
//   the vector is STLport's three words.
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                             // 0x00993EB0
};
class CTextureReplacementDef { public:
    class CEntry { public:
        long OriginalTexture;                               // +0x00
        long NewTexture;                                    // +0x04
    };
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
template <> struct CPersistTraits<CTextureReplacementDef::CEntry> {
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream, const CTextureReplacementDef::CEntry* value) const
    {
        stream.WriteSLONG(value->OriginalTexture);
        stream.WriteSLONG(value->NewTexture);
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
template void __fastcall GFSerialiseVectorBinaryOut<CTextureReplacementDef::CEntry>(CDataOutputStreamWriter& stream, std::vector<CTextureReplacementDef::CEntry>& value);