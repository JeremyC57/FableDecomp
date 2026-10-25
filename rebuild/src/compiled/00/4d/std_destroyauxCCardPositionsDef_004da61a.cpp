// std::__destroy_aux<CCardPositionsDef*>(first, last, const __false_type&) -- destroy each CCardPositionsDef in [first, last)
// Leaf: ??$__destroy_aux@PAVCCardPositionsDef@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA61A, 28 bytes, `ret 4` (/Gr: ecx = first, edx = last, the tag on the stack).
// Identity: STLport `__destroy_aux(first, last, __false_type)`: one `_Destroy(&*first)` per element, i.e. a call of
// the virtual destructor (slot 0, the scalar deleting destructor with flag 0) at a 0x48 stride. Callers: the
// vector<CCardPositionsDef> _M_clear 0x004EBD8D and that vector's _M_insert_overflow; the element is CCardPositionsDef (the vector's
// copy-construct helper 0x004EBDF1 stores vtable 0x012439DC = `.?AVCCardPositionsDef@@`). The old DestroyRange_Elem / vector _Destroy labels
// were descriptive / propagated.
#pragma optimize("s", on)

class CCardPositionsDef {
public:
    virtual ~CCardPositionsDef();
    unsigned char _body[0x44];
};

namespace std {
struct __false_type { };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall __destroy_aux<CCardPositionsDef*>(CCardPositionsDef*, CCardPositionsDef*, const __false_type&);
}
