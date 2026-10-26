// CTCDHeroSuit::OnCreate() -- fetch the hero-suit sub-def from the owner's def, then list every suit part the def names as (part, not yet worn)
// Leaf: ?OnCreate@CTCDHeroSuit@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FF8A0, 178 bytes, __fastcall, plain `ret`; CTCDHeroSuit vtable
//   0x01242004 slot 4 (abs ref 0x01242014; slot 4 of the CTCBase interface is OnCreate).
// Name: Ego_r.pdb `?OnCreate@CTCDHeroSuit@@UAEXXZ`; the catalog's "TreeTraverse_AbilityDefPopulator" is descriptive.
// Callees: CDefClassBase::GetSubDef<CHeroSuitDef>(CDefPointer<const CHeroSuitDef>*) const 0x005935C4 (manifest
//   propagates a CAbilityDef instantiation), STLport vector<pair<EClothingSuitPart, bool> >::_M_insert_overflow(pointer,
//   const value_type&, const __false_type&, size_type, bool) 0x007FFAB0.
// Layout = Ego_r.pdb: CTCDHeroSuit PDef (CDefPointer<const CHeroSuitDef>) +0x0c, SuitParts vector<pair<EClothingSuitPart,
//   bool> > +0x10 (8-byte elements); CHeroSuitDef SuitParts map<EClothingSuitPart, long> +0x28 (retail STLport tree:
//   header node pointer first, node key +0x10); CThing PDef +0x70. Local model: no engine header for these classes.
// Shape: GetSubDef through the owner's def pointer with no add-ref (`operator->` on the member); a `for` over the
//   def's map with end() re-read through PDef every pass and STLport's static `_M_increment` inlined; STLport
//   push_back inlined: placement-new (null-tested) of the pair at _M_finish then `add [finish],8`, else
//   `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`; the empty __false_type tag (user ctor, no zero store)
//   sits in the top byte of the spare frame word. Retail (LTCG) knew _M_insert_overflow only reads x, so the
//   make_pair temporary's `false` byte is stored once above the loop and the fast-path copy stores the constant 0:
//   that needs (1) STLport's explicit memberwise pair copy constructor (the implicit one copies two dwords) and
//   (2) a same-TU stand-in body of _M_insert_overflow compiled above the caller under `auto_inline(off)` (the
//   GOTCHAS callee-knowledge technique; it is not the graded function).
inline void* operator new(unsigned int, void* p) { return p; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71 import)
extern "C" void __cdecl free(void* p);                // 0x00BFEA14 (MSVCR71 import)

enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };

namespace std {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __o) : first(__o.first), second(__o.second) {}   // STLport's explicit copy constructor
};
template <class _T1, class _T2> inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
    return pair<_T1, _T2>(__x, __y);
}

template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned long __fill_len,
                            bool __atend);  // 0x007FFAB0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in for _M_insert_overflow 0x007FFAB0 (retail holds it 0x210 bytes below this function; LTCG let
// retail see that it only reads x): STLport 4.x body with the malloc/free allocator, compiled out of line above the
// caller so VC7.1 knows the pushed element is not written by the call.
#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                         unsigned long __fill_len, bool __atend)
{
    const unsigned long __old_size = _M_finish - _M_start;
    const unsigned long __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = __len ? (_Tp*)malloc(__len * sizeof(_Tp)) : 0;
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = _M_start; __p != __position; ++__p, ++__new_finish)
        new (__new_finish) _Tp(*__p);
    if (__fill_len == 1) {
        new (__new_finish) _Tp(__x);
        ++__new_finish;
    } else {
        for (unsigned long __n = __fill_len; __n > 0; --__n, ++__new_finish)
            new (__new_finish) _Tp(__x);
    }
    if (!__atend)
        for (_Tp* __q = __position; __q != _M_finish; ++__q, ++__new_finish)
            new (__new_finish) _Tp(*__q);
    if (_M_start)
        free(_M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)
}

struct SSuitPartNode {                               // STLport _Rb_tree_node<pair<const EClothingSuitPart, long> >
    long           _M_color;
    SSuitPartNode* _M_parent;                        // +0x04
    SSuitPartNode* _M_left;                          // +0x08
    SSuitPartNode* _M_right;                         // +0x0c
    EClothingSuitPart first;                         // +0x10
    long           second;                           // +0x14
};
struct CSuitPartConstIterator {
    const SSuitPartNode* _M_node;
    CSuitPartConstIterator(const SSuitPartNode* node) : _M_node(node) {}
    bool operator!=(const CSuitPartConstIterator& other) const { return _M_node != other._M_node; }
    const SSuitPartNode* operator->() const { return _M_node; }
    static const SSuitPartNode* _M_increment(const SSuitPartNode* node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            const SSuitPartNode* y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
    CSuitPartConstIterator& operator++()
    {
        _M_node = _M_increment(_M_node);
        return *this;
    }
};
struct CSuitPartMap {                                // STLport map<EClothingSuitPart, long> (12 bytes in retail)
    SSuitPartNode* _M_header;
    unsigned long  _M_node_count;
    unsigned long  _M_key_compare;
    CSuitPartConstIterator begin() const { return CSuitPartConstIterator(_M_header->_M_left); }
    CSuitPartConstIterator end() const { return CSuitPartConstIterator(_M_header); }
};

class CHeroSuitDef {
public:
    unsigned char _pad00[0x28];
    CSuitPartMap  SuitParts;                         // +0x28
};

template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

class CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CHeroSuitDef>* out) const;   // 0x005935C4 (GetSubDef<CHeroSuitDef>)
};

class CThing {
public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
};

class CTCDHeroSuit {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void OnCreate();                         // slot 4
    CThing*                          ParentThing;    // +0x04 (CTCBase)
    unsigned char                    _pad08[4];      // +0x08 CTCBase flags
    CDefPointer<const CHeroSuitDef>  PDef;           // +0x0c
    std::vector<std::pair<EClothingSuitPart, bool> > SuitParts;   // +0x10
};

void CTCDHeroSuit::OnCreate()
{
    ParentThing->PDef->GetSubDef(&PDef);
    for (CSuitPartConstIterator it = PDef->SuitParts.begin(); it != PDef->SuitParts.end(); ++it)
        SuitParts.push_back(std::make_pair(it->first, false));
}