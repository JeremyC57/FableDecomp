// NUISystem::CChangingStateComponent::ForceState(unsigned long state, bool immediate) -- jump a front-end component straight to `state`: record the request, drop the pending state queue, remember the previous state, notify the change, settle at once when the state allows it, then force the same state into every child
// Leaf: ?ForceState@CChangingStateComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052CAF0, length 243 bytes (`ret 8`).
// Callees: free @ 0x00BFEA14 (STLport list node deallocation); virtual slots 135 (+0x21c, state-changed notification),
//          44 (+0xb0, "reached immediately" predicate), 41 (+0xa4, settled notification) on this and 42 (+0xa8, this
//          same ForceState) on each child.
// Layout: the retail CChangingStateComponent of the landed AddChildDynamically 0x0052CAB0 / Update 0x0052C7E0 rows
//          (CComponent 0x134, StatesBeingDone +0x134, StatesDone +0x138, StatesToDo +0x13c (4-byte STLport list head),
//          CurrentState +0x144, TargetState +0x148, RequestedState +0x14c, PreviousState +0x158) plus the child vector
//          of CCountedPointer<CComponent> at +0xb0 (8-byte elements, `sar 3`). The slot names are descriptive.
// Shape: STLport `list::clear()` inline (cursor advanced before the free, head re-read after every call, then
//          self-linked); `PreviousState = CurrentState; CurrentState = TargetState;` (both loaded before the stores);
//          `CurrentState = TargetState = RequestedState` stores TargetState first; the child loop re-reads the vector
//          bounds and RequestedState every pass (the virtual call may change them), `immediate` is hoisted into ebx.
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

namespace NUISystem {
class CComponent;
struct CCountedComponentPointer {                // CCountedPointer<NUISystem::CComponent>
    CComponent* Object;                         // +0
    void*       Info;                           // +4
};
struct CComponentPointerVector {                 // STLport vector<CCountedPointer<CComponent> >
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    CCountedComponentPointer* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CCountedComponentPointer& operator[](unsigned int n) { return *(_M_start + n); }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct CStateList {                              // STLport list<unsigned long>: the 4-byte sentinel pointer
    _List_node_base* _M_node;
    void clear()
    {
        _List_node_base* __cur = _M_node->_M_next;
        while (__cur != _M_node) {
            _List_node_base* __tmp = __cur;
            __cur = __cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040();
    virtual void OnStateReached();   // slot 41 (+0xa4)
    virtual void ForceState(unsigned long state, bool immediate);   // slot 42 (+0xa8)
    virtual void v043();
    virtual bool IsStateReachedImmediately(unsigned long state);   // slot 44 (+0xb0)
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void OnStateChanged(unsigned long state);   // slot 135 (+0x21c)
    unsigned char _pad04[0xac];
    CComponentPointerVector Children;           // +0xb0
    unsigned char _pad0bc[0x78];
};

class CChangingStateComponent : public CComponent {
public:
    unsigned long StatesBeingDone;              // +0x134
    unsigned long StatesDone;                   // +0x138
    CStateList    StatesToDo;                   // +0x13c
    float         ParentUpdateTime;             // +0x140
    unsigned long CurrentState;                 // +0x144
    unsigned long TargetState;                  // +0x148
    unsigned long RequestedState;               // +0x14c
    float         UpdateTime;                   // +0x150
    bool          PreviousUpdateChanged;        // +0x154
    unsigned long PreviousState;                // +0x158
    void ForceState(unsigned long state, bool immediate);
};

void CChangingStateComponent::ForceState(unsigned long state, bool immediate)
{
    RequestedState = state;
    StatesDone = 0;
    StatesBeingDone = 0;
    StatesToDo.clear();
    PreviousState = CurrentState;
    CurrentState = TargetState;
    OnStateChanged(state);
    if (IsStateReachedImmediately(state)) {
        CurrentState = TargetState = RequestedState;
        OnStateReached();
    }
    for (unsigned int i = 0; i < Children.size(); ++i)
        Children[i].Object->ForceState(RequestedState, immediate);
}

}
