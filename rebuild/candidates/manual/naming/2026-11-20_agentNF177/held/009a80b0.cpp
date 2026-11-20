// CBankFileManager::BankExists(const CCharString& bank) -- resolve the bank alias, then report whether the bank is known: in retail-bank mode whether any open retail bank's entry index holds it, otherwise whether it is a registered bank
// Leaf: ?BankExists@CBankFileManager@@QBE_NABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A80B0, 145 bytes, thiscall `ret 4`. PDB (Ego_r): `public: bool __fastcall
// CBankFileManager::BankExists(class CCharString const &) const`. Catalog name "GetLanguageData" is wrong (the callee-canon
// label file already says BankExists). Callers: eight sites 0x00415099..0x00415236 and 0x009D9B5A.
// Callees: BankAliasMap.find @ 0x009AB5D0, CCharString copy ctor @ 0x0099EC30, a retail bank's entry-index find @ 0x009AB4F0
// (map at +0x18 of the bank; as the landed FindContainedBank 0x009A7F80), Banks.find @ 0x009AB560, ~CCharString @ 0x0099EAE0.
// Layout: retail CBankFileManager: Banks +0x04, RetailBanks +0x10 (STLport list<CCountedPointer<NBankFileManager::
// CRetailBank> > head node; the node holds the pointer at +8), UsingRetailBanks +0x14, BankAliasMap +0x18.
// Shape: same-TU noinline bodies for the three map finds and CBasicString<char>::operator< (VC7.1 then caches the
// list head across the calls), each find result a named local compared with end() after the call; the two-return
// LookupBankAlias helper (as GetDevBankFilePath 0x009A7CA0: each arm pushes its own copy source), the retail
// branch walks the list through the counted pointer's operator-> without copying it; separate `return true` /
// `return false` exits (mov al,1 / xor al,al), the false exit laid right after the loop.
// Port (NF177, q4391 / q4392): moved off the row-local CBasicStringChar / CCharString / CRetailMap / CRetailList / CBankFileManager
// models onto fable_string.h, stlport/fable_containers.h (map, list) and fable_retail_banks.h. The three map finds are the STLport const
// member-template find over the out-of-line _M_find (0x009AB5D0 BankAliasMap, 0x009AB4F0 a retail bank's ContainedBanks, 0x009AB560
// Banks; D1), whose bodies this TU sees but keeps out of line (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE), with the one CBasicString<char>
// operator< 0x00429950 body (FABLE_RETAIL_CBASICSTRING_LESS_BODY): VC7.1 then keeps the list head in a register across the calls.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKEXISTS
#define FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP
#define FABLE_RETAIL_CBANKFILEMANAGER_LOOKUPBANKALIAS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"

bool CBankFileManager::BankExists(const CCharString& bankHandle) const
{
    CCharString name = LookupBankAlias(bankHandle);
    if (UsingRetailBanks) {
        for (std::list<CCountedPointer<NBankFileManager::CRetailBank> >::const_iterator it = RetailBanks.begin();
             it != RetailBanks.end(); ++it) {
            std::map<CCharString, NBankFileManager::CContainedBank>::const_iterator entry = (*it)->ContainedBanks.find(name);
            if (entry != (*it)->ContainedBanks.end())
                return true;
        }
    }
    else {
        std::map<CCharString, CBankInfo>::const_iterator info = Banks.find(name);
        if (info != Banks.end())
            return true;
    }
    return false;
}
