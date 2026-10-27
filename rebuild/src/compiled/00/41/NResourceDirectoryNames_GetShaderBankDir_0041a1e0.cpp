// NResourceDirectoryNames::GetShaderBankDir() -- the constant data folder L"Data\\Shaders\\" as a CWideString
// Leaf: ?GetShaderBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041A1E0, 17 bytes, plain `ret` (`__fastcall` free function: hidden result in ecx).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?GetShaderBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ; the PDBs list the
//   NResourceDirectoryNames getters in one order and retail 0x0041A060.. has the same sequence of literals (see the landed
//   GetLevelDir 0x0041A060); this one is the eleventh (after the landed GetMeshBankDir 0x0041A1C0; before the landed GetEngineCacheDir 0x0041A200); literal 0x0122F470.
// Callees: CWideString::CWideString(const wchar_t*) 0x0099B6B0.
// Shape (family K082, representative 0x0041A0A0): the landed GetLevelDir 0x0041A060 with its own literal.
#include "fable_string.h"

namespace NResourceDirectoryNames {
CWideString __fastcall GetShaderBankDir()
{
    return CWideString(L"Data\\Shaders\\");
}
}