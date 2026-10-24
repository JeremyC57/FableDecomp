// CEngine::AddRenderLayer(CEngineLayerRenderer* layer) -- reports "Engine: Add Render Layer" to the progress display, appends the layer to the engine's render-layer list and returns it
// Leaf: AddRenderLayer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B262C0, length 116 bytes, `ret 4`.
// Signature from Ego_r.pdb: ?AddRenderLayer@CEngine@@AAEAAVCEngineLayerRenderer@@PAV2@@Z (private; returns the layer by
//   reference). Callers: CEngine::Initialise @ 0x00B26360, each result feeding CEngineLayerRenderer::AddComponent (0x00B2AC80).
// Callees: CCharString::CCharString(const char*, long = -1) @ 0x0099EBF0, NProgressDisplay::DisplayProgress(const
//   CCharString&, float, bool, bool) @ 0x009E9F40 (__fastcall: text in ecx, the first bool in dl, the float and the second
//   bool on the stack), CCharString::~CCharString @ 0x0099EAE0, vector<CEngineLayerRenderer*>::_M_insert_overflow @ 0x00B29470.
// Shape: the literal reaches DisplayProgress through an implicit CCharString conversion temporary (re-addressed with `lea`
//   after its constructor, GOTCHAS wave 11/15) in the `push ecx` frame word; STLport push_back inlined on RenderLayers,
//   with the empty __true_type tag and x both addressed at the `layer` argument slot (same-TU noinline growth body that
//   never reads the tag); `return *layer` re-reads the argument on both paths.
// Model: engine/CEngine.h carries the PDB offsets (RenderLayers +0x168 behind three 16-byte PDB CArrays); retail CArray is
//   12 bytes and RenderLayers sits at +0x15C, so CEngine is modelled locally with the retail offset.
inline void* operator new(unsigned int, void* where) { return where; }

namespace _STL {

struct __true_type { __true_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {   // the end-of-storage pointer
    _Value _M_data;
};

template <class _Tp, class _Alloc> class vector {         // retail STLport vector (12 bytes)
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

void* _M_grow_storage(void* vector, unsigned int count);   // stand-in for the reallocation inside the growth path

#pragma auto_inline(off)
// Same-TU body of the growth template (retail instantiates _M_insert_overflow in this TU): it reads x through its const
// reference and never touches the empty tag, which lets VC7.1 home the tag on the caller's argument slot as retail does.
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                                                  unsigned int __fill_len, bool __atend)
{
    _Tp* __new_start = (_Tp*)_M_grow_storage(this, __fill_len);
    if (__new_start)
        *__new_start = __x;
}
#pragma auto_inline(on)

}   // namespace _STL

template <class T> class CArray : public _STL::vector<T, _STL::allocator<T> > { };   // Fable's CArray over the STLport vector

class CCharString {
public:
    void* PString;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool flag1, bool flag2);   // 0x009E9F40
}

class CEngineLayerRenderer;

class CEngine {
public:
    unsigned char                 _pad_0x000[0x15C];
    CArray<CEngineLayerRenderer*> RenderLayers;            // +0x15C (retail; PDB +0x168)
    CArray<void*>                 EngineComponents;        // +0x168 (retail; PDB +0x178)
    CEngineLayerRenderer& AddRenderLayer(CEngineLayerRenderer* layer);
};

CEngineLayerRenderer& CEngine::AddRenderLayer(CEngineLayerRenderer* layer)
{
    NProgressDisplay::DisplayProgress("Engine: Add Render Layer", -1.0f, false, false);
    RenderLayers.push_back(layer);
    return *layer;
}