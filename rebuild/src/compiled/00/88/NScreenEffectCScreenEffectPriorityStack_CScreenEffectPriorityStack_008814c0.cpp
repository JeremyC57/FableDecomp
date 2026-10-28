// NScreenEffect::CScreenEffectPriorityStack::~CScreenEffectPriorityStack() -- reset the vtable, then destroy the members in reverse: per effect kind (screen filter, displacement, radial blur) the key set (erase the tree, free the header) and the three priority lists (destroy each entry through its virtual destructor, free the nodes and the sentinel)
// Leaf: ??1CScreenEffectPriorityStack@NScreenEffect@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008814C0, 464 bytes (as cut), thiscall, plain `ret`.
// Identity: the catalog's "CEnvironment_Destructor" is descriptive. The vtable stored first, 0x01278058, is the one the
//   landed constructor 0x00881370 installs; the landed scalar deleting destructor 0x00884BE0 calls this body. Layout
//   (Ego_r.pdb CScreenEffectPriorityStack with retail 4-byte STLport lists and 12-byte sets, 0xCC bytes): vptr +0,
//   RadialParamStack list<CRadialBlurEntry>[3] +4, RadialBlurKeys set<CStackEntry*> +0x10, CurrentRadialBlurParams
//   +0x1C, CurrentRadialBlurPosParams +0x4C, DisplacementParamStack list<CDisplacementMonochromeEffectColourEntry>[3]
//   +0x6C, DisplacementKeys +0x78, CurrentDisplacementParam +0x84, ScreenFilterParamStack list<CScreenFilterEntry>[3]
//   +0x94, ScreenFilterKeys +0xA0, CurrentScreenFilterParam +0xAC.
// Callees: _Rb_tree::_M_erase(node) 0x00882BC0 (thiscall, out of line; landed), free 0x00BFEA14 (the STLport malloc
//   allocator); each list entry is destroyed through vtable slot 0 (`push 0 ; call [eax]`, its virtual destructor).
// Shape: a user-declared empty virtual destructor (the vtable store; a generated one drops it), the members' inline
//   STLport destructors: `_Rb_tree::~_Rb_tree() { clear(); }` (node-count test, _M_erase(root), leftmost / root /
//   rightmost / count reset) then `~_Rb_tree_base` frees the header; each list array walked backwards (the counted
//   array-destructor loop) with `~_List_base() { clear(); deallocate(sentinel); }`, clear() inlined.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };

// ---- list ----
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
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
};

// ---- set ----
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Tp, class _Alloc>
class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
    ~_Rb_tree() { clear(); }
private:
    void _M_erase(_Link_type __x);                          // 0x00882BC0
public:
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
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
};

} // namespace std

class CRGBFloatColour {
public:
    float R, G, B, A;
};

namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CStackEntry {                                    // polymorphic base of the three entry kinds
        virtual ~CStackEntry();
    };
    struct CRadialBlurEntry : public CStackEntry { unsigned char _body[0x5C]; };
    struct CDisplacementMonochromeEffectColourEntry : public CStackEntry { unsigned char _body[0x1C]; };
    struct CScreenFilterEntry : public CStackEntry { unsigned char _body[0x38]; };

    virtual ~CScreenEffectPriorityStack();

    std::list<CRadialBlurEntry>                         RadialParamStack[3];        // +0x04
    std::set<CStackEntry*>                              RadialBlurKeys;             // +0x10
    float                                               CurrentRadialBlurParams[12];    // +0x1C CRadialBlurParams
    float                                               CurrentRadialBlurPosParams[8];  // +0x4C CRadialBlurPositionParams
    std::list<CDisplacementMonochromeEffectColourEntry> DisplacementParamStack[3];  // +0x6C
    std::set<CStackEntry*>                              DisplacementKeys;           // +0x78
    CRGBFloatColour                                     CurrentDisplacementParam;   // +0x84
    std::list<CScreenFilterEntry>                       ScreenFilterParamStack[3];  // +0x94
    std::set<CStackEntry*>                              ScreenFilterKeys;           // +0xA0
    float                                               CurrentScreenFilterParam[8];    // +0xAC CScreenFilterParams
};
}

NScreenEffect::CScreenEffectPriorityStack::~CScreenEffectPriorityStack()
{
}