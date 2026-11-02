// CFireHeartPatternDef::~CFireHeartPatternDef() -- the compiler-generated destructor: free the PadActive block, then the inline CDefClassBase / CDefPointeeBase destructors (CDefPointeeBase vtable reset) and ~CResource
// Leaf: ??1CFireHeartPatternDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34F80, 33 bytes, thiscall, tail `jmp ~CResource`.
// Identity (proven): callers are the slot-0 `??_G` 0x00D34F60 of the RTTI-less def vtable 0x012CCC50 and the landed
//   CDefRecordVector Resize 0x00F33D90 (it builds a temporary CFireHeartPatternDef with the ctor 0x00F33DC0 and destroys it
//   here); 0x012CCC50 is CFireHeartPatternDef's vtable (the landed Copy 0x00D34FB0 / Transfer 0x00D35070 / ctor). Ego_r.pdb
//   (0x00D47AC0) and FableWin.pdb (0x03B5FF90) spell ??1CFireHeartPatternDef@@UAE@XZ; the one member is PadActive
//   (vector<bool>, retail +0x28). No 0x012CCC50 store: the destructor is the implicit one. The catalog label
//   "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14, CResource::~CResource 0x009FC550.
// Shape: PadActive's STLport ~vector (retail vector<bool> is the generic 12-byte template, as the landed Copy row models it),
//   then engine/CDefClassBase.h's inline `virtual ~CDefClassBase() {}` (its dead vtable store dropped) and
//   engine/CDefPointeeBase.h's inline `virtual ~CDefPointeeBase() {}` (store 0x01230BA0), then the out-of-line ~CResource:
//   the same tail as CDefClassBase's `??_G` 0x0042DAB0. DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this
//   out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std

class CFireHeartPatternDef : public CDefClassBase {         // vtable 0x012CCC50 (no RTTI), 0x34 bytes
public:
    CFireHeartPatternDef();                                 // 0x00F33DC0
    std::vector<bool> PadActive;                            // +0x28
    virtual void Transfer(CPersistContext& context);        // slot 18 0x00D35070
    virtual void Copy(const CDefClassBase* other);          // slot 19 0x00D34FB0
    virtual unsigned long GetSizeofClass();                 // slot 20 0x00D34EF0
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CFireHeartPatternDef* def)
{
    def->CFireHeartPatternDef::~CFireHeartPatternDef();
}