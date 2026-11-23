// CWADFile::GetPathname() const -- the bank's symbol name of this WAD entry, converted to a wide string (returned by value)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39C40, length 45 bytes (+ int3 padding). CWADFile vtable 0x0129CF8C slot 11.
// Leaf: ?GetPathname@CWADFile@@UBE?AVCWideString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CWADFile.h (class-model switch FABLE_RETAIL_CWADFILE; was ?GetPathname@CWADFile_Methods@@QAE?AVCWideString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Callees: CBankFile::GetEntrySymbol(unsigned long) @ 0x009CC1D0 (CCharString by hidden pointer), CCharString::ToWideString
// @ 0x0099E480 (CWideString by hidden pointer), CCharString::~CCharString @ 0x0099EAE0 (the manifest's "_Cons_val<...>" label
// there is propagated).
// Shape: the caller's hidden result pointer is held in esi and pushed first for ToWideString; the CCharString temporary is
// homed in that pointer's dead argument slot (`lea edx,[esp+10h]`) and destroyed after the conversion.
// Preamble: the define / include block of the landed precedent CWADFile_MovePosition_00a39c00.cpp.
#define FABLE_RETAIL_CWADFILE
#include "engine/CWADFile.h"

class CBankFile
{
public:
    CCharString GetEntrySymbol(unsigned long) const;   // 0x009CC1D0
};

class CBankFileAsync : public CBankFile { public:
};

CWideString CWADFile::GetPathname() const
{
    return Bank->GetEntrySymbol(Index).ToWideString();
}