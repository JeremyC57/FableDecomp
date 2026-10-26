// CActiveFile::CActiveFile(const CActiveFile& other) -- memberwise copy of a file-installer active file: handles, offsets and sizes, the counted file-info pointer (AddRef), the start time, the zlib decompressor, both STLport lists (fresh sentinel + range insert) and the decompression state
// Leaf: ??0CActiveFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C72B0, length 304 bytes (`ret 4`).
// Callees: CCompressorZlib::CCompressorZlib(const CCompressorZlib&) @ 0x009C7060 (landed), malloc @ 0x00BFEA0E,
//          list<CPendingWrite>::_M_insert_dispatch @ 0x009C69C0, list<CActiveRead*>::_M_insert_dispatch @ 0x009C6A10
//          (manifest "CActiveFile::CopyList_20byte/_12byte": node sizes 0x14 and 0x0C).
// Callers: the CFileInstaller active-file vector's fill/resize paths (0x009C5456, 0x009C76C5, 0x009C77AC, ...).
// Layout: the PDB CActiveFile (CFileInstaller flavour, 200 bytes) with retail containers: the CCompressorZlib is
// 0x58 bytes (landed 0x009C7060: 12-byte vector), each STLport list is its 4-byte sentinel pointer, so the lists
// sit at +0x90/+0x94 and the decompression state at +0x98..+0xac (engine/CActiveFile.h is the PDB layout:
// Decompressor 0x5c, lists 12 bytes -- local model).
// Shape: the compiler's memberwise copy written out; the CCountedPointer copy tests Info after the store
// (`test ; mov ; je ; inc [eax]`); the STLport list copy is `_List_base(alloc)` (proxy zero store, malloc'd
// sentinel linked to itself) + `insert(begin(), x.begin(), x.end())` -> out-of-line `_M_insert_dispatch(pos,
// first, last, __false_type)`. `iterator` has a user copy ctor (built in its argument slot: `push ecx ;
// mov ecx,esp ; mov [ecx],eax`), `const_iterator` only a converting one (plain pushes); the tag is a named
// local (the dead `other` slot, `lea ebp,[esp+14h]`).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CFileInfo;
class CCPPointerInfo { public: long RefCount; };
struct CBufferChunk;

struct CCountedPointerFileInfo {
    CFileInfo*      Data;
    CCPPointerInfo* Info;
    CCountedPointerFileInfo(const CCountedPointerFileInfo& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};

class CCompressorZlib { public:                     // retail 0x58 bytes
    void* _vtable;
    unsigned char _body[0x54];
    CCompressorZlib(const CCompressorZlib& other);   // 0x009C7060
};

struct __false_type {};
template <class T> struct allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : _MaybeReboundAlloc {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : _List_node_base { _Tp _M_data; };

struct _Nonconst_traits {};
struct _Const_traits {};
template <class _Tp, class _Traits> struct _List_iterator;
template <class _Tp> struct _List_iterator<_Tp, _Nonconst_traits> {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp> struct _List_iterator<_Tp, _Const_traits> {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator<_Tp, _Nonconst_traits>& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> struct _List_base {
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    _List_base(const allocator<_Tp>&) : _M_node(allocator<_List_node<_Tp> >(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp> struct list : _List_base<_Tp> {
    typedef _List_iterator<_Tp, _Nonconst_traits> iterator;
    typedef _List_iterator<_Tp, _Const_traits> const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    allocator<_Tp> get_allocator() const { return allocator<_Tp>(); }
    void _M_insert_dispatch(iterator __pos, const_iterator __first, const_iterator __last, const __false_type&);
    void insert(iterator __pos, const_iterator __first, const_iterator __last)
    {
        __false_type __tag;
        _M_insert_dispatch(__pos, __first, __last, __tag);
    }
    list(const list& __x) : _List_base<_Tp>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
};

struct CPendingWrite { unsigned long Offset; void* PBuffer; unsigned long Size; };   // CFileInstaller::CPendingWrite (12 bytes)
struct CActiveRead;

struct CActiveFile {
    void*           SrcFileHandle;          // +0x00
    void*           DstFileHandle;          // +0x04
    unsigned long   CurrentSrcOffset;       // +0x08
    unsigned long   NoBytesRead;            // +0x0c
    unsigned long   NoBytesWritten;         // +0x10
    unsigned long   InputSize;              // +0x14
    unsigned long   OutputSize;             // +0x18
    CCountedPointerFileInfo PFileInfo;      // +0x1c
    unsigned long   MinWriteSize;           // +0x24
    unsigned long   MinReadSize;            // +0x28
    double          StartTime;              // +0x30
    CCompressorZlib Decompressor;           // +0x38
    list<CPendingWrite> PendingWrites;      // +0x90
    list<CActiveRead*>  PendingDecompression;   // +0x94
    unsigned long   DecompressSrcPos;       // +0x98
    unsigned long   DecompressDstPos;       // +0x9c
    CBufferChunk*   PDecompressSrcBuffer;   // +0xa0
    CBufferChunk*   PDecompressDstBuffer;   // +0xa4
    unsigned long   DecompressDstUsed;      // +0xa8
    bool            DecompressFinish;       // +0xac
    CActiveFile(const CActiveFile& other);
};

void list<CPendingWrite>::_M_insert_dispatch(iterator, const_iterator, const_iterator, const __false_type&);   // 0x009C69C0
void list<CActiveRead*>::_M_insert_dispatch(iterator, const_iterator, const_iterator, const __false_type&);    // 0x009C6A10

CActiveFile::CActiveFile(const CActiveFile& other)
    : SrcFileHandle(other.SrcFileHandle), DstFileHandle(other.DstFileHandle), CurrentSrcOffset(other.CurrentSrcOffset),
      NoBytesRead(other.NoBytesRead), NoBytesWritten(other.NoBytesWritten), InputSize(other.InputSize),
      OutputSize(other.OutputSize), PFileInfo(other.PFileInfo), MinWriteSize(other.MinWriteSize),
      MinReadSize(other.MinReadSize), StartTime(other.StartTime), Decompressor(other.Decompressor),
      PendingWrites(other.PendingWrites), PendingDecompression(other.PendingDecompression),
      DecompressSrcPos(other.DecompressSrcPos), DecompressDstPos(other.DecompressDstPos),
      PDecompressSrcBuffer(other.PDecompressSrcBuffer), PDecompressDstBuffer(other.PDecompressDstBuffer),
      DecompressDstUsed(other.DecompressDstUsed), DecompressFinish(other.DecompressFinish)
{
}