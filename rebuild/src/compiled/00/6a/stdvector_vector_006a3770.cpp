// std::vector::~vector() -- (vector<NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser >) out-of-line destructor: destroy every element, then free the vector's own block
// Leaf: ??1?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3770, 49 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): vector<NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser >::~vector; evidence: its only caller is CTCBulletTime's 0x0079D7E0 (0x0079DAAF), destroying its local vector at [esp+0x34] at the end: the local is filled by the push_back 0x006A3C70 (0x0079D9D4) with {intelligent pointer, true} entries and handed by const reference to CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo 0x006B85D0 (0x0079DA68) -- Ego_r.pdb ?ScreenFilterFadeTo@CEnvironmentSpellEffectInterpolator@@QAEPAXMW4ScreenEffectPriority@NScreenEffect@@MMMMABVCRGBFloatColour@@ABV?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@...; Ego_r.pdb's CThingGraphicArgPasser is 9 bytes {CIntelligentPointer<CThing> thing, bool bypassSet}, the retail stride.
// Label: the manifest's "ResetRings" is propagated (family J007: six destroy-and-free bodies whose element destructor runs on the element itself -- STLport ~vector (with the inline
//   ~_Vector_base) and the protected _M_clear() that vector::operator= calls when it reallocates have the same code; retail has
//   no identical-code folding, so each address is one (function, element) pair).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 on `thing`, free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: STLport 4.6 -- `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline __destroy_aux loop,
//   stride 9) then the block freed when non-null.
// Model: agent952 H071/H072 (the 0x004D2720 shape).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CThing;
class CBaseIntelligentPointer {                               // 8 bytes: vptr + PData
public:
    virtual ~CBaseIntelligentPointer();                        // 0x00A01C10
    void* PData;                                               // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
#pragma pack(push, 1)
    struct CThingGraphicArgPasser {                            // 9 bytes (Ego_r.pdb: packed)
        CIntelligentPointer<CThing> thing;                     // +0x00
        bool                        bypassSet;                 // +0x08
    };
#pragma pack(pop)
};
}

template std::vector<NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser, std::allocator<NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser > >::~vector();