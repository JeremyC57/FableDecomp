// CProfileManager::~CProfileManager() -- tear down the profile manager: walk the ProfileTree, clear the tree and both name maps, then the member destructors free the ProfileTree, ProfileNameToID, IDToProfileName and ProfileList nodes and sentinels
// Leaf: ??1CProfileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9020, 308 bytes, thiscall. Callers: 0x009A6BB3, 0x009A73B7 (CSystemManager
//   teardown). The chunk label "CNavigableTextUIManager_Destructor" is wrong: the layout is CProfileManager's (ctor 0x009F9160).
// Layout: see 0x009F9160 (ProfileList +0x4C, IDToProfileName +0x50, ProfileNameToID +0x5C, ProfileTree +0x68).
// Callees: list<CProfileTreeEntry> _List_base::clear @ 0x009F9B30 (out of line), _Rb_tree::_M_erase for
//   map<long,CCharString> @ 0x00451B4B and map<CCharString,long> @ 0x0043020A, 0x00BFEA14 free.
// Shape: the body is an empty walk over ProfileTree (VC7.1 keeps it: the loop is not provably finite), ProfileTree.clear(),
//   IDToProfileName.clear(), ProfileNameToID.clear(); the member destructors then run in reverse order, with
//   list<CProfile*>'s clear inline. Node frees have no null test, sentinel frees do (STLport ~_List_base / ~_Rb_tree_base).
extern "C" void __cdecl free(void* block);                          // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _List_iterator<_Tp>& __x) : _List_iterator_base(__x._M_node) {}
    _List_iterator<_Tp>& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    ~_List_base()
    {
        clear();
        if (_M_node._M_data != 0)
            _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
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

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    void clear() { _List_base<_Tp, _Alloc>::clear(); }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Tp, class _Alloc>
class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base()
    {
        if (_M_header._M_data != 0)
            _M_header.deallocate(_M_header._M_data, 1);
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    ~_Rb_tree() { clear(); }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);
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
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    void clear() { _M_t.clear(); }
    _Rb_tree<_Key, value_type, _Compare, _Alloc> _M_t;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

}

struct CCharStringData;
class CCharString {
public:
    CCharStringData* m_pStringData;
    bool operator<(const CCharString& other) const;
};

struct CWideStringData;
class CWideString {
public:
    CWideStringData* m_pStringData;
    ~CWideString();                                 // 0x0099B510
};

class CRenderManager;
class CProfileManager;

class CProfile {                                    // 0x48 bytes
public:
    CProfileManager* Manager;                       // +0x00
    double StartTime;                               // +0x08
    double TotalTime;                               // +0x10
    double LastTime;                                // +0x18
    bool Running;                                   // +0x20
    double Percentage;                              // +0x28
    CWideString Name;                               // +0x30
    long TreeDepth;                                 // +0x34
    bool NewNode;                                   // +0x38
    long Visible;                                   // +0x3C
    long CallCounter;                               // +0x40
    long UpdatedCallCounter;                        // +0x44
};

enum EDrawMode { DRAW_MODE_0 = 0 };
enum EEvalMode { EVAL_MODE_0 = 0, EVAL_MODE_1 = 1 };

class CProfileManager {
public:
    struct CProfileTreeEntry {                      // 0x60 bytes
        CProfile Profile;                           // +0x00
        long ID;                                    // +0x48
        std::vector<CProfileTreeEntry*> Children;  // +0x4C
        CProfileTreeEntry* Parent;                  // +0x58
    };

    ~CProfileManager();

    bool UpPressed;                                 // +0x00
    bool DownPressed;                               // +0x01
    bool TogglePressed;                             // +0x02
    bool FlipAllPressed;                            // +0x03
    std::_List_iterator<CProfile*> CursorPosition;  // +0x04
    bool AutoDisplayHierarchy;                      // +0x08
    bool Valid;                                     // +0x09
    double ApplicationTimerStart;                   // +0x10
    double ApplicationTotalTime;                    // +0x18
    bool MinUpdated;                                // +0x20
    double MinApplicationTotalTime;                 // +0x28
    double StoredTotalTime;                         // +0x30
    long NoTurns;                                   // +0x38
    CRenderManager* RenderManager;                  // +0x3C
    EDrawMode DrawMode;                             // +0x40
    EEvalMode EvalMode;                             // +0x44
    bool NewProfilesRegistered;                     // +0x48
    std::list<CProfile*> ProfileList;               // +0x4C
    std::map<long, CCharString> IDToProfileName;    // +0x50
    std::map<CCharString, long> ProfileNameToID;    // +0x5C
    std::list<CProfileTreeEntry> ProfileTree;       // +0x68
    CProfileTreeEntry* CurrentTreeEntry;            // +0x6C
    bool NeedsSaving;                               // +0x70
};

namespace std {
template <> void _List_base<CProfileManager::CProfileTreeEntry,
                            allocator<CProfileManager::CProfileTreeEntry> >::clear();   // 0x009F9B30
}

CProfileManager::~CProfileManager()
{
    for (std::list<CProfileTreeEntry>::iterator it = ProfileTree.begin(); it != ProfileTree.end(); ++it)
        ;
    ProfileTree.clear();
    IDToProfileName.clear();
    ProfileNameToID.clear();
}