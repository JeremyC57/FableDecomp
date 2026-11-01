// $E_TITLE_NAME() -- dynamic initializer of the anonymous-namespace CCharString TITLE_NAME ("EXPRESSIONS TITLE"): construct it from the literal (length -1), then register its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121AE9C (unaligned: an /Os-packed TU, directly after 0x0121AE90), 29 bytes,
//   in `.text$yc`, reached from the .CRT$XCU table entry 0x013743E4, plain `ret`; registers the atexit destructor
//   0x0122917A (`mov ecx,0x013B8C60 ; jmp ~CCharString`).
// Identity (proven): both PDBs list exactly two `anonymous namespace'::`dynamic initializer for 'TITLE_NAME'' entries
//   (Ego_r 0x0100DE7E / 0x0100E119, FableWin 03FE0FF8 / 03FE4ED6); retail holds exactly two "<SCREEN> TITLE" CCharString
//   initializers: "ITEMS TITLE" (0x0121A8C1, followed by Item_Descriptions / Item_Name_Table / Item_Icon = the PDB's
//   ITEM_DESCRIPTION_STRING / ITEM_NAME_TABLE_STRING / ITEM_ICON_STRING after the second TITLE_NAME) and this
//   "EXPRESSIONS TITLE", which ends its TU as FableWin's first TITLE_NAME ends its TU (after the NInventory header
//   statics). Its readers are the expressions screen of CTCInventoryAbilities (0x005F436B, 0x005F447F, 0x005F4957,
//   0x005F4EFA, 0x005F6157: ConstructPCExpressionCategoryList / GetExpressionsListUIComponent / ConstructPCExpressionsList).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, _atexit 0x004012BC.
// Shape: the compiler-generated initializer of a namespace-scope object with a constructor and destructor.
#include "fable_string.h"      // CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0

namespace {
CCharString TITLE_NAME("EXPRESSIONS TITLE", -1);
}