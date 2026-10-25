// NUISystem::CSpellContainer::UnAssignSpell(unsigned long slot) -- undoes a spell assignment in a slot: a pending new spell there is deleted and the slot's base spell restored; otherwise the slot's child is deleted and replaced (also as the base spell) by a fresh empty-slot component that copies the old child's per-state position and colour
// Leaf: ?UnAssignSpell@CSpellContainer@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00545B10, 730 bytes (0x2DA + int3 padding to 736), thiscall `ret 4`, /O2 esp frame
//   (sub esp,18h). Slot 144 of the CSpellContainer vtable 0x0124786C (absolute ref 0x01247AAC). Identity: Ego_r.pdb
//   ?UnAssignSpell@CSpellContainer@NUISystem@@UAEXK@Z (slot 145 is SetEmptySlotDef 0x00546060, 143 AssignSpell 0x005458A0,
//   146 AcceptAssignments 0x00545670); the catalog's "AnimationEntry_ProcessLoad" is wrong.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (list header nodes, 0xc bytes), CDeletion(const CDeletion&) 0x0042CD84 and
//   ~list<CComponent*> 0x0042ABCA (the pending-spell branch; the other branch inlines both),
//   list<CComponent*>::_M_insert_dispatch(iterator, const_iterator, const_iterator, const __false_type&) 0x0042B4C3,
//   CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall) / CreateComponent(long) 0x0041D21B, operator new 0x00BFEA1A /
//   delete 0x00BFE9BC, CCountedPointer delete function 0x00428B10, map<unsigned long, CUIState>::operator[] 0x0042D5B1;
//   virtual slots: CComponent SetDeletion(CDeletion) 62 (+0xf8) / GetStates 82 (+0x148), CSpellContainer
//   AddChildDynamically 142 (+0x238).
// Layout: retail CComponent Children (vector<CCountedPointer<CComponent>>) +0xb0; CSpellContainer (Ego_r.pdb - 0x30)
//   BaseSpells +0x15c, NewSpell (pair<CComponent*, unsigned long>) +0x168, EmptySlot +0x170; CDeletion {Method,
//   list<CComponent*> AssociatedParents} (8 bytes, passed by value); CUIState Position +0x08, Colour +0x18.
// Shape: both branches build a CDeletion local (STLport list ctor: header node malloc + self-link) with Method 1 and pass
//   it by value: VC7.1 calls the copy ctor / ~list out of line in the first branch and inlines them in the second (the
//   list copy is the range insert, whose __false_type tag sits in the live `slot` slot: the insert has a same-TU stand-in
//   that never reads the tag; the const_iterators have the implicit copy, the iterator STLport's converting ctor).
//   The pending branch restores BaseSpells[NewSpell.second]. The counted-pointer ctor tests the stored Data (the GetStates
//   call then reuses eax on the null path); the vector index is STLport `*(begin() + n)`; the per-state copy evaluates the
//   old child's state first; the map's operator[] has a same-TU stand-in (as in AddTextLine 0x00546320).
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
void __cdecl operator delete(void* p);                                               // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);                                        // 0x00BFEA1A

void InsertListRange(void* list, void* position, void* first, void* last);             // stand-in helper
void* LookupState(void* map, unsigned long key);                                       // stand-in helper

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                                                          // +0x00
    _List_node_base* _M_prev;                                                          // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                                                       // +0x08
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef allocator<_List_node<_Tp> > _Node_allocator_type;
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Node_allocator_type> _M_node;   // +0x00
    _List_base(const _Alloc&) : _M_node(_Node_allocator_type(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
    list(const list& __x) : _List_base<_Tp, _Alloc>(_Alloc())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    ~list() { }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    template <class _InputIter>
    __declspec(noinline) void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last,
                                                 const __false_type&)
    {                                               // 0x0042B4C3 (same-TU stand-in: never reads the tag)
        InsertListRange(this, __pos._M_node, __first._M_node, __last._M_node);
    }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                                                // +0x00
    _Base_ptr _M_parent;                                                               // +0x04
    _Base_ptr _M_left;                                                                 // +0x08
    _Base_ptr _M_right;                                                                // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                                             // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr _M_node);
};
template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
template <class _Value> struct _Rb_tree_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                                                     // +0x00
    unsigned int        _M_node_count;                                                 // +0x04
    unsigned char       _M_key_compare;                                                // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    __declspec(noinline) _Tp& operator[](const _Key& __k)
    {                                               // 0x0042D5B1 (same-TU stand-in: reads the key)
        return *(_Tp*)LookupState(this, __k);
    }
};

}

