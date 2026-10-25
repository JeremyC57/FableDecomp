// CTCTextureDecal::PeekEngineDecals() -- when dirty, rebuild the engine decal list: one engine decal per decalled texture (map keyed by texture), its overlaid blends in decal order
// Leaf: ?PeekEngineDecals@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070A570, 518 bytes, __fastcall, `ret`, returns const vector<CEngineTextureDecal>&.
// PDB: ?PeekEngineDecals@CTCTextureDecal@@QAEABV?$vector@VCEngineTextureDecal@@V?$allocator@VCEngineTextureDecal@@@std@@@std@@XZ
//   (Ego_r 0x006813E0); catalog "CEngineInternalPrimitive_RebuildDecals" was descriptive.
// Callees: vector<CEngineTextureDecal>::erase(first, last) 0x0070B150 (clear()), malloc 0x00BFEA0E (the map header),
//   map<long, CEngineTextureDecal>::find 0x0070A860, vector<CCompositeBlendDesc>::push_back 0x006CB010 (x2),
//   vector<CCompositeBlendDesc>(const vector&) 0x005BB346 (x2; catalog "std::vector::operator="), map insert_unique
//   0x0070AFE0, vector<CEngineTextureDecal>::_M_insert_overflow 0x0070B240, the tree _M_erase 0x0070ADB0 (catalog
//   "LTextTreeWalkThrough::BuildTreeArray"), free 0x00BFEA14.
// Layout (retail STLport): Decals list +0x0C (4-byte node pointer; CEntry {EntryID, TextureID, DecalDesc} at node +8),
//   EngineDecals +0x10 (12 bytes), CurrentAllocEntryID +0x1C, Dirty +0x20; CEngineTextureDecal 20 bytes
//   {OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends}; map node value at +0x10.
// Shape: if Dirty: clear(), a local map<long, CEngineTextureDecal> (its _Rb_tree_base allocates the header before the
//   derived _M_node_count(0): retail's `[hdr]=0 ; call malloc ; [hdr]=eax ; [count]=0`), per decal find then either a new
//   decal (texture, id 0) inserted as value_type(texture, decal) or push_back onto the found blends; then push_back
//   every map value (inline STLport push_back; the tree increment is the static `_Rb_global<bool>::_M_increment(node)`
//   on a node copy, retail's `mov eax,esi`), Dirty = false before the map dies (clear() + free header); return the
//   vector. The blend vector's copy/push_back are out-of-line COMDATs, declared only (as the landed 0x0070B240 does);
//   the STLport helpers this TU instantiated are same-TU noinline bodies.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CRGBColour {
public:
    unsigned char B, G, R, A;
};
class CCompositeBlendDesc {                         // 16 bytes
public:
    unsigned long TextureIndex;                     // +0x00
    long          BlendType;                        // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                    // +0x08
    CRGBColour    SpecularColour;                   // +0x0C
};
class CCompositeBlendDescVector {                   // vector<CCompositeBlendDesc> (retail STLport, 12 bytes)
public:
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
    CCompositeBlendDescVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CCompositeBlendDescVector(const CCompositeBlendDescVector& __x);   // 0x005BB346
    ~CCompositeBlendDescVector()
    {
        if (_M_start != 0)
            free(_M_start);
    }
    CCompositeBlendDescVector& operator=(const CCompositeBlendDescVector& __x);
    void push_back(const CCompositeBlendDesc& __x);                    // 0x006CB010
};
class CEngineTextureDecal {                         // Ego_r.pdb; 20 bytes retail
public:
    long OriginalTextureIndex;                      // +0x00
    long DecalID;                                   // +0x04
    CCompositeBlendDescVector OverlaidBlends;       // +0x08
    CEngineTextureDecal(long texture) : OriginalTextureIndex(texture), DecalID(0) {}
};

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> struct less { less() {} bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
    void deallocate(_Tp* __p) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

// ---- list (Decals) ----
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    iterator begin() const { return iterator(_M_node._M_data->_M_next); }
    iterator end() const { return iterator(_M_node._M_data); }
};

// ---- vector (EngineDecals) ----
#pragma auto_inline(off)
// Same-TU template bodies (retail keeps them out of line in this TU).
template <class _Tp> __declspec(noinline) _Tp* __fastcall __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)   // 0x0070AA50
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(__result, *__first);
    return __result;
}
template <class _Tp> __declspec(noinline) _Tp* __fastcall __uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x, const __false_type&)   // 0x0070AAF0
{
    for ( ; __n > 0; --__n, ++__first)
        _Construct(__first, __x);
    return __first;
}
template <class _Tp> __declspec(noinline) _Tp* __fastcall __copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)   // 0x0070AD20
{
    for ( ; __first != __last; ++__first, ++__result)
        *__result = *__first;
    return __result;
}
#pragma auto_inline(on)

