// CCachedFile::OpenFromFile(CAFile& file, CASuspendableProcess* psuspendable) -- adopt an open disk file: its pathname, its position, the open
// flags, and a counted pointer to a clone of the file
// Leaf: ?OpenFromFile@CCachedFile@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B210, length 77 bytes (+ int3 padding).
// Callees: CAFile slot 11 GetPathname (+0x2C, CWideString through the hidden pointer), CWideString::operator= 0x0099B7D0,
//   CWideString::~CWideString 0x0099B510, CAFile slot 7 GetPosition (+0x1C), slot 14 Clone (+0x38), CCountedPointer<CAFile>::Reset 0x0050F550.
// Shape: the pathname temporary is homed in `file`'s dead argument slot (the reference was loaded into edi first); the file's vtable is
//   re-read before every virtual call. The suspendable-process argument is unused.
// NF39 (2026-11-16): moved off its local CCachedFile / CAFile / CWideString / counted-pointer models (pointer + `long mode` arguments) onto the
//   shared engine/CCachedFile.h (which names 0x00A5B210), engine/CAFile.h (the original Ego_r TPI virtual order: slot 7 is GetPosition, slot 14
//   Clone -- the old model's "GetLength" / "GetSharedFile" slot names were guesses) and engine/CCountedPointer.h. Evidence: Ego_r.pdb + FableWin.pdb
//   ?OpenFromFile@CCachedFile@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z (the only overload); retail caller 0x00821AEF pushes the process then
//   &file (CMap::LoadFromFile 0x00821A80, already bound).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CAFILE
#include "engine/CCachedFile.h"
#include "engine/CASuspendableProcess.h"

void CCachedFile::OpenFromFile(CAFile& file, CASuspendableProcess*)
{
    Pathname = file.GetPathname();
    Position = file.GetPosition();
    Opened = true;
    BufferValid = false;
    PDiskFile.Reset(file.Clone());
}
