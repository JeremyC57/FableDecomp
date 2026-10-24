// NUISystem::CScrollableList::OnDeactivate() -- hide the list over 0.2 s and stop observing events 37 and 36
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x005562A0, 37 bytes (plain `ret`).
// Callees: CScrollableList::HideList(float) @ 0x005559D0, the CObserver subobject's virtual slot 4 (+0x10, IgnoreEvent(EEvent),
// 0x0052D940 in CObserver's vtable 0x01246020) twice.
// Label: CScrollableList vtable 0x0124A72C slot 141 (rtti-vtables.tsv); the CRedefinerList override in the same slot is the
// landed OnDeactivate 0x00556580. Ego_r.pdb names ?OnDeactivate@CScrollableList@NUISystem@@UAEXXZ and
// ?HideList@CScrollableList@NUISystem@@IAEXM@Z. The landed 0x00556580 comment calls slot 4 "SetState"; in the CObserver
// vtable it is IgnoreEvent (slot 3 is the landed ObserveEvent 0x0052DA20).
// Layout: the CObserver base sits at +0x04 (vptr, set<EEvent>, PreventObservation).
// Shape: 0.2f pushed as an immediate (0x3E4CCCCD); `this` adjusted to the observer once (`add esi,4`) for both calls.
namespace NUISystem {
enum EEvent { EVENT_36 = 36, EVENT_37 = 37 };
struct _Rb_tree_node_base;
struct CEventSet {                                 // STLport set<EEvent>, 12 bytes
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
};
class CObserver {
public:
    virtual void v000();
    virtual void ProcessEvent(EEvent event) = 0;   // slot 1
    virtual bool IsObservingEvent(EEvent event);   // slot 2
    virtual void ObserveEvent(EEvent event);       // slot 3
    virtual void IgnoreEvent(EEvent event);        // slot 4 (+0x10)
    CEventSet EventsToObserve;                     // +0x04
    bool      PreventObservation;                  // +0x10
};
class CComponentRoot {
public:
    virtual void v000();
};
class CScrollableList : public CComponentRoot, public CObserver {
public:
    void HideList(float time);                     // 0x005559D0
    void OnDeactivate();
};

void CScrollableList::OnDeactivate()
{
    HideList(0.2f);
    IgnoreEvent(EVENT_37);
    IgnoreEvent(EVENT_36);
}
}