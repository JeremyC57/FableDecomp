// std::__find_if(CCountedPointer<CActionEventBase>* first, CCountedPointer<CActionEventBase>* last, CIsActionEventFinished pred, const random_access_iterator_tag&) -- STLport four-way unrolled find_if over a creature action's event pointers: the first event whose Finished flag is set, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00695E30, length 128 bytes, `ret 8`.
// Callers: CCreatureActionBase::FrameUpdate 0x00694020 (landed; 0x00694099: the inline remove_if over ActionEvents),
//   0x0069622A, 0x0069661E.
// Callees: none.
// Label: catalog "std::find_if_nonzero" is descriptive; STLport 4.x `_algobase.c` random-access `__find_if` verbatim
//   (`__trip_count = (__last - __first) >> 2; for ( ; __trip_count > 0 ; --__trip_count) { four tests }` then the
//   `switch (__last - __first) { case 3: case 2: case 1: ... case 0: default: return __last; }` fall-through tail).
// Layout: Ego_r.pdb CCountedPointer<NCreatureAction::CActionEventBase> {Data +0, CCPPointerInfo* Info +4} (8 bytes),
//   CActionEventBase {vtable, bool Finished +4, ActivationFrame, PThing, PCameraManager}; the predicate is the PDB's empty
//   NCreatureAction::CIsActionEventFinished (Ego_r's remove_if<..., CIsActionEventFinished> over CCreatureActionBase::ActionEvents).
// Shape: /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, the empty predicate and the tag reference on
//   the stack); the predicate's bool rides in bl.
class CThingCreatureBase;
class CGameCameraManager;

class CCPPointerInfo { public:                                      // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void*);                    // +0x04 (called with the object in ecx)
    void*         Data;                                      // +0x08
};

template <class T> class CCountedPointer {                   // Ego_r.pdb, 8 bytes
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    T* operator->() const { return Data; }
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;                             // operator delete 0x00BFE9BC
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

namespace NCreatureAction {
class CActionEventBase {                                     // Ego_r.pdb, 20 bytes
public:
    virtual void v00();                                      // +0x00 vtable
    bool                Finished;                            // +0x04
    long                ActivationFrame;                     // +0x08
    CThingCreatureBase* PThing;                              // +0x0C
    CGameCameraManager* PCameraManager;                      // +0x10
};
class CIsActionEventFinished {                               // Ego_r.pdb, empty predicate
public:
    bool operator()(const CCountedPointer<CActionEventBase>& event) const { return event->Finished; }
};
}

typedef CCountedPointer<NCreatureAction::CActionEventBase> TActionEventPtr;

namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred, const random_access_iterator_tag&)
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
}

template TActionEventPtr* __fastcall std::__find_if<TActionEventPtr*, NCreatureAction::CIsActionEventFinished>(TActionEventPtr*, TActionEventPtr*, NCreatureAction::CIsActionEventFinished, const std::random_access_iterator_tag&);