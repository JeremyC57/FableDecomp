// CBankFileManager::GetRetailBankFilePath(const CCharString& bank) -- resolve the bank alias, build BasePath + the bank's retail bank path, create that retail bank file the first time it is asked for, and return the path; an unknown bank throws CGenericException("<name> bank not found!")
// Bounds: int3-after-noreturn row, landed against the N+1 span: only the int3 VC7.1 emits after the final noreturn _CxxThrowException call (q.py strips it from the retail row); RELOCATION_MATCH against the 223-byte N+1 span read from Fable.exe (probe/n1.py). Land against an N+1 row (GOTCHAS: int3-after-noreturn rows LANDED).
// Leaf: ?GetRetailBankFilePath@CBankFileManager@@QAE?AVCWideString@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7D40, 223 bytes (the int3 VC7.1 emits after the final noreturn
// _CxxThrowException call included; one more 0xCC pads the 224-byte row), thiscall `ret 8` (hidden CWideString result +
// bank). PDB (Ego_r): `public: class CWideString __fastcall CBankFileManager::GetRetailBankFilePath(class CCharString
// const &)` (QAE: not const -- it records the created bank). Catalog name "CBankFileManager_FindOrCreateBank" is
// descriptive. Caller 0x009CB9D4.
// Callees: BankAliasMap.find @ 0x009AB5D0 and Banks.find @ 0x009AB560 (out of line, return the node), CCharString copy
// ctor @ 0x0099EC30, operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall), CreatedRetailBanks
// .find -> _M_find @ 0x009AA4C0 and .insert -> insert_unique @ 0x009AAEB0 (this chunk), CreateRetailBank @ 0x009A7720
// (landed), CWideString copy ctor @ 0x0099B720 (the by-value return, no NRVO), ~CWideString @ 0x0099B510,
// ~CCharString @ 0x0099EAE0, operator+(const CCharString&, const char*) @ 0x0099F600, CCharString::operator const char*
// @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84 (ThrowInfo 0x013692F8: CGenericException : CExceptionBase, 1 byte).
// Layout: Ego_r.pdb CBankFileManager with retail containers: Banks +0x04, BankAliasMap +0x18, BasePath +0x24,
// CreatedRetailBanks +0x28; CBankInfo::RetailBankFilePath at node value +4 (node +0x18).
// Shape: the landed FindBank (0x009A7CA0) front end -- the key is a frame local copy-constructed through the two-return
// ResolveAlias helper -- then the path homed in the dead bank argument slot, `CreatedRetailBanks.find(path) ==
// CreatedRetailBanks.end()` (end() read before the call), and the pair<iterator, bool> insert result in the frame. No
// /EHsc in this TU: nothing is destroyed on the throw path.
// Port (NF184, 2026-11-20, q4393): moved off the row-local CWideString / CCharString / CBankInfo / CRetailMap / std::pair / _Rb_tree /
//   set / CBankFileManager models onto fable_string.h, stlport/fable_map.h (map, set) and fable_retail_banks.h (the NF177 BANKFILEPATHS /
//   LOOKUPBANKALIAS opt-ins and the NF184 CREATEDRETAILBANKS / CREATERETAILBANK opt-ins). The two map finds are the STLport member-template
//   find over the out-of-line _M_find (0x009AB5D0 BankAliasMap through LookupBankAlias, 0x009AB560 Banks; D1), the set find the out-of-line
//   _M_find 0x009AA4C0, the insert the tree's out-of-line insert_unique 0x009AAEB0, declaration-only here
//   (FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_DECL_ONLY): with STLport's body visible the TU emitted a pick-any insert_unique<CWideString>
//   copy comparing the converted text pointers (fable_string.h declares no CWideString::operator< 0x0099AF70), unlike the owner row.
//   Both forms grade the same here. The other COMDATs the binding table owns elsewhere (iterator == / != 0x0041C820 / 0x0041C830)
//   grade MATCH at their owners.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_FIND
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_DECL_ONLY
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKFILEPATHS
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP
#define FABLE_RETAIL_CBANKFILEMANAGER_LOOKUPBANKALIAS
#define FABLE_RETAIL_CBANKFILEMANAGER_CREATEDRETAILBANKS
#define FABLE_RETAIL_CBANKFILEMANAGER_CREATERETAILBANK
#define FABLE_RETAIL_CGENERICEXCEPTION_TEXT_CTOR
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"
#include "engine/CGenericException.h"

CWideString CBankFileManager::GetRetailBankFilePath(const CCharString& bankHandle)
{
    CCharString name = LookupBankAlias(bankHandle);
    std::map<CCharString, CBankInfo>::iterator info = Banks.find(name);
    if (info != Banks.end()) {
        CWideString path = BasePath + (*info).second.RetailBankFilePath;
        if (CreatedRetailBanks.find(path) == CreatedRetailBanks.end()) {
            CreateRetailBank(path);
            CreatedRetailBanks.insert(path);
        }
        return path;
    }
    throw CGenericException(name + " bank not found!");
}
