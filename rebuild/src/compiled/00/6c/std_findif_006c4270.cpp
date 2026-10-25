// std::__find_if(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CIsThingDead pred, const random_access_iterator_tag&) -- STLport random-access find_if: return the first null or dead thing pointer, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C4270, 301 bytes (+ int3 padding to 303), __fastcall `ret 8`.
// Identity: the catalog's "FindFirst_DeadThing" is a descriptive auto label. The landed CTCFadeOutAndIn::
//   UpdateFadingThings 0x006C3E80 names this callee `find_if(.., CIsThingDead, const random_access_iterator_tag&)`: its
//   three callers are that inline remove_if (0x006C3EA7), the out-of-line find_if front end 0x006C43A0 and the
//   out-of-line remove_if 0x006C43C0; the element is CIntelligentPointer<CThing> (8 bytes) and the predicate the empty
//   CIsThingDead (`GetPItem() == 0 || GetPItem()->IsDead()`, the landed remove_copy_if 0x006C4210 model).
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50 (twice per element).
// Shape: STLport 4.6 _algobase.c `__find_if(.., const random_access_iterator_tag&)` verbatim from a /Gr TU (`__fastcall`:
//   first in ecx -> esi, last in edx -> ebp; the empty predicate and the tag reference on the stack): `trip_count =
//   (last - first) >> 2` (`sar edi,5` on the byte distance), the 4x unrolled predicate tests, then the `switch (last -
//   first)` fall-through tail for 3/2/1 (the `dec eax ; je` chain), every hit returning first and the default last. The
//   Dead bit test's constant 1 is hoisted into bl for the whole function.
class CBaseObject;
class CBaseObjectPointer;

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char StateBits;                                // +0x91 Dead 0x01 (PDB CThing +0xA1)
    bool IsDead() const { return (StateBits & 0x01) != 0; }
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    CBaseObjectPointer* PData;                              // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CIsThingDead {
public:
    CIsThingDead() { }
    bool operator()(const CIntelligentPointer<CThing>& thing) const
    {
        return thing.GetPItem() == 0 || thing.GetPItem()->IsDead();
    }
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template CIntelligentPointer<CThing>* __fastcall __find_if(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*,
                                                           CIsThingDead, const random_access_iterator_tag&);

} // namespace std