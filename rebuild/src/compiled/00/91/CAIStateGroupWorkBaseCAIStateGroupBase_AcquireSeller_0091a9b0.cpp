// CAIStateGroup_WorkBase<CAIStateGroupBase>::AcquireSeller() -- collect the creatures with a shop-keeper component (TC 0x90) within 20 units of our creature, then keep drawing a random one off the list: a candidate already in a creature interaction is skipped, otherwise our creature leaves its current interaction, starts a new type-8 interaction and the seller joins it
// Leaf: ?AcquireSeller@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091A9B0, 471 bytes (+ int3 padding), plain `ret`.
// Ego_r.pdb: ?AcquireSeller@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IAEXXZ.
// Callees: CThing::GetPos @ 0x004C73D0, malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (STLport list nodes), __ftol2 @ 0x00BFEA70,
//   CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::FindThingsInRoughArea<CFilter_Bind2<CThingFilter_HasTC,
//   CThingFilter_In2DRange>, CThingCreatureBase> @ 0x0091A750, CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890,
//   CThingCreatureBase::RemoveFromCreatureInteraction @ 0x006629B0 / NewCreatureInteraction @ 0x00662970 /
//   AddToCreatureInteraction @ 0x00662550.
// Layout: CAIStateGroupBase PBrain +0x04 (CAIBrain PCreature +0x18); retail CThingCreatureBase PCreatureInteraction +0x12C
//   (counted pointer, data word first). Filters per the landed CAIStateGroup_MinionWatchFires::IsRunnable 0x00936210.
// Shape: the search box is the integer creature position +-20; the filters are temporaries bound by `&&`; the size test walks the
//   list each pass; the random index is the landed seed step (`seed*0x24A1 + 0x24DF`, GFROR13, unsigned `% count`); the
//   index walk is a for loop that erases the chosen node (STLport erase, plain free); the same-TU search stand-in keeps the
//   list header in a register. Four details carry the schedule: C3DVector has a user copy ctor (the filter copy reloads
//   the position from its frame copy instead of holding it in callee-saved registers), the search box is a named local
//   built before the filter temporaries, the list ctor self-links a local node pointer before storing it, and the random
//   index is assigned in two arms (`index = r % range` / `index = 0`: retail's `mov ecx,edx ; jmp` / `xor ecx,ecx`).
//   Speed-optimised (default) TU.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
long __fastcall GFROR13(unsigned long); // 0x00497890

class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                          // +0x00
    _List_node_base* _M_prev;                          // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                       // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;                          // the header node (malloc'd)
    list()
    {
        _List_node_base* __tmp = (_List_node_base*)malloc(sizeof(_Node));
        __tmp->_M_next = __tmp;
        __tmp->_M_prev = __tmp;
        _M_node = __tmp;
    }
    ~list()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node->_M_next;
        while (__cur != _M_node) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        free(__n);
        return iterator(__next_node);
    }
};
}

class CCreatureInteraction;
struct CCountedPointerCreatureInteraction {            // CCountedPointer<CCreatureInteraction>
    CCreatureInteraction* Data;
};
class CThing { public:
    const C3DVector& GetPos() const;                    // CThing::GetPos 0x004C73D0
};
class CThingCreatureBase : public CThing {
public:
    unsigned char _pad00[0x12c];
    CCountedPointerCreatureInteraction PCreatureInteraction;   // +0x12C
    void RemoveFromCreatureInteraction();              // 0x006629B0
    void NewCreatureInteraction(long type);            // 0x00662970 (ECreatureInteractionType)
    void AddToCreatureInteraction(const CCountedPointerCreatureInteraction& interaction);   // 0x00662550
};

class CThingFilter_HasTC { public:
    long TCType;
    CThingFilter_HasTC(long type) : TCType(type) { }
};
class CThingFilter_In2DRange { public:
    C3DVector Pos;
    float Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CThingSearchTools { public:
    void FindThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_In2DRange>& filter,
                               std::list<CThingCreatureBase*>& things);   // 0x0091A750
};
// The search instantiation lives in this TU (0x0091A750, just above): a same-TU noinline stand-in that only reads its
// arguments tells VC7.1 the list's address never escapes, so the header pointer stays in a register across every
// later call (retail esi). Not landed from here.
void SearchSink_91a750(const CThingSearchTools* self, long x0, long tcType, float range, std::_List_node_base* header);
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::FindThingsInRoughArea(const C2DBoxI& area,
    const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_In2DRange>& filter, std::list<CThingCreatureBase*>& things)
{
    SearchSink_91a750(this, area.X0, filter.m_pFirst->TCType, filter.m_pSecond->Range, things._M_node);
}
#pragma auto_inline(on)

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                     // +0x18
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                  // +0x04
    unsigned char _pad08[0x0c];
    CThingSearchTools* PeekThingSearchTools();         // 0x008FD000
    unsigned long* GetPWorldSeed();                    // 0x008FCFD0
    unsigned long GetRandomIndex(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        unsigned long index;
        if (range != 0)
            index = rotated % range;
        else
            index = 0;
        return index;
    }
};

template <class Base> class CAIStateGroup_WorkBase : public Base {
public:
    void AcquireSeller();
};

template <class Base> void CAIStateGroup_WorkBase<Base>::AcquireSeller()
{
    C3DVector pos = this->PBrain->PCreature->GetPos();
    CThingCreatureBase* seller = 0;
    std::list<CThingCreatureBase*> sellers;
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 20, y - 20, x + 20, y + 20);
    this->PeekThingSearchTools()->FindThingsInRoughArea(area,
                                                        CThingFilter_HasTC(0x90) && CThingFilter_In2DRange(pos, 20.0f), sellers);
    while (sellers.size() != 0) {
        unsigned long count = sellers.size();
        unsigned long index = this->GetRandomIndex(count);
        for (std::list<CThingCreatureBase*>::iterator it = sellers.begin(); it != sellers.end(); ++it) {
            if (index == 0) {
                seller = *it;
                sellers.erase(it);
                break;
            }
            --index;
        }
        if (seller->PCreatureInteraction.Data != 0)
            continue;
        if (this->PBrain->PCreature->PCreatureInteraction.Data != 0)
            this->PBrain->PCreature->RemoveFromCreatureInteraction();
        this->PBrain->PCreature->NewCreatureInteraction(8);
        seller->AddToCreatureInteraction(this->PBrain->PCreature->PCreatureInteraction);
    }
}

template class CAIStateGroup_WorkBase<CAIStateGroupBase>;