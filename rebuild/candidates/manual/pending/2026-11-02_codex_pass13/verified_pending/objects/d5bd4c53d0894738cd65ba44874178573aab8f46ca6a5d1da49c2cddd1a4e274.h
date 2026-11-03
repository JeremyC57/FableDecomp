#ifdef FABLE_RETAIL_RENDER2D_TYPES
#ifndef FABLE_ENGINE_CBANKFILE_RENDER2D_H
#define FABLE_ENGINE_CBANKFILE_RENDER2D_H
#include "rebuild_abi.h"
class CBankStateBlock;class CDataInputStream;class CDataOutputStream;class CSymbolMap;class CBankFileDataTypeCount;class CBankFileEntryStats;class CFileDataInputStream;
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CPackedUIntArray.h"
#include "engine/CChecksumCacheFile.h"
#include "engine/CVectorMap.h"
#include "stlport/fable_map.h"
class CDiskFileWin32;class CBankFileEntryUpdateData;
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CBankFile : public CBaseClassNonCopyable {
private: class CRuntimeData {
public: unsigned long DataOffset;
public: unsigned long DataSize;
public: unsigned char DataType;
public: bool Valid;
};

public: class CProgressCallback;
public: static bool TraceLoadingInfo;
private: unsigned long Size;
private: CArray<CCharString> Symbols;
private: CArray<unsigned long> Checksums;
private: CArray<CBankFile::CRuntimeData> RuntimeData;
private: CArray<CBankFileEntryUpdateData *> UpdateData;
private: CPackedUIntArray PackedDataOffset;
private: CPackedUIntArray PackedDataSize;
private: CPackedUIntArray PackedDataType;
private: CPackedUIntArray PackedValid;
private: unsigned long OpenFlags;
private: CCountedPointer<CDiskFileWin32> BankFile;
private: CWideString Filename;
private: CCharString BankHandle;
private: bool RetailMode;
private: bool ReadOnly;
private: bool WrittenToFile;
private: bool Solid;
private: unsigned long ChangeCount;
private: bool FileValid;
private: bool UpdatingFlag;
private: unsigned long Alignment;
private: unsigned long BankFormatVersion;
private: unsigned long HeaderSize;
private: unsigned long FileInformationSize;
private: unsigned long FileSize;
private: std::map<CCharString,unsigned long,std::less<CCharString>,std::allocator<std::pair<CCharString const ,unsigned long> > > SymbolIndices;
private: std::map<CCharString,unsigned long,std::less<CCharString>,std::allocator<std::pair<CCharString const ,unsigned long> > > OldSymbolIndices;
private: std::multimap<CCharString,unsigned long,std::less<CCharString>,std::allocator<std::pair<CCharString const ,unsigned long> > > FilenameIndices;
private: CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> > SymbolCRCIndices;
private: CArray<unsigned long> BlockOrder;
private: CBankFile::CProgressCallback * ProgressCallback;
private: CCharString HeaderEnumerationType;
private: bool WarningFlag;
private: CChecksumCacheFile ChecksumCache;
public: CBankFile();
public: virtual ~CBankFile();
public: virtual bool OpenReadOnly(const CCharString &,unsigned long);
public: virtual void OpenReadWrite(const CCharString &,unsigned long,unsigned long);
public: virtual bool OpenFileReadOnly(const CWideString &,unsigned long);
public: virtual void OpenFileReadWrite(const CWideString &,unsigned long,unsigned long);
public: virtual void Close();
public: const CWideString & GetBankFilename() const;
public: CWideString GetBankPathname() const;
public: CCharString GetBankHandle() const;
public: void SetBankHandle(const CCharString &);
public: bool EntryExists(unsigned long) const;
public: const CBankFileEntryUpdateData * PeekEntryUpdateData(unsigned long) const;
public: bool HasEntrySymbols() const;
public: CCharString GetEntrySymbol(unsigned long) const;
public: unsigned long GetEntryDataType(unsigned long) const;
public: unsigned long GetEntryDataSize(unsigned long) const;
public: unsigned long GetEntryDataOffset(unsigned long) const;
public: unsigned long GetEntryChecksum(unsigned long) const;
public: unsigned long GetEntrySymbolCRC(unsigned long) const;
public: void ReadEntryData(unsigned long,unsigned long,void *,unsigned long);
public: bool EntrySetUpdate(unsigned long,bool);
public: long GetSize() const;
public: long GetMaxEntryIndex() const;
public: unsigned long CountEntries() const;
public: bool IsReadOnly() const;
public: bool IsRetailBank() const;
public: unsigned long GetChangeCount() const;
public: void ClearEntries();
public: void AddEntry(const CCharString &,unsigned long,const CCharString &,const CCountedPointer<CBankStateBlock>);
public: virtual void AddEntryMultipleFiles(const CCharString &,unsigned long,const CArray<CCharString> &,const CCountedPointer<CBankStateBlock>);
public: virtual void Update();
public: void InvalidateEntry(const CCharString &);
public: void BeginUpdateEntries();
public: void UpdateEntry(unsigned long,CDataInputStream &,unsigned long,CDataInputStream &,unsigned long,bool);
public: void UpdateEntry(unsigned long);
public: void UpdateEntryInfoOnly(unsigned long,CDataInputStream &,unsigned long);
public: void EndUpdateEntries();
public: void SetEnumerationType(const CCharString &);
public: void WriteSymbolHeaderFile();
public: void SplitSymbol(const CCharString &,CCharString *,CCharString *);
public: bool IsSymbolPublic(const CCharString &) const;
public: void CreateSymbolMap(CSymbolMap &,bool) const;
public: unsigned long FindIndexBySymbol(const CCharString &) const;
public: unsigned long FindIndexBySymbolCRC(unsigned long) const;
public: unsigned long FindIndexByFilename(const CCharString &,long) const;
public: CArray<unsigned long> FindIndicesByFilename(const CCharString &,long,bool) const;
public: unsigned long FindIndexByPathname(const CCharString &,long) const;
public: const CCharString & GetNextSymbolContaining(const CCharString &,const CCharString &) const;
public: unsigned long GetTotalDataSize() const;
public: unsigned long GetUnusedSpace() const;
public: unsigned long GetAlignment() const;
public: unsigned long Align(unsigned long) const;
public: bool IsAligned(unsigned long) const;
public: void SetProgressCallback(CBankFile::CProgressCallback *);
public: static void AbortBankCreation();
public: static bool IsAborted();
public: void InternalResizeEntryData(unsigned long,unsigned long);
public: void InternalWriteEntryData(unsigned long,unsigned long,const void *,unsigned long);
public: unsigned long InternalWriteEntryList(CDataOutputStream &);
public: unsigned long InternalGetDataFormatVersion();
protected: virtual unsigned long GetDataFormatVersion()=0;
protected: virtual unsigned long AdjustUpdateEntries();
protected: virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount> &);
protected: virtual void NotifyBankOpened();
protected: virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char> &);
protected: virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char> &);
protected: virtual void NotifyEntryRemoved(unsigned long);
protected: virtual void NotifyPostEntriesLoaded();
protected: virtual void PrepareDataForInclusion(unsigned long,const CArray<CCharString> &,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream &,CDataOutputStream &);
protected: virtual bool IsDiskFile(unsigned long,unsigned long);
protected: virtual CDateAndTime GetLastModifiedTime(const CCharString &);
protected: virtual CCharString GetEnumerationType(unsigned long) const;
protected: void SetBankUpdateStatus(const CCharString &);
protected: void WriteBankLog(const CCharString &);
protected: void WriteBankWarning(const CCharString &);
protected: void WriteBankMeshStats(const CBankFileEntryStats &);
protected: void AddDependency(const CCharString &);
protected: void ReadData(unsigned long,void *,unsigned long) const;
private: void CloseEntries();
private: void SetBankUpdateProgress(unsigned long,unsigned long);
private: void WriteData(unsigned long,CDataInputStream &,unsigned long,unsigned long);
private: void WritePadding(unsigned long);
private: void MakeWritable();
private: bool ReadFileInformation(bool);
private: bool ReadEntryList(CFileDataInputStream &,unsigned long,unsigned long);
private: void WriteFileInformation(bool);
private: bool ReadRecoveryFile(unsigned long);
private: void WriteRecoveryFile(unsigned long,unsigned long,unsigned long);
private: void DeleteRecoveryFile();
private: CWideString GetRecoveryFilePath();
private: unsigned long GetRandomValidationValue();
private: void AllocateBuffers(unsigned long);
private: bool IsEntryInitialised(unsigned long) const;
private: CCharString ConvertPathToFilename(const CCharString &) const;
private: void InitEntry(unsigned long,const CCharString &,const CCharString &);
private: void PostInitEntries();
private: void PackRuntimeData();
private: void DeleteEntry(unsigned long);
private: unsigned long FindEmptyBlockOffset(unsigned long);
private: unsigned long GetAllocatedBlockSize(unsigned long);
private: void InitBlockOrder();
private: void InvalidateBlockOrder();
};
#endif
#else
#ifndef FABLE_ENGINE_CBANKFILE_H
#define FABLE_ENGINE_CBANKFILE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py, then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CBankFile = 0x174.
// Retail layout: STLport 4.6 containers (CArray/map/multimap 0xc, CVectorMap 0x10), sizeof 0x110. Every member to
// WarningFlag +0xf4 is at the offset the landed CBankFile::ClearEntries 0x009CCC00 model uses (InternalClose
// 0x009CD850 likewise to SymbolCRCIndices +0xd0); ChecksumCache (0x18) ends at 0x110, the CBankFileAsync base
// boundary of ctor 0x009D5F80 and RTTI mdisp CResourceBank@0x164 in CGraphicDataBank / NGameText::CDataBank.
// The Ego_r Dependencies / CurrentEntryDependencies members are absent from the retail (TLC) object.
// FABLE_RETAIL_BANK_FILE (the former opt-in) is now the only layout; defining it is harmless.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CBankFileEntryDependencies;
class CCPPointerInfo;
class CCharStringData;
class CDiskFileWin32;
struct CProgressCallback;
class CWideStringData;

