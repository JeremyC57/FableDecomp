#ifndef FABLE_ENGINE_CBANKFILEMANAGER_H
#define FABLE_ENGINE_CBANKFILEMANAGER_H
// New header (B24C4, row 0x00402510 GFMain): CBankFileManager, Ego_r.pdb member names and order (76 bytes there, Dinkumware);
// the retail STLport containers make it 0x38 (as the landed constructor 0x009A7AC0 models it: Banks +0x04, RetailBanks +0x10,
// UsingRetailBanks +0x14, BankAliasMap +0x18, BasePath +0x24, CreatedRetailBanks +0x28, FileVersion +0x34; vtable 0x0129B018).
// The member functions are out of line in retail (Ego_r.pdb publics; the addresses GFMain calls). Declarations only.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
class CBankFile;
namespace NBankFileManager { class CRetailBank; }
class CBankFileManager : public CBaseClass {
public:
    class CBankInfo {                                       // Ego_r.pdb: 20 bytes
    public:
        CWideString DevBankFilePath;                        // +0x00
        CWideString RetailBankFilePath;                     // +0x04
        CWideString DevHeaderFilePath;                      // +0x08
        CWideString RetailHeaderFilePath;                   // +0x0c
        CBankFile*  RegisteredBank;                         // +0x10
    };
    void SetUsingRetailBanks(bool using_retail_banks);                              // 0x009A76D0
    void SetBankAlias(const CCharString& alias, const CCharString& bank_name);      // 0x009A8150
    void OpenIniFile(const CWideString& filename);                                  // 0x009A8170
    void OpenRetailBank(const CWideString& filename, bool resident);                // 0x009A8840
private:
    std::map<CCharString, CBankInfo>                           Banks;               // +0x04
    std::list<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;         // +0x10
    bool                                                        UsingRetailBanks;    // +0x14
    std::map<CCharString, CCharString>                         BankAliasMap;        // +0x18
    CWideString                                                 BasePath;            // +0x24
    std::set<CWideString>                                      CreatedRetailBanks;  // +0x28
    unsigned long                                               FileVersion;         // +0x34
};
FABLE_STATIC_ASSERT(sizeof(CBankFileManager) == 0x38);
namespace NBankFileManager {
extern CBankFileManager GBankFileManager;                                           // 0x013CA79C
void __fastcall SetHeaderDir(const CWideString& directory);                         // 0x009A76C0
}
#endif
