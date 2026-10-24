// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CMusicEntry>& value) -- write the element count, then every music entry through its traits
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B422, 76 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: Ego_r.pdb ??$GFSerialiseVectorBinaryOut@VCMusicEntry@@...; CMusicEntry (0x10 bytes) = {unsigned long ID,
//   CDefString Filename, float Gain, bool CacheToHD}.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count), CPersistTraits<CMusicEntry>::TransferBinaryOut @ 0x0046B46E (landed).
// Shape: the generic template (landed 0x005C12A1): the traits holds a DefaultVal built by the inline CMusicEntry() --
//   ID 0 and CacheToHD false share the zero register (ebx, which then becomes the byte offset), Filename -1 from CDefString(),
//   Gain through `fldz ; fstp` (a size-TU float zero) -- then `count = value.size()` (`sar esi,4`), `WriteSLONG(count)` and
//   `traits.TransferBinaryOut(stream, &value[i])` with the count run down in esi. Size TU with a frame pointer:
//   `optimize("y", off)` + `("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CMusicEntry/CDefString are local (no engine header);
//   the vector is STLport's three words.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                             // 0x00993EB0
};
struct CDefString {
    long TablePos;                                          // +0x00 def-string table index (-1 = unset)
    CDefString() : TablePos(-1) { }
};
struct CMusicEntry {
    unsigned long ID;                                       // +0x00
    CDefString    Filename;                                 // +0x04
    float         Gain;                                     // +0x08
    bool          CacheToHD;                                // +0x0c
    CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) { }
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
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;   // 0x0046B46E
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
template void __fastcall GFSerialiseVectorBinaryOut<CMusicEntry>(CDataOutputStreamWriter& stream, std::vector<CMusicEntry>& value);