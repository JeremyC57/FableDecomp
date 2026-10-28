// CShaderStats::CShaderStats() -- (anonymous-namespace class) shader-change statistics object: base CBaseClass, both current shader names start as NoneStr, both change maps start empty
// Leaf: ??0CShaderStats@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098DD10, length 128 bytes (126 + int3 padding), plain `ret`.
// PDB (Ego_r, donor 0x00AE8B40): ??0CShaderStats@?A0x9e483253@@QAE@XZ; RTTI vtable 0x0129A078 (?A0x2365ad56::CShaderStats).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, CCharString copy ctor @ 0x0099EC30 (from CShaderStats::NoneStr
// @ 0x013BC440, twice), malloc @ 0x00BFEA0E (each map's 0x1C-byte header node).
// Caller: the dynamic initializer @ 0x01223CE5 (the file-static stats object).
// Label: the manifest's "NHeroInformationScreens_CBase_ctor" is propagated; the PDB names CShaderStats.
// Layout (Ego_r): CurrentPixelShaderName +0x04, PixelShaderChanges +0x08 (map<pair<CCharString,CCharString>,
// CInitialisedValue<unsigned long,0> >), CurrentVertexShaderName +0x14 (+0x18 in the debug build; the STLport map is 12
// bytes, the debug Dinkumware one 16), VertexShaderChanges +0x18.
// Shape: STLport 4.6 _Rb_tree default ctor inlined twice -- _Rb_tree_base nulls the header then allocates it
// (allocate(1) = malloc(0x1C)), node count 0, then _M_empty_initialize (red colour, null root, leftmost/rightmost =
// header, each through a reload of the header pointer).
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class CBaseClass {
public:
    CBaseClass();                               // 0x0099A2D0
    virtual ~CBaseClass();
};

namespace std {

template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> struct less { };

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(size_t count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const Alloc& alloc, Value p) : Alloc(alloc), _M_data(p) { }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;
};

template <class Value, class Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<Value> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const Alloc&) : _M_header(allocator<_Node>(), 0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};

template <class Key, class Value, class Compare, class Alloc> class _Rb_tree : public _Rb_tree_base<Value, Alloc> {
public:
    size_t _M_node_count;
    Compare _M_key_compare;
    _Rb_tree(const Compare& comp = Compare(), const Alloc& alloc = Alloc())
        : _Rb_tree_base<Value, Alloc>(alloc), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // _S_rb_tree_red
        this->_M_header._M_data->_M_parent = 0;             // root
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    public:
};

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef pair<const Key, T> value_type;
    _Rb_tree<Key, value_type, Compare, Alloc> _M_t;
    map() : _M_t(Compare(), Alloc()) { }
};

}   // namespace std

namespace {

template <class T, int Initial> class CInitialisedValue {
public:
    T Value;
};

class CShaderStats : public CBaseClass {
public:
    static const CCharString NoneStr;           // 0x013BC440
    CCharString CurrentPixelShaderName;         // +0x04
    std::map<std::pair<CCharString, CCharString>, CInitialisedValue<unsigned long, 0> > PixelShaderChanges;   // +0x08
    CCharString CurrentVertexShaderName;        // +0x14
    std::map<std::pair<CCharString, CCharString>, CInitialisedValue<unsigned long, 0> > VertexShaderChanges;  // +0x18
    CShaderStats();
    virtual ~CShaderStats();
};

CShaderStats::CShaderStats()
    : CurrentPixelShaderName(NoneStr), CurrentVertexShaderName(NoneStr)
{
}

}   // namespace

// keeps the internal-linkage constructor emitted (retail constructs the file-static stats object)
void* KeepShaderStats(void* where)
{
    return new (where) CShaderStats;
}