#pragma pack(push, 1)
class CBankFile {
public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long               Size;                        // +0x04
    unsigned char               Symbols[0xc];                // +0x08 CArray<CCharString>
    unsigned char               Checksums[0xc];              // +0x14 CArray<unsigned_long>
    unsigned char               RuntimeData[0xc];            // +0x20 CArray<CBankFile::CRuntimeData>
    unsigned char               UpdateData[0xc];             // +0x2c CArray<CBankFileEntryUpdateData_*>
    unsigned char               PackedDataOffset[0x10];      // +0x38 CPackedUIntArray
    unsigned char               PackedDataSize[0x10];        // +0x48 CPackedUIntArray
    unsigned char               PackedDataType[0x10];        // +0x58 CPackedUIntArray
    unsigned char               PackedValid[0x10];           // +0x68 CPackedUIntArray
    unsigned long               OpenFlags;                   // +0x78
    CDiskFileWin32*             BankFile_Data;               // +0x7c CCountedPointer<CDiskFileWin32>::Data
    CCPPointerInfo*             BankFile_Info;               // +0x80 CCountedPointer<CDiskFileWin32>::Info
    CWideStringData*            Filename_PStringData;        // +0x84 CWideString::PStringData
    CCharStringData*            BankHandle_PStringData;      // +0x88 CCharString::PStringData
    bool                        RetailMode;                  // +0x8c
    bool                        ReadOnly;                    // +0x8d
    bool                        WrittenToFile;               // +0x8e
    bool                        Solid;                       // +0x8f
    unsigned long               ChangeCount;                 // +0x90
    bool                        FileValid;                   // +0x94
    bool                        UpdatingFlag;                // +0x95
    unsigned char               _pad_0x96[0x2];              // +0x96
    unsigned long               Alignment;                   // +0x98
    unsigned long               BankFormatVersion;           // +0x9c
    unsigned long               HeaderSize;                  // +0xa0
    unsigned long               FileInformationSize;         // +0xa4
    unsigned long               FileSize;                    // +0xa8
    unsigned char               SymbolIndices[0xc];          // +0xac map<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               OldSymbolIndices[0xc];       // +0xb8 map<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               FilenameIndices[0xc];        // +0xc4 multimap<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               SymbolCRCIndices[0x10];      // +0xd0 CVectorMap<unsigned_long,unsigned_long,CKeyPairCompareLess<unsigned_long,unsigned_long>_>
    unsigned char               BlockOrder[0xc];             // +0xe0 CArray<unsigned_long>
    CProgressCallback*          ProgressCallback;            // +0xec
    CCharStringData*            HeaderEnumerationType_PStringData; // +0xf0 CCharString::PStringData
    bool                        WarningFlag;                 // +0xf4
    unsigned char               _pad_0xf5[0x3];              // +0xf5
    unsigned char               ChecksumCache[0x18];         // +0xf8 CChecksumCacheFile
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CBankFile) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CBankFile, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Size) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Symbols) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Checksums) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CBankFile, RuntimeData) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CBankFile, UpdateData) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedDataOffset) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedDataSize) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedDataType) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedValid) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CBankFile, OpenFlags) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankFile_Data) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankFile_Info) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Filename_PStringData) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankHandle_PStringData) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CBankFile, RetailMode) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CBankFile, ReadOnly) == 0x8d);
FABLE_STATIC_ASSERT(offsetof(CBankFile, WrittenToFile) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Solid) == 0x8f);
FABLE_STATIC_ASSERT(offsetof(CBankFile, ChangeCount) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CBankFile, FileValid) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CBankFile, UpdatingFlag) == 0x95);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Alignment) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankFormatVersion) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CBankFile, HeaderSize) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CBankFile, FileInformationSize) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CBankFile, FileSize) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CBankFile, SymbolIndices) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CBankFile, OldSymbolIndices) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CBankFile, FilenameIndices) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CBankFile, SymbolCRCIndices) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CBankFile, BlockOrder) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CBankFile, ProgressCallback) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CBankFile, HeaderEnumerationType_PStringData) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CBankFile, WarningFlag) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CBankFile, ChecksumCache) == 0xf8);

#endif // FABLE_ENGINE_CBANKFILE_H

#endif
