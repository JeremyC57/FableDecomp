// CTCQuickAccessMenu::GetItemsListUI(long& numItems, NUISystem::CComponent* parent) -- build the quick-access items list: sort the hero's inventory (expressions skipped) into trophies / potions / gifts / produce / others by UI category, count the items, and when any category is non-empty create the list component and add one category entry per tab (gifts checked for a usable target)
// Leaf: ?GetItemsListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@AAJPAV23@@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006276F0, length 1260 bytes, `ret 8`.
// Callees: CManager::GetInstance @ 0x0041E5F2, CManager::CreateComponent @ 0x0041DB1D, CThing::GetTC<CTCInventory>
//   (inline, interface 0x11, CVectorMap::LowerBound @ 0x0040F020 inlined), NInventory::CTCInventoryBase::GetAllItems @ 0x005BF482,
//   CTCInventoryItem::IsExpression @ 0x005D9830, CTCInventoryItem::GetUIInventoryCategory @ 0x005D96F0, std::vector::push_back
//   (STLport 4.6, _M_insert_overflow out of class), NUISystem::CList::InitialiseOffsets (vtable +0x270),
//   NGameText::CDataBank::GetTextBySymbol @ 0x009C95E0 via CMainGameComponent::Get() @ 0x013B86A0 (PTextBank +0x14),
//   CWideString ctor/dtor/operator= @ 0x0099AED0 / 0x0099B510 / 0x0099B7D0, ConstructItemCategoryList @ 0x00626E40 (same TU).
// Names (PDB): every function, member and class here is spelled from FableWin.pdb / Ego_r.pdb; mangled leaf identical to FableWin.
// Layout: Ego_r.pdb CTCQuickAccessMenu : CTCBase (ParentThing via engine/CTCBase.h); retail CThing interface bitset +0x20 / map +0x44.
// Shape: authored in one file with ConstructItemCategoryList (retail neighbours in one TU); the STLport list / _Rb_tree / vector
//   bodies are the real 4.6 text so the out-of-line COMDATs (insert_unique @ 0x004E539F, _M_erase, ~list @ 0x0042ABCA) bind as retail.
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
typedef unsigned short wchar_t;                         // VC7.1 without /Zc:wchar_t (lane convention for wide text)

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    void __cdecl Sprintf(const wchar_t* format, ...);   // 0x0099BA70
};
// FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z: a header inline (out-of-line COMDAT 0x00405F00), expanded here.
inline CWideString __fastcall GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}
class CTCInventoryStats { public:
    static CWideString __fastcall GetXString();         // 0x005CC772 ?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ
};

extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
inline void __cdecl operator delete(void*, void*) { }
namespace std {                                         // STLport 4.6 (the retail library), the parts this TU uses
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // "VC++ seems to need this explicit copy constructor"
};
template <class _T1, class _T2> inline pair<_T1, _T2> __cdecl make_pair(const _T1& __x, const _T2& __y) { return pair<_T1, _T2>(__x, __y); }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

// ---- vector
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    typedef int difference_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    reference operator[](size_type __n) { return *(begin() + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);      // <pair<long,long> > 0x0059400C
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}

// ---- list
struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_node = this->_M_node->_M_next; return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef allocator<_List_node<_Tp> > _Node_allocator_type;
    typedef _Alloc allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_List_node<_Tp>*)0)
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
    void clear();
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Node_allocator_type> _M_node;   // +0x00
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()                   // STLport _list.c (out of class)
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef _List_node<_Tp> _Node;
    typedef typename _Base::allocator_type allocator_type;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    allocator_type get_allocator() const { return allocator_type(); }
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    ~list() { }
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
    iterator insert(iterator __position, const _Tp& __x)   // <CComponent*> 0x0042AB8A
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
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {                                                   // <const_iterator> 0x0042B4C3 (landed body)
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};

// ---- _Rb_tree / map
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type&) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef bool _Color_type;
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef value_type& reference;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef typename _Base::allocator_type allocator_type;
    enum { _S_rb_tree_red = false, _S_rb_tree_black = true };
protected:
    size_type _M_node_count;                            // +0x04
    _Compare _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type& __cdecl _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& __cdecl _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
    static reference __cdecl _S_value(_Link_type __x) { return __x->_M_value_field; }
    static const _Key& __cdecl _S_key(_Link_type __x) { return _KeyOfValue()(_S_value(__x)); }
    static _Color_type& __cdecl _S_color(_Link_type __x) { return (_Color_type&)(__x->_M_color); }
