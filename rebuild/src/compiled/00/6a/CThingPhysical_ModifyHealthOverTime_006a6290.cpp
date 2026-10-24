// CThingPhysical::ModifyHealthOverTime(float amount, long frames) -- queue a health-over-time effect: push a {frames, amount} helper onto the thing's HealthOverTimeEffects list
// Leaf: ?ModifyHealthOverTime@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A6290, length 70 bytes (+ int3 padding), `ret 8`.
// Callees: malloc @ 0x00BFEA0E (the 0x10-byte list node: next, prev, the 8-byte helper).
// Identity: Ego_r.pdb `?ModifyHealthOverTime@CThingPhysical@@QAEXMJ@Z`; the caller CTCHeroStats::ApplyBonusItemEffects
//   0x0057C4F1 passes a float and a `_ftol` result on the owning CThingPhysical (`mov ecx,ebx`). The catalog's
//   "CTCInventoryProcessor_EnqueueItemEffect" is a descriptive auto label.
// Layout: retail CThingPhysical keeps HealthOverTimeEffects (STLport list, the 4-byte sentinel pointer) at +0x98, as
//   the landed CThingPhysical::SerialiseGameState 0x006A5AF0 model; engine/CThingPhysical.h places it at +0xa8 (the
//   PDB build is larger), so `this` is modelled locally. CHealthOverTimeHelper is 8 bytes: the long frame count +0,
//   the float amount +4 (the node copy stores the long argument at +8 and the float at +0xc).
// Shape: the helper temporary is built in the frame (`sub esp,8`; its float word stored, its long copy-propagated
//   from edi) and passed to the inline STLport push_back -> insert(end(), x) -> _M_create_node chain.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};

template <class _Tp>
struct _List_node : _List_node_base {
    _Tp _M_data;                                        // +0x08
};

template <class _Tp>
class list {                                            // retail STLport list: the sentinel node pointer only
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;                           // +0x00
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* __x) : _M_node(__x) { }
        iterator(const iterator& __x) : _M_node(__x._M_node) { }
    };
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new ((void*)&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};


class CThingPhysical {
public:
    class CHealthOverTimeHelper {
    public:
        long  FramesLeft;                                // +0x00
        float HealthPerFrame;                            // +0x04
        CHealthOverTimeHelper(float amount, long frames) : FramesLeft(frames), HealthPerFrame(amount) { }
    };
    unsigned char _pad00[0x98];                          // +0x00 .. +0x98 (CThing and the CThingPhysical head)
    list<CHealthOverTimeHelper> HealthOverTimeEffects;   // +0x98
    void ModifyHealthOverTime(float amount, long frames);
};

void CThingPhysical::ModifyHealthOverTime(float amount, long frames)
{
    HealthOverTimeEffects.push_back(CHealthOverTimeHelper(amount, frames));
}