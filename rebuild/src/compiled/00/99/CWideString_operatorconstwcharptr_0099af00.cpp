// CWideString::operator const wchar_t*() const -- the string's text, or the shared empty string when unassigned
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099AF00, 15 bytes: `mov eax,[ecx] ; test eax,eax ; jne ; mov eax,0x0129A8E0 ; ret ;
//   mov eax,[eax] ; ret` (thiscall).
// Leaf: ??BCWideString@@QBEPBGXZ
// wchar_t: the TLC build mangles native wchar_t (the PDBs' `_W`); the rebuild compiles without /Zc:wchar_t, where
//   fable_string.h's wchar_t is the <stddef.h> unsigned short typedef (`G`), so the Leaf carries the `G` form that the
//   landed callers bind (owner decision recorded in NOTES).
// Identity: Ego_r.pdb and FableWin.pdb list ??BCWideString@@QBEPB_WXZ, ?GetAsCharArray@CWideString@@QBEPB_WXZ,
//   ?Contains@CWideString@@QBE_NABV1@@Z in that order; retail has two byte-identical accessors 0x0099AF00 / 0x0099AF10
//   followed by Contains 0x0099AF20 (wcsstr of the two texts, `ret 4`), inside the CWideString TU (default ctor 0x0099AED0,
//   operator< 0x0099AF70). Callers are wide-text users (CSubtitleRenderer, FormatStringForArea, CreateSaveGame). The landed
//   body modelled a __fastcall free CCharString "operator char const*"; the catalog name was already right. FINDINGS 409.
#include "fable_string.h"

CWideString::operator const wchar_t*() const
{
    if (storage_ == 0)
        return g_FableEmptyWideString_0129A8E0;
    return storage_->text;
}
