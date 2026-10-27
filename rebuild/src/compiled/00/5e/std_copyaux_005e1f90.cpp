// std::__copy_aux(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry>* result, const __false_type&) -- STLport copy dispatch for a non-trivial element: forward to the random-access __copy with the iterator category and distance type of a pointer
// Leaf: ??$__copy_aux@PAU?$pair@KVCAnimationEntry@@@std@@PAU12@@std@@YIPAU?$pair@KVCAnimationEntry@@@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe @ 0x005E1F90, 20 bytes, `__fastcall` (first ecx, last edx; result and the __false_type reference on
// the stack), `ret 8`.
// Identity (probable): STLport 4.6 `_algobase.h` `__copy_aux(first, last, result, const __false_type&) { return
// __copy(first, last, result, _STLP_ITERATOR_CATEGORY(first, _InputIter), _STLP_DISTANCE_TYPE(first, _InputIter)); }` --
// the only callee is the landed std::__copy<pair<unsigned long, CAnimationEntry>*, ..., int> 0x005E18B0 (same element:
// the 0x14-byte (CRC, CAnimationEntry) pair, Ego_r.pdb CAnimationEntry), first/last passed through in ecx/edx. It sits in
// that CAnimationEntry STL run (0x005E18B0 .. 0x005E20C0); no direct caller (the xref at 0x0041C3EC is not a code
// reference): an unreferenced COMDAT.
// Callees: std::__copy<pair<unsigned long, CAnimationEntry>*, pair<unsigned long, CAnimationEntry>*, int> 0x005E18B0.
// Shape: `push 0` (the (ptrdiff_t*)0 distance type), the empty random_access_iterator_tag temporary homed in the dead
// `result` argument slot (`lea eax,[esp+8]`), result re-pushed, the call; not a tail jump because the tag lives in this
// frame.
// Model: the landed __copy 0x005E18B0's element model.
class CIVCountedPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template <class T>
class CIVCountedPointer { public:
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

class CDefString { public:                             // one word (Ego_r.pdb: TablePos)
    long TablePos;                              // +0x00
};
class CAnimComponentMap : public CIVCountedPointeeBase { public: };
class CAnimationEntry { public:                        // 16 bytes
    long                                 BankIndex;     // +0x00
    CDefString                           AnimName;      // +0x04
    CDefString                           GroupName;     // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0C
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
struct __false_type { };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Tp> inline random_access_iterator_tag __fastcall iterator_category(const _Tp*) { return random_access_iterator_tag(); }
template <class _Tp> inline int* __fastcall distance_type(const _Tp*) { return (int*)(0); }

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*);      // 0x005E18B0

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, iterator_category(__first), distance_type(__first));
}
typedef pair<unsigned long, CAnimationEntry> CAnimationEntryPair;
template CAnimationEntryPair* __fastcall __copy_aux(CAnimationEntryPair*, CAnimationEntryPair*, CAnimationEntryPair*,
                                                    const __false_type&);
}