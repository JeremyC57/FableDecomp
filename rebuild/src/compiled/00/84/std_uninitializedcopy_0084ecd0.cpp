// std::__uninitialized_copy(const CCountedPointer<NCameraModes::CSubComponentBase>* first, last, CCountedPointer<..>* result, const __false_type&) -- copy-construct a counted-pointer range into raw storage
// Leaf: ??$__uninitialized_copy@PBV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@PBV1@0PAV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe @ 0x0084ECD0, 50 bytes, __fastcall (first ecx, last edx; result and the tag on the stack), `ret 8`,
// the end of the constructed range in eax.
// Identity (proven by callers): its two callers are 0x0084EFD0 (malloc(n * 8) then this copy = vector::_M_allocate_and_copy)
// and 0x0084F740 (vector::operator=, which also calls the bound
// ??$__destroy_aux@PAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@@std@@... 0x0056DBF0 and __copy 0x0084F130) --
// both are members of std::vector<CCountedPointer<NCameraModes::CSubComponentBase> > (NCameraModes::CBase::SubComponents;
// the vector's landed _M_insert_overflow 0x0084FAB0 sits in the same TU). The body is byte-identical to the bound
// non-const instantiation 0x0084EB90 (??$__uninitialized_copy@PAV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@PAV1@...);
// the copy-from-a-const-vector callers make this one the `const T*` source instantiation (retail has no identical-code
// folding, so each address is one instantiation).
// Label correction: catalog "_gapscan::sub_0084ecd0".
// Callees: none (CCountedPointer's copy constructor is inline: copy Data and Info, then `++Info->RefCount` behind the null test;
// the placement new's null test is the `test eax,eax`).
// Layout: Ego_r.pdb CCountedPointer<T> = { T* Data, CCPPointerInfo* Info } and CCPPointerInfo = { RefCount, DeleteFunc, Data },
// as the landed 0x0084FAB0 models them.
// Shape: STLport 4.6 `__uninitialized_copy(.., __false_type)` (no _STLP_TRY in retail) under /Gr.
inline void* operator new(unsigned int, void* p) { return p; }

namespace NCameraModes { class CSubComponentBase; }

class CCPPointerInfo { public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CCountedPointer<NCameraModes::CSubComponentBase>* __uninitialized_copy(
    const CCountedPointer<NCameraModes::CSubComponentBase>*, const CCountedPointer<NCameraModes::CSubComponentBase>*,
    CCountedPointer<NCameraModes::CSubComponentBase>*, const __false_type&);
}