template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for ( ; __first != __last; ++__first)
        __first->~_Tp();
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp* begin() const { return this->_M_start; }
    _Tp* end() const { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    void _M_clear();                                // 0x0070B1C0
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x0070B240
    _Tp* erase(_Tp* __first, _Tp* __last);          // 0x0070B150
    void clear() { erase(begin(), end()); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1, true);
    }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start);
}
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
template <class _Tp, class _Alloc>
__declspec(noinline) _Tp* vector<_Tp, _Alloc>::erase(_Tp* __first, _Tp* __last)
{
    _Tp* __i = __copy(__last, this->_M_finish, __first, __false_type());
    _Destroy(__i, this->_M_finish);
    this->_M_finish = __i;
    return __first;
}
#pragma auto_inline(on)

// ---- map (the per-texture engine decals) ----
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __node)
    {
        if (__node->_M_right != 0) {
            __node = __node->_M_right;
            while (__node->_M_left != 0)
                __node = __node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = __node->_M_parent;
            while (__node == __y->_M_right) {
                __node = __y;
                __y = __y->_M_parent;
            }
            if (__node->_M_right != __y)
                __node = __y;
        }
        return __node;
    }
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map : public _Rb_tree_base<pair<const _Key, _Tp>, _Alloc> {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_iterator<value_type> iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;

    map() : _Rb_tree_base<value_type, _Alloc>(_Alloc()), _M_node_count(0), _M_key_compare()
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    ~map() { clear(); }
    iterator begin() const { return iterator(this->_M_header._M_data->_M_left); }
    iterator end() const { return iterator(this->_M_header._M_data); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Node*)this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    void _M_erase(_Node* __x);                      // 0x0070ADB0
    iterator find(const _Key& __k);                 // 0x0070A860
    pair<iterator, bool> insert(const value_type& __v);   // 0x0070AFE0 (insert_unique)
};

#pragma auto_inline(off)
template <class _Key, class _Tp, class _Compare, class _Alloc>
__declspec(noinline) void map<_Key, _Tp, _Compare, _Alloc>::_M_erase(_Node* __x)
{
    while (__x != 0) {
        _M_erase((_Node*)__x->_M_right);
        _Node* __y = (_Node*)__x->_M_left;
        __x->_M_value_field.~value_type();
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}
template <class _Key, class _Tp, class _Compare, class _Alloc>
__declspec(noinline) typename map<_Key, _Tp, _Compare, _Alloc>::iterator map<_Key, _Tp, _Compare, _Alloc>::find(const _Key& __k)
{
    _Rb_tree_node_base* __y = this->_M_header._M_data;
    _Rb_tree_node_base* __x = this->_M_header._M_data->_M_parent;
    while (__x != 0) {
        if (!_M_key_compare(((_Node*)__x)->_M_value_field.first, __k)) {
            __y = __x;
            __x = __x->_M_left;
        }
        else
            __x = __x->_M_right;
    }
    if (__y == this->_M_header._M_data || _M_key_compare(__k, ((_Node*)__y)->_M_value_field.first))
        __y = this->_M_header._M_data;
    return iterator(__y);
}
template <class _Key, class _Tp, class _Compare, class _Alloc>
__declspec(noinline) pair<typename map<_Key, _Tp, _Compare, _Alloc>::iterator, bool> map<_Key, _Tp, _Compare, _Alloc>::insert(const value_type& __v)
{
    // Stand-in: the real insert_unique (descent, decrement, _M_insert 0x0070AEA0) is a landed row.
    _Node* __z = this->_M_header.allocate(1);
    new (&__z->_M_value_field) value_type(__v);
    __z->_M_left = 0;
    __z->_M_right = 0;
    __z->_M_parent = this->_M_header._M_data;
    this->_M_header._M_data->_M_parent = __z;
    ++_M_node_count;
    return pair<iterator, bool>(iterator(__z), true);
}
#pragma auto_inline(on)
}

class CTCTextureDecal {
public:
    class CEntry {                                  // 24 bytes
    public:
        long EntryID;                               // +0x00
        long TextureID;                             // +0x04
        CCompositeBlendDesc DecalDesc;              // +0x08
    };
    virtual void v00();
    void* ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    std::list<CEntry> Decals;                       // +0x0C (retail STLport list, 4 bytes)
    std::vector<CEngineTextureDecal> EngineDecals;  // +0x10
    long CurrentAllocEntryID;                       // +0x1C
    bool Dirty;                                     // +0x20
    const std::vector<CEngineTextureDecal>& PeekEngineDecals();
};

const std::vector<CEngineTextureDecal>& CTCTextureDecal::PeekEngineDecals()
{
    if (Dirty) {
        EngineDecals.clear();
        std::map<long, CEngineTextureDecal> decalMap;
        for (std::list<CEntry>::iterator it = Decals.begin(); it != Decals.end(); ++it) {
            std::map<long, CEngineTextureDecal>::iterator found = decalMap.find(it->TextureID);
            if (found == decalMap.end()) {
                CEngineTextureDecal decal(it->TextureID);
                decal.OverlaidBlends.push_back(it->DecalDesc);
                decalMap.insert(std::map<long, CEngineTextureDecal>::value_type(it->TextureID, decal));
            }
            else
                found->second.OverlaidBlends.push_back(it->DecalDesc);
        }
        for (std::map<long, CEngineTextureDecal>::iterator it = decalMap.begin(); it != decalMap.end(); ++it)
            EngineDecals.push_back(it->second);
        Dirty = false;
    }
    return EngineDecals;
}