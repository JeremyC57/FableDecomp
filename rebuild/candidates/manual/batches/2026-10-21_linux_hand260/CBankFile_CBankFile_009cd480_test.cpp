#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_packed, g_strings, g_wide, g_cacheSize = -1;
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() {} };
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
struct CWideString { void* PStringData; CWideString() : PStringData(0) { ++g_wide; } };
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A { T _M_data; _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { } };
template <class T> struct _Vector_base {
    T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CArray : _Vector_base<T> { explicit CArray(const allocator<T>& a = allocator<T>()) : _Vector_base<T>(a) { } };
struct CPackedUIntArray { unsigned int* PackedInts; unsigned int Size; unsigned char Bits; unsigned int Bias;
    CPackedUIntArray() : PackedInts(0), Size(0), Bits(0), Bias(0) { ++g_packed; } };
struct CDiskFilePointer { void* Data; void* Info; CDiskFilePointer() : Data(0), Info(0) {} };
struct SRbNode { bool _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; void* _M_key; unsigned int _M_value; };
struct CSymbolTree {
    SRbNode* _M_header; unsigned int _M_node_count; unsigned char _M_key_compare;
    CSymbolTree() : _M_header(0)
    {
        _M_header = (SRbNode*)std::malloc(sizeof(SRbNode));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
struct SKeyPair { unsigned int Key, Value; };
struct CCRCMap { CArray<SKeyPair> Entries; unsigned char Compare; bool Dirty; CCRCMap() : Dirty(false) {} };
struct CChecksumCacheFile { int _body[7]; CChecksumCacheFile(int bufferSize) { g_cacheSize = bufferSize; } };
struct CBankFile : CBaseClassNonCopyable {
    unsigned int Size;
    CArray<CCharString> Symbols; CArray<unsigned int> Checksums; CArray<unsigned int> RuntimeData; CArray<void*> UpdateData;
    CPackedUIntArray PackedDataOffset, PackedDataSize, PackedDataType, PackedValid;
    unsigned int OpenFlags;
    CDiskFilePointer BankFile;
    CWideString Filename;
    CCharString BankHandle;
    bool RetailMode, ReadOnly, WrittenToFile, Solid;
    unsigned int ChangeCount;
    bool FileValid, UpdatingFlag;
    unsigned int Alignment, BankFormatVersion, HeaderSize, FileInformationSize, FileSize;
    CSymbolTree SymbolIndices, OldSymbolIndices, FilenameIndices;
    CCRCMap SymbolCRCIndices;
    CArray<unsigned int> BlockOrder;
    void* ProgressCallback;
    CCharString HeaderEnumerationType;
    bool WarningFlag;
    CChecksumCacheFile ChecksumCache;
    CBankFile() : ChecksumCache(0x10000)
    {
        HeaderSize = 0; FileInformationSize = 0; FileSize = 0; ProgressCallback = 0; Size = 0;
        BankFormatVersion = 0x73; Alignment = 1;
    }
};
int main()
{
    void* mem = std::malloc(sizeof(CBankFile));
    std::memset(mem, 0xcd, sizeof(CBankFile));
    CBankFile* b = new (mem) CBankFile;
    bool ok = b->Size == 0 && b->Symbols._M_start == 0 && b->UpdateData._M_end_of_storage._M_data == 0
        && g_packed == 4 && g_strings == 2 && g_wide == 1 && g_cacheSize == 0x10000
        && b->BankFile.Data == 0 && b->BankFile.Info == 0 && b->BankFormatVersion == 0x73 && b->Alignment == 1
        && b->HeaderSize == 0 && b->FileInformationSize == 0 && b->FileSize == 0 && b->ProgressCallback == 0
        && b->SymbolIndices._M_node_count == 0 && b->SymbolIndices._M_header->_M_left == b->SymbolIndices._M_header
        && b->FilenameIndices._M_header->_M_right == b->FilenameIndices._M_header && b->OldSymbolIndices._M_header->_M_parent == 0
        && !b->OldSymbolIndices._M_header->_M_color && !b->SymbolCRCIndices.Dirty && b->BlockOrder._M_finish == 0
        && b->SymbolIndices._M_header != b->OldSymbolIndices._M_header;
    if (!ok) { std::printf("009cd480_TEST FAIL\n"); return 1; }
    std::printf("009cd480_TEST PASS\n");
    return 0;
}
