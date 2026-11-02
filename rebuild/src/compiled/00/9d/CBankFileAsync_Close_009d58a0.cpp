// CBankFileAsync::Close() -- close the threaded file unless the bank is in retail mode, then close the CBankFile base
// Leaf: ?Close@CBankFileAsync@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D58A0, 36 bytes, thiscall, tail `jmp CBankFile::Close`.
// Identity (proven): slot 5 of three vtables -- CBankFileAsync 0x0129B8D4 (RTTI .?AVCBankFileAsync@@) and the derived banks
//   CWADBank 0x012442DC / CStaticMapBankFile 0x012A1EE4 that inherit it -- where CBankFile's own slot 5 (vtable 0x0129B54C) is
//   CBankFile::Close 0x009CE360, the tail here; derived Close overrides reach it by `jmp` (0x009FED1F, 0x00A26C5B) and one
//   qualified call (0x005022B3). Ego_r.pdb spells ?Close@CBankFileAsync@@UAEXXZ (donor 0x00B29920, a 0x30-byte body) and
//   lays CBankFileAsync out as CBankFile + ThreadedFile (CCountedPointer<CThreadedFile>) first: retail +0x110, the CBankFile
//   boundary engine/CBankFile.h documents (ctor 0x009D5F80). The callee is CThreadedFile::Close (Ego_r / FableWin
//   ?Close@CThreadedFile@@QAEXXZ; catalog agrees). The catalog label (the mangled CBankFileAsync::Close) agrees.
// Callees: CThreadedFile::Close 0x0098EC40, CBankFile::Close 0x009CE360.
// Shape: `if (!ThreadedFile.IsNull() && !RetailMode) ThreadedFile->Close(); CBankFile::Close();` -- the pointer is loaded
//   into ecx before the RetailMode byte test (it is the call's `this`), the qualified base call becomes the tail jump.
// Layout: CBankFile is on an engine-header hold (engine/CBankFile.h is flattened: vptr as a data member), so it is declared
//   locally as the polymorphic class with engine/CBankFile.h's retail member list (RetailMode +0x8C) and the Ego_r.pdb
//   virtual spellings for slots 1-5 (header debt). CCountedPointer from engine/CCountedPointer.h.
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"

class CCharString;
class CBankFileEntryDependencies;
class CCPPointerInfo;
class CCharStringData;
class CDiskFileWin32;
struct CProgressCallback;
class CWideStringData;
class CWideString;
class CThreadedFile {
public:
    void Close();                                           // 0x0098EC40
};

class CBankFile {                                           // vtable 0x0129B54C, retail 0x110 bytes (engine/CBankFile.h members; local: that header is on hold)
public:
    virtual ~CBankFile();                                                           // slot 0
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags);        // slot 1
    virtual void OpenReadWrite(const CCharString& name, unsigned long flags, unsigned long alignment);   // slot 2 0x009CB960
    virtual bool OpenFileReadOnly(const CWideString& name, unsigned long flags);    // slot 3 0x009D1090
    virtual void OpenFileReadWrite(const CWideString& name, unsigned long flags, unsigned long alignment);   // slot 4 0x009D1160
    virtual void Close();                                                           // slot 5 0x009CE360
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

class CBankFileAsync : public CBankFile {                   // vtable 0x0129B8D4
public:
    virtual void Close();                                   // slot 5
    CCountedPointer<CThreadedFile> ThreadedFile;            // +0x110
};

void CBankFileAsync::Close()
{
    if (!ThreadedFile.IsNull() && !RetailMode)
        ThreadedFile->Close();
    CBankFile::Close();
}