class C2DVector {
public:
    float X;
    float Y;
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
};

struct CCPPointerInfo {
    long RefCount;                                                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);                                         // +0x04
    void* Data;                                                                        // +0x08
    CCPPointerInfo(long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                                              // +0x00
    CCPPointerInfo* Info;                                                              // +0x04
    static void __fastcall DeleteData(void* data);                                     // 0x00428B10
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Info = new CCPPointerInfo(1, &DeleteData, Data);
        else
            Info = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        CCPPointerInfo* info = o.Info;
        T* data = o.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};
namespace std {

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    reference operator[](size_type __n) { return *(begin() + __n); }
};

}

namespace NUISystem {

struct CUIState {                           // map value (key: the state index)
    unsigned long StateChangeFlag;          // +0x00
    long          StateChangeType;          // +0x04
    C2DVector     Position;                 // +0x08
    C2DVector     Zoom;                     // +0x10
    CRGBColour    Colour;                   // +0x18
    float         UpdateTime;               // +0x1c
    bool          LinearChange;             // +0x20
};
typedef std::map<unsigned long, CUIState> CStateMap;

class CComponent;
enum EDeletionMethod { DELETION_METHOD_NONE, DELETION_METHOD_1 };
class CDeletion {
public:
    EDeletionMethod              Method;                                               // +0x00
    std::list<CComponent*>       AssociatedParents;                                    // +0x04
};

class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void SetDeletion(CDeletion deletion);                                      // slot 62 (+0xf8)
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual CStateMap& GetStates();                                                    // slot 82 (+0x148)
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual void VSlot102();
    virtual void VSlot103();
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void VSlot114();
    virtual void VSlot115();
    virtual void VSlot116();
    virtual void VSlot117();
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void VSlot125();
    virtual void VSlot126();
    virtual void VSlot127();
    virtual void VSlot128();
    virtual void VSlot129();
    virtual void VSlot130();
    virtual void VSlot131();
    virtual void VSlot132();
    virtual void VSlot133();
    virtual void VSlot134();
    virtual void VSlot135();
    virtual void VSlot136();
    virtual void VSlot137();
    virtual void VSlot138();
    virtual void VSlot139();
    virtual void VSlot140();
    virtual void VSlot141();
    unsigned char _pad04[0xb0 - 4];
    std::vector<CCountedPointer<CComponent> > Children;                                // +0xb0
    unsigned char _padbc[0x15c - 0xbc];
};
class CSpellContainer : public CComponent {
public:
    virtual void AddChildDynamically(CCountedPointer<CComponent>& child, unsigned long index);   // slot 142 (+0x238)
    virtual void VSlot143();
    virtual void UnAssignSpell(unsigned long slot);                                    // slot 144 (+0x240)
    std::vector<CCountedPointer<CComponent> > BaseSpells;                              // +0x15c
    std::pair<CComponent*, unsigned long> NewSpell;                                    // +0x168
    long                                EmptySlot;                                     // +0x170
};
class CFrontEndManager {
public:
    static CFrontEndManager* __fastcall GetInstance();                                 // 0x0041E5F2
    CComponent* CreateComponent(long definition);                                      // 0x0041D21B
};

void CSpellContainer::UnAssignSpell(unsigned long slot)
{
    if (NewSpell.first != 0 && NewSpell.second == slot) {
        CDeletion deletion;
        deletion.Method = DELETION_METHOD_1;
        NewSpell.first->SetDeletion(deletion);
        AddChildDynamically(BaseSpells[NewSpell.second], NewSpell.second);
        NewSpell.first = 0;
    }
    else {
        CDeletion deletion;
        deletion.Method = DELETION_METHOD_1;
        Children[slot]->SetDeletion(deletion);
        CCountedPointer<CComponent> emptySlot(CFrontEndManager::GetInstance()->CreateComponent(EmptySlot));
        CStateMap& states = emptySlot->GetStates();
        for (CStateMap::iterator it = states.begin(); it != states.end(); ++it) {
            states[it->first].Position = Children[slot]->GetStates()[it->first].Position;
            states[it->first].Colour = Children[slot]->GetStates()[it->first].Colour;
        }
        AddChildDynamically(emptySlot, slot);
        BaseSpells[slot] = emptySlot;
    }
}

}