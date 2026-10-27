// std::__copy(NGameText::CDataBankGroupEntry* first, NGameText::CDataBankGroupEntry* last, NGameText::CDataBankGroupEntry* result, const random_access_iterator_tag&, int*) -- STLport random-access copy over 4-byte intrusive counted pointers: assigns n = last - first elements with the element's inlined operator= (release the old pointee, add-ref the new one)
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CA6C0, 82 bytes, /Gr __fastcall (first ecx, last edx), `ret 0Ch`.
// Identity: caller NGameText::CDataBank::CloseTextAndGroups 0x009C8660: inline erase(begin(), end()) of GroupEntryBuffer (+0x1F8, CArray<NGameText::CDataBankGroupEntry> per FableWin/Ego_r PDB); the PDB element is 4 bytes {CIVCountedPointer<CGroupEntry> PGroup}, matching the 4-byte stride.
// Shape (family K010, representative 0x0069CB80): STLport 4.6 `__copy(first, last, result, const
//   random_access_iterator_tag&, _Distance*)`: `for (_Distance n = last - first; n > 0; --n) { *result = *first;
//   ++first; ++result; }`. The element assignment is the intrusive pointer's operator= (self test; release through
//   the pointee's inline Release(): `if (--count == 0) Destroy()` = `dec [ecx+4] ; jne ; call [vt+4]` on a register
//   copy, then the clear; copy; add-ref). The pointee's count sits at +4 behind its vptr (PDB).
class CIVCountedPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                  // +0x04 (PDB: ulong IVRefCount)
    void Release() const { if (--IVRefCount == 0) Destroy(); }
};
namespace NGameText { class CGroupEntry; }
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                     // +0x00 (PDB: CIVCountedPointeeBase* Object)
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
namespace NGameText {
class CDataBankGroupEntry {                            // 4 bytes (PDB: CIVCountedPointer<CGroupEntry> PGroup)
public:
    CIVCountedPointer<CGroupEntry> PGroup;             // +0x00; the implicit operator= assigns it
};
}

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

template NGameText::CDataBankGroupEntry* __fastcall __copy<NGameText::CDataBankGroupEntry*, NGameText::CDataBankGroupEntry*, int>(
    NGameText::CDataBankGroupEntry*, NGameText::CDataBankGroupEntry*, NGameText::CDataBankGroupEntry*, const random_access_iterator_tag&, int*);
}