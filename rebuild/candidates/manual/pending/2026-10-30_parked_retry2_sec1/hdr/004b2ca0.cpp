// CQuestManager::GetActiveQuestSectionNames(set<CCharString>& names) const -- the load sections of the active quests: each quest's section name, then (while the day / night is active) every name with "%DayOnly" / "%NightOnly" appended plus "NULL%DayOnly" / "NULL%NightOnly"
// Grade: DIFFER(856v832) -- the lane builds an `and esp,-8` frame (ebp frame pointer) for the 8-byte insert_unique result returned through a hidden pointer inside the loops (RTM and QFE alike); retail keeps a plain 0x24 frame with this in ebx and the vector finishes in ebp/edi; 4 grades
// Leaf: ?GetActiveQuestSectionNames@CQuestManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B2CA0, 832 bytes, `ret 4` (Ego_r.pdb
//   ?GetActiveQuestSectionNames@CQuestManager@@QBEXAAV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@@Z).
//   Callers: CQuestManager::UninitializeDaySection 0x004B3260 / UninitializeNightSection 0x004B35E0 (landed; they call it
//   "BuildLoadSections"), 0x004B3438, 0x004B362D, CWorldMap::GetLoadableSections 0x004FF490.
// Callees: _Rb_tree<CCharString>::insert_unique 0x004AC380, vector<CCharString>::reserve 0x004BA7A0 and
//   _M_insert_overflow 0x00433530 (same-TU noinline stand-in: the empty tag shares the live `names` slot),
//   operator+(const CCharString&, const char*) 0x0099F600 / (const CCharString&, const CCharString&) 0x0099F570 /
//   (const char*, const CCharString&) 0x0099F690 (`__fastcall`), CCharString copy 0x0099EC30 / ~ 0x0099EAE0, free 0x00BFEA14.
// Globals: g_DayOnlySuffix 0x013BAC18 "DayOnly", g_NightOnlySuffix 0x013BAC1C "NightOnly"; "%" 0x01239434, "NULL%" 0x01239438.
// Layout: CQuestManager ActiveQuests (list<CCountedPointer<CActiveQuest>>) +0x38, DayActive +0x8C, NightActive +0x8D
//   (landed UninitializeDaySection model); CActiveQuest SectionName +0x30.
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
extern CCharString g_DayOnlySuffix;                     // 0x013BAC18 "DayOnly"
extern CCharString g_NightOnlySuffix;                   // 0x013BAC1C "NightOnly"

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct _Rb_tree_node_base;
struct _Rb_insert_result {                              // pair<iterator, bool> as a plain aggregate (landed 0x004AD3D0 model)
    _Rb_tree_node_base* first;
    bool                second;
};
template <class _Tp> struct less { };
template <class _Tp> struct _Identity { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                               // +0x00
    _Base_ptr _M_parent;                              // +0x04
    _Base_ptr _M_left;                                // +0x08
    _Base_ptr _M_right;                               // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                            // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    } else {
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
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const allocator<_Node>& __a) : _M_header(__a, (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;

    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator<_Node>()), _M_node_count(0) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }

    void _M_erase(_Link_type __x);
    _Rb_insert_result insert_unique(const _Value& __v);   // 0x00514B50 (char) / 0x00514C10 (unsigned long)
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase((_Link_type)__x->_M_right);
        _Link_type __y = (_Link_type)__x->_M_left;
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    unsigned int size() const { return _M_t._M_node_count; }
    _Rb_insert_result insert(const _Key& __x) { return _M_t.insert_unique(__x); }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void reserve(unsigned int __n);                     // 0x004BA7A0
#pragma auto_inline(off)
    // 0x00433530 (same-TU stand-in: only its call matters; it never touches the tag, so VC homes the tag in a live slot)
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        if (__atend && __fill_len == 1 && __position == this->_M_finish)
            _Construct(this->_M_start, __x);
    }
#pragma auto_inline(on)
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> const_iterator;
    _List_node_base* _M_node;
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
}

class CActiveQuest {
public:
    unsigned char _pad00[0x30];
    CCharString   SectionName;                          // +0x30
};

class CQuestManager {
public:
    unsigned char _pad00[0x38];
    std::list<CCountedPointer<CActiveQuest> > ActiveQuests;   // +0x38
    unsigned char _pad3c[0x8c - 0x3c];
    bool          DayActive;                            // +0x8C
    bool          NightActive;                          // +0x8D
    void GetActiveQuestSectionNames(std::set<CCharString>& names) const;
};

void CQuestManager::GetActiveQuestSectionNames(std::set<CCharString>& names) const
{
    for (std::list<CCountedPointer<CActiveQuest> >::const_iterator quest = ActiveQuests.begin(); quest != ActiveQuests.end(); ++quest)
        names.insert((*quest)->SectionName);
    std::vector<CCharString> dayNames;
    std::vector<CCharString> nightNames;
    if (DayActive) {
        dayNames.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            dayNames.push_back(*it + "%" + g_DayOnlySuffix);
        dayNames.push_back("NULL%" + g_DayOnlySuffix);
    }
    if (NightActive) {
        nightNames.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            nightNames.push_back(*it + "%" + g_NightOnlySuffix);
        nightNames.push_back("NULL%" + g_NightOnlySuffix);
    }
    for (std::vector<CCharString>::iterator day = dayNames.begin(); day != dayNames.end(); ++day)
        names.insert(*day);
    for (std::vector<CCharString>::iterator night = nightNames.begin(); night != nightNames.end(); ++night)
        names.insert(*night);
}
