// NResourceDirectoryNames::GetGraphicBankDir() -- the constant data folder L"Data\\Graphics\\" as a CWideString
// Leaf: ?GetGraphicBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041A100, 17 bytes, plain `ret` (`__fastcall` free function: hidden result in ecx).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?GetGraphicBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ; the PDBs list the
//   NResourceDirectoryNames getters in one order and retail 0x0041A060.. has the same sequence of literals (see the landed
//   GetLevelDir 0x0041A060); this one is the sixth (after GetHDMiscDir 0x0041A0C0 and GetBrushDir); literal 0x0122F41C.
// Callees: CWideString::CWideString(const wchar_t*) 0x0099B6B0.
// Shape (family K082, representative 0x0041A0A0): the landed GetLevelDir 0x0041A060 with its own literal.
#include "fable_string.h"

namespace NResourceDirectoryNames {
CWideString __fastcall GetGraphicBankDir()
{
    return CWideString(L"Data\\Graphics\\");
}
}