// CAIStateGroup_HandyMan::AcquireWorkSpot() -- drop the current work spot and pick a random other object with the work-spot property within 20 units (searched in the 60-unit square round the creature); false when there is none
// Leaf: ?AcquireWorkSpot@CAIStateGroup_HandyMan@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091B010, length 351 bytes, plain `ret`. Ego_r.pdb name (private, bool);
//   Ego_r donor order: GetAllThingsInRoughArea<CThingObject, list<CThingObject*>, CFilter_Bind2<CThingFilter_ObjectHasProperty,
//   CThingFilter_In2DRange> > immediately before it, IsRunnable / Update after it.
// Callees: CThing::GetPos 0x004C73D0, malloc 0x00BFEA0E (the list header), _ftol 0x00BFEA70, CAIStateGroupBase::
//   PeekThingSearchTools 0x008FD000, CThingSearchTools::GetAllThingsInRoughArea<...> 0x0091ADB0, list<CThingObject*>::remove
//   0x0091AD70, _List_base<CThingObject*>::~_List_base 0x00792240 (STLport list has no destructor of its own; out of line on both exits), CAIStateGroupBase::GetPWorldSeed
//   0x008FCFD0, GFROR13 0x00497890 (GFRandom inline; the pick walks with STLport's signed bidirectional __advance).
// Shape: same-TU noinline bodies of the search, remove and ~list show VC7.1 the list header never changes, so it stays in
//   esi; the filter binder (pointers to its two filters) reuses the dead position copy's slot. C3DVector's user-declared
//   copy constructor keeps the position copy in memory (re-read for the range filter), freeing ebp for the shared zero;
//   the box is a named local built before the call; empty() then size() (VC folds the first walk into the test); the
//   random index is taken before begin() is read.
// Layout: Ego_r.pdb CAIStateGroup_HandyMan (WorkSpot +0x14), CFilter_Bind2 {F1, F2 pointers}, CThingFilter_In2DRange
//   {Pos, Range}, CThingFilter_ObjectHasProperty {Property}, C2DBoxI {TLX, TLY, BRX, BRY}.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

namespace std {
template <class T> class allocator { };
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T, class A> class _List_base { public:
    typedef _List_node<T> _Node;
    _Node* _M_node;
    _List_base()
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~_List_base();
};
template <class T, class A = allocator<T> > class list : public _List_base<T, A> { public:
    typedef _List_node<T> _Node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_Node* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    void push_back(const T& x)
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_data = x;
        _Node* prev = _M_node->_M_prev;
        node->_M_next = _M_node;
        node->_M_prev = prev;
        prev->_M_next = node;
        _M_node->_M_prev = node;
    }
    void remove(const T& value);
};
}

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
class CThing { public:
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingObject : public CThing { public: };
class CThingCreatureBase : public CThing { public: };
enum EObjectProperty { };
class CThingFilter_ObjectHasProperty { public:
    EObjectProperty Property;
    CThingFilter_ObjectHasProperty(EObjectProperty property) : Property(property) {}
};
class CThingFilter_In2DRange { public:
    C3DVector Pos;
    float Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) {}
};
template <class F1T, class F2T> class CFilter_Bind2 { public:
    const F1T* F1;
    const F2T* F2;
    CFilter_Bind2(const F1T& f1, const F2T& f2) : F1(&f1), F2(&f2) {}
};
class CThingSearchTools { public:
    template <class T, class C, class F> void GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& things) const;
};
CThingObject* FindThingInRoughArea(const C2DBoxI* box, const void* filter);

// Same-TU bodies of 0x0091ADB0, 0x0091AD70 and 0x00792240, kept out of line as in retail.
template <class T, class C, class F> __declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& things) const
{
    CThingObject* thing = FindThingInRoughArea(&box, &filter);
    if (thing)
        things.push_back(thing);
}
template <class T, class A> __declspec(noinline) void std::list<T, A>::remove(const T& value)
{
    _Node* cur = _M_node->_M_next;
    while (cur != _M_node) {
        _Node* next = cur->_M_next;
        if (cur->_M_data == value) {
            cur->_M_prev->_M_next = next;
            next->_M_prev = cur->_M_prev;
            free(cur);
        }
        cur = next;
    }
}
template <class T, class A> __declspec(noinline) std::_List_base<T, A>::~_List_base()
{
    _Node* cur = _M_node->_M_next;
    while (cur != _M_node) {
        _Node* tmp = cur;
        cur = cur->_M_next;
        free(tmp);
    }
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
    if (_M_node)
        free(_M_node);
}

namespace std {
template <class I> inline void advance(I& it, long n)          // STLport __advance, bidirectional
{
    if (n > 0)
        while (n--) it = it->_M_next;
    else
        while (n++) it = it->_M_prev;
}
}

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                      // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                   // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
};
class CAIStateGroup_HandyMan : public CAIStateGroupBase {
    CThingObject* WorkSpot;                             // +0x14
    bool AcquireWorkSpot();
};

bool CAIStateGroup_HandyMan::AcquireWorkSpot()
{
    C3DVector pos = GetCreature()->GetPos();
    CThingObject* old_work_spot = WorkSpot;
    WorkSpot = 0;
    std::list<CThingObject*> work_spots;
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI box(x - 30, y - 30, x + 30, y + 30);
    PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(box,
        CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_In2DRange>(
            CThingFilter_ObjectHasProperty((EObjectProperty)0x4000000), CThingFilter_In2DRange(pos, 20.0f)),
        work_spots);
    work_spots.remove(old_work_spot);
    if (work_spots.empty())
        return false;
    long index = GFRandom(work_spots.size(), GetPWorldSeed());
    std::_List_node<CThingObject*>* it = work_spots._M_node->_M_next;
    std::advance(it, index);
    WorkSpot = it->_M_data;
    return true;
}