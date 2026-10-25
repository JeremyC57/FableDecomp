// std::__find_if(const CCountedPointer<NShape::CShape>* first, const CCountedPointer<NShape::CShape>* last, NShape::CIsShapeTrack pred, const random_access_iterator_tag&) -- STLport unrolled-by-four search for the first shape the track predicate accepts (shape type 1); returns last when none does
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007649D0, length 496 bytes (__fastcall: first ecx, last edx, the empty
//   predicate dword and the tag reference on the stack, `ret 8`; 0x1EA bytes + int3 pad).
// Identity: Ego_r.pdb has the predicate `??RCIsShapeTrack@NShape@@QBE_NV?$CCountedPointer@VCShape@NShape@@@@@Z` (by-value
//   counted pointer) and the find_if over vector<CCountedPointer<NShape::CShape> >; the catalog label
//   "RBTree_Find_Comparator" is descriptive (there is no tree). Callers: the landed CTCShapeManager::GetBestTrack
//   0x00761B50 (which already names this body as `__find_if<CCountedPointer<NShape::CShape>*, NShape::CIsShapeTrack>`)
//   and 0x00764E1A (CTCShapeManager code, catalog "CScriptBase::RemovePredicate").
// Callees: NShape::CShape virtual slot 1 (+0x04, the shape type) and the count block's destroy member pointer through
//   the inlined predicate; operator delete 0x00BFE9BC (the by-value pointer's last release).
// Shape: STLport 4.x `__find_if(.., const random_access_iterator_tag&)` verbatim: `trip_count = (last - first) >> 2`,
//   four `if (pred(*first)) return first; ++first;` per trip, then `switch (last - first)` falling through cases 3, 2,
//   1 (no increment after case 1) to `return last`. Each pred call copies the element into the predicate's by-value
//   CCountedPointer (inline add-ref), calls GetType() == 1 and releases the copy inline.
void __cdecl operator delete(void* block);                                         // 0x00BFE9BC

template <class T> struct SCountBlock {
    long m_references;                                      // +0x00
    void (T::*m_pfnDestroy)();                              // +0x04
    T*   m_pItem;                                           // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    SCountBlock<T>* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->m_references == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            operator delete(Info);
        }
    }
    T* operator->() const { return Data; }
};

namespace NShape {

class CShape {
public:
    virtual void v00();
    virtual long GetType() const;                           // slot 1 (+0x04)
    void DeleteData();
};

class CIsShapeTrack {
public:
    bool operator()(CCountedPointer<CShape> shape) const { return shape->GetType() == 1; }
};

}

namespace std {

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

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

template const CCountedPointer<NShape::CShape>* __fastcall
__find_if(const CCountedPointer<NShape::CShape>*, const CCountedPointer<NShape::CShape>*, NShape::CIsShapeTrack,
          const random_access_iterator_tag&);

}