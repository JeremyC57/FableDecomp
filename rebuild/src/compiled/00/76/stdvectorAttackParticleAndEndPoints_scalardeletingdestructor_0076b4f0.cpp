// std::vector<AttackParticleAndEndPoints>::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the inline destructor destroys the AttackParticleAndEndPoints range of its vector through the out-of-line __destroy_aux and frees the storage, then the object is freed when bit 0 of the flags is set
// Leaf: ??_G?$vector@UAttackParticleAndEndPoints@@V?$allocator@UAttackParticleAndEndPoints@@@std@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B4F0, 55 bytes (+ int3 padding to 64), thiscall `ret 4`, returns this.
// Identity (proven): std::vector<AttackParticleAndEndPoints>::`scalar deleting destructor'; evidence: its callee 0x0076A2A0 destroys 0x18-byte elements: ~CBaseIntelligentPointer 0x00A01C10 at +0x10 then ~CCharString 0x0099EAE0 at +0x4 and +0x0 -- Ego_r.pdb AttackParticleAndEndPoints (24 bytes, CIntelligentPointer<CThing> ParticleEffect +0x10 after a 16-byte head); its callers are CTCObjectAugmentations::AttachAugmentationParticles 0x007677E0 and this TU's vector helpers, and Ego_r.pdb CTCObjectAugmentations::AttackParticleEffects is vector<vector<AttackParticleAndEndPoints> >; FableWin.pdb names ??_G?$vector@UAttackParticleAndEndPoints@@V?$allocator@UAttackParticleAndEndPoints@@@std@@@std@@QAEPAXI@Z
// Label: the manifest's "Vector_Destroy_QPersistInfo" is propagated (family H064: six deleting destructors that differ only in the
//   __destroy_aux they call; retail has no identical-code folding, so each address is one class's COMDAT). No call or
//   data reference reaches this address: VC7.1 emits the ??_G COMDAT for every `delete p` and explicit `p->~T()` of a
//   class with a destructor (checked with a two-line probe), inlines it at the site, and the retail link kept it.
// Callees: std::__destroy_aux<AttackParticleAndEndPoints*> @ 0x0076A2A0 (`__fastcall` first/last, the empty __false_type tag by reference,
//   homed on the flags argument's slot: `lea eax,[esp+8]`), free 0x00BFEA14 (the null-tested
//   ~_Vector_base deallocate), operator delete 0x00BFE9BC.
// Layout: local models (no engine header for these classes); STLport 4.6 vector = 12 bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter>
void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&);   // out of line
template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x0
    _Tp* _M_finish;                                     // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std

struct AttackParticleAndEndPoints {                    // 0x18 bytes: two CCharStrings, two words, CIntelligentPointer<CThing> +0x10
    unsigned char Data[0x18];
    ~AttackParticleAndEndPoints();
};

// Same-TU stand-in for the out-of-line __destroy_aux 0x0076A2A0: a plain function above the caller under auto_inline(off)
// that never touches the tag, so VC7.1 homes the empty tag temporary on the live flags slot as retail does (a declared-only
// callee opens a `push ecx` frame word; a template stand-in is inlined despite the pragma -- GOTCHAS 2026-10-20).
namespace std {
#pragma auto_inline(off)
void __fastcall __destroy_aux(AttackParticleAndEndPoints* __first, AttackParticleAndEndPoints* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
#pragma auto_inline(on)
}

// __destroy_aux<vector<AttackParticleAndEndPoints>*> (CTCObjectAugmentations::AttackParticleEffects is a vector of these):
// the explicit destructor call that emits the COMDAT.
void DestroyAttackParticleVector_0076b4f0(std::vector<AttackParticleAndEndPoints>* p) { p->~vector(); }