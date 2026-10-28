// CTCInventoryQuests::~CTCInventoryQuests() -- deletes every UI component and clears the list, then destroys the members (quest-card primitives and items, the available-quest map, the displayed quests) and both bases
// Leaf: ??1CTCInventoryQuests@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061A500, 344 bytes, thiscall, no frame; ends `jmp 0x005C00A0`. Speed TU (`add esp, 4`).
// Identity (proven): the two vtable stores are CTCInventoryQuests's RTTI vtables (0x012585B4 primary, 0x012585A0 for the
//   NHeroInformationScreens::CBase subobject at +0x130 -- the stores of the landed-label ctor 0x0061A420), the base store
//   0x0123A3E0 is CBase's own vtable, the tail jump is NInventory::CTCInventoryBase::~CTCInventoryBase 0x005C00A0, and slot 0
//   of vtable 0x012585B4 is the scalar deleting destructor 0x00625090 whose call is this body. The members follow Ego_r.pdb
//   CTCInventoryQuests in reverse at the retail offsets (base 0x130 + CBase vptr, 12-byte STLport containers; operator
//   new(0x188) in CTCInventoryQuests::Alloc 0x004D30A8 is the retail sizeof).
// Callees: memmove 0x00BFEAE6 (Components.clear(), STLport __copy_trivial), _Rb_tree<CThing*, ...>::_M_erase 0x00623560,
//   ~CWideString 0x0099B510 (CQuestCardItem::ItemText), free 0x00BFEA14, ~CTCInventoryBase 0x005C00A0; the element
//   destructors of CEnginePrimitiveHandle / CIntelligentPointer<CThing> are virtual calls through slot 0.
// Label correction: catalogued as "CEngine::OnPreDeviceReset".
// No engine/ header exists for CTCInventoryQuests (header debt); the local view carries the PDB names at retail offsets.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
class CThing;
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                           // 0x0099B510
};
class CEnginePrimitiveHandle {
public:
    virtual ~CEnginePrimitiveHandle();
    void* PPrimitive;
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CQuestCardItem { public:
    long        ItemIndex;                                    // +0
    CWideString ItemText;                                     // +4
};
enum EScreenType { };
namespace NUISystem { class CComponent { public: virtual ~CComponent(); }; }
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = (_Tp*)__copy_trivial(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, allocator<_Rb_tree_node<_Tp> > > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    ~_Rb_tree() { clear(); }
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                            // 0x00623560
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}
namespace NInventory {
class CTCInventoryBase {                                      // vtable 0x01253DBC, retail sizeof 0x130
public:
    virtual ~CTCInventoryBase();                              // 0x005C00A0
    unsigned char _pad_0x04[0x8];
    std::vector<NUISystem::CComponent*> Components;          // +0x0C
    unsigned char _pad_0x18[0x118];
};
}
namespace NHeroInformationScreens {
class CBase {                                                 // vtable 0x0123A3E0
public:
    virtual ~CBase() { }
};
}
class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    virtual ~CTCInventoryQuests();
    std::vector<CEnginePrimitiveHandle> InventoryQuestCardSelectPrimitiveHandles;   // +0x134
    std::vector<CQuestCardItem> QuestCardItemList;                                  // +0x140
    bool IsGuildValid;                                                              // +0x14C
    bool InventoryScreen;                                                           // +0x14D
    long QuestCardCount;                                                            // +0x150
    bool Teleport;                                                                  // +0x154
    bool QuestCardActivatable;                                                      // +0x155
    bool EnableQuestCardListInitialise;                                             // +0x156
    bool ActiveTestQuestMode;                                                       // +0x157
    long TestQuestSelectedIndex;                                                    // +0x158
    long TestQuestTopIndex;                                                         // +0x15C
    bool TestQuestShowAll;                                                          // +0x160
    long TestQuestListSize;                                                         // +0x164
    EScreenType PreviousScreenType;                                                 // +0x168
    std::map<CThing*, NUISystem::CComponent*> AvailableQuests;                      // +0x16C
    std::vector<CIntelligentPointer<CThing> > DisplayQuests;                        // +0x178
    float AccumFadeTime;                                                            // +0x184
};

CTCInventoryQuests::~CTCInventoryQuests()
{
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
}