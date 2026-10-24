// _global::GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CExplosionRing>& value) -- write the element count, then every explosion ring through its own TransferBinaryOut
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EB1A8, 49 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the save arm of the CPersistContext vector transfer at 0x004EB17A.. (load arm: GFSerialiseVectorBinaryIn
//   <CExplosionRing> @ 0x004EB219, this chunk); the 16-byte stride (`sar esi,4`, Ego_r.pdb CExplosionRing {float RingRadius,
//   long NumExplosions, float AngleOffset, float SecondsBeforeSpawnNewRing}) and the element writer 0x004EB1D9 (manifest
//   CExplosionRing::TransferBinaryOut) agree with the catalog's <CExplosionRing>.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count), CExplosionRing::TransferBinaryOut(CDataOutputStream&)
//   @ 0x004EB1D9 (thiscall on the element).
// Shape: the generic template (landed 0x0046AFF4) with an empty traits specialization whose inline TransferBinaryOut forwards
//   to the element's member (`value->TransferBinaryOut(stream)`: ecx = &value[i], the stream kept in ebp and pushed); the
//   index folds into a byte offset (ebx), the count runs down (esi). Size TU without a frame pointer:
//   `#pragma optimize("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CExplosionRing is the PDB's 16-byte record (no engine
//   header); the vector is STLport's three words.
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                             // 0x00993EB0
};
class CExplosionRing {
public:
    float RingRadius;                                       // +0x00
    long  NumExplosions;                                    // +0x04
    float AngleOffset;                                      // +0x08
    float SecondsBeforeSpawnNewRing;                        // +0x0c
    void TransferBinaryOut(CDataOutputStreamWriter& stream) const;   // 0x004EB1D9
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
template <> struct CPersistTraits<CExplosionRing> {
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream, const CExplosionRing* value) const
    {
        value->TransferBinaryOut(stream);
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
template void __fastcall GFSerialiseVectorBinaryOut<CExplosionRing>(CDataOutputStreamWriter& stream, std::vector<CExplosionRing>& value);