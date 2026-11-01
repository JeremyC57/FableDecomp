// StaticInitialization::DynamicInitializer_NEWLINE() -- `anonymous namespace'::`dynamic initializer for 'NEWLINE'`: construct the CTCInventoryWeapons TU's wide newline string from L"\n" and register its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121AB3F, 27 bytes: push 0x01231728 (L"\n") ; mov ecx,0x013B8BA8 ; call 0x0099B6B0 ;
//   push 0x01228FD9 ; call _atexit 0x004012BC ; pop ecx ; ret.
// Identity (proven): compiler-emitted, not a library object: the only reference is its .CRT$XCU slot at 0x0137438C, it
//   runs the engine's CWideString(const wchar_t*) 0x0099B6B0 on 0x013B8BA8 with the literal L"\n" (0x01231728: 0A 00 00 00)
//   and registers the atexit thunk 0x01228FD9 (`mov ecx,0x013B8BA8 ; jmp ~CWideString`). Every other user of 0x013B8BA8 is in
//   the CTCInventoryWeapons TU (0x005C3444 .. 0x005C988F: `+=` separators in the weapon descriptions), so the object is
//   TU-local. Ego_r.pdb and FableWin.pdb list that TU's anonymous-namespace initializers in order: ... CATEGORY_CHILD_SELL_LIST_
//   STRING, BANK_HANDLE, NEWLINE, WEAPON_DESCRIPTION_STRING, AUG_DESCRIPTION_STRING, WEAPON_TITLE_STRING ...; retail's run
//   is the same order with matching literals: 0x013B8BA0 "Category_Child_Sell_List" ... 0x013B8BA4 "GBANK_MAIN" (BANK_HANDLE),
//   0x013B8BA8 L"\n" (NEWLINE, the only wide string), 0x013B8BAC "Weapon_Descriptions", 0x013B8BB0
//   "Augmentation_Descriptions", 0x013B8BB4 "Weapon_Title". The TU's anonymous-namespace hash in both PDBs is ?A0x0a34196c
//   (?PutSheatheObjectInCarrySlot@?A0x0a34196c@@YIXAAVCThing@@0AAM@Z); the retail data symbol's hash and const-ness are not
//   in any name set (const is taken from its read-only use).
// Callees: CWideString::CWideString(const wchar_t*) 0x0099B6B0, _atexit 0x004012BC (the `_$E2` thunk 0x01228FD9 comes with it).
// Shape: one namespace-scope object with a dynamic constructor; VC7.1 emits the `_$E1` initializer and the `_$E2` atexit
//   thunk. The function sits at an unaligned address (0x0121AB3F), as do the TU's other initializers: the TU is /Os, and
//   the body is identical either way.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0, ~CWideString @ 0x0099B510

namespace {
const CWideString NEWLINE(L"\n");                          // 0x013B8BA8
}
