// Dtor_CCharStringBase_004a9140 -- destroy the two CWideString members of a two-string object (+0x4 first, then +0x0), in reverse member
// order as an implicit destructor does (retail: `push esi ; mov esi,ecx ; lea ecx,[esi+4] ; call 0x0099B510 ; mov ecx,esi ; pop esi ;
// jmp 0x0099B510`, 19 bytes)
// Leaf: ?fn_004a9140@@YIXPAUDV@@@Z
// NF193 (q4284): both calls bind the PDB destructor ??1CWideString@@QAE@XZ (0x0099B510; Ego_r.pdb / FableWin.pdb name sets, fable_string.h)
//   instead of the stand-in ?m@DV@@QAEXXZ; the header-less local DV keeps its token, now with its two CWideString members. The owning class
//   is not identified (the catalog keeps the descriptive name).
#include "fable_string.h"
struct DV { CWideString First; CWideString Second; };
void __fastcall fn_004a9140(DV* self){ self->Second.~CWideString(); self->First.~CWideString(); }
