// CBankFile::OpenReadOnly(const CCharString& bankHandle, unsigned long flags) -- reset the open state read-only; in retail-bank mode (not forced to disk) find the bank inside its retail container, check the data format version, share the container's disk file and read the entry list from the contained bank's info block; otherwise open the dev bank file by path
// Grade: DIFFER(385v385) -- block order only: retail lays the FindContainedBank-failed exit (~CCountedPointer<CRetailBank> + return false) inline right after the find test and the version check jumps back to it; VC7.1 here merges the two identical exits at the version check's position (8 lines, all from that one block move). 16 probes (probe/009d06f0.log). Next step: a same-TU noinline body for CBankFileManager::FindContainedBank / ~CCountedPointer<CRetailBank> (branch-weight facts), or a whole-TU build with the neighbouring CBankFile rows.
// Leaf: ?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D06F0, 385 bytes, thiscall `ret 8`, returns bool; CBankFile vtable 0x0129B54C slot 1
//   (rtti-vtables.tsv; slot 2 OpenReadWrite 0x009CB960, slot 3 OpenFileReadOnly 0x009D1090 landed). Ego_r.pdb + FableWin.pdb
//   ?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z (the catalog's mangled label is right).
// Callees: CCharString::operator= 0x0099EFB0; on g_BankFileManager 0x013CA79C (UsingRetailBanks +0x14 = 0x013CA7B0, inline
//   IsUsingRetailBanks): FindContainedBank 0x009A7F80, GetDevBankFilePath 0x009A7CA0 (BasePath + the bank info's first path,
//   throws when unknown); ~CCountedPointer<NBankFileManager::CRetailBank> 0x009A9D60, CCountedPointer<CDiskFileWin32>
//   operator= 0x009A9BF0 and dtor 0x009A9BB0 (out of line; the copy ctor of the inline CRetailBank::GetDiskFile result is
//   inline), CFileDataInputStream::CFileDataInputStream(CAFile&, unsigned long) 0x00994700, CDataInputStream::SetPosition
//   0x00993BC0 (virtual, called directly on the local), CBankFile::ReadEntryList 0x009CFBC0, ~CFileDataInputStream 0x00994780,
//   ~CWideString 0x0099B510; this's virtual slots 8 (+0x20, GetDataFormatVersion, pure in CBankFile), 11 (+0x2C,
//   NotifyBankOpened) and 3 (+0x0C, OpenFileReadOnly).
// Layout: engine/CBankFile.h retail offsets (OpenFlags +0x78, BankFile +0x7C, Filename +0x84, BankHandle +0x88, RetailMode +0x8C,
//   ReadOnly +0x8D, WrittenToFile +0x8E, Solid +0x8F, ChangeCount +0x90, FileValid +0x94, UpdatingFlag +0x95, Alignment
//   +0x98); the header is a flat layout with a `__vftable` member, so the class is a local model of it with the virtuals
//   (NOTES.md "Header debt"). NBankFileManager::CContainedBank (5 dwords) and CRetailBank (DiskFile +0x04) from Ego_r.pdb;
//   CBankFileManager UsingRetailBanks +0x14 (Ego_r +0x20 less the STLport map/list savings).
extern "C" void __cdecl free(void* block);
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString { public:
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharStringData* PStringData;
};
class CWideStringData;
class CWideString { public:
    ~CWideString();                                         // 0x0099B510
    CWideStringData* PStringData;
};
class CAFile;
class CDiskFileWin32;
class CDataInputStream { public:                            // 0x18 bytes
    virtual ~CDataInputStream();
    virtual void SetPosition(unsigned long position);       // 0x00993BC0 (Ego_r UAE)
    unsigned long  StreamPos;                               // +0x04
    long           StreamSize;                              // +0x08
    unsigned char* CurrentSourcePtr;                        // +0x0C
    unsigned long  SourceChunkStreamPos;                    // +0x10
    long           SourceChunkBytesRemaining;               // +0x14
};
class CFileDataInputStream : public CDataInputStream { public:   // 0x24 bytes
    CFileDataInputStream(CAFile& file, unsigned long bufferSize);   // 0x00994700
    virtual ~CFileDataInputStream();                        // 0x00994780
    CAFile*        File;                                    // +0x18
    unsigned char* Buffer;                                  // +0x1C
    long           BufferSize;                              // +0x20
};
class CAFile { };
class CDiskFileWin32 : public CAFile { };

