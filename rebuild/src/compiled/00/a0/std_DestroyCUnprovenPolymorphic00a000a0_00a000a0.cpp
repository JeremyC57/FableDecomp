// std::_Destroy<CUnprovenPolymorphic_00a000a0>(CUnprovenPolymorphic_00a000a0*) -- runs an element's virtual destructor in place
// Retail: Fable.exe 0x00A000A0, 7 bytes: `mov eax,[ecx] ; push 0 ; call [eax] ; ret` -- an explicit `p->~T()` through
// the slot-0 scalar deleting destructor with flag 0 (destroy, do not free), pointer in ecx (__fastcall).
// Identity: no caller (the one xref, from _PDATA 0x014526C0, is packed unwind data, not a pointer); it sits among
// STLport element helpers (0x009FFC50..0x00A001E0). STLport 4.6 `_Destroy(_Tp* __pointer) { __pointer->~_Tp(); }`
// over a class with a virtual destructor gives exactly this body; the element type is unproven, so it is the
// per-address placeholder CUnprovenPolymorphic_00a000a0 (descriptive).
// Leaf: ??$_Destroy@VCUnprovenPolymorphic_00a000a0@@@std@@YIXPAVCUnprovenPolymorphic_00a000a0@@@Z
class CUnprovenPolymorphic_00a000a0 {
public:
    virtual ~CUnprovenPolymorphic_00a000a0();
};

namespace std {
    template <class _Tp>
    inline void __fastcall _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

    template void __fastcall _Destroy<CUnprovenPolymorphic_00a000a0>(CUnprovenPolymorphic_00a000a0*);
}