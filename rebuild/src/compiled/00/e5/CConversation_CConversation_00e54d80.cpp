// CConversation::~CConversation() -- implicit destructor of the conversation definition: destroy the AnimLoop, Animation, Dialogue and Speaker CCharString vectors (each string, then the storage), then the inline def bases (CDefPointeeBase vptr) and ~CResource
// Leaf: ??1CConversation@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54D80, 182 bytes, thiscall, `jmp` tail to ~CResource.
// Identity (proven): (1) its caller 0x00E54D60 is the scalar deleting destructor in slot 0 of CConversation's vtable
//   0x012E5FF0 (the vtable the landed copy ctor 0x00E54CA0 stores); the other callers are the vector<CConversation> helpers
//   _M_fill_insert 0x00F33810 (x2), the CPersistContext TransferableVectorOfSubComponents<CConversation> Resize 0x00F33B60
//   (landed: binds this address as ??1CConversation@@UAE@XZ) and 0x00F334E0; (2) Ego_r.pdb / FableWin.pdb ??1CConversation@@UAE@XZ;
//   (3) the body destroys exactly Ego_r's CConversation vectors in reverse member order at retail STLport offsets (Speaker
//   +0x2c, Dialogue +0x38, Animation +0x44, AnimLoop +0x50 after long Lines +0x28 on the packed 0x25-byte CDefClassBase).
//   The catalog label CTCInventoryMap::ResetPrimitiveHandles is propagated and wrong.
// Callees: CCharString::~CCharString 0x0099EAE0, free 0x00BFEA14 (STLport malloc allocator deallocate), ~CResource 0x009FC550.
// Shape: compiler-generated dtor: no CConversation store, STLport 4.6 ~vector (_Destroy loop + ~_Vector_base null-tested
//   free) four times, the inline ~CDefClassBase / ~CDefPointeeBase (only the CDefPointeeBase vtable 0x01230BA0 store survives),
//   `jmp ~CResource` (engine/CDefClassBase.h retail opt-in). The helper at the end is not a retail function: it destroys the
//   def by name so VC7.1 emits the implicit `??1` COMDAT.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                        // : CDefPointeeBase (vtable 0x01230BA0) : CResource (~CResource 0x009FC550)
extern "C" void __cdecl free(void* block);               // 0x00BFEA14
namespace std {
// STLport 4.6 stl/_construct.h, stl/_alloc.h, stl/_vector.h (the members this row needs)
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
};
}
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};
class CConversation : public CDefClassBase {                    // vtable 0x012E5FF0 (no RTTI)
public:
    CConversation(const CConversation& other);                  // 0x00E54CA0
    virtual void Transfer(CPersistContext&);                    // slot 18
    virtual void Copy(const CDefClassBase*);                    // slot 19
    virtual unsigned long GetSizeofClass();                     // slot 20
    long                     Lines;                             // +0x28
    std::vector<CCharString> Speaker;                           // +0x2c
    std::vector<CCharString> Dialogue;                          // +0x38
    std::vector<CCharString> Animation;                         // +0x44
    std::vector<CCharString> AnimLoop;                          // +0x50
};
void TUHarness_DestroyCConversation(CConversation* conversation)   // emission helper, not a retail function
{
    conversation->CConversation::~CConversation();
}