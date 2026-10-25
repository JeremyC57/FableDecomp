// std::remove_copy_if(CCountedPointer<CActionEventBase>* first, CCountedPointer<CActionEventBase>* last, CCountedPointer<CActionEventBase>* result, CIsActionEventFinished pred) -- STLport remove_copy_if over a creature action's event pointers: assign every unfinished event down to result, return the new end
// Leaf: ??$remove_copy_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00696240, length 114 bytes (+ int3 padding), `ret 8`.
// Callers: CCreatureActionBase::FrameUpdate 0x00694020 (landed; the inline remove_if over ActionEvents) and two more.
// Callees: the event's CCPPointerInfo::DeleteFunc (`call [eax+4]`, the object in ecx) and operator delete @ 0x00BFE9BC,
//   both inside the inline CCountedPointer::operator=.
// Label: catalog "std::vector_copy_filtered" is descriptive; STLport 4.x `_algo.h` `remove_copy_if` verbatim (`for ( ;
//   __first != __last; ++__first) if (!__pred(*__first)) { *__result = *__first; ++__result; } return __result;`).
// Layout: Ego_r.pdb CCountedPointer<NCreatureAction::CActionEventBase> {Data +0, CCPPointerInfo* Info +4} (8 bytes),
//   CActionEventBase {vtable, bool Finished +4, ActivationFrame, PThing, PCameraManager}; the predicate is the PDB's empty
//   NCreatureAction::CIsActionEventFinished (Ego_r's remove_if<..., CIsActionEventFinished> over CCreatureActionBase::ActionEvents).
// Shape: /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, result and the empty predicate on the stack;
//   last spilled to the `push ecx` word). CCountedPointer::operator= as the landed _Move bodies (0x0084F130): the source's
//   block and object read first, release + adopt only when the blocks differ.
class CThingCreatureBase;
class CGameCameraManager;

struct CCPPointerInfo {                                      // Ego_r.pdb, 12 bytes
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
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for (; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
}

template TActionEventPtr* __fastcall std::remove_copy_if<TActionEventPtr*, TActionEventPtr*, NCreatureAction::CIsActionEventFinished>(TActionEventPtr*, TActionEventPtr*, TActionEventPtr*, NCreatureAction::CIsActionEventFinished);