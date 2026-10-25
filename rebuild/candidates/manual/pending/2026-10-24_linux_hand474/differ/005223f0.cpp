// CThingManager::LoadLoadableGlobalThingsFromFile(CCountedPointer<CAFile> file, long version) -- when global things load from file (GlobalThingLoadingBehaviour 1), read the file's things for the world map's loadable sections and activate them
// Grade: DIFFER(306v306) -- frame 0x24 vs retail 0x1C: retail overlays the discarded set::insert pair<iterator,bool> result with the later vector<CThing*> local (+0x10), ours keeps separate slots (every other byte matches modulo the shifted esp offsets)
// Leaf: ?LoadLoadableGlobalThingsFromFile@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005223F0, 320 bytes (0x132 + int3 padding), thiscall `ret 0Ch` (the 8-byte
//   CCountedPointer by value, then the version).
// Identity: Ego_r.pdb ?LoadLoadableGlobalThingsFromFile@CThingManager@@QAEXV?$CCountedPointer@VCAFile@@@@J@Z; the
//   GlobalThingLoadingBehaviour test (+0x80) and WorldMap (+0x28) match the CThingManager constructor 0x00523540.
//   Catalog label "LoadGlobalThingBatch" is descriptive.
// Callees: malloc 0x00BFEA0E (the set header), CCharString copy 0x0099EC30 (from the global section name 0x013BAB08),
//   set<CCharString>::insert_unique 0x004AC380, ~CCharString 0x0099EAE0, CWorldMap::GetLoadableSections slot 26
//   (+0x68, 0x004FF490; its by-value set built straight into LoadFromFile's argument slot), CThingManager::LoadFromFile
//   0x00521AE0 (landed), CThingManager::ActivateThings 0x0051E2F0 (landed), free 0x00BFEA14, the set's _M_erase
//   0x004ABA70, operator delete 0x00BFE9BC (the file pointer's count block).
// Shape: a local set<CCharString> holding the global section name (built and destroyed, never passed), an empty
//   vector<CThing*>, LoadFromFile(file, version, WorldMap->GetLoadableSections(), things, 0, 2), ActivateThings.
//   The name must come from an inline returning CCharString by value (retail re-leas the temporary rather than reusing
//   the copy constructor's eax); STLport pair / tree iterators need user constructors for the hidden-pointer result.
// Tried: (a) POD pair (returned in edx:eax), (b) pair + iterator constructors, (c) the by-value name getter (this
//   file: registers and sizes match), (d) STLport set::insert with the named `__p` + const_iterator conversion (same
//   as c). Next: give insert_unique a same-TU noinline body, or scope the insert in an inline helper so its result
//   slot is released before the vector is declared.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator<(const CCharString& other) const;
};

struct CCPPointerInfo {
    unsigned long refs;                                     // +0x0
    void (__fastcall* destroy)(void*);                      // +0x4
    void* object;                                           // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              object;                                 // +0x0
    CCPPointerInfo* control;                                // +0x4
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control)
    {
        if (control)
            ++control->refs;
    }
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
        }
    }
};

namespace std {
template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) {}
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(const A& a = A()) : _Vector_base<T, A>(a) {}
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class T> struct _Identity { const T& operator()(const T& x) const { return x; } };

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base { V _M_value_field; };
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
};

template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _Rb_tree_base(const A&) : _M_header(allocator<_Node>(), (_Rb_tree_node_base*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_iterator<V> iterator;
    unsigned int _M_node_count;
    C _M_key_compare;
    _Rb_tree(const C& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    void _M_erase(_Rb_tree_node<V>* x);                     // 0x004ABA70
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Rb_tree_node<V>*)this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    pair<iterator, bool> insert_unique(const V& v);         // 0x004AC380
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(C(), A()) {}
    pair<iterator, bool> insert(const K& x) { return _M_t.insert_unique(x); }
};
}

class CAFile;
class CThing;
class CASuspendableProcess;
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25();
    virtual std::set<CCharString> GetLoadableSections() const;   // slot 26 (+0x68), 0x004FF490
};
enum EGlobalThingLoadingBehaviour { GLOBAL_THING_LOADING_BEHAVIOUR_0, GLOBAL_THING_LOADING_BEHAVIOUR_1 };

extern CCharString g_GlobalThingSectionName;                // 0x013BAB08
inline CCharString GetGlobalThingSectionName() { return g_GlobalThingSectionName; }

class CThingManager {
public:
    unsigned char _pad00[0x28];
    CWorldMap* WorldMap;                                    // +0x28
    unsigned char _pad2c[0x80 - 0x2c];
    EGlobalThingLoadingBehaviour GlobalThingLoadingBehaviour;   // +0x80
    void LoadFromFile(CCountedPointer<CAFile> file, long version, std::set<CCharString> names,
                      std::vector<CThing*>& things, CASuspendableProcess* process, int mode);   // 0x00521AE0
    void ActivateThings(std::vector<CThing*>& things);    // 0x0051E2F0
    void LoadLoadableGlobalThingsFromFile(CCountedPointer<CAFile> file, long version);
};

void CThingManager::LoadLoadableGlobalThingsFromFile(CCountedPointer<CAFile> file, long version)
{
    if (GlobalThingLoadingBehaviour == GLOBAL_THING_LOADING_BEHAVIOUR_1) {
        std::set<CCharString> sections;
        sections.insert(GetGlobalThingSectionName());
        std::vector<CThing*> things;
        LoadFromFile(file, version, WorldMap->GetLoadableSections(), things, 0, 2);
        ActivateThings(things);
    }
}
