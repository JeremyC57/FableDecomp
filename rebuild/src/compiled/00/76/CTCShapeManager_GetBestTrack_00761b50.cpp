// CTCShapeManager::GetBestTrack(const NShape::CTrack** out) const -- find the first persistent shape that is a track; write it through out and return whether one exists
// Leaf: ?GetBestTrack@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00761B50, 56 bytes, `ret 4`.
// Identity: Ego_r.pdb ?GetBestTrack@CTCShapeManager@@QBE_NPAPBVCTrack@NShape@@@Z (the manifest's "Map_FindByKey" is a
//   forge autoname). Callers: CTCCameraPointTrack::Validate 0x0075DC48, CTCCameraPointTrack::GetActiveCameraPosition
//   0x0075DCA6, 0x0087B893.
// Callee: STLport __find_if<CCountedPointer<NShape::CShape>*, NShape::CIsShapeTrack>(first, last, pred,
//   const random_access_iterator_tag&) @ 0x007649D0 (/Gr: first ecx, last edx; the unrolled-by-4 loop that copies each
//   element into the by-value predicate, calls CShape slot 1 GetType and tests `== 1`).
// Shape: `std::find_if(PersistantShapes.begin(), PersistantShapes.end(), NShape::CIsShapeTrack())` -- the empty POD
//   predicate is value-initialised (`mov byte [esp+8],0`) and pushed as a dword, the iterator tag is homed in the out
//   argument slot; end is re-read after the call; the hit's shape pointer is stored through out.
// Layout: retail CTCShapeManager (as the landed 0x00760910): PersistantShapes (vector<CCountedPointer<NShape::CShape> >,
//   8-byte elements {Data, Info}) +0x0C/+0x10/+0x14, SelectedShape +0x18.
struct CThing;
struct CCPPointerInfo;

namespace NShape {
class CShape {
public:
    virtual void v00();
};
class CTrack : public CShape {
};
}

template <class T> class CCountedPointer {
public:
    T* Data;                                      // +0x00
    CCPPointerInfo* Info;                         // +0x04
    T* operator->() const { return Data; }
};

namespace NShape {
struct CIsShapeTrack {
    bool operator()(CCountedPointer<CShape> shape) const;
};
}

namespace _STL {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _Tp> inline random_access_iterator_tag iterator_category(const _Tp*)
{
    return random_access_iterator_tag();
}

// 0x007649D0 is retail's out-of-line COMDAT; the same-TU body (STLport's loop, the tag never touched) lets VC7.1 home
// the caller's empty tag temporary in the out argument slot as retail does.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Predicate>
__declspec(noinline) _RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last,
                                                            _Predicate __pred, const random_access_iterator_tag&)
{
    for ( ; __first != __last; ++__first)
        if (__pred(*__first))
            break;
    return __first;
}
#pragma auto_inline(on)

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, iterator_category(__first));
}

template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp* begin() const { return _M_start; }
    const _Tp* end() const { return _M_finish; }
};
}

class CTCShapeManager {
public:
    virtual void v00();
    CThing* ParentThing;                                                  // +0x04 (CTCBase)
    bool ToBeRemoved;                                                     // +0x08
    bool IsInGlobalUpdateTCs;                                             // +0x09
    _STL::vector<CCountedPointer<NShape::CShape> > PersistantShapes;      // +0x0C
    const CCountedPointer<NShape::CShape>* SelectedShape;                 // +0x18
    bool GetBestTrack(const NShape::CTrack** out) const;
};

bool CTCShapeManager::GetBestTrack(const NShape::CTrack** out) const
{
    const CCountedPointer<NShape::CShape>* it =
        _STL::find_if(PersistantShapes.begin(), PersistantShapes.end(), NShape::CIsShapeTrack());
    if (it != PersistantShapes.end()) {
        *out = static_cast<const NShape::CTrack*>((*it).operator->());
        return true;
    }
    return false;
}