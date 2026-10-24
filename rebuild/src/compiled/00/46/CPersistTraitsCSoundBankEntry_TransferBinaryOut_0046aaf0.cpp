// CPersistTraits<CSoundBankEntry>::TransferBinaryOut(CDataOutputStream*, const CSoundBankEntry*) -- write ID, the three def-string indices and the DVD flag
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046AAF0, 67 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0 (ID), WriteLong @ 0x00993EB0 (Filename, SymbolFilename,
//   LipSyncBankHandle), WriteEBOOL @ 0x00993E30 (DVD).
// Layout (PDB CSoundBankEntry, 0x14 bytes): ID +0, Filename +4, SymbolFilename +8, LipSyncBankHandle +0xC (CDefString:
//   4-byte def-string indices), DVD +0x10. No engine header exists; local model with the PDB member names.
// Size-optimised TU: #pragma optimize("s", on) (the `push dword ptr [esi+N]` argument form).
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                    // 0x00993E30
    void WriteULong(unsigned long value);           // 0x00993EA0
    void WriteLong(long value);                     // 0x00993EB0
};

struct CSoundBankEntry {
    unsigned long ID;                   // +0x00
    long          Filename;             // +0x04 CDefString index
    long          SymbolFilename;       // +0x08 CDefString index
    long          LipSyncBankHandle;    // +0x0C CDefString index
    bool          DVD;                  // +0x10
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CSoundBankEntry> {
    void TransferBinaryOut(CDataOutputStream* stream, const CSoundBankEntry* value);
};

void CPersistTraits<CSoundBankEntry>::TransferBinaryOut(CDataOutputStream* stream, const CSoundBankEntry* value)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)stream;
    out->WriteULong(value->ID);
    out->WriteLong(value->Filename);
    out->WriteLong(value->SymbolFilename);
    out->WriteLong(value->LipSyncBankHandle);
    out->WriteEBOOL(value->DVD);
}