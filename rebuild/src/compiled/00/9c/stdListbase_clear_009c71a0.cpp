// std::_List_base::clear() -- (_List_base<CFileInstaller::CActiveFile>) STLport 4.6 list clear for CFileInstaller::ActiveFiles: walk the nodes, run each active file's implicit destructor (both pending lists, the zlib decompressor, the file-info counted pointer) and free the node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C71A0, 264 bytes, thiscall. Callers: 0x009C5296, 0x009C7583, 0x009C7763 (the
//   unreferenced list destructors beside it). The chunk label "DataItemList_DestroyAll" is descriptive.
// Element: CFileInstaller::CActiveFile, retail 0xB0 bytes (copy constructor 0x009C72B0): PFileInfo CCountedPointer at +0x1C
//   (Data +0x1C, Info +0x20), StartTime +0x30, Decompressor CCompressorZlib at +0x38 (retail 0x58 bytes), PendingWrites
//   list<CPendingWrite> +0x90, PendingDecompression list<CActiveRead*> +0x94, DecompressSrcPos.. +0x98..+0xA8,
//   DecompressFinish +0xAC. engine/CActiveFile.h is the Ego_r.pdb Dinkumware layout (12-byte lists, 0x5C-byte
//   compressor) and does not fit the bytes, so the model is local.
// Callees: CCompressorZlib::~CCompressorZlib @ 0x00A3ACC0, CCPPointerInfo delete function through Info+4 (__fastcall, ecx =
//   Info->Data), 0x00BFE9BC operator delete (the info block), 0x00BFEA14 free (list nodes and sentinels).
// Shape: node frees have no null test, the pending-list sentinel frees do (~_List_base); the counted pointer re-reads
//   Info after the decrement and clears Data/Info last.
extern "C" void __cdecl free(void* block);                          // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    ~_List_base()
    {
        clear();
        if (_M_node._M_data != 0)
            _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
};

}

class CCPPointerInfo { public:
    unsigned long RefCount;                         // +0x00
    void (__fastcall* DeleteFunc)(void* data);      // +0x04
    void* Data;                                     // +0x08
};

template <class T> class CCountedPointer {
public:
    T* Data;                                        // +0x00
    CCPPointerInfo* Info;                           // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CCompressorZlib {                             // retail 0x58 bytes
public:
    unsigned char _data[0x58];
    ~CCompressorZlib();                             // 0x00A3ACC0
};

class CFileInstaller {
public:
    class CFileInfo;
    class CActiveRead;
    class CBufferChunk;
    class CPendingWrite { public:                          // 0x0C bytes
        unsigned long Offset;
        unsigned long Size;
        CBufferChunk* Buffer;
    };
    class CActiveFile { public:                            // retail 0xB0 bytes
        void* SrcFileHandle;                        // +0x00
        void* DstFileHandle;                        // +0x04
        unsigned long CurrentSrcOffset;             // +0x08
        unsigned long NoBytesRead;                  // +0x0C
        unsigned long NoBytesWritten;               // +0x10
        unsigned long InputSize;                    // +0x14
        unsigned long OutputSize;                   // +0x18
        CCountedPointer<const CFileInfo> PFileInfo; // +0x1C
        unsigned long MinWriteSize;                 // +0x24
        unsigned long MinReadSize;                  // +0x28
        double StartTime;                           // +0x30
        CCompressorZlib Decompressor;               // +0x38
        std::list<CPendingWrite> PendingWrites;     // +0x90
        std::list<CActiveRead*> PendingDecompression;   // +0x94
        unsigned long DecompressSrcPos;             // +0x98
        unsigned long DecompressDstPos;             // +0x9C
        CBufferChunk* PDecompressSrcBuffer;         // +0xA0
        CBufferChunk* PDecompressDstBuffer;         // +0xA4
        unsigned long DecompressDstUsed;            // +0xA8
        bool DecompressFinish;                      // +0xAC
    };
};

template void std::_List_base<CFileInstaller::CActiveFile, std::allocator<CFileInstaller::CActiveFile> >::clear();