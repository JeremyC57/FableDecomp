#pragma once

#include <stddef.h>

#include "fable_string.h"

class CBasicRetailBankAliasMap
{
public:
    CCharString& operator[](const CCharString& key);
};

class CBasicRetailBankManager
{
public:
    void SetNavigatorEnabled(bool enabled);
    void AddBankAlias(
        const CCharString& genericName,
        const CCharString& platformName);

    fable_u8 unknown00_[0x14];
    bool navigatorEnabled_;
    fable_u8 unknown15_[3];
    CBasicRetailBankAliasMap aliases_;
};

FABLE_STATIC_ASSERT(
    offsetof(CBasicRetailBankManager, navigatorEnabled_) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(CBasicRetailBankManager, aliases_) == 0x18);

#ifdef FABLE_RETAIL_CBANKFILEMANAGER
// Retail opt-in (chunk 775, rows 0x009A8840 OpenRetailBank / 0x009A8170 OpenIniFile): Ego_r.pdb member names and order with the
// retail STLport sizes (map/set 12 bytes, list 4), so FileVersion lands on +0x34 as the default model's retailBankVersion_.
// NBankFileManager::CRetailBank (vtable 0x0129B010, ctor 0x009AC4D0): DiskFile +0x04, ThreadedFile +0x0C, UseDMA +0x14,
// ContainedBanks +0x18, FilePath +0x24 (sizeof 0x28, operator new(0x28) at 0x009A89A0).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CDiskFileWin32;
class CThreadedFile;
class CBankFile;
class CDataInputStream;
namespace NBankFileManager
{
    class CContainedBank
    { public:
        unsigned long DataVersion;                                       // +0x00
        unsigned long EntryCount;                                        // +0x04
        unsigned long InfoOffset;                                        // +0x08
        unsigned long InfoSize;                                          // +0x0C
        unsigned long Alignment;                                         // +0x10
#ifdef FABLE_RETAIL_NBANKFILEMANAGER_CCONTAINEDBANK_CTOR
        // NF85 opt-in (row 0x009AC530 map<CCharString, CContainedBank>::operator[]): FableWin.pdb ??0CContainedBank@NBankFileManager@@QAE@XZ,
        // the user default constructor (all five words zero), inline: STLport's `value_type(k, _Tp())` then builds the zeroed default in
        // its own frame temporary and copies it into the pair, as retail 0x009AC530 does (`sub esp,30h`); without it VC7.1 folds the
        // value-initialised POD straight into the pair. Layout unchanged.
        CContainedBank() : DataVersion(0), EntryCount(0), InfoOffset(0), InfoSize(0), Alignment(0) {}
#endif
    };
    class CRetailBank : public CBaseClassNonCopyable
    {
    public:
        CRetailBank();                                                   // 0x009AC4D0
        CCountedPointer<CDiskFileWin32> DiskFile;                        // +0x04
        CCountedPointer<CThreadedFile> ThreadedFile;                     // +0x0C
        bool UseDMA;                                                     // +0x14
        std::map<CCharString, CContainedBank> ContainedBanks;            // +0x18
        CCharString FilePath;                                            // +0x24
#ifdef FABLE_RETAIL_NBANKFILEMANAGER_CRETAILBANK_GETTHREADEDFILE
        // NF177 opt-in (row 0x009D56C0 CBankFileAsync::OpenReadOnly): FableWin.pdb
        // ?GetThreadedFile@CRetailBank@NBankFileManager@@QAE?AV?$CCountedPointer@VCThreadedFile@@@@XZ, inline in retail (no Fable.exe copy:
        // the caller copies ThreadedFile +0x0C into its temporary in place, 0x009D5779). Needs the CThreadedFile type (fable_threaded_file.h).
        CCountedPointer<CThreadedFile> GetThreadedFile() { return ThreadedFile; }
#endif
    };
}
FABLE_STATIC_ASSERT_AT(66, sizeof(NBankFileManager::CContainedBank) == 0x14);
FABLE_STATIC_ASSERT_AT(67, sizeof(NBankFileManager::CRetailBank) == 0x28);

