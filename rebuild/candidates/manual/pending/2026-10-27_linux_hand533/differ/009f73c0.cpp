// CVsyncCallbackTimer::OnPostSwapScreens() -- after the swap: retire finished GPU event queries (GetData), issue a new D3DQUERYTYPE_EVENT query for this frame, then advance the CPU and GPU frame stamps
// Grade: DIFFER(724v732) -- frame 0x1b8 vs retail 0x1bc: retail keeps the ++FrameCount address temporary in its own slot at [ebp-0x1bc] and orders the size locals differently; instruction stream otherwise aligned
// Leaf: ?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F73C0, length 732 bytes, thiscall, plain `ret`.
// PDB: ?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ (Ego_r.pdb; donor order ClearAllQueries, ~CVsyncCallbackTimer,
//   OnPreDeviceReset, OnPostSwapScreens: the landed OnPreDeviceReset 0x009F73A0 sits right before this row).
// Callees: list<CPendingQuery>::size 0x009F7DC0, push_back 0x009F7EC0, push_front 0x009F7E40, pop_back 0x009F7C50,
//   ClearAllQueries 0x009F7250, the iterator base ctor 0x0041C780 / _M_decr 0x0041C7B0 of the inlined back(),
//   GFGetTime 0x009E1BC0 (`?GFGetTime@@YINXZ`, __fastcall double), Sleep (import 0x0143FE1C).
//   COM: IDirect3DQuery9::GetData (slot 7, +0x1C) / Issue (slot 6, +0x18), IDirect3DDevice9::CreateQuery (slot 118, +0x1D8).
// Globals (landed names, CVsyncCallbackTimer ctor 0x009F70E0): g_CPUFrameStamps 0x013CB4B0 (2 x 0x18), g_GPUTimeStamp
//   0x013CB4E0, g_GPUPrevTimeStamp 0x013CB4E8, g_GPUFrameCount 0x013CB4F0, g_CurrentCPUFrameStamp 0x013CB4F4.
// TU: the vsync timer TU (0x009F7020..0x009F7F40) is compiled UNOPTIMISED (`#pragma optimize("", off)`) with inline
//   expansion on; STLport model copied from the landed ClearAllQueries 0x009F7250 (back() __forceinline).
// Pragma: #pragma optimize("", off).
#pragma optimize("", off)
extern "C" void __cdecl free(void* block);                         // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E
void* __cdecl operator new(unsigned int size, void* where);        // 0x00405C10 (placement new, called out of line)
namespace std {

struct input_iterator_tag { input_iterator_tag() {} };
struct bidirectional_iterator_tag : input_iterator_tag { bidirectional_iterator_tag() {} };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {                                        // compiled optimised elsewhere (ICF-shared bodies)
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x);                      // 0x0041C780
    void _M_incr();                                                 // 0x0041C7A0
    void _M_decr();                                                 // 0x0041C7B0
    bool operator!=(const _List_iterator_base& __y) const;          // 0x0041C7C0
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
    _List_iterator& operator--() { _M_decr(); return *this; }
};

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
    return __distance(__first, __last, bidirectional_iterator_tag());
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

class __malloc_alloc {
public:
    static void* allocate(unsigned int __n) { void* __result = malloc(__n); return __result; }
};

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    template <class _Tp1> allocator(const allocator<_Tp1>&) {}
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)__malloc_alloc::allocate(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    void clear();
};

template <class _Tp, class _Alloc> class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __tmp = this->_M_node.allocate(1);
        _Construct(&__tmp->_M_data, __x);
        return __tmp;
    }
    public:
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    unsigned int size() const
    {
        unsigned int __result = distance(begin(), end());
        return __result;
    }
    __forceinline _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
};

template <class _Tp, class _Alloc> void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*) this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*) __cur->_M_next;
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

}


struct IDirect3DDevice9;

