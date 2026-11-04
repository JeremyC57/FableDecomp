// NGameText::CDataBank::GetTextInfo(unsigned long index) const -- the text-bank entry record for a bank index, or null when the entry's data type is not the text type (0)
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7D00, length 37 bytes.
// Callees: CBankFile::GetEntryDataType(unsigned long) @ 0x009CC210 (on `this`: CDataBank derives from CBankFile).
// Shape: the index is kept in edi across the call (used again to index the entry table); the type test is
// `test eax,eax ; jne` and the success path is the `if` body (fall-through), the null return sits after it. The entry table is the first word
// (m_pFirst) of the CArray<CDataBankTextEntry*> Entries member, at +0x1e0 in retail (PDB +0x254: every CArray/map in
// the shipped CBankFile/CDataBank is smaller than in Ego_r.pdb, so the PDB header cannot be included).
// FableWin: ?GetTextInfo@CDataBank@NGameText@@QBEPBVCDataBankTextEntry@2@K@Z (const record pointer).
namespace NGameText { class CDataBankTextEntry; }

#include "engine/CArray.h"

class CBankFile
{
public:
    unsigned long GetEntryDataType(unsigned long) const;   // 0x009CC210
};
class CBankFileAsync : public CBankFile
{
public:
};

namespace NGameText {
class CDataBank : public CBankFileAsync { public:               // CBankFile base (0x90 bytes in retail) + the text-bank state
    void*                   __vftable;               // +0x000
    unsigned char           _bankFile[0x1e0 - 4];    // +0x004 .. +0x1e0: CBankFile state and the data-bank buffers
    CArray<CDataBankTextEntry*> Entries;             // +0x1e0 (PDB name)
    const CDataBankTextEntry* GetTextInfo(unsigned long index) const;
};

const CDataBankTextEntry* CDataBank::GetTextInfo(unsigned long index) const
{
    if (GetEntryDataType(index) == 0)
        return Entries[index];
    return 0;
}
}
