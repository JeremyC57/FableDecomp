// CBankFile::UpdateEntry(unsigned long index) -- the bank-entry update step SetupEditorAnimation calls between
//   BeginUpdateEntries and EndUpdateEntries; empty in the TLC build (`ret 4`)
// Leaf: ?UpdateEntry@CBankFile@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CBA80, 3 bytes, `ret 4` (thiscall, one stack argument). Its only reference in the exe is
//   the call at 0x00B3BDBA in CEngineResourceManager::SetupEditorAnimation 0x00B3BC90 (one pushed index), which the landed row
//   already binds as ?UpdateEntry@CBankFile@@QAEXK@Z.
// Name (NF241, 2026-11-22, CODEXP27 L107): Ego_r.pdb and FableWin.pdb both carry the S_PUB32 ?UpdateEntry@CBankFile@@QAEXK@Z
//   (Ego_r 0001:7455216, FableWin 0002:25188960: S_GPROC32 `void CBankFile::(unsigned long)`, 525 / 1104 bytes there), and the one retail
//   caller passes one index between BeginUpdateEntries 0x009CE8D0 and EndUpdateEntries 0x009CEA70, as the PDBs' callers do. The TLC
//   body is empty. Was the stand-in ?M@C009cba80@@QAEXH@Z (catalog _global::Nop_ret4_009cba80), bound by no landed row.
// Model: engine/CBankFile.h (FABLE_RETAIL_CBANKFILE_ENTRIES + _EDITOR_ENTRY_UPDATE declare the member, as the binder 0x00B3BC90 uses).
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CBANKFILE_EDITOR_ENTRY_UPDATE
#include "engine/CBankFile.h"
void CBankFile::UpdateEntry(unsigned long index)
{
}
