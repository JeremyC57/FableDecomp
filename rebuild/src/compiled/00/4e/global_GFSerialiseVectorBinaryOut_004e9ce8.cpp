// _global::GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CMusicEntry>& value) -- write the element count, then every music entry through its persist traits
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9CE8, 76 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CMusicEntry> instantiation; the 16-byte stride (`sar esi,4`), the inline DefaultVal and the traits
//   writer 0x004E9D34 (manifest CPersistTraits<CMusicEntry>::TransferBinaryOut) agree; the only caller is the
//   CPersistContext vector transfer at 0x004E9CD5 (save arm).
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count), CPersistTraits<CMusicEntry>::TransferBinaryOut @ 0x004E9D34.
// Shape: the generic template of the landed 0x0046AA91: the traits object holds a DefaultVal entry, here default-constructed
//   inline (Ego_r.pdb CMusicEntry {ulong ID, CDefString Filename, float Gain, bool CacheToHD}: ID 0 and CacheToHD false from
//   the zero register, Filename's TablePos -1 = `or [ebp-10h],-1`, Gain 0.0 = `fldz ; fstp`); `count = value.size()`
//   (`sar 4`), `WriteSLONG(count)`, then `traits.TransferBinaryOut(stream, &value[i])` with the index folded into a byte
//   offset (ebx) and the count running down (esi); the stream is spilled for the loop. Size TU with a frame pointer:
//   `optimize("y", off)` + `("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CMusicEntry is the PDB's 16-byte record (no engine
//   header); the vector is STLport's three words.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStreamWriter : CDataOutputStream {
};
class CDefString {
public:
    long TablePos;                                          // +0x00 def-string table index
    CDefString() : TablePos(-1) { }
};
class CMusicEntry {
public:
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
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;   // 0x004E9D34
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