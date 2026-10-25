// CThingManager::SaveEntitiesToScript(CCharString& script, long version, const vector& things, vector& names) -- writes the things as script text: the set of their serialisation section names (copied into names), then per section an "XXXSectionStart <name>;" block of "NewThing <type>; ... EndThing;" records (each thing's virtual Save) closed by "XXXSectionEnd;"
// Leaf: ?SaveEntitiesToScript@CThingManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00521EB0, 682 bytes (0x2AA + int3 padding to 688), thiscall `ret 10h`, /O2 esp
//   frame (sub esp,18h). Identity: Ego_r.pdb ?SaveEntitiesToScript@CThingManager@@QAEXAAVCCharString@@JABV?$vector@
//   PAVCThing@@...@std@@AAV?$vector@VCCharString@@...@Z (the catalog's "Save_WriteEntityHierarchy" is descriptive).
//   Callers: CThingManager::RuntimeSaveGameHero 0x00523B40 and the level/global savers.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the set header node, 0x14 bytes), set<CCharString> insert_unique
//   0x004AC380 / _Rb_tree::_M_erase 0x004ABA70, vector<CCharString>::erase(first, last) 0x0043336A (clear),
//   vector<CCharString>::_M_range_insert(pos, first, last, const bidirectional_iterator_tag&) 0x0052B6A0,
//   CCharString() 0x0099E4B0 / (const CCharString&) 0x0099EC30 / ~ 0x0099EAE0 / Reserve 0x0099EF90 / SetUseFastExtend
//   0x0099EF70 / operator+=(const CCharString&) 0x0099F0A0 / operator+=(const char*) 0x0099F100, operator+(const char*,
//   const CCharString&) 0x0099F690 and operator+(const CCharString&, const char*) 0x0099F600 (both __fastcall, hidden
//   result in ecx), Compare 0x00411570 (__fastcall, the inline CCharString equality), CThing::GetTypeName 0x004C7E10,
//   CThing::Save(CCharString&) (virtual slot 2, +0x08). Strings: "\r\nXXXSectionStart " 0x01245B90,
//   ";\r\n" 0x012353B8, "\r\n" 0x012397F8, "NewThing " 0x01245B84, "EndThing;\r\n" 0x01245B78,
//   "\r\nXXXSectionEnd;\r\n\r\n" 0x01245B60.
// Layout: retail CThingManager::SerialiseVersionNumber +0xd8 (my landed constructor 0x00523540); CThing::
//   SerialisationSectionName +0x58; CCharString data block {chars, length}.
// Shape: the twin of SaveEntitiesBinary 0x00522160 (same set/clear/range-insert prologue, version stored before the set
//   and after the range insert); each section is a by-value copy (in the dead `names` slot), the section header is one
//   `script += "..." + section + ";\r\n"` expression (the right literal pushed first), and each matching thing's text is
//   built in a reserved fast-extend local (in the dead `version` slot) and appended. Compare and the range insert have
//   same-TU stand-ins (reads only / never reads the tag), as in 0x00522160.
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

struct CCharStringData {
    const char*   m_pData;                                                             // +0x00
    unsigned long m_length;                                                            // +0x04
};
__declspec(noinline) long __fastcall Compare(const char* a, const char* b)
{                                                   // 0x00411570 (same-TU stand-in: reads only)
    for (;;) {
        char c = *a;
        if (c == 0 && *b == 0)
            return 0;
        if (c < *b)
            return -1;
        if (c > *b)
            return 1;
        ++a;
        ++b;
    }
}
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                                                     // 0x0099E4B0
    CCharString(const CCharString& other);                                             // 0x0099EC30
    ~CCharString();                                                                    // 0x0099EAE0
    void Reserve(long size);                                                           // 0x0099EF90
    void SetUseFastExtend(bool fast);                                                  // 0x0099EF70
    const CCharString& operator+=(const CCharString& other);                           // 0x0099F0A0
    const CCharString& operator+=(const char* text);                                   // 0x0099F100
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

CCharString __fastcall operator+(const char* left, const CCharString& right);         // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);         // 0x0099F600

void RangeInsertNames(void* names, void* position, void* first, void* last);           // stand-in helper

namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };

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

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
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
    _Rb_tree_iterator() : _M_node(0) { }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef allocator<_Rb_tree_node<_Value> > _Node_alloc;
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, _Node_alloc> _M_header;   // +0x00
    _Rb_tree_base(const _Alloc&) : _M_header(_Node_alloc(), (_Rb_tree_node<_Value>*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;                                                        // +0x04
    _Compare     _M_key_compare;                                                       // +0x08
    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    iterator begin() const { return iterator(_M_leftmost()); }
    iterator end() const { return iterator(this->_M_header._M_data); }
    unsigned int size() const { return _M_node_count; }
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
    pair<iterator, bool> insert_unique(const _Value& __v);                             // 0x004AC380
    void _M_erase(_Link_type __x);                                                     // 0x004ABA70
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(_Compare(), _Alloc()) { }
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
    unsigned int size() const { return _M_t.size(); }
    pair<iterator, bool> insert(const _Key& __x) { return _M_t.insert_unique(__x); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    iterator erase(iterator __first, iterator __last);                                 // 0x0043336A
    void clear() { erase(begin(), end()); }
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_range_insert(__pos, __first, __last, bidirectional_iterator_tag());
    }
    template <class _ForwardIter>
    __declspec(noinline) void _M_range_insert(iterator __pos, _ForwardIter __first, _ForwardIter __last,
                                              const bidirectional_iterator_tag&)
    {                                               // 0x0052B6A0 (same-TU stand-in: never reads the tag)
        RangeInsertNames(this, __pos, __first._M_node, __last._M_node);
    }
};

}

class CThing {
public:
    unsigned char _pad_0x04[0x58 - 4];
    CCharString   SerialisationSectionName;                                            // +0x58
    const CCharString& GetTypeName() const;                                            // 0x004C7E10
    virtual void v00();
    virtual void v01();
    virtual void Save(CCharString& script);                                            // slot 2 (+0x08)
};

class CThingManager {
public:
    unsigned char _pad_0x00[0xd8];
    long          SerialiseVersionNumber;                                              // +0xd8
    void SaveEntitiesToScript(CCharString& script, long version, const std::vector<CThing*>& things,
                              std::vector<CCharString>& names);
};

void CThingManager::SaveEntitiesToScript(CCharString& script, long version, const std::vector<CThing*>& things,
                                         std::vector<CCharString>& names)
{
    if (things.empty())
        return;
    SerialiseVersionNumber = version;
    std::set<CCharString> sections;
    for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it)
        sections.insert((*it)->SerialisationSectionName);
    names.clear();
    names.insert(names.end(), sections.begin(), sections.end());
    SerialiseVersionNumber = version;
    for (std::set<CCharString>::iterator s = sections.begin(); s != sections.end(); ++s) {
        CCharString section(*s);
        script += "\r\nXXXSectionStart " + section + ";\r\n";
        for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            if (thing->SerialisationSectionName == section) {
                CCharString thingScript;
                thingScript.Reserve(0x4000);
                thingScript.SetUseFastExtend(true);
                thingScript += "\r\n";
                thingScript += "NewThing ";
                thingScript += thing->GetTypeName();
                thingScript += ";\r\n";
                thing->Save(thingScript);
                thingScript += "EndThing;\r\n";
                script += thingScript;
            }
        }
        script += "\r\nXXXSectionEnd;\r\n\r\n";
    }
}