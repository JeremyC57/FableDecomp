// CWideString::GetAsCharArray() const -- the string's text, or the shared empty string when unassigned
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099AF10, 15 bytes: `mov eax,[ecx] ; test eax,eax ; jne ; mov eax,0x0129A8E0 ; ret ;
//   mov eax,[eax] ; ret` (thiscall), byte-identical to operator const wchar_t* 0x0099AF00 just before it (no ICF).
// Leaf: ?GetAsCharArray@CWideString@@QBEPBGXZ
// wchar_t: the TLC build mangles native wchar_t (the PDBs' `_W`); the rebuild compiles without /Zc:wchar_t, where
//   fable_string.h's wchar_t is the <stddef.h> unsigned short typedef (`G`), so the Leaf carries the `G` form that the
//   landed callers bind (owner decision recorded in NOTES).
// Identity: Ego_r.pdb and FableWin.pdb list ??BCWideString@@QBEPB_WXZ, ?GetAsCharArray@CWideString@@QBEPB_WXZ,
//   ?Contains@CWideString@@QBE_NABV1@@Z in that order = retail 0x0099AF00 / 0x0099AF10 / 0x0099AF20 (Contains: wcsstr,
//   `ret 4`) in the CWideString TU. Callers pass the text to wide APIs (GFMain, FableCheckMyDocumentsWritePermissions).
//   The catalog's "CCharString::operator_char_const*" is wrong, and the landed source defined operator const wchar_t*
//   here (the twin's name). FINDINGS chunk 409 ("0x99AF00/0x99AF10 named inconsistently").
// Layout: fable_string.h's CWideString (storage_ at +0, CWideStringData::text at +0) declares no GetAsCharArray, so the
//   class is modelled locally with the same layout.
#include <stddef.h>
class CWideStringData { public:
    wchar_t*      text;                         // +0x00
    unsigned long unknown04;
    unsigned long unknown08;
    long          owners;
};
extern wchar_t g_FableEmptyWideString_0129A8E0[1];   // 0x0129A8E0

class CWideString {
public:
    const wchar_t* GetAsCharArray() const;
private:
    CWideStringData* storage_;                  // +0x00
};

const wchar_t* CWideString::GetAsCharArray() const
{
    if (storage_ == 0)
        return g_FableEmptyWideString_0129A8E0;
    return storage_->text;
}