struct IDirect3DQuery9 {
    virtual long __stdcall QueryInterface(const void* riid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetDevice(IDirect3DDevice9** device);
    virtual int __stdcall GetType();
    virtual unsigned long __stdcall GetDataSize();
    virtual long __stdcall Issue(unsigned long issueFlags);
    virtual long __stdcall GetData(void* data, unsigned long size, unsigned long getDataFlags);
};

struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface(const void* riid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall TestCooperativeLevel();
    virtual unsigned int __stdcall GetAvailableTextureMem();
    virtual long __stdcall EvictManagedResources();
    virtual long __stdcall GetDirect3D(void** d3d9);
    virtual long __stdcall GetDeviceCaps(void* caps);
    virtual long __stdcall GetDisplayMode(unsigned int swapChain, void* mode);
    virtual long __stdcall GetCreationParameters(void* parameters);
    virtual long __stdcall SetCursorProperties(unsigned int x, unsigned int y, void* bitmap);
    virtual void __stdcall SetCursorPosition(int x, int y, unsigned long flags);
    virtual int __stdcall ShowCursor(int show);
    virtual long __stdcall CreateAdditionalSwapChain(void* parameters, void** swapChain);
    virtual long __stdcall GetSwapChain(unsigned int swapChain, void** result);
    virtual unsigned int __stdcall GetNumberOfSwapChains();
    virtual long __stdcall Reset(void* parameters);
    virtual long __stdcall Present(const void* src, const void* dst, void* window, const void* dirty);
    virtual long __stdcall GetBackBuffer(unsigned int swapChain, unsigned int index, int type, void** surface);
    virtual long __stdcall GetRasterStatus(unsigned int swapChain, void* status);
    virtual long __stdcall SetDialogBoxMode(int enable);
    virtual void __stdcall SetGammaRamp(unsigned int swapChain, unsigned long flags, const void* ramp);
    virtual void __stdcall GetGammaRamp(unsigned int swapChain, void* ramp);
    virtual long __stdcall CreateTexture(unsigned int w, unsigned int h, unsigned int levels, unsigned long usage, int format, int pool, void** texture, void* shared);
    virtual long __stdcall CreateVolumeTexture(unsigned int w, unsigned int h, unsigned int d, unsigned int levels, unsigned long usage, int format, int pool, void** texture, void* shared);
    virtual long __stdcall CreateCubeTexture(unsigned int edge, unsigned int levels, unsigned long usage, int format, int pool, void** texture, void* shared);
    virtual long __stdcall CreateVertexBuffer(unsigned int length, unsigned long usage, unsigned long fvf, int pool, void** buffer, void* shared);
    virtual long __stdcall CreateIndexBuffer(unsigned int length, unsigned long usage, int format, int pool, void** buffer, void* shared);
    virtual long __stdcall CreateRenderTarget(unsigned int w, unsigned int h, int format, int multiSample, unsigned long quality, int lockable, void** surface, void* shared);
    virtual long __stdcall CreateDepthStencilSurface(unsigned int w, unsigned int h, int format, int multiSample, unsigned long quality, int discard, void** surface, void* shared);
    virtual long __stdcall UpdateSurface(void* src, const void* srcRect, void* dst, const void* dstPoint);
    virtual long __stdcall UpdateTexture(void* src, void* dst);
    virtual long __stdcall GetRenderTargetData(void* renderTarget, void* dst);
    virtual long __stdcall GetFrontBufferData(unsigned int swapChain, void* dst);
    virtual long __stdcall StretchRect(void* src, const void* srcRect, void* dst, const void* dstRect, int filter);
    virtual long __stdcall ColorFill(void* surface, const void* rect, unsigned long color);
    virtual long __stdcall CreateOffscreenPlainSurface(unsigned int w, unsigned int h, int format, int pool, void** surface, void* shared);
    virtual long __stdcall SetRenderTarget(unsigned long index, void* surface);
    virtual long __stdcall GetRenderTarget(unsigned long index, void** surface);
    virtual long __stdcall SetDepthStencilSurface(void* surface);
    virtual long __stdcall GetDepthStencilSurface(void** surface);
    virtual long __stdcall BeginScene();
    virtual long __stdcall EndScene();
    virtual long __stdcall Clear(unsigned long count, const void* rects, unsigned long flags, unsigned long color, float z, unsigned long stencil);
    virtual long __stdcall SetTransform(int state, const void* matrix);
    virtual long __stdcall GetTransform(int state, void* matrix);
    virtual long __stdcall MultiplyTransform(int state, const void* matrix);
    virtual long __stdcall SetViewport(const void* viewport);
    virtual long __stdcall GetViewport(void* viewport);
    virtual long __stdcall SetMaterial(const void* material);
    virtual long __stdcall GetMaterial(void* material);
    virtual long __stdcall SetLight(unsigned long index, const void* light);
    virtual long __stdcall GetLight(unsigned long index, void* light);
    virtual long __stdcall LightEnable(unsigned long index, int enable);
    virtual long __stdcall GetLightEnable(unsigned long index, int* enable);
    virtual long __stdcall SetClipPlane(unsigned long index, const float* plane);
    virtual long __stdcall GetClipPlane(unsigned long index, float* plane);
    virtual long __stdcall SetRenderState(int state, unsigned long value);
    virtual long __stdcall GetRenderState(int state, unsigned long* value);
    virtual long __stdcall CreateStateBlock(int type, void** stateBlock);
    virtual long __stdcall BeginStateBlock();
    virtual long __stdcall EndStateBlock(void** stateBlock);
    virtual long __stdcall SetClipStatus(const void* status);
    virtual long __stdcall GetClipStatus(void* status);
    virtual long __stdcall GetTexture(unsigned long stage, void** texture);
    virtual long __stdcall SetTexture(unsigned long stage, void* texture);
    virtual long __stdcall GetTextureStageState(unsigned long stage, int type, unsigned long* value);
    virtual long __stdcall SetTextureStageState(unsigned long stage, int type, unsigned long value);
    virtual long __stdcall GetSamplerState(unsigned long sampler, int type, unsigned long* value);
    virtual long __stdcall SetSamplerState(unsigned long sampler, int type, unsigned long value);
    virtual long __stdcall ValidateDevice(unsigned long* passes);
    virtual long __stdcall SetPaletteEntries(unsigned int palette, const void* entries);
    virtual long __stdcall GetPaletteEntries(unsigned int palette, void* entries);
    virtual long __stdcall SetCurrentTexturePalette(unsigned int palette);
    virtual long __stdcall GetCurrentTexturePalette(unsigned int* palette);
    virtual long __stdcall SetScissorRect(const void* rect);
    virtual long __stdcall GetScissorRect(void* rect);
    virtual long __stdcall SetSoftwareVertexProcessing(int software);
    virtual int __stdcall GetSoftwareVertexProcessing();
    virtual long __stdcall SetNPatchMode(float segments);
    virtual float __stdcall GetNPatchMode();
    virtual long __stdcall DrawPrimitive(int type, unsigned int start, unsigned int count);
    virtual long __stdcall DrawIndexedPrimitive(int type, int baseVertex, unsigned int minIndex, unsigned int numVertices, unsigned int startIndex, unsigned int primCount);
    virtual long __stdcall DrawPrimitiveUP(int type, unsigned int count, const void* data, unsigned int stride);
    virtual long __stdcall DrawIndexedPrimitiveUP(int type, unsigned int minIndex, unsigned int numVertices, unsigned int primCount, const void* indices, int format, const void* data, unsigned int stride);
    virtual long __stdcall ProcessVertices(unsigned int srcStart, unsigned int destIndex, unsigned int count, void* dst, void* decl, unsigned long flags);
    virtual long __stdcall CreateVertexDeclaration(const void* elements, void** decl);
    virtual long __stdcall SetVertexDeclaration(void* decl);
    virtual long __stdcall GetVertexDeclaration(void** decl);
    virtual long __stdcall SetFVF(unsigned long fvf);
    virtual long __stdcall GetFVF(unsigned long* fvf);
    virtual long __stdcall CreateVertexShader(const unsigned long* function, void** shader);
    virtual long __stdcall SetVertexShader(void* shader);
    virtual long __stdcall GetVertexShader(void** shader);
    virtual long __stdcall SetVertexShaderConstantF(unsigned int start, const float* data, unsigned int count);
    virtual long __stdcall GetVertexShaderConstantF(unsigned int start, float* data, unsigned int count);
    virtual long __stdcall SetVertexShaderConstantI(unsigned int start, const int* data, unsigned int count);
    virtual long __stdcall GetVertexShaderConstantI(unsigned int start, int* data, unsigned int count);
    virtual long __stdcall SetVertexShaderConstantB(unsigned int start, const int* data, unsigned int count);
    virtual long __stdcall GetVertexShaderConstantB(unsigned int start, int* data, unsigned int count);
    virtual long __stdcall SetStreamSource(unsigned int stream, void* data, unsigned int offset, unsigned int stride);
    virtual long __stdcall GetStreamSource(unsigned int stream, void** data, unsigned int* offset, unsigned int* stride);
    virtual long __stdcall SetStreamSourceFreq(unsigned int stream, unsigned int divider);
    virtual long __stdcall GetStreamSourceFreq(unsigned int stream, unsigned int* divider);
    virtual long __stdcall SetIndices(void* indexData);
    virtual long __stdcall GetIndices(void** indexData);
    virtual long __stdcall CreatePixelShader(const unsigned long* function, void** shader);
    virtual long __stdcall SetPixelShader(void* shader);
    virtual long __stdcall GetPixelShader(void** shader);
    virtual long __stdcall SetPixelShaderConstantF(unsigned int start, const float* data, unsigned int count);
    virtual long __stdcall GetPixelShaderConstantF(unsigned int start, float* data, unsigned int count);
    virtual long __stdcall SetPixelShaderConstantI(unsigned int start, const int* data, unsigned int count);
    virtual long __stdcall GetPixelShaderConstantI(unsigned int start, int* data, unsigned int count);
    virtual long __stdcall SetPixelShaderConstantB(unsigned int start, const int* data, unsigned int count);
    virtual long __stdcall GetPixelShaderConstantB(unsigned int start, int* data, unsigned int count);
    virtual long __stdcall DrawRectPatch(unsigned int handle, const float* segs, const void* info);
    virtual long __stdcall DrawTriPatch(unsigned int handle, const float* segs, const void* info);
    virtual long __stdcall DeletePatch(unsigned int handle);
    virtual long __stdcall CreateQuery(int type, IDirect3DQuery9** query);   // slot 118 (+0x1D8)
};

extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // import 0x0143FE1C
double __fastcall GFGetTime();                                       // 0x009E1BC0 ?GFGetTime@@YINXZ

struct CVsyncFrameStamp {                           // 0x18 bytes
    double TimeStamp;                               // +0x00
    double PrevTimeStamp;                           // +0x08
    long FrameCount;                                // +0x10
    long Readers;                                   // +0x14
};
extern CVsyncFrameStamp g_CPUFrameStamps[2];        // 0x013CB4B0
extern double g_GPUTimeStamp;                       // 0x013CB4E0
extern double g_GPUPrevTimeStamp;                   // 0x013CB4E8
extern long g_GPUFrameCount;                        // 0x013CB4F0
extern long g_CurrentCPUFrameStamp;                 // 0x013CB4F4

class CVsyncCallbackTimer {
public:
    struct CPendingQuery {
        IDirect3DQuery9* Query;
        long             Frame;
        double           TimeStamp;
    };
    IDirect3DDevice9* Device;                                                     // +0x00
    std::list<CPendingQuery, std::allocator<CPendingQuery> > FreeQueoryObjects;   // +0x04
    std::list<CPendingQuery, std::allocator<CPendingQuery> > PendingQueories;     // +0x08
    long PendingFrame;                                                            // +0x0c
    long FinishedFrame;                                                           // +0x10
    void OnPostSwapScreens();
private:
    void ClearAllQueries();                                                       // 0x009F7250
};

void CVsyncCallbackTimer::OnPostSwapScreens()
{
    unsigned long flags = 1;                        // D3DGETDATA_FLUSH
    bool device_lost = false;
    while (PendingQueories.size() > 0) {
        long result = PendingQueories.back().Query->GetData(0, 0, flags);
        if (result == 0) {                          // S_OK: the GPU reached this query
            long pending = PendingQueories.size();
            long free_count = FreeQueoryObjects.size();
            FinishedFrame = PendingQueories.back().Frame;
            FreeQueoryObjects.push_back(PendingQueories.back());
            PendingQueories.pop_back();
        } else if (result == 1) {                   // S_FALSE: not yet
            Sleep(0);
        } else {                                    // device lost
            ClearAllQueries();
            device_lost = true;
            break;
        }
    }
    if (!device_lost) {
        if (FreeQueoryObjects.size() == 0) {
            CPendingQuery query;
            long create_result = Device->CreateQuery(8, &query.Query);   // D3DQUERYTYPE_EVENT
            if (create_result == 0)
                FreeQueoryObjects.push_back(query);
        }
        if (FreeQueoryObjects.size() > 0) {
            CPendingQuery query = FreeQueoryObjects.back();
            FreeQueoryObjects.pop_back();
            long issue_result = query.Query->Issue(1);                   // D3DISSUE_END
            PendingQueories.push_front(query);
        }
    }
    ++PendingFrame;
    g_CPUFrameStamps[g_CurrentCPUFrameStamp].PrevTimeStamp = g_CPUFrameStamps[g_CurrentCPUFrameStamp].TimeStamp;
    g_CPUFrameStamps[g_CurrentCPUFrameStamp].TimeStamp = GFGetTime();
    ++g_CPUFrameStamps[g_CurrentCPUFrameStamp].FrameCount;
    g_GPUPrevTimeStamp = g_GPUTimeStamp;
    g_GPUTimeStamp = GFGetTime();
    ++g_GPUFrameCount;
}
