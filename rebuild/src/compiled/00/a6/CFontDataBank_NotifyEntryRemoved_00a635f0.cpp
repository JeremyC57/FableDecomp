// CFontDataBank::NotifyEntryRemoved(unsigned long index) -- Delete the entry and clear its slot.
// Leaf: ?NotifyEntryRemoved@CFontDataBank@@MAEXK@Z
// Identity: own vtable129da84 slot14 and original PDB protected virtual method. Delete through original entry base; reread array after virtual deletion.
#define CBankFile CBankFile_HeaderLayout
#include "engine/CBankFile.h"
#undef CBankFile
#define CFontDataBank CFontDataBank_HeaderLayout
#include "engine/CFontDataBank.h"
#undef CFontDataBank
#define CFontDataBankEntry CFontDataBankEntry_HeaderLayout
#include "engine/CFontDataBankEntry.h"
#undef CFontDataBankEntry
class CBankFileEntryDependencies;
class CCPPointerInfo;
class CCharStringData;
class CDiskFileWin32;
struct CProgressCallback;
class CWideStringData;

class CBaseClass{public:virtual ~CBaseClass(){}};class CBaseClassNonCopyable:public CBaseClass{};
#pragma pack(push,1)
class CBankFile:public CBaseClassNonCopyable{public:
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
};class CBankFileAsync:public CBankFile{public:unsigned char UnusedAsyncPayload[84];};
#pragma pack(pop)
namespace std{template<class T>class allocator{};template<class T,class A>struct _STLP_alloc_proxy:A{T _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T& operator[](unsigned long n){return this->_M_start[n];}};}
template<class T>class CArray:public std::vector<T>{};
// Layout: active owner/entry headers are flattened; restore original RTTI CBankFile/CBankFileAsync bases and CBaseClassNonCopyable entry. Shared bank272/async356 retained; Fonts at288. StateBlock opaque and untouched, real CArray pointers. Entry fields are never accessed, only its original virtual destructor.
class CFontDataBankEntry:public CBaseClassNonCopyable{public:virtual ~CFontDataBankEntry(){}};
class CFontDataBank:public CBankFile{public:unsigned char StateBlock[16];CArray<CFontDataBankEntry*> Fonts;protected:virtual void NotifyEntryRemoved(unsigned long);};
void CFontDataBank::NotifyEntryRemoved(unsigned long index){delete Fonts[index];Fonts[index]=0;}