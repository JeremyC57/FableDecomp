// CEngineLandscapeMemoryManager::FreeDisposalList() -- empty the three generations of delayed-disposal lists: destroy every queued texture, vertex buffer and index buffer and give its block back to the global allocator
// Leaf: ?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

class CMemoryAllocator { public:
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* p);   // 0x009F1F90
};
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90
extern bool g_bFreeGlobalAllocatorWhenEmpty_013caa94;   // 0x013CAA94

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _Nonconst_traits { };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _InputIterator> inline unsigned int distance(_InputIterator __first, _InputIterator __last)
{
    unsigned int __n = 0;
    while (__first != __last) { ++__first; ++__n; }
    return __n;
}
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    unsigned int size() { return distance(begin(), end()); }
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
        return iterator((_Node*)__next_node);
    }
    void pop_front() { erase(begin()); }
};
}   // namespace std

class CTexture {                                           // non-polymorphic (no RTTI vtable); ~CTexture is inline
public:
    void Uninitialise();                                   // 0x009F9F70
    ~CTexture() { Uninitialise(); }
};
class CVertexBufferWin32 {                                 // vtable 0x0129BA20 (slot 0 ??_G 0x009E2890)
public:
    virtual ~CVertexBufferWin32();
};
class CIndexBuffer {                                       // vtable 0x0129EF2C (slot 0 ??_G 0x00AC2070)
public:
    virtual ~CIndexBuffer();
};
class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;

class CFailedAllocationHandler {                           // vtable 0x0129B874
public:
    virtual ~CFailedAllocationHandler() { }
    virtual bool HandleFailedAllocation(unsigned long size) = 0;
};
class CMovableResource {                                   // vtable 0x0129B860
public:
    virtual ~CMovableResource() { }
    virtual void RelocateData(long id, void* from, void* to, long size) = 0;
    virtual bool CanRelocateData(long id) const = 0;
    virtual unsigned long Verify() const;
};

class CEngineLandscapeMemoryManager : public CFailedAllocationHandler, public CMovableResource {
public:
    CMovableResourceMemoryPool* VirtualMemPool;            // +0x08
    CXboxBufferMemoryPool*      PhysicalMemPool;           // +0x0C
    std::list<CTexture*>           TextureDisposalList[3];       // +0x10
    std::list<CVertexBufferWin32*> VertexBufferDisposalList[3];  // +0x1C
    std::list<CIndexBuffer*>       IndexBufferDisposalList[3];   // +0x28
    long                        CurrentDisposalListIndex;  // +0x34
    bool                        InsideDefragment;          // +0x38
    bool                        PoolAllocationEnabled;     // +0x39
    long                        AllocationCount;           // +0x3C
    bool                        EnableDelayedResourceDestruction;   // +0x40
    virtual ~CEngineLandscapeMemoryManager();
    void FreeVirtual(void* block);                         // 0x00BD9B90
    void DeleteIndexBuffer(CIndexBuffer* buffer);
    void DeleteVertexBuffer(CVertexBufferWin32* buffer);
    void DeleteTexture(CTexture* texture);                 // 0x00BD9CC0
    void FreeDisposalList();                               // 0x00BD9CE0
private:
    virtual bool HandleFailedAllocation(unsigned long size);
    virtual void RelocateData(long id, void* from, void* to, long size);
    virtual bool CanRelocateData(long id) const;
};

void CEngineLandscapeMemoryManager::FreeVirtual(void* block)
{
    if (block) {
        GFEnterAllocatorLock();
        if (g_pGlobalAllocator_013caa90) {
            g_pGlobalAllocator_013caa90->InternalDeallocate(block);
            if (g_bFreeGlobalAllocatorWhenEmpty_013caa94 && g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
        GFLeaveAllocatorLock();
    }
}

void CEngineLandscapeMemoryManager::DeleteIndexBuffer(CIndexBuffer* buffer)
{
    buffer->~CIndexBuffer();
    FreeVirtual(buffer);
}

void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* buffer)
{
    buffer->~CVertexBufferWin32();
    FreeVirtual(buffer);
}

void CEngineLandscapeMemoryManager::DeleteTexture(CTexture* texture)
{
    texture->~CTexture();
    FreeVirtual(texture);
}

void CEngineLandscapeMemoryManager::FreeDisposalList()
{
    for (long i = 0; i < 3; ++i) {
        while (TextureDisposalList[i].size() != 0) {
            DeleteTexture(TextureDisposalList[i].front());
            TextureDisposalList[i].pop_front();
        }
        while (VertexBufferDisposalList[i].size() != 0) {
            DeleteVertexBuffer(VertexBufferDisposalList[i].front());
            VertexBufferDisposalList[i].pop_front();
        }
        while (IndexBufferDisposalList[i].size() != 0) {
            DeleteIndexBuffer(IndexBufferDisposalList[i].front());
            IndexBufferDisposalList[i].pop_front();
        }
    }
}
