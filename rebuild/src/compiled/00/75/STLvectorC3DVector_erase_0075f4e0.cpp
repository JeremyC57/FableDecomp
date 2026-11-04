// std::vector<C3DVector>::erase(C3DVector* position) -- erase one shape point: shift the tail down one slot and drop the last
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075F4E0, 42 bytes, `ret 4`.
// Leaf: ?erase@?$vector@VC3DVector
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Label: the catalog's "MedianCut::GetNewTreeNode" is propagated and wrong. The only caller, NShape::CShape::UpdateDeletePoint
//        0x00762010, erases from CShape::Points (vector<C3DVector> at +0x08, Ego_r.pdb) the point named by the last
//        SelectedPoints iterator; the 12-byte stride and the copy helper match.
// Callees: the STLport random-access `__copy` for 12-byte elements @ 0x0074CA20 (`__fastcall`: first/last in ecx/edx; result,
//          `const random_access_iterator_tag&` and `(int*)0` on the stack, `ret 0Ch`; landed as the SElement12 copy).
// Shape: STLport `if (pos + 1 != end()) __copy(pos + 1, _M_finish, pos, tag, (int*)0); --_M_finish;` (C3DVector has a trivial
//        destructor, so _Destroy emits nothing) -- the landed vector<CCountedPointer>::erase 0x0084F570 shape; the empty tag
//        temporary lives in the dead `position` argument slot (`lea eax,[esp+10h]`).
#include "engine/C3DVector.h"
namespace std {
struct random_access_iterator_tag {
    random_access_iterator_tag() {}
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x0074CA20 <C3DVector*, C3DVector*, int>
template <class T> struct allocator { };
template <class T, class A = allocator<T> > struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* end() { return _M_finish; }
    T* erase(T* position);
};
template <class T> inline void _Destroy(T*) { }
template <> C3DVector* vector<C3DVector>::erase(C3DVector* position)
{
    if (position + 1 != end())
        __copy(position + 1, _M_finish, position, random_access_iterator_tag(), (int*)0);
    --_M_finish;
    _Destroy(_M_finish);
    return position;
}
}