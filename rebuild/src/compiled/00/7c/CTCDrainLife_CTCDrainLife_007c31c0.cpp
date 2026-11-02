// CTCDrainLife::~CTCDrainLife() -- drain-life spell thing component destructor (empty user body): destroy Shots, PItemRemoved and PDef, then ~CTCBase
// Leaf: ??1CTCDrainLife@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C31C0, int3 padding before and after, thiscall, tail `jmp ~CTCBase`.
// Identity (proven, two sources): (1) it stores vtable 0x0126E284 (RTTI .?AVCTCDrainLife@@ : CTCBase) and its sole caller is the
//   scalar deleting destructor 0x007C3E70, slot 0 of that vtable; (2) Ego_r.pdb 0x007C21C0 / FableWin.pdb 0x0270FBC0
//   ??1CTCDrainLife@@UAE@XZ. The catalog label "CTCCoopSpirit::~CTCCoopSpirit" is wrong (that class is 0x00670090).
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (each Shots element, then PItemRemoved), free 0x00BFEA14 (vector storage),
//   ??1CTCBase@@UAE@XZ (tail).
// Shape: empty user dtor; STLport 4.6 vector<CIntelligentPointer<CThing> > destructor inline (_Destroy loop over the virtual-dtor
//   elements, then _Vector_base frees the block), PItemRemoved, PDef (trivial), CTCBase.
// Layout: Ego_r.pdb CTCDrainLife with the 12-byte STLport vector, retail sizeof 0x30. Shared retail opt-ins for CTCBase / CClass /
//   CDefPointer; CBaseIntelligentPointer and the STLport templates are local (no shared header).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include "engine/CClass.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CThing : public CClass { };

namespace std {
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes; 1-aligned in retail, chunk 706 model)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                      // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // FableWin member set: Get / Set / -> / *
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};
#pragma pack(pop)

class CSpecialAbilitiesDrainLifeDef;

class CTCDrainLife : public CTCBase {
public:
    CTCDrainLife(CThing& thing);
    virtual ~CTCDrainLife();
    CDefPointer<const CSpecialAbilitiesDrainLifeDef>   PDef;              // +0x0c
    long                                               SpellLevel;        // +0x10
    CIntelligentPointer<CThing>                        PItemRemoved;      // +0x14
    std::vector<CIntelligentPointer<CThing> >          Shots;             // +0x1c
    bool                                               JustReleased;      // +0x28
    long                                               SpellDelayFrames;  // +0x2c
};
FABLE_STATIC_ASSERT(sizeof(CTCDrainLife) == 0x30);

CTCDrainLife::~CTCDrainLife()
{
}