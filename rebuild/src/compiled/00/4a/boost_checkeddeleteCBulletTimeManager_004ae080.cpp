// boost::checked_delete<CBulletTimeManager>(CBulletTimeManager* x) -- the scoped_ptr<CBulletTimeManager> deleter: destroy the bullet-time manager (its map<void*, CBulletTimeInfo> cleared and its header node freed) and free it
// Flags: /Gr
// Leaf: checked_delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE080, 79 bytes (80 with padding), plain `ret` (__fastcall x in ecx).
// Identity: FableWin.pdb `??$checked_delete@VCBulletTimeManager@@@boost@@YIXPAVCBulletTimeManager@@@Z`; callers are
//   scoped_ptr<CBulletTimeManager>::~scoped_ptr 0x004AE160 (`mov ecx,[ecx] ; jmp`) and ::reset 0x004AE170, and CWorld
//   at its PBulletTimeManager member +0x68 (0x004A6CAD, and 0x004A73BA right after `new CBulletTimeManager` 0x004C60F0).
// Callees: _Rb_tree<void*, pair<void* const, CBulletTimeInfo>, _Select1st, less<void*> >::_M_erase 0x004AA650, free
//   0x00BFEA14 (the header node, STLport malloc allocator), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CBulletTimeManager: map<void*, CBulletTimeInfo> BulletTimeInfoMap +0 (12 bytes in retail STLport),
//   then Override, UseDefaultMultiplier, LastStrikeActionTick, World, PlayerManager; CBulletTimeInfo {Multiplier,
//   SoundPitch}. No vtable; the destructor is implicit.
// Shape: boost `delete x;` (null-tested) with the implicit ~CBulletTimeManager inlined: STLport ~_Rb_tree = clear()
//   then ~_Rb_tree_base's `_M_header.deallocate(header, 1)` (`if (p) free(p)`), as the CGameTimeManager twin 0x004ADD30.
// Label correction: catalogued as `LTextTreeWalkThrough_DestroyPtr`; the real routine is
//   boost::checked_delete<CBulletTimeManager>.
// ---- STLport 4.x _Rb_tree skeleton (malloc allocator) ----
#include <stdlib.h>                                   // free @ 0x00BFEA14

typedef bool _Rb_tree_Color_type;

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
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _Compare>
struct _Rb_tree : public _Rb_tree_base<_Value> {
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                       // +0x04
    _Compare     _M_key_compare;                      // +0x08
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                    // out-of-line COMDAT (address per instantiation)
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
    ~_Rb_tree() { clear(); }
};

template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { };

class CWorld;
class CPlayerManager;

template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };

class CBulletTimeManager {
public:
    struct CBulletTimeInfo {
        float Multiplier;                             // +0x00
        float SoundPitch;                             // +0x04
    };
    _Rb_tree<void*, pair<void* const, CBulletTimeInfo>, less<void*> > BulletTimeInfoMap;   // +0x00 map (_M_erase 0x004AA650)
    bool            Override;                         // +0x0C
    bool            UseDefaultMultiplier;             // +0x0D
    long            LastStrikeActionTick;             // +0x10
    CWorld*         World;                            // +0x14
    CPlayerManager* PlayerManager;                    // +0x18
};

namespace boost {
template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void)sizeof(type_must_be_complete);
    delete x;
}
}

template void boost::checked_delete<CBulletTimeManager>(CBulletTimeManager*);