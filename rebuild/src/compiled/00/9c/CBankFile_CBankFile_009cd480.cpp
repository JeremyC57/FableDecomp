// CBankFile::CBankFile() -- construct an empty bank: empty entry arrays and packed tables, no file, empty name/handle strings, empty symbol/filename trees and CRC map, no block order, a 64 KB checksum cache; format version 0x73, alignment 1
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CD480, 402 bytes, no arguments.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CPackedUIntArray::CPackedUIntArray @ 0x00A629C0
//          (x4), CWideString::CWideString() @ 0x0099AED0, CCharString::CCharString() @ 0x0099E4B0 (x2), malloc @ 0x00BFEA0E
//          (three 24-byte tree header nodes), CChecksumCacheFile::CChecksumCacheFile(long bufferSize) @ 0x00A60C90
//          (manifest label "CBankFile_ParseHeader" is propagated; called with 0x10000). Vtable: CBankFile 0x0129B54C.
// Layout: the retail CBankFile layout of the landed rows (EndUpdateEntries 0x009CEA70 / ClearEntries): engine/CBankFile.h
//         carries the PDB offsets with 16-byte CArray/map containers, retail CArrays and STLport trees are 12 bytes:
//         Size +4, Symbols/Checksums/RuntimeData/UpdateData CArrays +0x08/+0x14/+0x20/+0x2C, four CPackedUIntArray
//         +0x38..+0x68, OpenFlags +0x78, BankFile counted pointer +0x7C, Filename +0x84, BankHandle +0x88, bools +0x8C..,
//         ChangeCount +0x90, Alignment +0x98, BankFormatVersion +0x9C, HeaderSize +0xA0, FileInformationSize +0xA4,
//         FileSize +0xA8, SymbolIndices/OldSymbolIndices/FilenameIndices trees +0xAC/+0xB8/+0xC4, SymbolCRCIndices
//         CVectorMap +0xD0 (vector, Compare, Dirty +0xDD), BlockOrder +0xE0, ProgressCallback +0xEC,
//         HeaderEnumerationType +0xF0, WarningFlag +0xF4, ChecksumCache +0xF8.
// Shape: the init list in declaration order with one zero register: the CArrays' three words each, the packed arrays'
//        out-of-line ctors, the counted pointer's two words, the strings, each STLport tree (header word zeroed, header
//        node malloc'd, count zeroed, then _M_empty_initialize re-reading the header), the vector map and block order;
//        the remaining scalars are body assignments.
// Leaf: ??0CBankFile
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                         // 0x0099A2F0
};

class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

class CWideString { public:
    void* PStringData;
    CWideString();                                   // 0x0099AED0
};

template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {   // STLport: the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> class CArray : public _Vector_base<T> { public:   // the engine's CArray is the STLport vector (12 bytes in retail)
    explicit CArray(const allocator<T>& a = allocator<T>()) : _Vector_base<T>(a) { }
};

struct CPackedUIntArray {                            // 16 bytes
    unsigned long* PackedInts;
    unsigned long  Size;
    unsigned char  Bits;
    unsigned long  Bias;
    CPackedUIntArray();                              // 0x00A629C0
};

class CDiskFileWin32;
class CCPPointerInfo;
struct CDiskFilePointer {                            // CCountedPointer<CDiskFileWin32>
    CDiskFileWin32* Data;
    CCPPointerInfo* Info;
    CDiskFilePointer() : Data(0), Info(0) {}
};

struct SRbNode { bool _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; CCharString _M_key; unsigned long _M_value; };
struct CSymbolTree {                                 // map / multimap<CCharString, unsigned long>: header, count, compare
    SRbNode*      _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;                    // empty std::less<CCharString>
    CSymbolTree() : _M_header(0)
    {
        _M_header = (SRbNode*)malloc(sizeof(SRbNode));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct SKeyPair { unsigned long Key; unsigned long Value; };
struct CCRCMap {                                     // CVectorMap<ulong, ulong, CKeyPairCompareLess<ulong,ulong>>
    CArray<SKeyPair> Entries;                        // +0x00 vector<pair<ulong,ulong>>
    unsigned char  Compare;                          // +0x0C
    bool           Dirty;                            // +0x0D
    CCRCMap() : Dirty(false) {}
};

struct CChecksumCacheFile {                          // 0x1C
    long _body[7];                                   // Filename, Entries tree, BufferSize, NeedsSaving
    CChecksumCacheFile(long bufferSize);             // 0x00A60C90
};

struct CProgressCallback;

class CBankFile : public CBaseClassNonCopyable { public:           // vtable 0x0129B54C
    unsigned long          Size;                     // +0x04
    CArray<CCharString>    Symbols;                  // +0x08
    CArray<unsigned long>  Checksums;                // +0x14
    CArray<unsigned long>  RuntimeData;              // +0x20 CArray<CBankFile::CRuntimeData>
    CArray<void*>          UpdateData;               // +0x2C CArray<CBankFileEntryUpdateData*>
    CPackedUIntArray       PackedDataOffset;         // +0x38
    CPackedUIntArray       PackedDataSize;           // +0x48
    CPackedUIntArray       PackedDataType;           // +0x58
    CPackedUIntArray       PackedValid;              // +0x68
    unsigned long          OpenFlags;                // +0x78
    CDiskFilePointer       BankFile;                 // +0x7C
    CWideString            Filename;                 // +0x84
    CCharString            BankHandle;               // +0x88
    bool                   RetailMode;               // +0x8C
    bool                   ReadOnly;                 // +0x8D
    bool                   WrittenToFile;            // +0x8E
    bool                   Solid;                    // +0x8F
    unsigned long          ChangeCount;              // +0x90
    bool                   FileValid;                // +0x94
    bool                   UpdatingFlag;             // +0x95
    unsigned long          Alignment;                // +0x98
    unsigned long          BankFormatVersion;        // +0x9C
    unsigned long          HeaderSize;               // +0xA0
    unsigned long          FileInformationSize;      // +0xA4
    unsigned long          FileSize;                 // +0xA8
    CSymbolTree            SymbolIndices;            // +0xAC
    CSymbolTree            OldSymbolIndices;         // +0xB8
    CSymbolTree            FilenameIndices;          // +0xC4
    CCRCMap                SymbolCRCIndices;         // +0xD0
    CArray<unsigned long>  BlockOrder;               // +0xE0
    CProgressCallback*     ProgressCallback;         // +0xEC
    CCharString            HeaderEnumerationType;    // +0xF0
    bool                   WarningFlag;              // +0xF4
    CChecksumCacheFile     ChecksumCache;            // +0xF8
    CBankFile();
};

CBankFile::CBankFile()
    : ChecksumCache(0x10000)
{
    HeaderSize = 0;
    FileInformationSize = 0;
    FileSize = 0;
    ProgressCallback = 0;
    Size = 0;
    BankFormatVersion = 0x73;
    Alignment = 1;
}