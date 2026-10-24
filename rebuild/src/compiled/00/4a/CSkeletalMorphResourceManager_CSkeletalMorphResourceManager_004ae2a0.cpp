// CSkeletalMorphResourceManager::CSkeletalMorphResourceManager() -- construct the skeletal-morph resource manager singleton: CTBaseSingleton base, then an empty CachedMorphs map (STLport header node allocated and self-linked)
// Leaf: ??0CSkeletalMorphResourceManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE2A0, 65 bytes (80 with padding), plain `ret` (__fastcall, returns this).
// Identity: the stored vtable 0x01238F84 is `.?AVCSkeletalMorphResourceManager@@` by its RTTI locator (slot 0 is the
//   scalar deleting destructor 0x004AE3B0, landed); Ego_r.pdb `??0CSkeletalMorphResourceManager@@QAE@XZ`; callers
//   0x004A2EF1, 0x004A76DD, 0x004AE379 (the CTBaseSingleton Get / Reset paths).
// Callees: 0x00BFEA0E malloc (STLport malloc allocator: `_M_header.allocate(1)` folds to malloc(0x1C)).
// Layout: Ego_r.pdb CSkeletalMorphResourceManager (vptr +0; map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >
//   CachedMorphs +4: retail STLport map = header +4, node count +8, empty comparator +0xC); nodes are 0x1C bytes
//   (_Rb_tree_node_base 0x10 + CCharString 4 + the 8-byte CCountedPointer).
// Shape: the inline protected CTBaseSingleton() base ctor (its vtable store dead), STLport `_Rb_tree()`:
//   `_Rb_tree_base(a) : _M_header(a, 0) { _M_header._M_data = _M_header.allocate(1); }`, then `_M_node_count(0)` and
//   `_M_empty_initialize()` (color red = 0, root 0, leftmost = rightmost = header).
// Label correction: catalogued as `LTextTree_InitSentinel`; the real routine is the CSkeletalMorphResourceManager ctor.
#include <stdlib.h>                                   // malloc @ 0x00BFEA0E

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

class C3DSkeletalMorphTarget;
template <class T> class CCountedPointer {
public:
    T*    Data;
    long* RefCount;
};

typedef bool _Rb_tree_Color_type;
const _Rb_tree_Color_type _S_rb_tree_red = false;

struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;                     // +0x00
    _Rb_tree_node_base* _M_parent;                    // +0x04
    _Rb_tree_node_base* _M_left;                      // +0x08
    _Rb_tree_node_base* _M_right;                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                            // +0x10
};

template <class _Tp> struct allocator {
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    _Rb_tree_base(const allocator<_Node>& __a) : _M_header(__a, (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
};

template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const; };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };

template <class _Key, class _Value, class _Compare>
struct _Rb_tree : public _Rb_tree_base<_Value> {
    unsigned int _M_node_count;                       // +0x04
    _Compare     _M_key_compare;                      // +0x08
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = _S_rb_tree_red;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree() : _Rb_tree_base<_Value>(allocator<_Rb_tree_node<_Value> >()), _M_node_count(0), _M_key_compare(_Compare())
    {
        _M_empty_initialize();
    }
};

template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() { }
protected:
    CTBaseSingleton() { }
};

class CSkeletalMorphResourceManager : public CTBaseSingleton<CSkeletalMorphResourceManager> {
public:
    _Rb_tree<CCharString, pair<const CCharString, CCountedPointer<C3DSkeletalMorphTarget> >, less<CCharString> > CachedMorphs;   // +0x04
    CSkeletalMorphResourceManager();
    virtual ~CSkeletalMorphResourceManager();
};

CSkeletalMorphResourceManager::CSkeletalMorphResourceManager()
{
}