// CBusyResourceManager::OnPostSwapScreens() -- after a present: retire every finished GPU event query (or the oldest ones while more than 8 are in flight) into the free list, recording the finished frame; then issue a fresh event query (a recycled one, else a new one from the device) for this frame
// Grade: RELOCATION_MATCH (421 bytes) -- matched by modelling STLport 4.6 list::size() as distance(begin(), end()) (input-iterator __distance copying the first iterator) in place of a raw node-counting loop.
// Leaf: ?OnPostSwapScreens@CBusyResourceManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A618D0, 421 bytes, thiscall, plain `ret`.
// Identity: the layout is Ego_r.pdb CBusyResourceManager (FreeQueoryObjects +0x00, PendingQueories +0x04 -- STLport
//   lists of 4 bytes --, PendingFrame +0x08, FinishedFrame +0x0C, D3DDevice +0x10; CPendingQuery {IDirect3DQuery9* Query;
//   long Frame;} = the 8-byte node payload), and of its PDB methods (SetDevice, ClearAllQueries, OnPostSwapScreens)
//   only OnPostSwapScreens fits a no-argument body that issues and polls queries. Its landed neighbours are this class's
//   ctor 0x00A61880 (PendingFrame = 1, FinishedFrame = 0), ClearAllQueries 0x00A61A80 and dtor 0x00A61B10 (catalogued as
//   "CDirectInputDeviceLists_00a61b10"). No direct callers (an inlined call site elsewhere). Catalog label
//   "Deque_Process_And_Rebalance" is descriptive.
// Callees: IDirect3DQuery9::GetData (slot 7, D3DGETDATA_FLUSH) and Issue (slot 6, D3DISSUE_END),
//   IDirect3DDevice9::CreateQuery (slot 118, D3DQUERYTYPE_EVENT), malloc 0x00BFEA0E / free 0x00BFEA14 (the node allocator).
// Shape: STLport list whose size() is distance(begin(), end()); the two counter-less walks before the move are unused size() calls VC7.1
//   keeps (it cannot prove the walk terminates).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

struct IDirect3DQuery9 {
    virtual long __stdcall QueryInterface(const void* iid, void** out) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual long __stdcall GetDevice(void** device) = 0;
    virtual long __stdcall GetType() = 0;
    virtual unsigned long __stdcall GetDataSize() = 0;
    virtual long __stdcall Issue(unsigned long issueFlags) = 0;                                    // slot 6
    virtual long __stdcall GetData(void* data, unsigned long size, unsigned long getDataFlags) = 0; // slot 7
};

