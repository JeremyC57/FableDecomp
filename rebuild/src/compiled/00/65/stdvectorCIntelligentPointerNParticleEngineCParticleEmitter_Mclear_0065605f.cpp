// std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_clear() -- STLport: destroy [start, finish),
// then release the block
// Leaf: ?_M_clear@?$vector@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065605F, 35 bytes, thiscall `ret`: `__destroy_aux(start, finish, __false_type())`
//   (0x0043F850, `__fastcall` -- ecx = first, edx = last, the empty tag by reference in the high byte of the `push ecx`
//   slot, `lea eax,[esp+7]`), then `if (start) free(start)` (0x00BFEA14).
// Identity (FINDINGS chunk 383; was a `__fastcall OnReadFinished(AutoTinyActiveFile*)` stand-in): its only caller is the
//   landed vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_insert_overflow 0x006566C3 (which already
//   declares 0x0065605F as this vector's _M_clear; element pinned there through _M_fill_insert 0x006568F2 and the
//   CIntelligentPointer copy ctor 0x00654DAF), and the callee's body is the 8-byte-stride loop of `p->~T()` through
//   vtable slot 0 with flag 0 -- CIntelligentPointer's virtual destructor (CBaseIntelligentPointer, 8 bytes).
// Shape: STLport 4.x `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start,
//   _M_end_of_storage._M_data - _M_start); }` with _Destroy -> __destroy_aux(.., __false_type) and the null-tested
//   deallocate inline (`__forceinline` _Destroy, as in the landed caller 0x006566C3: the size TU otherwise keeps it out
//   of line). The tag has a user-declared constructor (repo idiom: no zero-fill of the temporary).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* PItem;                               // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

namespace std {
struct __false_type { __false_type() { } };

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x0043F850

template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    void _M_clear();
};

template <class _Tp>
void vector<_Tp>::_M_clear()
{
    _Destroy(_M_start, _M_finish);
    _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
}

template void vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_clear();
}
