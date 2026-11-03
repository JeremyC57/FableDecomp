#ifdef FABLE_RETAIL_BANK_SYMBOLS
#ifndef FABLE_ENGINE_CBANKFILEMANAGER_H
#define FABLE_ENGINE_CBANKFILEMANAGER_H
#include "engine/CBaseClass.h"
#include "engine/CWideString.h"
class CCharString;
class CBankFileManager:public CBaseClass{public:virtual ~CBankFileManager();CWideString GetDevHeaderFilePath(const CCharString&)const;CWideString GetRetailHeaderFilePath(const CCharString&)const;private:unsigned char Banks[12],RetailBanks[4];bool UsingRetailBanks;unsigned char _pad_0x15[3];unsigned char BankAliasMap[12];CWideString BasePath;unsigned char CreatedRetailBanks[12];unsigned long FileVersion;};
namespace NBankFileManager{extern CBankFileManager GBankFileManager;}
#endif
#else

#endif
