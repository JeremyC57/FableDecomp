// CBankFile::InternalClose() -- release everything an open bank holds: delete every update-data entry and free the update/runtime/symbol arrays, drop the bank file counted pointer, empty the symbol, old-symbol and filename trees and the CRC vector map, free the four packed arrays and zero the size
// Leaf: InternalClose
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CD850, length 538 bytes (+ int3 padding), plain `ret`.
// Label correction: catalogued as "CDefStringTable::Destructor" (propagated); the body works on the retail CBankFile
//   layout (+0x2c UpdateData .. +0xc4 FilenameIndices), it is called first by CBankFile::~CBankFile (0x009CE180) and
//   CBankFile::Close (0x009CE360, PDB ?Close@CBankFile@@UAEXXZ) is a bare `jmp` to it. Name descriptive.
// Callees: operator delete[] @ 0x00BFEB1C, CSmallVector<CCharString,8>::~CSmallVector @ 0x009D2110 (the entry's
//          filenames), operator delete @ 0x00BFE9BC, memmove @ 0x00BFEAE6, free @ 0x00BFEA14,
//          CArray<CCharString> storage release @ 0x009D3FB0 (the out-of-line ~CArray body), STLport tree _M_erase
//          @ 0x00579435, CPackedUIntArray storage release @ 0x00A629D0 (the out-of-line ~CPackedUIntArray body).
// Layout: the retail CBankFile of the landed CBankFile::CBankFile 0x009CD480 / ClearEntries 0x009CCC00 (12-byte
//   CArrays and trees; engine/CBankFile.h carries the PDB's 16-byte containers).
// Shape: each CArray release is the landed ~CArray<CCharString> 0x009D3FB0 DeallocateMemory (capacity test, erase as
//   `last = copy(last, last, first)`, detach through Exchange, free); pointer elements copy through STLport's
//   __copy_trivial (memmove), 12-byte elements through the counted copy loop; the tree clears and the CRC vector-map
//   clear are the landed ClearEntries forms; the counted pointer reset re-reads its info member after the decrement.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
void operator delete(void* p);                               // 0x00BFE9BC
void operator delete[](void* p);                             // 0x00BFEB1C

template <class P> inline P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template <class T> __forceinline T* CopyRange(T* first, T* last, T* result)
{
    for (long n = last - first; n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}
template <class T> __forceinline T** CopyRange(T** first, T** last, T** result)
{
    return last == first ? result
                         : (T**)((char*)memmove(result, first, (char*)last - (char*)first) + ((char*)last - (char*)first));
}

template <class T> struct CArray {                           // retail STLport vector: first, last, end_of_storage
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    __forceinline void DeallocateMemory()
    {
        if (capacity() > 0) {
            last = CopyRange(last, last, first);
            T* p = Exchange(first, (T*)0);
            Exchange(last, (T*)0);
            end_of_storage = 0;
            if (p)
                free(p);
        }
    }
};
struct CCharString { void* PStringData; };
template <> void CArray<CCharString>::DeallocateMemory();    // 0x009D3FB0 (out of line: the strings are destroyed)

struct CSmallFilenameVector {                                // CSmallVector<CCharString, 8>
    unsigned char _body[8];
    ~CSmallFilenameVector();                                 // 0x009D2110
};
struct CBankFileEntryUpdateData {
    unsigned long        StateBlockCRC;                      // +0x00
    unsigned long        InfoSize;                           // +0x04
    char*                InfoBuffer;                         // +0x08
    CSmallFilenameVector Filenames;                          // +0x0c
    bool                 RequiresUpdate;                     // +0x14
    bool                 Exists;                             // +0x15
    ~CBankFileEntryUpdateData()
    {
        if (InfoBuffer) {
            delete[] InfoBuffer;
            InfoBuffer = 0;
        }
        InfoSize = 0;
    }
};
struct CRuntimeData { unsigned long Offset, Size, Type; };  // 12 bytes
struct SKeyPair { unsigned long Key; unsigned long Value; };

struct CPackedUIntArray {
    unsigned long* PackedInts; unsigned long Size; unsigned char Bits; unsigned long Bias;
    void DeallocateMemory();                                 // 0x00A629D0
};

struct CCPPointerInfo {
    long Count;                                              // +0
    void (__fastcall* Destroy)(void*);                       // +4
    void* Object;                                            // +8
};
struct CDiskFileWin32;
struct CDiskFilePointer {                                    // CCountedPointer<CDiskFileWin32>
    CDiskFileWin32* Data;
    CCPPointerInfo* Info;
    void Reset()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};

struct CSymbolTree {
    struct Node { unsigned char Color; Node* Parent; Node* Left; Node* Right; };
    Node* Head; unsigned long Count; unsigned char Comparator;
    void Erase(Node* node);                                  // 0x00579435
    void Clear()
    {
        if (Count) {
            Erase(Head->Parent);
            Head->Left = Head; Head->Parent = 0; Head->Right = Head; Count = 0;
        }
    }
};
struct CCRCMap {
    SKeyPair* Start; SKeyPair* Finish; SKeyPair* End;
    unsigned char Comparator; bool Dirty;
    void Clear() { Finish = CopyRange(Finish, Finish, Start); }
};

class CBankFile {
public:
    virtual ~CBankFile();
    unsigned long                       Size;                // +0x04
    CArray<CCharString>                 Symbols;             // +0x08
    CArray<unsigned long>               Checksums;           // +0x14
    CArray<CRuntimeData>                RuntimeData;         // +0x20
    CArray<CBankFileEntryUpdateData*>   UpdateData;          // +0x2c
    CPackedUIntArray                    PackedDataOffset;    // +0x38
    CPackedUIntArray                    PackedDataSize;      // +0x48
    CPackedUIntArray                    PackedDataType;      // +0x58
    CPackedUIntArray                    PackedValid;         // +0x68
    unsigned long                       OpenFlags;           // +0x78
    CDiskFilePointer                    BankFile;            // +0x7c
    unsigned char                       _pad84[0xac - 0x84];
    CSymbolTree                         SymbolIndices;       // +0xac
    CSymbolTree                         OldSymbolIndices;    // +0xb8
    CSymbolTree                         FilenameIndices;     // +0xc4
    CCRCMap                             SymbolCRCIndices;    // +0xd0
    void InternalClose();
};

void CBankFile::InternalClose()
{
    for (CBankFileEntryUpdateData** it = UpdateData.first; it != UpdateData.last; ++it) {
        CBankFileEntryUpdateData* entry = *it;
        if (entry)
            delete entry;
    }
    UpdateData.DeallocateMemory();
    RuntimeData.DeallocateMemory();
    Symbols.DeallocateMemory();
    BankFile.Reset();
    SymbolIndices.Clear();
    SymbolCRCIndices.Clear();
    OldSymbolIndices.Clear();
    FilenameIndices.Clear();
    PackedDataOffset.DeallocateMemory();
    PackedDataSize.DeallocateMemory();
    PackedDataType.DeallocateMemory();
    PackedValid.DeallocateMemory();
    Size = 0;
}