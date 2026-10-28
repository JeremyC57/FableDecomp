// CScriptedMapBrush::CScriptedMapBrush(const CScriptedMapBrushInit& init) -- construct a brush from its init block: one cell per game-area square, empty thing sets/map, the area, name, creation time and pattern copied
// Leaf: ??0CScriptedMapBrush@@QAE@ABVCScriptedMapBrushInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082E600, 213 bytes (+11 int3), thiscall `ret 4`.
// Identity: Ego_r.pdb name set ??0CScriptedMapBrush@@QAE@ABVCScriptedMapBrushInit@@@Z; CScriptedMapBrushInit (Ego_r.pdb,
//   28 bytes): GameArea (C2DBoxI) +0, Name +0x10, CreationTimeSeconds +0x14, CreationPattern +0x18. The catalog label
//   "CScriptedMapBrush_CreateFromDefinition" is descriptive.
// Callees: CBaseObject::CBaseObject() 0x0099A380, vector<CScriptedMapCell>(size_type n) 0x0082F630, malloc 0x00BFEA0E
//   (the three tree headers), CCharString(const CCharString&) 0x0099EC30.
// Layout: retail CScriptedMapBrush (0x54 bytes; Ego_r.pdb is 0x64 with 16-byte debug containers), as the landed copy
//   constructor 0x00827040: CBaseObject base (vptr, IntelligentPointer +4), Cells +0x08, ThingsToDelete +0x14,
//   ThingsToMove +0x20, ThingsToAdd +0x2c (STLport set/map: header, count, compare), GameArea +0x38, BrushName +0x48,
//   CreationTimeSeconds +0x4c, CreationPattern +0x50. engine/CScriptedMapBrush.h is the donor layout, so the model is local.
// Shape: init list in declaration order -- Cells(width * height) (BRX - TLX times BRY - TLY), the three STLport trees'
//   inline constructors (_Rb_tree_base: header zeroed then malloc(node); count 0; _M_empty_initialize: red colour,
//   null root, leftmost/rightmost = header, each re-reading the header), the area through C2DBoxI's memberwise copy,
//   the name copy constructor, the time and pattern as dword moves.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                 // 0x00BFEA14

class C3DVector {
public:
    float X, Y, Z;
};
class C3DAngles {
public:
    float X, Y, Z;
};
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;                                // +0x00 .. +0x0c
    C2DBoxI() { }
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) { }
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
};
class CCharString {
public:
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const CCharString& other);                  // 0x0099EC30
};
class CBaseObject {
public:
    void* IntelligentPointer;                               // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class A, class B> struct pair {
    A first;
    B second;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                           _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};

template <class _Value, class _Node, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& a, _Value p) : _Alloc(a), _M_data(p) { }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    protected:
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
    static bool& _S_color(_Link_type x) { return x->_M_color; }
    private:
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = false;          // _S_rb_tree_red
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    public:
    _Rb_tree(const _Compare& comp, const _Alloc& a)
        : _Rb_tree_base<_Value, _Alloc>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    iterator end() { return iterator(this->_M_header._M_data); }
    private:
    _Link_type _M_find(const _Key& k) const;                // 0x0082EEB0 (map) / 0x0082EF20 (set): the node in eax
    public:
    iterator find(const _Key& k) { return iterator(_M_find(k)); }
    void erase(iterator position)
    {
        _Link_type y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(position._M_node,
            this->_M_header._M_data->_M_parent, this->_M_header._M_data->_M_left, this->_M_header._M_data->_M_right);
        this->_M_header.deallocate(y, 1);
        --_M_node_count;
    }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    typedef _Rb_tree<_Key, _Key, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(_Compare(), _Alloc()) { }
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& x) { return _M_t.find(x); }
    void erase(iterator position) { _M_t.erase(position); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    map() : _M_t(_Compare(), _Alloc()) { }
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& x) { return _M_t.find(x); }
    void erase(iterator position) { _M_t.erase(position); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    explicit vector(unsigned int n);                        // 0x0082F630
};
}

class CScriptedMapBrushInit {                               // Ego_r.pdb (28 bytes)
public:
    C2DBoxI     GameArea;                                   // +0x00
    CCharString Name;                                       // +0x10
    float       CreationTimeSeconds;                        // +0x14
    long        CreationPattern;                            // +0x18
};

class CScriptedMapBrush : public CBaseObject {              // vtable 0x01272518; retail 0x54 bytes (12-byte containers)
public:
    class CScriptedMapCell {                                // 20 bytes
    public:
        unsigned long m_words[5];
    };
    class CThingPosAndOrientation {                         // 24 bytes
    public:
        C3DVector Position;                                 // +0x00
        C3DAngles FacingAngles;                             // +0x0c
    };
    std::vector<CScriptedMapCell>                                   Cells;                 // +0x08
    std::set<unsigned __int64>                                      ThingsToDelete;        // +0x14
    std::map<unsigned __int64, CThingPosAndOrientation>             ThingsToMove;          // +0x20
    std::set<unsigned __int64>                                      ThingsToAdd;           // +0x2c
    C2DBoxI                                                          GameArea;              // +0x38
    CCharString                                                      BrushName;             // +0x48
    float                                                            CreationTimeSeconds;   // +0x4c
    long                                                             CreationPattern;       // +0x50
    virtual void v00();
    CScriptedMapBrush();
    CScriptedMapBrush(const CScriptedMapBrushInit& init);
    void RemoveThing(unsigned __int64 uid);
};

CScriptedMapBrush::CScriptedMapBrush(const CScriptedMapBrushInit& init)
    : Cells(init.GameArea.GetWidth() * init.GameArea.GetHeight()), GameArea(init.GameArea), BrushName(init.Name),
      CreationTimeSeconds(init.CreationTimeSeconds), CreationPattern(init.CreationPattern)
{
}