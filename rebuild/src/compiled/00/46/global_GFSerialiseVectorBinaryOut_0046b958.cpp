// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CMusicSetEntry>& value) -- write the element count, then every music set's normal/danger music IDs and loop count
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B958, 88 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CActionInputControl> is propagated (that record is 28 bytes). CSoundDef::Transfer (landed
//   0x0046A9CE) hands its fourth vector, MusicSetEntries (+0x60), to CPersistContext::TransferBinary @ 0x0046B92A, whose save
//   half is this body; Ego_r.pdb ??$GFSerialiseVectorBinaryOut@VCMusicSetEntry@@...; CMusicSetEntry (12 bytes) =
//   {long NormalMusicID, long DangerMusicID, long LoopCount}.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count, then the three members of each element).
// Shape: the generic template (landed 0x005C12A1) with the set's traits inlined to three WriteSLONG calls (`__forceinline`: the
//   size TU would call a plain inline member out of line); `count = value.size()` (`push 0Ch ; pop ecx ; idiv`), the byte
//   offset in ebp, the count run down in the `push ecx` frame word. Size TU, no frame pointer: `#pragma optimize("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CMusicSetEntry is local (no engine header); the vector is
//   STLport's three words.
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                             // 0x00993EB0
};
class CMusicSetEntry { public:
    long NormalMusicID;                                     // +0x00
    long DangerMusicID;                                     // +0x04
    long LoopCount;                                         // +0x08
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
template <> struct CPersistTraits<CMusicSetEntry> {
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream, const CMusicSetEntry* value) const
    {
        stream.WriteSLONG(value->NormalMusicID);
        stream.WriteSLONG(value->DangerMusicID);
        stream.WriteSLONG(value->LoopCount);
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
template void __fastcall GFSerialiseVectorBinaryOut<CMusicSetEntry>(CDataOutputStreamWriter& stream, std::vector<CMusicSetEntry>& value);