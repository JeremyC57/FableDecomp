#ifdef FABLE_RETAIL_TEXTURE_CACHE
#ifndef FABLE_ENGINE_CBANKFILE_H
#define FABLE_ENGINE_CBANKFILE_H
#include "engine/CBaseClassNonCopyable.h"
class CCharString;class CWideString;class CDateAndTime;class CBankStateBlock;
class CBankFileDataTypeCount;class CDataOutputStream;
template<class T>class CArray;template<class T>class CCountedPointer;
// Layout: native complete110-byte class, original noncopyable base and all20 virtual slots.
// 9ccc00/9cd850 member evidence and9d5f80 constructor prove native base boundary.
class CBankFile:public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_TEXTURE_CACHE
 unsigned long GetEntryDataType(unsigned long)const;
 void BeginUpdateEntries();void EndUpdateEntries();
#endif
 CBankFile();virtual ~CBankFile();
public:
 virtual bool OpenReadOnly(const CCharString &,unsigned long);
public:
 virtual void OpenReadWrite(const CCharString &,unsigned long,unsigned long);
public:
 virtual bool OpenFileReadOnly(const CWideString &,unsigned long);
public:
 virtual void OpenFileReadWrite(const CWideString &,unsigned long,unsigned long);
public:
 virtual void Close();
public:
 virtual void AddEntryMultipleFiles(const CCharString &,unsigned long,const CArray<CCharString> &,const CCountedPointer<CBankStateBlock>);
public:
 virtual void Update();
protected:
 virtual unsigned long GetDataFormatVersion();
protected:
 virtual unsigned long AdjustUpdateEntries();
protected:
 virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount> &);
protected:
 virtual void NotifyBankOpened();
protected:
 virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char> &);
protected:
 virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char> &);
protected:
 virtual void NotifyEntryRemoved(unsigned long);
protected:
 virtual void NotifyPostEntriesLoaded();
protected:
 virtual void PrepareDataForInclusion(unsigned long,const CArray<CCharString> &,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream &,CDataOutputStream &);
protected:
 virtual bool IsDiskFile(unsigned long,unsigned long);
protected:
 virtual CDateAndTime GetLastModifiedTime(const CCharString &);
protected:
 virtual CCharString GetEnumerationType(unsigned long) const;
private:
 unsigned long Size;
 unsigned char Symbols[12]; // CArray<CCharString>, native+08
 unsigned char Checksums[12]; // CArray<unsigned long>, native+14
 unsigned char RuntimeData[12]; // CArray<CBankFile::CRuntimeData>, native+20
 unsigned char UpdateData[12]; // CArray<CBankFileEntryUpdateData*>, native+2c
 unsigned char PackedDataOffset[16],PackedDataSize[16],PackedDataType[16],PackedValid[16]; // CPackedUIntArray, native+38..77
 unsigned long OpenFlags;
 unsigned char BankFile[8]; // CCountedPointer<CDiskFileWin32>, native+7c
 unsigned char Filename[4],BankHandle[4]; // CWideString+84, CCharString+88
 bool RetailMode,ReadOnly,WrittenToFile,Solid;
 unsigned long ChangeCount;
 bool FileValid,UpdatingFlag;
 unsigned long Alignment,BankFormatVersion,HeaderSize,FileInformationSize,FileSize;
 unsigned char SymbolIndices[12],OldSymbolIndices[12]; // map<CCharString,unsigned long>, native+ac/+b8
 unsigned char FilenameIndices[12]; // multimap<CCharString,unsigned long>, native+c4
 unsigned char SymbolCRCIndices[16]; // CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> >, native+d0
 unsigned char BlockOrder[12]; // CArray<unsigned long>, native+e0
 unsigned char ProgressCallback[4]; // Original CBankFile::CProgressCallback*, native+ec
 unsigned char HeaderEnumerationType[4]; // CCharString, native+f0
 bool WarningFlag;
 unsigned char _pad_f5[3];
 unsigned char ChecksumCache[24]; // CChecksumCacheFile, native+f8

};
FABLE_STATIC_ASSERT(sizeof(CBankFile)==0x110);
#endif

#else
#if defined(FABLE_RETAIL_STATBOX) || defined(FABLE_RETAIL_TEXTURE_CACHE)
#ifndef FABLE_ENGINE_CBANKFILE_H
#define FABLE_ENGINE_CBANKFILE_H
#include "engine/CBaseClassNonCopyable.h"
class CCharString;class CWideString;class CDateAndTime;class CBankStateBlock;
class CBankFileDataTypeCount;class CDataOutputStream;
template<class T>class CArray;template<class T>class CCountedPointer;
// Layout: native complete110-byte class, original noncopyable base and all20 virtual slots.
// 9ccc00/9cd850 member evidence and9d5f80 constructor prove native base boundary.
class CBankFile:public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_TEXTURE_CACHE
 unsigned long GetEntryDataType(unsigned long)const;
 void BeginUpdateEntries();void EndUpdateEntries();
#endif
 CBankFile();virtual ~CBankFile();
public:
 virtual bool OpenReadOnly(const CCharString &,unsigned long);
public:
 virtual void OpenReadWrite(const CCharString &,unsigned long,unsigned long);
public:
 virtual bool OpenFileReadOnly(const CWideString &,unsigned long);
public:
 virtual void OpenFileReadWrite(const CWideString &,unsigned long,unsigned long);
public:
 virtual void Close();
public:
 virtual void AddEntryMultipleFiles(const CCharString &,unsigned long,const CArray<CCharString> &,const CCountedPointer<CBankStateBlock>);
public:
 virtual void Update();
protected:
 virtual unsigned long GetDataFormatVersion();
protected:
 virtual unsigned long AdjustUpdateEntries();
protected:
 virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount> &);
protected:
 virtual void NotifyBankOpened();
protected:
 virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char> &);
protected:
 virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char> &);
protected:
 virtual void NotifyEntryRemoved(unsigned long);
protected:
 virtual void NotifyPostEntriesLoaded();
protected:
 virtual void PrepareDataForInclusion(unsigned long,const CArray<CCharString> &,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream &,CDataOutputStream &);
protected:
 virtual bool IsDiskFile(unsigned long,unsigned long);
protected:
 virtual CDateAndTime GetLastModifiedTime(const CCharString &);
protected:
 virtual CCharString GetEnumerationType(unsigned long) const;
private:unsigned char _pad_0x04[0x10c];
};
FABLE_STATIC_ASSERT(sizeof(CBankFile)==0x110);
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

#endif
