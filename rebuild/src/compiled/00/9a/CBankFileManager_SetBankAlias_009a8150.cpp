// CBankFileManager::SetBankAlias(const CCharString& bank_handle, const CCharString& alias) -- BankAliasMap[alias] = bank_handle
// Leaf: ?SetBankAlias@CBankFileManager@@QAEXABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A8150, length 28 bytes (32 with padding).
// Identity (NF87, 2026-11-17): Ego_r.pdb + FableWin.pdb ?SetBankAlias@CBankFileManager@@QAEXABVCCharString@@0@Z; Ego_d.pdb names
//   the parameters bank_handle, alias; the map at +0x18 is CBankFileManager::BankAliasMap (Ego_r.pdb, map<CCharString, CCharString>,
//   fable_retail_banks.h). The old catalog name CStringTable_009a8150::SetEntry was descriptive.
// Callees: 0x009AC700 (map<CCharString, CCharString>::operator[], thiscall, the key; returns the mapped
// string), 0x0099EFB0 (CCharString::operator=(const CCharString&)).
// Shape: both arguments loaded, the VALUE pushed first (it is the argument of the outer operator= call,
// evaluated before its object expression) and left on the stack across the operator[] call, which takes the
// KEY; then `mov ecx,eax ; call operator=` and `ret 8` (no tail jump: the pending argument is in the way).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_SETBANKALIAS
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"

void CBankFileManager::SetBankAlias(const CCharString& bank_handle, const CCharString& alias)
{
    BankAliasMap[alias] = bank_handle;
}
