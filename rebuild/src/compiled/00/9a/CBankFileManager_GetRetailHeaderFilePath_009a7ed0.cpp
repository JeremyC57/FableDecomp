// CBankFileManager::GetRetailHeaderFilePath(const CCharString& bank) -- resolve the bank alias, then return BasePath + the bank's RetailHeaderFilePath, or L"" when the bank is unknown or has no such path
// Leaf: ?GetRetailHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7ED0, length 171 (+5 int3) bytes, ret 8 (hidden CWideString result).
// Callees: map<CCharString,CCharString>::find @ 0x009AB5D0 (BankAliasMap; catalog "BinaryTree_Search"), CCharString copy ctor
// @ 0x0099EC30, map<CCharString,CBankInfo>::find @ 0x009AB560 (Banks; catalog "map_lower_bound"), CWideString copy ctor @ 0x0099B720,
// CWideString::GetLength @ 0x0099B220, operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall),
// CWideString::~CWideString @ 0x0099B510, CWideString(const wchar_t*) @ 0x0099B6B0 on L"" @ 0x0122D70C, CCharString::~CCharString
// @ 0x0099EAE0.
// Shape: the alias lookup is the inline by-value LookupBankAlias (Ego_r.pdb; alias value or the argument), so each arm
// pushes its own reference for the named copy (a ternary pushes once after a join); the path copy is homed in the dead bank-argument slot; the empty-path test falls into the L"" exit. Twin of
// GetDevHeaderFilePath 0x009A7E20.
// Layout: retail CBankFileManager (see CreateRetailBank 0x009A7720): Banks +0x04, BankAliasMap +0x18, BasePath +0x24;
// Ego_r.pdb CBankFileManager::CBankInfo (DevBankFilePath, RetailBankFilePath, DevHeaderFilePath +8,
// RetailHeaderFilePath +0xc, RegisteredBank).
// Port (NF177, q4391 / q4392): moved off the row-local CCharString / CWideString / CBankInfo / CStringMap / CBankFileManager models
// onto fable_string.h, stlport/fable_map.h and fable_retail_banks.h (Ego_r.pdb: the member is const, QBE; the two map finds are the
// STLport const member-template find over the out-of-line _M_find 0x009AB5D0 / 0x009AB560, D1).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP
#define FABLE_RETAIL_CBANKFILEMANAGER_LOOKUPBANKALIAS
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"

CWideString CBankFileManager::GetRetailHeaderFilePath(const CCharString& bankHandle) const
{
    CCharString name = LookupBankAlias(bankHandle);
    std::map<CCharString, CBankInfo>::const_iterator info = Banks.find(name);
    if (info != Banks.end()) {
        CWideString path((*info).second.RetailHeaderFilePath);
        if (path.GetLength())
            return BasePath + path;
    }
    return CWideString(L"");
}
