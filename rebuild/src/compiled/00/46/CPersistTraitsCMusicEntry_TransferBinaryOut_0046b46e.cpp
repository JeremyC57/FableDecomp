// CPersistTraits<CMusicEntry>::TransferBinaryOut(CDataOutputStream*, const CMusicEntry*) -- write ID, Filename index, Gain and CacheToHD
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B46E, 61 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0 (the ID), WriteLong @ 0x00993EB0 (Filename), WriteFloat @ 0x00993EE0
//   (`fld ; push ecx ; fstp [esp]`), WriteEBOOL @ 0x00993E30. The ID goes through a different overload in the two
//   instantiation TUs (0x0046B46E writes it unsigned, 0x004E9D34 signed).
// Layout (PDB CMusicEntry, 0x10 bytes): ID +0, Filename (CDefString: a 4-byte def-string index) +4, Gain +8, CacheToHD +0xC.
//   No engine header exists for CMusicEntry; local model with the PDB member names.
// Size-optimised TU: #pragma optimize("s", on) (the `push dword ptr [esi+N]` argument form).
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                    // 0x00993E30
    void WriteULong(unsigned long value);           // 0x00993EA0
    void WriteLong(long value);                     // 0x00993EB0
    void WriteFloat(float value);                   // 0x00993EE0
};

struct CMusicEntry {
    unsigned long ID;               // +0x00
    long          Filename;         // +0x04 CDefString index
    float         Gain;             // +0x08
    bool          CacheToHD;        // +0x0C
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CMusicEntry> {
    void TransferBinaryOut(CDataOutputStream* stream, const CMusicEntry* value);
};

void CPersistTraits<CMusicEntry>::TransferBinaryOut(CDataOutputStream* stream, const CMusicEntry* value)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)stream;
    out->WriteULong(value->ID);
    out->WriteLong(value->Filename);
    out->WriteFloat(value->Gain);
    out->WriteEBOOL(value->CacheToHD);
}