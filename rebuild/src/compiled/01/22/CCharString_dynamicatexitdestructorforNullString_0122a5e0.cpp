// CCharString::`dynamic atexit destructor for 'NullString'' -- the inline ~CCharString on the static empty string 0x013BD804 (CCharString::UnassignString 0x0099E9B0, then one live string fewer)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A5E0, 17 bytes (`mov ecx,13BD804h ; call 0099E9B0 ; dec [13BD800h] ; ret`), in
//   `.text$yd`; registered with atexit by its dynamic initializer 0x01223EC0 (`push 122A5E0h ; call _atexit`).
// Identity (proven): Ego_r.pdb `dynamic atexit destructor for 'CCharString::NullString'' (0x0101E240) and FableWin.pdb the same
//   (0x04012360), each beside its `dynamic initializer for 'CCharString::NullString'' (Ego_r 0x0101AAD0, FableWin 0x0400A4F0),
//   which is the landed 0x01223EC0; Ego_d.pdb PUB32 ?NullString@CCharString@@2V1@A (public static). UnassignString is
//   ?UnassignString@CCharString@@IAEXXZ (both PDBs, protected).
// Label: the catalog's std::_Cons_val<...CParticleMorphs::CEntry...> was a propagated label; the row was the free-function
//   stand-in AutoTinyMethodTarget::Invoke on g_AutoTinyMethodObject / g_AutoTinyMethodCounter.
// Shape: VC7.1 emits a static's atexit destructor ($E2) only in the TU that defines the static, so this row defines
//   CCharString::NullString as its initializer row 0x01223EC0 does (the static initializer / atexit pair: EXTRA_RULES
//   2026-11-20, one known LNK2005 on NullString between the two objects, queued for the TU-emitter rule). The live-string
//   count 0x013BD800 is only declared here (0x01223EC0 defines it), under that row's landed spelling; Ego_d.pdb PUB32
//   ?NoStrings@CCharString@@1JA (fable_string.h's protected NoStrings) is queued for it.
// Inferred: the inline default constructor and destructor bodies (0x0099E4B0 / 0x0099EAE0 are their out-of-line copies,
//   and this object's pick-any copies equal them).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_NULLSTRING
#include "fable_string.h"

extern long g_CCharStringInstanceCount;                  // 0x013BD800 (defined by 0x01223EC0)

inline CCharString::CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
inline CCharString::~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }

CCharString CCharString::NullString;                     // 0x013BD804
