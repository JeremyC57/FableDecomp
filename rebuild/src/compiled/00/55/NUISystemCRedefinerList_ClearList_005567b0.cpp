// NUISystem::CRedefinerList::ClearList() -- remove every non-null child component of the redefiner list, then reset the key redefiners' button counts
// Leaf: ?ClearList@CRedefinerList@NUISystem@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005567B0, 84 bytes, thiscall, tail `jmp CKeyRedefiner::ResetButtonCounts`.
// Identity (proven): Ego_r (0x005CF980) and FableWin (0x022AF2D0) both define the protected ?ClearList@CRedefinerList@NUISystem@@IAEXXZ;
//   the Ego_r CRedefinerList block (OnDeactivate 0x005CF930, ClearList 0x005CF980 .. NotifyOfRedefinition 0x005CFD40) keeps the retail
//   order (ClearList 0x005567B0 < NotifyOfRedefinition 0x00556980, GOTCHAS chunk 146); its callers 0x00557003 / 0x005570DC are the
//   landed CRedefinerList::Refresh 0x00557000 (which names this callee) and OnActivate 0x005570F0. The landed binding spells it QAE
//   (public): the PDB access is protected (NAME_FIXES).
// Callees: CComponent::RemoveChild(unsigned long) through the primary vtable slot 64 (+0x100; Ego_r / FableWin
//   ?RemoveChild@CComponent@NUISystem@@UAEXK@Z, body 0x00533BC0 indexes Children at +0xB0 with the argument),
//   NUISystem::CKeyRedefiner::ResetButtonCounts() 0x00557840 (both PDBs ?ResetButtonCounts@CKeyRedefiner@NUISystem@@SIXXZ, a static
//   __fastcall member: the body zeroes the two CKeyRedefiner static counts at 0x013B8ACC / 0x013B8AD0).
// Layout (engine/CObserver.h via the layout-rename include; RTTI CRedefinerList : CScrollableList : CChangingStateComponent : CComponent : CUIInterface @0, CObserver @4, CActionParam
//   @0x18): CObserver = vptr + EventsToObserve (STLport set, 12) + PreventObservation (0x14 bytes), CActionParam = vptr +
//   ActionParamType; CComponent's own members from +0x20 in Ego_r order with retail container sizes, Children (STLport
//   vector<CCountedPointer<CComponent> >, 8-byte elements) at +0xB0 (Ego_r 0xB8: two Dinkumware containers before it). No engine header
//   exists for these NUISystem classes; the 64 virtual slots before RemoveChild are unnamed here (a virtual name emits no symbol).
// Shape: `for (unsigned long i = 0; i < Children.size(); )` with STLport's size() recomputed after every body (the loop calls
//   out), `if (!Children[i].IsNull()) RemoveChild(i); else ++i;` -- RemoveChild shifts the next child into slot i.
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#define CObserver CObserverFields                               // engine/CObserver.h is flattened (vptr as data): layout-rename include
#include "engine/CObserver.h"
#undef CObserver

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x0
    _Tp* _M_finish;                                             // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
};
}   // namespace std

namespace NUISystem {

class CKeyRedefiner {
public:
    static void __fastcall ResetButtonCounts();                 // 0x00557840
};

class CUIInterface {                                            // RTTI: no base (vptr only)
public:
    virtual ~CUIInterface();                                    // slot 0
};
enum EEvent { };
class CObserver {                                               // at +0x04 in CComponent; virtual list as the landed CObservable::SetEvent 0x0055CB10
public:
    virtual ~CObserver();
    virtual void ProcessEvent(EEvent) = 0;
    virtual bool IsObservingEvent(EEvent);
    virtual void ObserveEvent(EEvent);
    virtual void IgnoreEvent(EEvent);
    virtual void IgnoreAllEvents();
    virtual void ObserveAllEvents();
    virtual void SetPreventObserving(bool);
protected:
    unsigned char EventsToObserve[0xc];                         // +0x04 set<EEvent> (STLport)
    bool          PreventObservation;                           // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CObserver) == sizeof(CObserverFields));
class CActionParam {                                            // at +0x18 in CComponent
public:
    virtual ~CActionParam();
    long ActionParamType;                                       // +0x04 EActionParamType
};

class CComponent : public CUIInterface, public CObserver, public CActionParam {   // primary vtable slots 0-64 (RemoveChild 64)
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void RemoveChild(unsigned long index);              // slot 64 0x00533BC0
    unsigned char Name[0x4];                                    // +0x20 CCharString
    unsigned char States[0xc];                                  // +0x24 map<unsigned long, CUIState> (STLport)
    float         Time;                                         // +0x30
    float         Position[2], TargetPosition[2], InitialPosition[2], ParentPosition[2], RenderPosition[2];   // +0x34 C2DVector x5
    float         Zoom[2], TargetZoom[2], InitialZoom[2], ParentZoom[2], RenderZoom[2];                       // +0x5c C2DVector x5
    unsigned long Colour, TargetColour, InitialColour, ParentColour, RenderColour;                            // +0x84 CRGBColour x5
    float         PositionTimeElapsed, PositionTime, ZoomTimeElapsed, ZoomTime, ColourTimeElapsed, ColourTime;   // +0x98
    std::vector<CCountedPointer<CComponent> > Children;         // +0xb0
};
class CChangingStateComponent : public CComponent { };
class CScrollableList : public CChangingStateComponent { };

class CRedefinerList : public CScrollableList {                 // vtables 0x0124A994 (primary), 0x0124A968, 0x0124A970
protected:
    void ClearList();
};

void CRedefinerList::ClearList()
{
    for (unsigned long i = 0; i < Children.size(); ) {
        if (!Children[i].IsNull())
            RemoveChild(i);
        else
            ++i;
    }
    CKeyRedefiner::ResetButtonCounts();
}

}   // namespace NUISystem