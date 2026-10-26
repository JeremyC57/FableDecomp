// _global::UninitCopy_PairULongAnimationEntry_005e16f0(const std::pair<unsigned long,CAnimationEntry>* first, const std::pair<unsigned long,CAnimationEntry>* last, std::pair<unsigned long,CAnimationEntry>* result, const __false_type&) -- STLport __uninitialized_copy over CAnimationSet's 20-byte {key, CAnimationEntry} map elements: placement-copy, add-ref'ing each entry's component map
// Leaf: UninitCopy_PairULongAnimationEntry_005e16f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E16F0, length 76 bytes (+ int3 padding), `ret 8`.
// Callers: 12 vector/CArray helpers of the CVectorMap<unsigned long, CAnimationEntry> TU (e.g. the allocate-and-copy
// @ 0x005E17C0: `malloc(n * 20)`, then this call with `lea` of a dead slot as the tag reference).
// Layout: the element of CAnimationSet::Anims (landed pair<unsigned long, CAnimationEntry>::operator= @ 0x005E0E20):
// first +0x00, then the PDB CAnimationEntry (BankIndex +0x04, AnimName +0x08, GroupName +0x0C, Components +0x10, an
// intrusive CIVCountedPointer whose pointee keeps the count at +4).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx, result + the tag reference on the stack). Placement new (null test on the destination) of the pair's implicit copy
// constructor: the key, BankIndex, the two CDefStrings, then the counted pointer's copy (`p = o.p ; if (p) ++p->count`,
// `test ebx,ebx ; mov [edi+4],ebx ; je ; inc [ebx+4]`). The result (eax throughout) is returned.
// Label correction: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` is propagated.
inline void* operator new(unsigned int, void* p) { return p; }

class CIVCountedPointeeBase { public:
    virtual void v0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long m_refCount;                            // +0x04
};
template<class T> class CIVCountedPointer { public:    // intrusive counted pointer (4 bytes)
    T* m_pObject;
    __forceinline CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject)
    {
        if (m_pObject)
            ++m_pObject->m_refCount;
    }
};
class CDefString { public: long TablePos; };
class CAnimComponentMap : public CIVCountedPointeeBase { public: };

class CAnimationEntry { public:                        // PDB layout, 0x10 bytes
    long BankIndex;                             // +0x00
    CDefString AnimName;                        // +0x04
    CDefString GroupName;                       // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0c
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
typedef std::pair<unsigned long, CAnimationEntry> CAnimPair;   // 0x14 bytes

struct __false_type { __false_type() { } };

CAnimPair* __fastcall UninitCopy_PairULongAnimationEntry_005e16f0(const CAnimPair* first, const CAnimPair* last, CAnimPair* result, const __false_type&)
{
    CAnimPair* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CAnimPair(*first);
    return cur;
}