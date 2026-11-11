// CCharString::operator const char*() const -- the text, or the shared empty NullChar when the string has no data
// Leaf: ??BCCharString@@QBEPBDXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099E4C0, 15 bytes: mov eax,[ecx] ; test eax,eax ; jne ; mov eax,offset NullChar ; ret ;
//   mov eax,[eax] ; ret.
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ??BCCharString@@QBEPBDXZ (fable_string.h declares it so). Was the
//   __fastcall stand-in CCharString_operator_char_const_ptr_b(const CCharString*) returning g_EmptyCharString_0129AAF4.
// Body: the inline GetAsCharArray() (fable_string.h retail branch; NullChar 0x0129AAF4 is CCharString's protected static).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"

CCharString::operator const char*() const
{
    return GetAsCharArray();
}