namespace NBankFileManager {
class CContainedBank { public:
    CContainedBank() : DataVersion(0), EntryCount(0), InfoOffset(0), InfoSize(0), Alignment(0) { }
    unsigned long DataVersion;                              // +0x00
    unsigned long EntryCount;                               // +0x04
    unsigned long InfoOffset;                               // +0x08
    unsigned long InfoSize;                                 // +0x0C
    unsigned long Alignment;                                // +0x10
};
class CThreadedFile;
class CRetailBank { public:
    virtual ~CRetailBank();
    CCountedPointer<CDiskFileWin32> GetDiskFile() { return DiskFile; }
    CCountedPointer<CDiskFileWin32> DiskFile;               // +0x04
    CCountedPointer<CThreadedFile>  ThreadedFile;           // +0x0C
    bool                            UseDMA;                 // +0x14
};
}
class CBankFileManager { public:
    bool FindContainedBank(const CCharString& bankHandle, NBankFileManager::CContainedBank& containedBank,
                           CCountedPointer<NBankFileManager::CRetailBank>& retailBank);   // 0x009A7F80
    CWideString GetDevBankFilePath(const CCharString& bankHandle) const;                   // 0x009A7CA0
    bool IsUsingRetailBanks() const { return UsingRetailBanks; }
    virtual ~CBankFileManager();
    unsigned char Banks[0xC];                               // +0x04 map<CCharString, CBankInfo>
    unsigned char RetailBanks[0x4];                         // +0x10 list<CCountedPointer<CRetailBank> >
    bool          UsingRetailBanks;                         // +0x14
};
extern CBankFileManager g_BankFileManager;                  // 0x013CA79C

class CBankFile { public:                                   // vtable 0x0129B54C
    virtual ~CBankFile();                                                           // slot 0
    virtual bool OpenReadOnly(const CCharString& bankHandle, unsigned long flags);  // slot 1
    virtual void OpenReadWrite(const CCharString& bankHandle, unsigned long flags, unsigned long alignment);   // slot 2
    virtual bool OpenFileReadOnly(const CWideString& filename, unsigned long flags);                          // slot 3
    virtual void OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment); // slot 4
    virtual void Close();                                                           // slot 5
    virtual void Update();                                                          // slot 6 (order of 6/7 unproven)
    virtual void AddEntryMultipleFiles();                                           // slot 7 (signature not modelled)
protected:
    virtual unsigned long GetDataFormatVersion() = 0;                               // slot 8 (+0x20)
    virtual unsigned long AdjustUpdateEntries();                                    // slot 9
    virtual void NotifyBankSize();                                                  // slot 10 (signature not modelled)
    virtual void NotifyBankOpened();                                                // slot 11 (+0x2C)
public:
    unsigned long Size;                                     // +0x04
    unsigned char Symbols[0xC];                             // +0x08 CArray<CCharString>
    unsigned char Checksums[0xC];                           // +0x14 CArray<unsigned long>
    unsigned char RuntimeData[0xC];                         // +0x20 CArray<CBankFile::CRuntimeData>
    unsigned char UpdateData[0xC];                          // +0x2C CArray<CBankFileEntryUpdateData*>
    unsigned char PackedDataOffset[0x10];                   // +0x38 CPackedUIntArray
    unsigned char PackedDataSize[0x10];                     // +0x48 CPackedUIntArray
    unsigned char PackedDataType[0x10];                     // +0x58 CPackedUIntArray
    unsigned char PackedValid[0x10];                        // +0x68 CPackedUIntArray
    unsigned long OpenFlags;                                // +0x78
    CCountedPointer<CDiskFileWin32> BankFile;               // +0x7C
    CWideString   Filename;                                 // +0x84
    CCharString   BankHandle;                               // +0x88
    bool          RetailMode;                               // +0x8C
    bool          ReadOnly;                                 // +0x8D
    bool          WrittenToFile;                            // +0x8E
    bool          Solid;                                    // +0x8F
    unsigned long ChangeCount;                              // +0x90
    bool          FileValid;                                // +0x94
    bool          UpdatingFlag;                             // +0x95
    unsigned long Alignment;                                // +0x98
private:
    bool ReadEntryList(CFileDataInputStream& stream, unsigned long count, unsigned long entries);   // 0x009CFBC0
};

bool CBankFile::OpenReadOnly(const CCharString& bankHandle, unsigned long flags)
{
    ReadOnly = true;
    OpenFlags = flags;
    BankHandle = bankHandle;
    ChangeCount = 0;
    FileValid = false;
    WrittenToFile = false;
    Solid = false;
    UpdatingFlag = false;
    if (g_BankFileManager.IsUsingRetailBanks() && !(flags & 4)) {
        RetailMode = true;
        NBankFileManager::CContainedBank containedBank;
        CCountedPointer<NBankFileManager::CRetailBank> retailBank;
        if (!g_BankFileManager.FindContainedBank(bankHandle, containedBank, retailBank))
            return false;
        if (containedBank.DataVersion != GetDataFormatVersion())
            return false;
        Alignment = containedBank.Alignment;
        BankFile = retailBank->GetDiskFile();
        CFileDataInputStream stream(*BankFile, 0x4000);
        stream.SetPosition(containedBank.InfoOffset);
        ReadEntryList(stream, containedBank.EntryCount + 1, containedBank.EntryCount);
        NotifyBankOpened();
        return true;
    }
    CWideString path = g_BankFileManager.GetDevBankFilePath(bankHandle);
    return OpenFileReadOnly(path, flags);
}
