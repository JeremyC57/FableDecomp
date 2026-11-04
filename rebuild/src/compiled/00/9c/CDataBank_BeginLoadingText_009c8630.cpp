// CDataBank::BeginLoadingText(unsigned long index) -- start the asynchronous load of a text entry: only when the bank entry's data type is the text type (0) and the entry record exists
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8630, length 42 bytes.
// Callees: CBankFile::GetEntryDataType(unsigned long) @ 0x009CC210 (on `this`), CDataBankTextEntry::BeginLoading(unsigned long, CDataBank*) @ 0x009C8340
// (ecx = the entry record, arguments pushed right to left: the bank, then the index).
// Shape: the index stays in edi across the type call and indexes the Entries table (first word of the CArray at
// +0x1e0 in retail, PDB +0x254); `test eax,eax ; jne` is the type-0 test, then the null test of the record and
// the two pushes `push esi ; push edi`. The group sibling @ 0x009C8220 tests for type 1 and calls 0x009C80F0.
struct CDataBank;

struct CDataBankTextEntry {
    void BeginLoading(unsigned long index, CDataBank* bank);    // 0x009C8340
};

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

struct CDataBank : public CBankFileAsync {                      // CBankFile base (0x90 bytes in retail) + the text-bank state
    void*                       __vftable;               // +0x000
    unsigned char               _bankFile[0x1e0 - 4];    // +0x004 .. +0x1e0
    CArray<CDataBankTextEntry*> Entries;                 // +0x1e0 (PDB name; CDataBankEntry* in the PDB)
    void BeginLoadingText(unsigned long index);
};

void CDataBank::BeginLoadingText(unsigned long index)
{
    if (GetEntryDataType(index) == 0) {
        CDataBankTextEntry* entry = Entries[index];
        if (entry)
            entry->BeginLoading(index, this);
    }
}