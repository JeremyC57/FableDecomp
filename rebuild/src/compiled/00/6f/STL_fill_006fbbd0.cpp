// _STL::fill(_STL::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >* first, _STL::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >* last, const _STL::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >& value) -- assign one (animation id, event list) pair to every element of a range
// Leaf: ??$fill@PAU?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FBBD0, 71 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`; `last` is spilled to the frame (`push ecx` slot) and reloaded after the call.
// Identity: the manifest label agrees with the bytes: 12-byte stride = {unsigned long +0, CSmallVector<CEvent,32>
//   +4 (8 bytes: Start, Size)}; the member is assigned through CSmallVector::Copy @ 0x006FB5C0 (manifest
//   engine_api), guarded by a self-assignment test. Ego_r.pdb has CVectorMap<unsigned long, CSmallVector<CEvent,32>,
//   ...> in NAnimationEvents::CManager, whose 12-byte pairs these are. One caller, 0x006FC8A6.
// Callees: CSmallVector<NAnimationEvents::CEvent,32>::Copy(const CSmallVector&) @ 0x006FB5C0.
// Shape: STLport fill with pair's implicit operator= inline: first by register, then CSmallVector's inline
//   operator= (`if (&other != this) Copy(other);`: the operand order fixes `cmp ebx,edi`); &val.second and &first->second are both strength-reduced.

namespace NAnimationEvents {
class CEvent;
}
template <class T, int N> class CSmallVector {           // 8 bytes
public:
    T*  Start;                                           // +0x00
    int Size;                                            // +0x04
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Copy(const CSmallVector& other);                // 0x006FB5C0 for <CEvent, 32>
};
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef _STL::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > CAnimEventsPair;

namespace _STL {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall _STL::fill<CAnimEventsPair*, CAnimEventsPair>(CAnimEventsPair*, CAnimEventsPair*, const CAnimEventsPair&);