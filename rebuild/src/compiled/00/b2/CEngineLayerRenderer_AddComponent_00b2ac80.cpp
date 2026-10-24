// CEngineLayerRenderer::AddComponent(CEngineComponent* component) -- appends a (non-null) engine component to the layer's component list
// Leaf: AddComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AC80, length 55 bytes, `ret 4`.
// Signature from Ego_r.pdb: ?AddComponent@CEngineLayerRenderer@@QAEXPAVCEngineComponent@@@Z. Callers: CEngine::Initialise
//   @ 0x00B26360 (41 sites, right after `CEngine::AddRenderLayer` @ 0x00B262C0 returns the new layer) and 0x00B23888.
// Callees: vector<CEngineComponent*>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x00B292B0 (catalogued
//   Std_Vector_Insert_Range).
// Layout (Ego_r.pdb): CEngineLayerRenderer = vptr, Layer +4, Flags +8, SceneFilterFlags +0xC, CArray<CEngineComponent*>
//   Components +0x10 (retail STLport vector, 12 bytes, so the class is 0x1C -- CEngine::Initialise allocates 0x1C).
// Shape: null test, then STLport push_back inlined (`_Construct` = placement new with its null test, `add [ecx+4],4`; else
//   `_M_insert_overflow(_M_finish, x, __true_type(), 1, true)` with the empty tag and x both addressed at the dead
//   `component` argument slot -- reproduced with a same-TU noinline body of the growth path that never reads the tag).
// Model: no engine header for CEngineLayerRenderer; modelled locally from the PDB layout.
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

class CEngineComponent;

class CEngineLayerRenderer {
public:
    virtual void Render();                                  // slot 0 (vptr +0x00)
    long                    Layer;                          // +0x04 EEngineRenderLayer
    unsigned long           Flags;                          // +0x08 ERenderRenderLayerFlags
    unsigned long           SceneFilterFlags;               // +0x0C
    CArray<CEngineComponent*> Components;                   // +0x10
    void AddComponent(CEngineComponent* component);
};

void CEngineLayerRenderer::AddComponent(CEngineComponent* component)
{
    if (component)
        Components.push_back(component);
}