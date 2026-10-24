// _global::CopyBackward_PairULongAnimationEntry_005e0e70(PairULongAnimationEntry*, PairULongAnimationEntry*, PairULongAnimationEntry*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over pair<unsigned long, CAnimationEntry>, returns the moved-back result
// Leaf: CopyBackward_PairULongAnimationEntry_005e0e70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0E70, length 144 bytes.
// Callees: the shared component map's virtual slot 1 (+0x04, destroy on the last reference); no direct calls.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in ecx,
// last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n) *--result = *--last;`
// with the signed count computed once; the moved-back result is returned.
// Element (the manifest label is right; 20 bytes, 0x66666667 multiply + `sar 3`): Ego_r.pdb CAnimationEntry (16) =
// { long BankIndex; CDefString AnimName; CDefString GroupName; CIVCountedPointer<CAnimComponentMap> Components }.
// The implicit operator= moves the key, BankIndex and the two one-word CDefStrings, then runs the inlined intrusive
// CIVCountedPointer::operator=: when the pointers differ, release the old object (`dec [obj+4]`, virtual slot 1 at
// zero, store 0), adopt the source's pointer and add a reference. The count lives in the dead `result` argument
// slot; the strength-reduced src+0x10/dst+0x10 pointer pair addresses the counted pointer.
struct CAnimComponentMap {                      // intrusively counted: count at +4
    virtual void v00();
    virtual void Destroy();                      // slot 1 (+0x04)
    long m_refCount;                             // +0x04
};

template <class T>
struct CIVCountedPointer {                       // one word: the counted object
    T* Object;
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};

struct CDefString {                              // Ego_r.pdb: 4 bytes
    long TablePos;                               // +0x00
};

struct CAnimationEntry {                         // Ego_r.pdb: 16 bytes
    long                                 BankIndex;    // +0x00
    CDefString                           AnimName;     // +0x04
    CDefString                           GroupName;    // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;   // +0x0C
};

struct PairULongAnimationEntry {                 // std::pair<unsigned long, CAnimationEntry>, 20 bytes
    unsigned long   first;                       // +0x00
    CAnimationEntry second;                      // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairULongAnimationEntry* __fastcall CopyBackward_PairULongAnimationEntry_005e0e70(PairULongAnimationEntry* first, PairULongAnimationEntry* last, PairULongAnimationEntry* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}