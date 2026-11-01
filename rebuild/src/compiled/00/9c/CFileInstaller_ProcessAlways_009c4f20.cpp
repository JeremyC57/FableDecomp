// CFileInstaller::ProcessAlways() -- process a queued read completion under the lock; bank a finished current write; retire every finished active file at the head of the list; then start writing the next active file
// Retail-volatile: PCompletedRead is written by the read-completion thread; retail re-reads [ebp+74h] for the push after the null test (0x009C4F20: `cmp [ebp+74h],esi ; je ; mov ecx,[ebp+74h]`), the VC7.1 codegen of a volatile member; the landed 0x009C42F0 carries the same qualifier. Owner 2026-11-01: recreate retail.
// Retail-volatile: ExitThread (+0x7A) is written by the main thread (shutdown / SetDirtyDiscError) and re-read by the installer thread: 0x009C35FC `mov al,[eax+7Ah]` then 0x009C3608 `mov byte ptr [eax+7Ah],1` (the discarded read, chunk 680; the landed 0x009C3590 / 0x009C3660 / 0x009C5690 carry the qualifier); not read in this body.
// Leaf: ?ProcessAlways@CFileInstaller@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C4F20, 490 bytes, thiscall `ret`. Name: Ego_r.pdb ?ProcessAlways@CFileInstaller@@IAEXXZ (protected).
// Callees: CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0; CFileInstaller::PostFileReadCompletion 0x009C4120 (Ego_r IAE);
//   list<CCountedPointer<const CFileInfo> >::remove 0x009C6E50; CActiveFile::OnWriteFinished 0x009C38C0;
//   CWideString::ToCharString() const 0x0099B2C0 (Ego_r QBE; bindings carry ConvertToCharString spellings);
//   _Rb_tree<CCharString,...>::insert_unique 0x004AC380; ~CCharString 0x0099EAE0; ~CCompressorZlib 0x00A3ACC0 (UAE);
//   StartWritingFile(CActiveFile&, unsigned long) 0x009C3660 (Ego_r IAE); free 0x00BFEA14 (STLport nodes); operator delete 0x00BFE9BC.
// Shape: ProcessFileReadCompletion (0x009C42F0) is a same-TU non-inline member that VC7.1 inlines; the null test reads
//   PCompletedRead through a `const&` view (that alone gives retail's NoTotalBytesWritten load/add/store); the head-of-list
//   loop tests `!empty() && front().IsWriteFinished()` and takes front() again in the body (retail's `lea esi,[eax+8]`
//   before the OutputSize read); the implicit ~CActiveFile expands the two STLport list dtors, the decompressor dtor and
//   the CCountedPointer dtor inside list::erase. Layout: Ego_r names, offsets re-derived for 4-byte STLport lists (header
//   debt: no engine/CFileInstaller.h; engine/CActiveFile.h flattens its lists and decompressor, so CActiveFile is local).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "engine/CCountedPointer.h"
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
class CCriticalSectionLock { public:
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool                   Locked;
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
};
class CCharStringData;
class CCharString { public:
    CCharStringData* PData;
    ~CCharString();                                      // 0x0099EAE0
};
class CWideString { public:
    void* PData;
    CCharString ToCharString() const;                    // 0x0099B2C0
};
class CCompressorZlib { public:                          // 0x58 bytes, polymorphic
    virtual ~CCompressorZlib();                          // 0x00A3ACC0
    unsigned char _body[0x54];
};
namespace std {
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
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
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> { public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    _Tp& front() { return *begin(); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void pop_front() { erase(begin()); }
    void remove(const _Tp& __val);                       // out of line
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const; };
struct _Rb_tree_node_base {
    bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class _Tp> struct _Const_traits { };
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
};
template <class _Value, class _Alloc> struct _Rb_tree_base {
    _Rb_tree_node<_Value>* _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> >    const_iterator;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    pair<iterator, bool> insert_unique(const _Value& __x);   // 0x004AC380
};
template <class _Tp> struct _Identity { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set { public:
    typedef _Key value_type;
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;
    pair<iterator, bool> insert(const value_type& __x) {
        typedef typename _Rep_type::iterator _Rep_iterator;
        pair<_Rep_iterator, bool> __p = _M_t.insert_unique(__x);
        return pair<iterator, bool>(__p.first, __p.second);
    }
};
}

class CFileInstaller { public:
    class CFileInfo { public:
        CWideString SrcPath;                             // +0x00
        CWideString DstPath;                             // +0x04
        long        TotalSize;                           // +0x08
        bool        Started;                             // +0x0c
        bool        Decompress;                          // +0x0d
    };
    class CActiveRead;
    class CPendingWrite { public: unsigned long a, b; void* c; };
    class CActiveFile { public:
        void*                             SrcFileHandle;         // +0x00
        void*                             DstFileHandle;         // +0x04
        unsigned long                     CurrentSrcOffset;      // +0x08
        unsigned long                     NoBytesRead;           // +0x0c
        unsigned long                     NoBytesWritten;        // +0x10
        unsigned long                     InputSize;             // +0x14
        unsigned long                     OutputSize;            // +0x18
        CCountedPointer<const CFileInfo>  PFileInfo;             // +0x1c
        unsigned long                     MinWriteSize;          // +0x24
        unsigned long                     MinReadSize;           // +0x28
        double                            StartTime;             // +0x30
        CCompressorZlib                   Decompressor;          // +0x38
        std::list<CPendingWrite>          PendingWrites;         // +0x90
        std::list<CActiveRead*>           PendingDecompression;  // +0x94
        unsigned long                     DecompressSrcPos;      // +0x98
        unsigned long                     DecompressDstPos;      // +0x9c
        void*                             PDecompressSrcBuffer;  // +0xa0
        void*                             PDecompressDstBuffer;  // +0xa4
        unsigned long                     DecompressDstUsed;     // +0xa8
        bool                              DecompressFinish;      // +0xac
        bool IsWriteFinished() const { return OutputSize != 0 && NoBytesWritten >= OutputSize; }
        void OnWriteFinished();                                  // 0x009C38C0
    };
    void*                                        __vftable;
    _RTL_CRITICAL_SECTION                        CriticalSection;        // +0x04
    unsigned char                                BufferChunks[0xc];      // +0x1c CArray<CBufferChunk>
    unsigned char*                               PReadBuffer;            // +0x28
    unsigned char*                               PWriteBuffer;           // +0x2c
    long                                         AvailableBuffers;       // +0x30
    long                                         AllocatedInputBuffers;  // +0x34
    std::list<CCountedPointer<const CFileInfo> > FilesToCopy;            // +0x38
    std::list<CCountedPointer<const CFileInfo> > FilesToFinish;          // +0x3c
    std::list<CActiveFile>                       ActiveFiles;            // +0x40
    std::set<CCharString>                        CompletedFiles;         // +0x44
    CActiveFile*                                 PCurrentReadFile;       // +0x50
    CActiveFile*                                 PCurrentWriteFile;      // +0x54
    long                                         NoActiveReads;          // +0x58
    long                                         NoActiveWrites;         // +0x5c
    bool                                         WriteMode;              // +0x60
    unsigned long                                BufferChunkSize;        // +0x64
    unsigned long                                NoBufferChunks;         // +0x68
    long                                         NoTotalBytesWritten;    // +0x6c
    long                                         TotalSizeOfAllFiles;    // +0x70
    CActiveRead* volatile                        PCompletedRead;         // +0x74 (volatile: see the Retail-volatile line)
    bool                                         ThreadFinished;         // +0x78
    bool                                         DecompressionThreadFinished;
    volatile bool                                ExitThread;             // +0x7a (volatile: see the Retail-volatile line)
    bool                                         Running;
    unsigned long                                Paused;                 // +0x7c
    bool                                         Completed;              // +0x80
protected:
    void PostFileReadCompletion(CActiveRead* read);                      // 0x009C4120
    void ProcessFileReadCompletion();                                    // 0x009C42F0 (inlined here)
    void StartWritingFile(CActiveFile& file, unsigned long offset);      // 0x009C3660
    void ProcessAlways();
};

void CFileInstaller::ProcessFileReadCompletion()
{
    CCriticalSectionLock lock(CriticalSection, false);
    if (PCompletedRead) {
        PostFileReadCompletion(PCompletedRead);
        PCompletedRead = 0;
    }
}

void CFileInstaller::ProcessAlways()
{
    ProcessFileReadCompletion();
    if (PCurrentWriteFile && PCurrentWriteFile->IsWriteFinished()) {
        NoTotalBytesWritten += PCurrentWriteFile->OutputSize;
        PCurrentWriteFile = 0;
    }
    while (!ActiveFiles.empty() && ActiveFiles.front().IsWriteFinished()) {
        CActiveFile& file = ActiveFiles.front();
        FilesToFinish.remove(file.PFileInfo);
        file.OnWriteFinished();
        CompletedFiles.insert(file.PFileInfo->SrcPath.ToCharString());
        ActiveFiles.pop_front();
        if (FilesToFinish.empty() && FilesToCopy.empty())
            Completed = true;
    }
    if (PCurrentWriteFile == 0 && !ActiveFiles.empty()) {
        CActiveFile& file = ActiveFiles.front();
        if (!file.PFileInfo->Decompress)
            StartWritingFile(file, file.InputSize);
        PCurrentWriteFile = &ActiveFiles.front();
    }
}