struct IDirect3DDevice9 {   // d3d9.h method order (only CreateQuery is called; the other slots keep the vtable layout)
    virtual long __stdcall QueryInterface() = 0; virtual long __stdcall AddRef() = 0; virtual long __stdcall Release() = 0; virtual long __stdcall TestCooperativeLevel() = 0;
    virtual long __stdcall GetAvailableTextureMem() = 0; virtual long __stdcall EvictManagedResources() = 0; virtual long __stdcall GetDirect3D() = 0; virtual long __stdcall GetDeviceCaps() = 0;
    virtual long __stdcall GetDisplayMode() = 0; virtual long __stdcall GetCreationParameters() = 0; virtual long __stdcall SetCursorProperties() = 0; virtual long __stdcall SetCursorPosition() = 0;
    virtual long __stdcall ShowCursor() = 0; virtual long __stdcall CreateAdditionalSwapChain() = 0; virtual long __stdcall GetSwapChain() = 0; virtual long __stdcall GetNumberOfSwapChains() = 0;
    virtual long __stdcall Reset() = 0; virtual long __stdcall Present() = 0; virtual long __stdcall GetBackBuffer() = 0; virtual long __stdcall GetRasterStatus() = 0;
    virtual long __stdcall SetDialogBoxMode() = 0; virtual long __stdcall SetGammaRamp() = 0; virtual long __stdcall GetGammaRamp() = 0; virtual long __stdcall CreateTexture() = 0;
    virtual long __stdcall CreateVolumeTexture() = 0; virtual long __stdcall CreateCubeTexture() = 0; virtual long __stdcall CreateVertexBuffer() = 0; virtual long __stdcall CreateIndexBuffer() = 0;
    virtual long __stdcall CreateRenderTarget() = 0; virtual long __stdcall CreateDepthStencilSurface() = 0; virtual long __stdcall UpdateSurface() = 0; virtual long __stdcall UpdateTexture() = 0;
    virtual long __stdcall GetRenderTargetData() = 0; virtual long __stdcall GetFrontBufferData() = 0; virtual long __stdcall StretchRect() = 0; virtual long __stdcall ColorFill() = 0;
    virtual long __stdcall CreateOffscreenPlainSurface() = 0; virtual long __stdcall SetRenderTarget() = 0; virtual long __stdcall GetRenderTarget() = 0; virtual long __stdcall SetDepthStencilSurface() = 0;
    virtual long __stdcall GetDepthStencilSurface() = 0; virtual long __stdcall BeginScene() = 0; virtual long __stdcall EndScene() = 0; virtual long __stdcall Clear() = 0;
    virtual long __stdcall SetTransform() = 0; virtual long __stdcall GetTransform() = 0; virtual long __stdcall MultiplyTransform() = 0; virtual long __stdcall SetViewport() = 0;
    virtual long __stdcall GetViewport() = 0; virtual long __stdcall SetMaterial() = 0; virtual long __stdcall GetMaterial() = 0; virtual long __stdcall SetLight() = 0;
    virtual long __stdcall GetLight() = 0; virtual long __stdcall LightEnable() = 0; virtual long __stdcall GetLightEnable() = 0; virtual long __stdcall SetClipPlane() = 0;
    virtual long __stdcall GetClipPlane() = 0; virtual long __stdcall SetRenderState() = 0; virtual long __stdcall GetRenderState() = 0; virtual long __stdcall CreateStateBlock() = 0;
    virtual long __stdcall BeginStateBlock() = 0; virtual long __stdcall EndStateBlock() = 0; virtual long __stdcall SetClipStatus() = 0; virtual long __stdcall GetClipStatus() = 0;
    virtual long __stdcall GetTexture() = 0; virtual long __stdcall SetTexture() = 0; virtual long __stdcall GetTextureStageState() = 0; virtual long __stdcall SetTextureStageState() = 0;
    virtual long __stdcall GetSamplerState() = 0; virtual long __stdcall SetSamplerState() = 0; virtual long __stdcall ValidateDevice() = 0; virtual long __stdcall SetPaletteEntries() = 0;
    virtual long __stdcall GetPaletteEntries() = 0; virtual long __stdcall SetCurrentTexturePalette() = 0; virtual long __stdcall GetCurrentTexturePalette() = 0; virtual long __stdcall SetScissorRect() = 0;
    virtual long __stdcall GetScissorRect() = 0; virtual long __stdcall SetSoftwareVertexProcessing() = 0; virtual long __stdcall GetSoftwareVertexProcessing() = 0; virtual long __stdcall SetNPatchMode() = 0;
    virtual long __stdcall GetNPatchMode() = 0; virtual long __stdcall DrawPrimitive() = 0; virtual long __stdcall DrawIndexedPrimitive() = 0; virtual long __stdcall DrawPrimitiveUP() = 0;
    virtual long __stdcall DrawIndexedPrimitiveUP() = 0; virtual long __stdcall ProcessVertices() = 0; virtual long __stdcall CreateVertexDeclaration() = 0; virtual long __stdcall SetVertexDeclaration() = 0;
    virtual long __stdcall GetVertexDeclaration() = 0; virtual long __stdcall SetFVF() = 0; virtual long __stdcall GetFVF() = 0; virtual long __stdcall CreateVertexShader() = 0;
    virtual long __stdcall SetVertexShader() = 0; virtual long __stdcall GetVertexShader() = 0; virtual long __stdcall SetVertexShaderConstantF() = 0; virtual long __stdcall GetVertexShaderConstantF() = 0;
    virtual long __stdcall SetVertexShaderConstantI() = 0; virtual long __stdcall GetVertexShaderConstantI() = 0; virtual long __stdcall SetVertexShaderConstantB() = 0; virtual long __stdcall GetVertexShaderConstantB() = 0;
    virtual long __stdcall SetStreamSource() = 0; virtual long __stdcall GetStreamSource() = 0; virtual long __stdcall SetStreamSourceFreq() = 0; virtual long __stdcall GetStreamSourceFreq() = 0;
    virtual long __stdcall SetIndices() = 0; virtual long __stdcall GetIndices() = 0; virtual long __stdcall CreatePixelShader() = 0; virtual long __stdcall SetPixelShader() = 0;
    virtual long __stdcall GetPixelShader() = 0; virtual long __stdcall SetPixelShaderConstantF() = 0; virtual long __stdcall GetPixelShaderConstantF() = 0; virtual long __stdcall SetPixelShaderConstantI() = 0;
    virtual long __stdcall GetPixelShaderConstantI() = 0; virtual long __stdcall SetPixelShaderConstantB() = 0; virtual long __stdcall GetPixelShaderConstantB() = 0; virtual long __stdcall DrawRectPatch() = 0;
    virtual long __stdcall DrawTriPatch() = 0; virtual long __stdcall DeletePatch() = 0;
    virtual long __stdcall CreateQuery(long type, IDirect3DQuery9** query) = 0;                     // slot 118 (+0x1D8)
};

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { free(p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp> struct _Nonconst_traits;
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};

struct input_iterator_tag { };
template <class _InputIterator>
inline int __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    int __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}
template <class _InputIterator>
inline int distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, input_iterator_tag());
}

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base(const _List_iterator_base& __x) : _M_node(__x._M_node) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
    _Self& operator--() { this->_M_decr(); return *this; }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef unsigned int size_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) {}
    iterator begin() const { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() const { return iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = distance(begin(), end());
        return __result;
    }
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void pop_back() { iterator __tmp = end(); erase(--__tmp); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

}   // namespace std

class CBusyResourceManager {
public:
    struct CPendingQuery {                                // Ego_r.pdb, 8 bytes
        IDirect3DQuery9* Query;                           // +0x00
        long             Frame;                           // +0x04
    };
    std::list<CPendingQuery> FreeQueoryObjects;           // +0x00
    std::list<CPendingQuery> PendingQueories;             // +0x04
    long                     PendingFrame;                // +0x08
    long                     FinishedFrame;               // +0x0c
    IDirect3DDevice9*        D3DDevice;                   // +0x10
    void OnPostSwapScreens();
};

void CBusyResourceManager::OnPostSwapScreens()
{
    while (PendingQueories.size() > 0 &&
           (PendingQueories.back().Query->GetData(0, 0, 1) == 0 || PendingQueories.size() > 8)) {
        PendingQueories.size();
        FreeQueoryObjects.size();
        FinishedFrame = PendingQueories.back().Frame;
        FreeQueoryObjects.push_back(PendingQueories.back());
        PendingQueories.pop_back();
    }
    if (FreeQueoryObjects.size() == 0) {
        CPendingQuery query;
        if (D3DDevice->CreateQuery(8, &query.Query) != 0) {
            FinishedFrame = PendingFrame - 2;
            return;
        }
        FreeQueoryObjects.push_back(query);
    }
    CPendingQuery query = FreeQueoryObjects.back();
    FreeQueoryObjects.pop_back();
    query.Query->Issue(1);
    PendingQueories.push_front(query);
    ++PendingFrame;
}