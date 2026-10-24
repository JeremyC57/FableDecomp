// _global::UninitCopy_PairAnimComponent_005e0c60(const PairAnimComponent* first, const PairAnimComponent* last, PairAnimComponent* result, const __false_type&) -- STLport __uninitialized_copy over pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>: placement-copy each 8-byte pair (add-ref'ing the component), return the advanced result
// Leaf: UninitCopy_PairAnimComponent_005e0c60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0C60, length 51 bytes (+ int3 padding), `ret 8`.
// Callers: the vector regrowth/assign helpers of the same TU @ 0x005E0D29, 0x005E251B, 0x005E2A1D, 0x005E2A73.
// Callees: none (the element copy is fully inline).
// Identity: the TU is the CAnimationSet / anim-component sort TU (landed neighbours: the 8-byte pair's __copy_backward
//   @ 0x005E0A00, its __unguarded_linear_insert @ 0x005E07F0, the pair's _Destroy @ 0x005E0160/0x005E0C00, the
//   __uninitialized_fill_n of the same pair right after this row @ 0x005E0CA0 with the identical element copy).
//   The catalog's `pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>` label is propagated: that pair is
//   16 bytes with a non-intrusive count block, this element is 8 bytes with an INTRUSIVE count at pointee +4.
// Layout: pair<EAnimComponent first +0x0, CIVCountedPointer<CAnimComponentBase> second +0x4>; the pointee keeps the
//   CIVCountedPointeeBase reference count at +4.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). Empty test before the esi save, placement-new null test,
//   the key word, then the counted pointer's copy (`test esi,esi ; mov [eax+4],esi ; je ; inc [esi+4]`).
inline void* operator new(unsigned int, void* where) { return where; }

enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

struct CAnimComponentBase {                  // CIVCountedPointeeBase-derived
    virtual void Destroy(int flags);         // slot 0
    virtual void Delete();                   // slot 1 (+0x04)
    unsigned long IVRefCount;                // +0x04
};

struct CIVCountedPointer {                   // CIVCountedPointer<CAnimComponentBase>, 4 bytes
    CAnimComponentBase* Object;              // +0x00
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct PairAnimComponent {                   // std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >, 8 bytes
    EAnimComponent    first;                 // +0x00
    CIVCountedPointer second;                // +0x04
    PairAnimComponent(const PairAnimComponent& other) : first(other.first), second(other.second) { }
};

struct __false_type { __false_type() { } };

PairAnimComponent* __fastcall UninitCopy_PairAnimComponent_005e0c60(const PairAnimComponent* first, const PairAnimComponent* last, PairAnimComponent* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) PairAnimComponent(*first);
    return result;
}