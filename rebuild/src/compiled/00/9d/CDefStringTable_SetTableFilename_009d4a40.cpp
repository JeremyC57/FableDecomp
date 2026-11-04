// CDefStringTable::SetTableFilename(const CWideString& filename) -- remember the file the definition string table loads from / saves to
// Leaf: ?SetTableFilename@CDefStringTable@@QAEXABVCWideString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D4A40, 8 bytes: `add ecx,30h ; jmp 0x0099B7D0` (TableFilename = filename, tail call to
//   CWideString::operator=; the returned reference is discarded).
// Identity (two sources): (1) Ego_r.pdb ?SetTableFilename@CDefStringTable@@QAEXABVCWideString@@@Z, in donor order between GetStringCRC and
//   WriteTable exactly as retail (GetStringCRC 0x009D4A20, this row, WriteTable 0x009D4A50); +0x30 is the Ego_r member TableFilename with
//   retail STLport container sizes; (2) its only caller, GFMain 0x00402D36, passes a CWideString built from the definitions path with
//   ecx = g_DefStringTable 0x013CA828. The catalog's "_Cons_val<..CParticleMorphs..>" was propagated.
// Callees: ??4CWideString@@QAEABV0@ABV0@@Z 0x0099B7D0.
// Header: engine/CDefStringTable.h (new), fable_string.h.
#include "engine/CDefStringTable.h"
void CDefStringTable::SetTableFilename(const CWideString& filename)
{
    TableFilename = filename;
}