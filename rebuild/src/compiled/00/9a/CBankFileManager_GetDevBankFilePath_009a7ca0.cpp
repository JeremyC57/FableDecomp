// CBankFileManager::GetDevBankFilePath(const CCharString& name) const -- resolve a bank name through the alias map, look it up among the registered banks and return its full path (base path + bank path); an unknown bank throws CGenericException("<name> bank not found!")
// Leaf: ?GetDevBankFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7CA0, 145 bytes (the int3 VC7.1 emits after the final noreturn call included), thiscall, ret 8 (hidden CWideString result + name).
// Shape: the alias copy is the inline by-value LookupBankAlias() helper with two returns (the two copy-constructor pushes are cross-jumped, as retail).
// Callees: BankAliasMap.find 0x009AB5D0 and Banks.find 0x009AB560 (out of line, return the node), CCharString copy
// constructor 0x0099EC30, operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall: hidden result
// in ecx, left operand in edx, right on the stack), CCharString::~CCharString 0x0099EAE0, operator+(const
// CCharString&, const char*) 0x0099F600 (__fastcall, same convention), CCharString::operator const char*
// 0x0099E4C0, _CxxThrowException 0x00BFEB84 (ThrowInfo 0x013692F8: CGenericException : CExceptionBase, 1 byte,
// no destructor).
// Layout: Ego_r.pdb CBankFileManager with retail containers (as Close 0x009A7990): Banks map head +4,
// BankAliasMap head +0x18, BasePath (CWideString) +0x24; map nodes carry the key at +0x10 and the value at
// +0x14 (the alias string / CBankInfo, whose first member is the bank path).
// Shape: the key is a named local in the `push ecx` frame word copy-constructed by LookupBankAlias (the pushed
// source is the node's value or the argument); the found path is the fall-through and
// builds the result straight into the caller's slot. The failure path builds the message temporary in the
// dead result-pointer slot and the exception object in the dead `name` slot; the TU has no /EHsc (no unwind
// frame, the key is never destroyed on the throw path).
// Port (NF177, q4391 / q4392): moved off the row-local CCharString / CWideString / CBankInfo / CRetailMap / CBankFileManager models onto
// fable_string.h, stlport/fable_map.h and fable_retail_banks.h. Name: Ego_r.pdb + FableWin.pdb
// ?GetDevBankFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z (const; was the descriptive FindBank, QAE); its landed
// callers 0x009CB960 / 0x009D06F0 already bind that spelling. The two map finds are the STLport const member-template find over the
// out-of-line _M_find 0x009AB5D0 / 0x009AB560 (D1). Ego_r.pdb names the alias step LookupBankAlias (inline, QBE, by value).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKFILEPATHS
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP
#define FABLE_RETAIL_CBANKFILEMANAGER_LOOKUPBANKALIAS
#define FABLE_RETAIL_CGENERICEXCEPTION_TEXT_CTOR
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"
#include "engine/CGenericException.h"

CWideString CBankFileManager::GetDevBankFilePath(const CCharString& bankHandle) const
{
    CCharString name = LookupBankAlias(bankHandle);
    std::map<CCharString, CBankInfo>::const_iterator info = Banks.find(name);
    if (info != Banks.end())
        return BasePath + (*info).second.DevBankFilePath;
    throw CGenericException(name + " bank not found!");
}