public:
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
private:
    void _M_erase(_Link_type __x);
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    _Rb_tree(const _Compare& __comp, const allocator_type& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
private:
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = _S_rb_tree_red;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    iterator end() { return iterator(this->_M_header._M_data); }
    size_type size() const { return _M_node_count; }
    pair<iterator, bool> insert_unique(const value_type& __x);   // <long, pair<const long, long> > 0x004E539F
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
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
private:
    _Rb_tree_node<_Value>* _M_find(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return __y;
    }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Rb_tree_node<_Value>* __x)
{                                                       // <long, pair<const long, long> > 0x004DA2AC
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef _Tp data_type;
    typedef pair<const _Key, _Tp> value_type;
private:
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::size_type size_type;
    typedef typename _Rep_type::allocator_type allocator_type;
    map() : _M_t(_Compare(), allocator_type()) { }
    iterator end() { return _M_t.end(); }
    size_type size() const { return _M_t.size(); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
    iterator find(const key_type& __x) { return _M_t.find(__x); }
};
// ---- bitset (STLport 4.6 _Base_bitset / bitset, the const test path)
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int __cdecl _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int __cdecl _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static _WordT __cdecl _S_maskbit(unsigned int __pos) { return (_WordT)1 << _S_whichbit(__pos); }
    _WordT _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
    typedef _Base_bitset<(_Nb + 31) / 32> _Base;
public:
    bool _Unchecked_test(unsigned int __pos) const { return (this->_M_getword(__pos) & _Base::_S_maskbit(__pos)) != (unsigned long)0; }
    bool operator[](unsigned int __pos) const { return _Unchecked_test(__pos); }
};
}   // namespace std
typedef std::vector<std::pair<long, long> > CItemVector;
typedef std::map<long, long> CItemMap;

namespace NUISystem {
class CComponent;
enum EDeletionMethod { DELETION_METHOD_NONE, DELETION_METHOD_1 };   // enumerator names descriptive (values only)
class CDeletion { public:                                      // Ego_r.pdb NUISystem::CDeletion (retail 8 bytes)
    EDeletionMethod             Method;                 // +0x00
    std::list<CComponent*>      AssociatedParents;      // +0x04
};
class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                             // slot 3 (+0x0C) FableWin ?FindChild@CComponent@NUISystem@@UAEPAV12@VCCharString@@@Z
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058();
    virtual void AddChild(CCountedPointer<CComponent>& child);                    // slot 59 (+0xEC) Ego_r ?AddChild@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@...@@@Z
    virtual void v060(); virtual void v061();
    virtual void SetDeletion(CDeletion deletion);                                  // slot 62 (+0xF8) Ego_r ?SetDeletion@CComponent@NUISystem@@UAEXVCDeletion@2@@Z
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void Clear();                                                          // slot 92 (+0x170) (name descriptive, as landed)
};
class CList : public CComponent { public:
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146();
    virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>& child);   // slot 152 (+0x260) manifest 0x0053AD07, Ego_r name set
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void InitialiseOffsets(unsigned long count);                             // slot 156 (+0x270) manifest 0x00536B4F, Ego_r name set
    virtual void v157();
    virtual void SetSelectedChild(long index);                                       // slot 158 (+0x278) (name descriptive)
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
    static CManager* __fastcall GetInstance();          // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}   // namespace NUISystem

enum ETCInterfaceType { };                              // enumerator names not in the name sets (values as casts)
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    std::bitset<274> UsingComponents;                   // +0x20
    CInterfaceMap  Components;                          // +0x44 (first/last of the 0x10-byte map)
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> bool GetTC(T** out)             // FableWin ??$GetTC@VCTCInventory..@CThing@@QAE_NPAPAV..@Z form
    {
        long type = T::StatGetInterfaceType();
        if (HasTC((ETCInterfaceType)type)) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    template <class T> bool PeekTC(T** out) const      // FableWin ??$PeekTC@$$CBVCTCTargetingBase@@@CThing@@QBE_NPAPBVCTCTargetingBase@@@Z
    {
        long type = T::StatGetInterfaceType();
        bool present = HasTC((ETCInterfaceType)type);
        if (present) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
        }
        return present;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)   // header template (out-of-line copies 0x0040DCE0 ...)
{
    return thing != 0 && !thing->Dead;
}
class CTCTargetingBase { public:                               // interface 8
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07();
    virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15();
    virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23();
    virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30(); virtual void t31();
    virtual void t32();
    virtual CThing* GetPTargetedThing() const;          // slot 33 (+0x84) Ego_r ?GetPTargetedThing@CTCTargetingBase@@UBEPAVCThing@@XZ
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)8; }
};

namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char                          _pad00[0x14];
    CCountedPointer<NGameText::CDataBank>  PTextBank;   // +0x14
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }   // FableWin ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ
};

enum EUIInventoryCategory { };                          // enumerator names not in the name sets (0..4; 4 = the default)
class CTCInventoryItem { public:
    static bool __fastcall IsExpression(long defIndex);                           // 0x005D9830
    static EUIInventoryCategory __fastcall GetUIInventoryCategory(long defIndex); // 0x005D96F0
};
namespace NInventory {
class CTCInventoryBase { public:
    void GetAllItems(CItemVector& items) const;         // 0x005BF482
};
}
class CTCInventory : public NInventory::CTCInventoryBase { public:   // interface 0x11 (GetInterfaceType 0x00594BC9)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x11; }
};

