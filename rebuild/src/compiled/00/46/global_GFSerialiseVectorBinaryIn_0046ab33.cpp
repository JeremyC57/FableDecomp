// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CSoundBankEntry>& out) -- read the element count, resize the sound-bank vector, then read every entry in place through its traits
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046AB33, 145 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CBrainBehaviour> is propagated. The 0x14 stride and the callees identify CSoundBankEntry
//   (Ego_r.pdb ??$GFSerialiseVectorBinaryIn@VCSoundBankEntry@@@@YIXAAVCMemoryDataInputStream@@...): 0x0046ABD6 is the landed
//   CPersistTraits<CSoundBankEntry>::TransferBinaryIn and 0x0046ABC4 (landed as "CIndexTriple") the entry's default constructor
//   (ID 0, the three CDefStrings -1; the byte twin of 0x0046AADE used by the writer 0x0046AA91).
// Callees: CSoundBankEntry::CSoundBankEntry() @ 0x0046ABC4 (the traits' DefaultVal), CDataInputStream::ReadWithSrcChunkOverflow
//   @ 0x00993CA0 (slow path of the inlined count Read), vector<CSoundBankEntry>::resize(n) @ 0x0046ACA8 (out of line here),
//   CPersistTraits<CSoundBankEntry>::TransferBinaryIn @ 0x0046ABD6.
// Shape: the generic template exactly as the landed 0x005C2AD8 (CItem): traits with an out-of-line-constructed DefaultVal,
//   inlined 4-byte count Read, `out.resize(count)` called out of line, then `traits.TransferBinaryIn(stream, &out[i])` with
//   the index folded into a byte offset (esi += 0x14) and the count run down in edi. Size TU with a frame pointer:
//   `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a reader adapter carrying the retail member functions; CSoundBankEntry is the PDB's
//   0x14-byte record (no engine header); the vector is STLport's three words.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
class CDefString { public:
    long TablePos;                                          // +0x00 def-string table index
};
class CSoundBankEntry { public:
    unsigned long ID;                                       // +0x00
    CDefString    Filename;                                 // +0x04
    CDefString    SymbolFilename;                           // +0x08
    CDefString    LipSyncBankHandle;                        // +0x0c
    bool          DVD;                                      // +0x10
    CSoundBankEntry();                                      // 0x0046ABC4
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize);                      // 0x0046ACA8
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0046ABD6
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
template void __fastcall GFSerialiseVectorBinaryIn<CSoundBankEntry>(CMemoryDataInputStreamReader& stream, std::vector<CSoundBankEntry>& out);