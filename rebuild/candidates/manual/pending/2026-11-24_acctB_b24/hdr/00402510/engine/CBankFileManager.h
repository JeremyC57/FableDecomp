#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CBANKFILEMANAGER
#define FABLE_ENGINE_GFMAIN_CBANKFILEMANAGER
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CBaseClass.h"
#include "fable_string.h"
class CCharString;
class CBankFileManager:public CBaseClass{
 unsigned char Banks[12],RetailBanks[4];bool UsingRetailBanks;unsigned char BankAliasMap[12];CWideString BasePath;unsigned char CreatedRetailBanks[12];unsigned long FileVersion;
public:void Close();void SetUsingRetailBanks(bool);void SetBankAlias(const CCharString&,const CCharString&);void OpenIniFile(const CWideString&);void OpenRetailBank(const CWideString&,bool);
};
FABLE_STATIC_ASSERT(sizeof(CBankFileManager)==0x38);
namespace NBankFileManager{extern CBankFileManager GBankFileManager;void __fastcall SetHeaderDir(const CWideString&);}
#endif

#else
#ifdef FABLE_RETAIL_BANK_SYMBOLS
#ifndef FABLE_ENGINE_CBANKFILEMANAGER_H
#define FABLE_ENGINE_CBANKFILEMANAGER_H
#include "engine/CBaseClass.h"
#include "fable_string.h"
class CCharString;
class CBankFileManager:public CBaseClass{public:virtual ~CBankFileManager();CWideString GetDevHeaderFilePath(const CCharString&)const;CWideString GetRetailHeaderFilePath(const CCharString&)const;private:unsigned char Banks[12],RetailBanks[4];bool UsingRetailBanks;unsigned char _pad_0x15[3];unsigned char BankAliasMap[12];CWideString BasePath;unsigned char CreatedRetailBanks[12];unsigned long FileVersion;};
namespace NBankFileManager{extern CBankFileManager GBankFileManager;}
#endif
#else

#endif

#endif