class CTCQuickAccessMenu : public CTCBase {
public:
private:
    void ContructItemLists(CItemMap& items, NUISystem::CList* list, NUISystem::CComponent* parent,
                           CCountedPointer<NUISystem::CComponent>& actions);   // 0x00626110
    void ConstructItemCategoryList(CWideString& title, NUISystem::CList* list, CItemVector& items,
                                   NUISystem::CComponent* parent, bool checkUsable);
    NUISystem::CComponent* GetItemsListUI(long& numItems, NUISystem::CComponent* parent);
};
using namespace NUISystem;

void CTCQuickAccessMenu::ConstructItemCategoryList(CWideString& title, CList* list, CItemVector& items, CComponent* parent,
                                                   bool checkUsable)
{
    CManager* fe = CManager::GetInstance();
    CWideString caption(title);
    long total = 0;
    for (unsigned long i = 0; i < items.size(); ++i)
        total += items[i].second;
    if (!items.empty()) {
        caption += CTCInventoryStats::GetXString();
        caption += GFIntToWideString(total);
    }
    CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(caption, 0.0f, 1.0f,
                                                                   "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW_FOR_QUICK_ACCESS",
                                                                   0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    list->AddScrollingChildAndSetOffsets(entry);
    if (items.empty()) {
        CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = DELETION_METHOD_1;
            arrow->SetDeletion(deletion);
        }
        entry->Clear();
    } else {
        CItemMap merged;
        for (long i = 0; i < (long)items.size(); ++i) {
            long key = items[i].first;
            CItemMap::iterator found = merged.find(key);
            if (found == merged.end())
                merged.insert(std::make_pair(key, items[i].second));
            else
                found->second += items[i].second;
        }
        CCountedPointer<CComponent> subList(fe->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false));
        CComponent* sub = subList.Data;
        entry->AddChild(subList);
        static_cast<CList*>(sub)->InitialiseOffsets(merged.size());
        bool usable = true;
        const CTCTargetingBase* targeting;
        if (checkUsable && ParentThing->PeekTC(&targeting) && !IS_THING_ALIVE(targeting->GetPTargetedThing()))
            usable = false;
        CCharString actionsName(usable ? "UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS" : "UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS_CANT_USE");
        CCountedPointer<CComponent> actions(CManager::GetInstance()->CreateComponent(actionsName, false));
        static_cast<CList*>(actions.Data)->SetSelectedChild(0);
        CComponent* actionsList = actions.Data;
        if (!usable) {
            CComponent* notUsable = actionsList->FindChild("UI_MENU_ENTRY_QUICK_ACCESS_ITEM_ACTION_NOT_USE");
            CComponent* arrow = notUsable->FindChild("UI_SELECTION_ARROW_RIGHT_FOR_QUICK_ACCESS");
            if (arrow) {
                CDeletion deletion;
                deletion.Method = DELETION_METHOD_1;
                arrow->SetDeletion(deletion);
            }
            notUsable->Clear();
        }
        ContructItemLists(merged, static_cast<CList*>(sub), parent, actions);
    }
}

CComponent* CTCQuickAccessMenu::GetItemsListUI(long& numItems, CComponent* parent)
{
    CCountedPointer<CComponent> actions(CManager::GetInstance()->CreateComponent("UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS", false));
    CTCInventory* inventory;
    ParentThing->GetTC(&inventory);
    CItemVector items;
    CItemVector trophies;
    CItemVector potions;
    CItemVector gifts;
    CItemVector produce;
    CItemVector others;
    inventory->GetAllItems(items);
    numItems = 0;
    for (unsigned long i = 0; i < items.size(); ++i) {
        if (CTCInventoryItem::IsExpression(items[i].first))
            continue;
        switch (CTCInventoryItem::GetUIInventoryCategory(items[i].first)) {
        case 0: trophies.push_back(items[i]); break;
        case 1: potions.push_back(items[i]); break;
        case 2: gifts.push_back(items[i]); break;
        case 3: produce.push_back(items[i]); break;
        case 4: others.push_back(items[i]); break;
        default: others.push_back(items[i]); break;
        }
        numItems += items[i].second;
    }
    CManager* manager = CManager::GetInstance();
    CList* list = 0;
    if (!trophies.empty() || !potions.empty() || !gifts.empty() || !produce.empty() || !others.empty()) {
        list = static_cast<CList*>(manager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false));
        list->InitialiseOffsets(5);
        CWideString name;
        name = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_TROPHIES");
        ConstructItemCategoryList(name, list, trophies, parent, false);
        name = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_POTIONS");
        ConstructItemCategoryList(name, list, potions, parent, false);
        name = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_GIFTS");
        ConstructItemCategoryList(name, list, gifts, parent, true);
        name = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_PRODUCE");
        ConstructItemCategoryList(name, list, produce, parent, false);
        name = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_OTHERS");
        ConstructItemCategoryList(name, list, others, parent, false);
    }
    return list;
}