// C3DMeshStats::GetAllDummyObjectNames(std::vector<CCharString>& names, bool sort) const -- UnpackNames(names, true, CCharString(), sort)
// Leaf: ?GetAllDummyObjectNames@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2F0F0, oracle length 45 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (landed default constructor), C3DMeshStats::UnpackNames(vector<CCharString>&,
//          bool, const CCharString&, bool) const @ 0x00A2ED10 (private), CCharString::~CCharString @ 0x0099EAE0 (the manifest's
//          `_Cons_val<...>` label on it is propagated; it is the string destructor called on the temporary).
//
// Retail bytes:  mov eax,[esp+8] ; push esi ; mov esi,ecx ; push eax ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov ecx,[esp+0Ch] ;
//                push eax ; push 1 ; push ecx ; mov ecx,esi ; call UnpackNames ; lea ecx,[esp+0Ch] ; call CCharString::~CCharString ; pop esi ; ret 8
// Shape: the second argument is forwarded as the last UnpackNames argument (loaded as a dword before the
// temporary is built), the empty CCharString temporary is constructed in the dead second argument's home
// slot (esp+0x10 after the two pushes = the `sort` slot) and destroyed after the call; `true` selects the
// dummy-object name table (the sibling @ 0x00A2F120 passes false for the helper points).
// Name fix (NF214, 2026-11-21, q12343): moved onto the shared engine/C3DMeshStats.h (FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES) and
//   fable_string.h; the local C3DMeshStats / CCharString models and the CCharStringVector stand-in are gone. Own leaf: both PDBs
//   (Ego_r.pdb 0x00B8E920, FableWin.pdb 0x031E4B50) spell it public const over std::vector<CCharString>& (was the stand-in
//   ?GetAllDummyObjectNames@C3DMeshStats@@QAEXAAUCCharStringVector@@_N@Z); the UnpackNames call binds the PDB
//   ?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@...@std@@_NABVCCharString@@1@Z (private const).
#define FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES
#include "engine/C3DMeshStats.h"

void C3DMeshStats::GetAllDummyObjectNames(std::vector<CCharString>& names, bool sort) const
{
    UnpackNames(names, true, CCharString(), sort);
}
