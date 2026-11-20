// CConsoleCommandBase::GetFormattedName() const -- the command's name followed by "(" (the help-bar form of a command)
// Leaf: ?GetFormattedName@CConsoleCommandBase@@MBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EA3A0, 26 bytes (`ret 4`: the hidden result pointer).
// Retail bytes: push esi ; mov esi,[esp+8] ; lea edx,[ecx+4] ; push 013CAA48h ; mov ecx,esi ; call 0099F570h ; mov eax,esi ; pop esi ; ret 4
// Callees: operator+(const CCharString&, const CCharString&) 0x0099F570 (__fastcall, ??H@YI?AVCCharString@@ABV0@0@Z in both PDBs: result in
//   ecx, left in edx, right pushed); OPEN_BRACKET 0x013CAA48, the "(" CCharString of the console TU's anonymous namespace (as 0x009EB2C0).
// Name fix (NF202, 2026-11-20, q12225): Ego_r.pdb 0x00B3D6A0 + FableWin.pdb 0x030A9E40 ?GetFormattedName@CConsoleCommandBase@@MBE?AVCCharString@@XZ
//   (protected virtual const). The derived CConsoleCommandBase_Methods stand-in, its local CCharString model, the cast data address and
//   the ?sub_99f570@@YI?AVCCharString@@PAXPBD@Z stand-in callee are replaced by engine/CConsoleCommandBase.h
//   (FABLE_RETAIL_CCONSOLECOMMANDBASE_GETFORMATTEDNAME, a partial retail mode) and fable_string.h (FABLE_RETAIL_CCHARSTRING_CONCAT_STRING).
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCONSOLECOMMANDBASE_GETFORMATTEDNAME
#include "engine/CConsoleCommandBase.h"

namespace { extern CCharString OPEN_BRACKET; }   // 0x013CAA48 ("(")

CCharString CConsoleCommandBase::GetFormattedName() const
{
    return Name + OPEN_BRACKET;
}
