// std::__find_if(CCountedPointer<CSubComponentBase>* first, CCountedPointer<CSubComponentBase>* last, CIsOfType pred, const random_access_iterator_tag&) -- STLport random-access find_if over a camera mode's sub-components: the first whose GetType() equals the wanted type, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084F010, 162 bytes, __fastcall under /Gr (ecx = first, edx = last; the 4-byte
//   predicate by value and the tag reference on the stack), `ret 8`.
// Identity: FableWin.pdb ??$find_if@...CCountedPointer@VCSubComponentBase@NCameraModes@@...VCIsOfType@NCameraModes@@@std@@
//   (the later Dinkumware spelling of the same instantiation); callers are the NCameraModes::CBase sub-component members
//   (HasSubComponent 0x0084E0B0, GetSubComponent 0x0084E0E0, RemoveSubComponent 0x0084E373, AddSubComponent
//   0x0084E843/857, TransferSubComponentsFrom 0x0084F2DA), each passing CBase::SubComponents (+0x48/+0x4c). The manifest
//   label "StdVector_LinearSearch" is an autoname.
// Shape: the STLport 4.x `__find_if` random-access body verbatim: trip count (last - first) >> 2 (`sar 5` on 8-byte
//   elements), four unrolled tests per trip, then a fall-through `switch (last - first)` for 3/2/1; CIsOfType compares the
//   element's virtual GetType() (slot 2, +8) with its stored type (ebx).
namespace NCameraModes {
enum ESubComponentType { };
class CSubComponentBase {
public:
    virtual void v00();
    virtual void v01();
    virtual ESubComponentType GetType() const;                  // slot 2 (+8)
};
}

class CCountedPointerInfo;
template <class T> class CCountedPointer {
public:
    T* operator->() const { return Data; }
    T*                   Data;
    CCountedPointerInfo* Info;
};

namespace NCameraModes {
class CIsOfType {
public:
    bool operator()(const CCountedPointer<CSubComponentBase>& subComponent) const { return subComponent->GetType() == Type; }
    ESubComponentType Type;
};
}

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
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

template CCountedPointer<NCameraModes::CSubComponentBase>* __fastcall
__find_if(CCountedPointer<NCameraModes::CSubComponentBase>*, CCountedPointer<NCameraModes::CSubComponentBase>*,
          NCameraModes::CIsOfType, const random_access_iterator_tag&);
}