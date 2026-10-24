// _STL::__copy(C3DAnimationStateAlpha* first, C3DAnimationStateAlpha* last, C3DAnimationStateAlpha* result, const random_access_iterator_tag&, int*) -- STLport random-access copy: assign *result++ = *first++, (last - first) times; returns the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe @ 0x00A3CF20, 203 bytes, `ret 0Ch`. /Gr TU written as explicit __fastcall: first in ecx, last in
//   edx, result + the tag reference + the (int*)0 distance pointer on the stack.
// Callers: 0x00A3D2EC (vector<C3DAnimationStateAlpha>::erase) and 0x00A3D3F9.
// Label: catalog "Vector_Erase_CDisplayModeDesc_Bulk" is descriptive and wrong; the element is C3DAnimationStateAlpha.
// Shape: STLport 4.6 _algobase.h `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`
//   -- count (last - first) / 20 by the 66666667h reciprocal, unrolled 4x by VC7.1 plus a remainder loop; the implicit
//   assignment copies the four data dwords and skips the vptr.

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

// Local polymorphic model (engine/C3DAnimationStateAlpha.h flattens the vptr into a data member, which cannot produce
// the per-level vtable stores of the implicit copy constructor). Layout per Ego_r.pdb: CIVCountedPointeeBase is
// {vptr, IVRefCount} (8 bytes), C3DAnimationState adds AnimationIndex +0x08 / AnimationTime +0x0C, and
// C3DAnimationStateAlpha adds AlphaWeight +0x10 (0x14 bytes). Copy constructor and assignment are implicit: the copy
// stores the three vtables 0x012354D0 / 0x01249F50 / 0x01265128 in base-to-derived order, the assignment skips them.
class CIVCountedPointeeBase {                               // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase();                       // slot 0
    unsigned long IVRefCount;                               // +0x04
};

class C3DAnimationState : public CIVCountedPointeeBase {    // vtable 0x01249F50
public:
    virtual ~C3DAnimationState();
    virtual long GetType() const;
    virtual void Validate();
    long  AnimationIndex;                                   // +0x08
    float AnimationTime;                                    // +0x0C
};

class C3DAnimationStateAlpha : public C3DAnimationState {   // vtable 0x01265128, sizeof 0x14
public:
    virtual long GetType() const;                           // 0x0070FE20
    virtual void Validate();                                // 0x00A3BE20
    float AlphaWeight;                                      // +0x10
};

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

template C3DAnimationStateAlpha* __fastcall _STL::__copy<C3DAnimationStateAlpha*, C3DAnimationStateAlpha*, int>(
    C3DAnimationStateAlpha*, C3DAnimationStateAlpha*, C3DAnimationStateAlpha*, const _STL::random_access_iterator_tag&, int*);