class CBankFileManager : public CBaseClass
{
public:
    class CBankInfo
    {
    public:
        CWideString DevBankFilePath;                                     // +0x00
        CWideString RetailBankFilePath;                                  // +0x04
        CWideString DevHeaderFilePath;                                   // +0x08
        CWideString RetailHeaderFilePath;                                // +0x0C
        CBankFile* RegisteredBank;                                       // +0x10
        // Ego_r.pdb ??0CBankInfo@CBankFileManager@@QAE@XZ / QAE@ABVCWideString@@000@Z, inline in retail (0x009A840B: four copy
        // constructions into the temporary, RegisteredBank = 0 last).
        CBankInfo() : RegisteredBank(0) { }
        CBankInfo(const CWideString& devBankFilePath, const CWideString& retailBankFilePath,
                  const CWideString& devHeaderFilePath, const CWideString& retailHeaderFilePath)
            : DevBankFilePath(devBankFilePath), RetailBankFilePath(retailBankFilePath),
              DevHeaderFilePath(devHeaderFilePath), RetailHeaderFilePath(retailHeaderFilePath), RegisteredBank(0) { }
    };
    void OpenRetailBank(const CWideString& pathname, bool startupPriority);   // 0x009A8840
    void OpenIniFile(const CWideString& pathname);                            // 0x009A8170
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_CLOSERETAILBANK
    // Opt-in (chunk 2051, row 0x0042EC7C CNewFrontendGameComponent::Run): Ego_r.pdb ?CloseRetailBank@CBankFileManager@@QAEXABVCWideString@@@Z, retail 0x009A78D0 (landed). Declaration only.
    void CloseRetailBank(const CWideString& name);                            // 0x009A78D0
#endif
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
    // Retail opt-in (chunk 849, row 0x009CF3A0 CBankFile::WriteSymbolHeaderFile): Ego_r.pdb / FableWin.pdb
    // ?GetDevHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z (0x009A7E20) and
    // ?GetRetailHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z (0x009A7ED0), out of line. Declarations only.
    CWideString GetDevHeaderFilePath(const CCharString& bankHandle) const;      // 0x009A7E20
    CWideString GetRetailHeaderFilePath(const CCharString& bankHandle) const;   // 0x009A7ED0
#endif
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_BANKFILEPATHS
    // NF177 opt-in (rows 0x009A7CA0 / 0x009A7D40): Ego_r.pdb + FableWin.pdb
    // ?GetDevBankFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z (const) and
    // ?GetRetailBankFilePath@CBankFileManager@@QAE?AVCWideString@@ABVCCharString@@@Z (not const), out of line. Declarations only.
    CWideString GetDevBankFilePath(const CCharString& bankHandle) const;        // 0x009A7CA0
    CWideString GetRetailBankFilePath(const CCharString& bankHandle);           // 0x009A7D40
#endif
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_FINDCONTAINEDBANK
    // NF177 opt-in (row 0x009D56C0): Ego_r.pdb ?FindContainedBank@CBankFileManager@@QAE_NABVCCharString@@AAVCContainedBank@NBankFileManager@@
    // AAV?$CCountedPointer@VCRetailBank@NBankFileManager@@@@@Z (0x009A7F80, landed; both outputs by reference). Declaration only.
    bool FindContainedBank(const CCharString& name, NBankFileManager::CContainedBank& contained_bank,
                           CCountedPointer<NBankFileManager::CRetailBank>& retail_bank);   // 0x009A7F80
#endif
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_BANKEXISTS
    // NF177 opt-in (row 0x009A80B0): Ego_r.pdb + FableWin.pdb ?BankExists@CBankFileManager@@QBE_NABVCCharString@@@Z. Declaration only.
    bool BankExists(const CCharString& bankHandle) const;                      // 0x009A80B0
#endif
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_LOOKUPBANKALIAS
    // NF177 opt-in (rows 0x009A7CA0 .. 0x009A80B0; needs FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP): Ego_r.pdb
    // ?LookupBankAlias@CBankFileManager@@QBE?AVCCharString@@ABV2@@Z, inline in retail (no Fable.exe copy; each caller looks the name
    // up in BankAliasMap through its _M_find 0x009AB5D0 and copy-constructs the alias value or the argument into its own local, one
    // copy-constructor push per return, as the landed 0x009A7F80 model).
    CCharString LookupBankAlias(const CCharString& alias) const
    {
        std::map<CCharString, CCharString>::const_iterator it = BankAliasMap.find(alias);
        if (it != BankAliasMap.end())
            return (*it).second;
        return alias;
    }
#endif
    void Close();
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_SETBANKALIAS
    // NF87 opt-in (row 0x009A8150): Ego_r.pdb + FableWin.pdb ?SetBankAlias@CBankFileManager@@QAEXABVCCharString@@0@Z; Ego_d.pdb names the
    // parameters bank_handle, alias (BankAliasMap[alias] = bank_handle).
    void SetBankAlias(const CCharString& bank_handle, const CCharString& alias);   // 0x009A8150
#endif
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_ISUSINGRETAILBANKS
    // Retail opt-in (wfE7, row 0x00B3CB30): FableWin.pdb ?IsUsingRetailBanks@CBankFileManager@@QBE_NXZ (0x02DD27A0; no Ego_r.pdb or Fable.exe
    // copy: inline, the CEngineResourceManager ctor tests the byte UsingRetailBanks +0x14 = 0x013CA7B0 in place, 0x00B3CD56).
    bool IsUsingRetailBanks() const { return UsingRetailBanks; }
#endif
private:
    bool ReadQuotedString(CCharString& text, CDataInputStream& stream);   // 0x009A77D0
protected:
    std::map<CCharString, CBankInfo> Banks;                              // +0x04
    std::list<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;   // +0x10
    bool UsingRetailBanks;                                               // +0x14
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP
    // NF87 opt-in (row 0x009A8150 SetBankAlias): the member as its Ego_r.pdb type, the retail 12-byte STLport map (operator[] 0x009AC700).
    std::map<CCharString, CCharString> BankAliasMap;                     // +0x18
#else
    unsigned char BankAliasMap[0x0C];                                    // +0x18 std::map<CCharString,CCharString>
#endif
    CWideString BasePath;                                                // +0x24
    unsigned char CreatedRetailBanks[0x0C];                              // +0x28 std::set<CWideString>
    unsigned long FileVersion;                                           // +0x34
};
FABLE_STATIC_ASSERT_AT(113, sizeof(CBankFileManager::CBankInfo) == 0x14);
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
// (same opt-in) the manager singleton NBankFileManager::GBankFileManager at 0x013CA79C: both PDBs name its
// "NBankFileManager::`dynamic initializer for 'GBankFileManager''" and atexit destructor.
namespace NBankFileManager { extern CBankFileManager GBankFileManager; }   // 0x013CA79C
#endif
#else
namespace NBankFileManager
{
    class CContainedBank
    { public:
        fable_u32 dataVersion_;
        fable_u32 entryCount_;
        fable_u32 infoOffset_;
        fable_u32 infoSize_;
        fable_u32 alignment_;
    };

