// CProfileManager::CProfileManager() -- construct the profile manager: clear the key latches and cursor, build the empty STLport ProfileList, IDToProfileName / ProfileNameToID maps and ProfileTree, register as the global manager, cache the display manager's render manager, push the root tree entry and reset the timing state
// Leaf: ??0CProfileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9160, 242 bytes, thiscall, returns this. Caller: CSystemManager::Initialise
//   (`new` 0x78 bytes at 0x009A6993). The chunk label "CEnginePrimitive_Initialize" is wrong.
// Layout (retail 0x78 bytes; member names/order from Ego_r.pdb CProfileManager, offsets re-derived for STLport's 4-byte
//   list and 12-byte map): UpPressed/DownPressed/TogglePressed +0..+2, CursorPosition +4, MinUpdated +0x20,
//   MinApplicationTotalTime +0x28, NoTurns +0x38, RenderManager +0x3C, DrawMode +0x40, EvalMode +0x44, ProfileList +0x4C,
//   IDToProfileName +0x50, ProfileNameToID +0x5C, ProfileTree +0x68, CurrentTreeEntry +0x6C, NeedsSaving +0x70.
// Callees: 0x00BFEA0E malloc (list/tree header nodes; allocate(1) folded), GFGetSystemManager @ 0x009A4EC0 (display
//   manager +0x60, its PRenderManager +8), list<CProfileTreeEntry>::insert(iterator) @ 0x009F9C40.
// Global: 0x013CB4F8 (the manager singleton; only written here).
// Shape: no EH frame in this TU; the 0.0 constant is the shared double pool at 0x0122ED70. The display/render manager
//   pointers are read as plain members: an inline accessor on a GFGetSystemManager() result leaves a dead `add eax,N`.
extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

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
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _List_iterator<_Tp>& __x) : _List_iterator_base(__x._M_node) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator<_Tp>& operator--() { _M_node = _M_node->_M_prev; return *this; }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp> iterator;
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) {}

    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    _Tp& front() { return *begin(); }
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator insert(iterator __position);                           // 0x009F9C40
    void push_back() { insert(end()); }
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
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    public:
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    map() : _M_t(_Compare(), _Alloc()) {}
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

class CCharStringData;
class CCharString {
public:
    CCharStringData* m_pStringData;
    bool operator<(const CCharString& other) const;
};

class CWideStringData;
class CWideString {
public:
    CWideStringData* m_pStringData;
};

class CRenderManager;

template <class T> class scoped_ptr {
public:
    T* ptr;
};

class CSystemManager;

class CDisplayManager {
public:
    int _padding_;                                  // +0x00
    CSystemManager* SystemManager;                  // +0x04
    scoped_ptr<CRenderManager> PRenderManager;      // +0x08
};

template <class T> class CCountedPointer {
public:
    T* Data;
};

class CSystemManager {
public:
    char _pad_00[0x60];
    CCountedPointer<CDisplayManager> PDisplayManager;   // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();                    // 0x009A4EC0

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

    CProfileManager();

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

CProfileManager* g_ProfileManager;                                  // 0x013CB4F8

CProfileManager::CProfileManager()
    : UpPressed(false), DownPressed(false), TogglePressed(false), CursorPosition(0), NeedsSaving(false)
{
    if (g_ProfileManager == 0)
        g_ProfileManager = this;
    if (GFGetSystemManager()->PDisplayManager.Data != 0)
        RenderManager = GFGetSystemManager()->PDisplayManager.Data->PRenderManager.ptr;
    ProfileTree.push_back();
    ProfileTree.front().Parent = 0;
    CurrentTreeEntry = &ProfileTree.back();
    MinApplicationTotalTime = 0.0;
    NoTurns = 0;
    DrawMode = DRAW_MODE_0;
    MinUpdated = false;
    EvalMode = EVAL_MODE_1;
}