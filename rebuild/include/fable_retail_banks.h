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
    };
}
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBank) == 0x14);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CRetailBank) == 0x28);

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
#ifdef FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
    // Retail opt-in (chunk 849, row 0x009CF3A0 CBankFile::WriteSymbolHeaderFile): Ego_r.pdb / FableWin.pdb
    // ?GetDevHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z (0x009A7E20) and
    // ?GetRetailHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z (0x009A7ED0), out of line. Declarations only.
    CWideString GetDevHeaderFilePath(const CCharString& bankHandle) const;      // 0x009A7E20
    CWideString GetRetailHeaderFilePath(const CCharString& bankHandle) const;   // 0x009A7ED0
#endif
    void Close();
private:
    bool ReadQuotedString(CCharString& text, CDataInputStream& stream);   // 0x009A77D0
protected:
    std::map<CCharString, CBankInfo> Banks;                              // +0x04
    std::list<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;   // +0x10
    bool UsingRetailBanks;                                               // +0x14
    unsigned char BankAliasMap[0x0C];                                    // +0x18 std::map<CCharString,CCharString>
    CWideString BasePath;                                                // +0x24
    unsigned char CreatedRetailBanks[0x0C];                              // +0x28 std::set<CWideString>
    unsigned long FileVersion;                                           // +0x34
};
FABLE_STATIC_ASSERT(sizeof(CBankFileManager::CBankInfo) == 0x14);
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

FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBank) == 0x14);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBankMap) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CRetailBank) == 0x28);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, diskFile_) == 0x04);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, threadedFile_) == 0x0C);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, useDma_) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, containedBanksHead_) == 0x18);
FABLE_STATIC_ASSERT(
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

FABLE_STATIC_ASSERT(sizeof(CBankFileManager::CBankInfo) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, basePathStorage_) == 0x24);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, retailBankVersion_) == 0x34);
#endif // FABLE_RETAIL_CBANKFILEMANAGER