    class CContainedBankMap
    {
    public:
        CContainedBank& operator[](const CCharString& key);

        void* head_;
        fable_u32 size_;
        fable_u32 allocator_;
    };

    class CRetailBank
    {
    public:
        CRetailBank();

        fable_u32 vtable_;
        void* diskFile_;
        void* diskFileControl_;
        void* threadedFile_;
        void* threadedFileControl_;
        bool useDma_;
        fable_u8 padding15_[3];
        void* containedBanksHead_;
        fable_u32 containedBankCount_;
        fable_u32 containedBanksAllocator_;
        void* filePathStorage_;
    };
}

FABLE_STATIC_ASSERT_AT(160, sizeof(NBankFileManager::CContainedBank) == 0x14);
FABLE_STATIC_ASSERT_AT(161, sizeof(NBankFileManager::CContainedBankMap) == 0x0C);
FABLE_STATIC_ASSERT_AT(162, sizeof(NBankFileManager::CRetailBank) == 0x28);
FABLE_STATIC_ASSERT_AT(163, 
    offsetof(NBankFileManager::CRetailBank, diskFile_) == 0x04);
FABLE_STATIC_ASSERT_AT(165, 
    offsetof(NBankFileManager::CRetailBank, threadedFile_) == 0x0C);
FABLE_STATIC_ASSERT_AT(167, 
    offsetof(NBankFileManager::CRetailBank, useDma_) == 0x14);
FABLE_STATIC_ASSERT_AT(169, 
    offsetof(NBankFileManager::CRetailBank, containedBanksHead_) == 0x18);
FABLE_STATIC_ASSERT_AT(171, 
    offsetof(NBankFileManager::CRetailBank, filePathStorage_) == 0x24);

class CBankFileManager
{
public:
    class CBankInfo
    {
    public:
        void* devBankFilePath_;
        void* retailBankFilePath_;
        void* devHeaderFilePath_;
        void* retailHeaderFilePath_;
        void* registeredBank_;
    };

    void OpenRetailBank(
        const CWideString& pathname,
        bool startupPriority);
    void OpenIniFile(const CWideString& pathname);
    void Close();

    fable_u8 unknown00_[0x24];
    void* basePathStorage_;
    fable_u8 unknown28_[0x0C];
    fable_u32 retailBankVersion_;
};

FABLE_STATIC_ASSERT_AT(199, sizeof(CBankFileManager::CBankInfo) == 0x14);
FABLE_STATIC_ASSERT_AT(200, 
    offsetof(CBankFileManager, basePathStorage_) == 0x24);
FABLE_STATIC_ASSERT_AT(202, 
    offsetof(CBankFileManager, retailBankVersion_) == 0x34);
#endif // FABLE_RETAIL_CBANKFILEMANAGER
