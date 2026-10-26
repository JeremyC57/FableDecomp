// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CSoundBankEntry>& value) -- write the element count, then every sound-bank entry through its traits
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046AA91, 77 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CBrainBehaviour> is propagated (CBrainBehaviour is 12 bytes). The 0x14 stride and the callees
//   identify CSoundBankEntry (Ego_r.pdb ??$GFSerialiseVectorBinaryOut@VCSoundBankEntry@@...): the traits writer 0x0046AAF0 is the
//   landed CPersistTraits<CSoundBankEntry>::TransferBinaryOut, and 0x0046AADE (landed as "C3DGroup2") is the entry's default
//   constructor (ID 0, the three CDefStrings -1).
// Callees: CSoundBankEntry::CSoundBankEntry() @ 0x0046AADE (the traits' DefaultVal, out of line), CDataOutputStream::WriteSLONG
//   @ 0x00993EB0 (the count), CPersistTraits<CSoundBankEntry>::TransferBinaryOut @ 0x0046AAF0.
// Shape: the generic template (landed 0x005C12A1): the traits object holds a DefaultVal entry, `count = value.size()` (`idiv`
//   by 0x14 after `push 14h ; pop ecx`), `WriteSLONG(count)`, then `traits.TransferBinaryOut(stream, &value[i])`; the index
//   folds into a byte offset in ebx and the count runs down in edi. Size TU with a frame pointer:
//   `optimize("y", off)` + `("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CSoundBankEntry is the PDB's 0x14-byte record (no engine
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
class CDefString { public:
    long TablePos;                                          // +0x00 def-string table index
};
class CSoundBankEntry { public:
    unsigned long ID;                                       // +0x00
    CDefString    Filename;                                 // +0x04
    CDefString    SymbolFilename;                           // +0x08
    CDefString    LipSyncBankHandle;                        // +0x0c
    bool          DVD;                                      // +0x10
    CSoundBankEntry();                                      // 0x0046AADE
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
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;   // 0x0046AAF0
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
template void __fastcall GFSerialiseVectorBinaryOut<CSoundBankEntry>(CDataOutputStreamWriter& stream, std::vector<CSoundBankEntry>& value);