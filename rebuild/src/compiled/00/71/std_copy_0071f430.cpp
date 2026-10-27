// std::__copy(const CDefPointer<const CThingBaseDef>* first, const CDefPointer<const CThingBaseDef>* last, CDefPointer<const CThingBaseDef>* result, const random_access_iterator_tag&, int*) -- STLport random-access copy over 4-byte intrusive counted pointers: assigns n = last - first elements with the element's inlined operator= (release the old pointee, add-ref the new one)
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071F430, 82 bytes, /Gr __fastcall (first ecx, last edx), `ret 0Ch`.
// Identity: only callers 0x007211D4/0x00721202 in vector<CDefPointer<const CThingBaseDef> >::operator= 0x00721150 (CTCHeroMorph::Tattoos, Ego_r.pdb member type): __copy_ptrs(x._M_start (const), ..) arms.
// Shape (family K010, representative 0x0069CB80): STLport 4.6 `__copy(first, last, result, const
//   random_access_iterator_tag&, _Distance*)`: `for (_Distance n = last - first; n > 0; --n) { *result = *first;
//   ++first; ++result; }`. The element assignment is the intrusive pointer's operator= (self test; release through
//   the pointee's inline Release(): `if (--count == 0) Destroy()` = `dec [ecx+4] ; jne ; call [vt+4]` on a register
//   copy, then the clear; copy; add-ref). The pointee's count sits at +4 behind its vptr (PDB).
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1 (+0x04)
    mutable long RefCount;                             // +0x04
    void Release() const { if (--RefCount == 0) Destroy(); }
};
class CThingBaseDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                           // +0x00 (PDB: CDefPointeeBase* Object)
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

namespace std {
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

template CDefPointer<const CThingBaseDef>* __fastcall __copy<const CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*, int>(
    const CDefPointer<const CThingBaseDef>*, const CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*, const random_access_iterator_tag&, int*);
}