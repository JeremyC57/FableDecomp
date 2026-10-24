// boost::checked_delete<CGameTimeManager>(CGameTimeManager* x) -- the scoped_ptr<CGameTimeManager> deleter: destroy the game-time manager (its set<long> StopTimeIndexes cleared and its header node freed) and free it
// Flags: /Gr
// Leaf: checked_delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ADD30, 84 bytes (96 with padding), plain `ret` (__fastcall x in ecx).
// Identity: FableWin.pdb `??$checked_delete@VCGameTimeManager@@@boost@@YIXPAVCGameTimeManager@@@Z`; callers are
//   scoped_ptr<CGameTimeManager>::~scoped_ptr 0x004ADEA0 (`mov ecx,[ecx] ; jmp`) and ::reset 0x004ADEB0, and CWorld
//   at its PGameTimeManager member +0x1C (0x004A6DF3, and 0x004A6F4B right after `new CGameTimeManager` 0x006BBC30).
// Callees: _Rb_tree<long, long, _Identity<long>, less<long> >::_M_erase 0x004A9CB0, free 0x00BFEA14 (the header node,
//   STLport malloc allocator), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CGameTimeManager: World +0, GameComponent +4, Time +8, PrevTime +0xC, TimeStep +0x10, DayCount
//   +0x14, UsingVillageTime +0x18, VillageTime +0x1C, VillageDay +0x20, FastForwardingToTime +0x21,
//   TimeFastForwardingTo +0x24, SecondsLeftToFastForwardTime +0x28, set<long> StopTimeIndexes +0x2C (12 bytes in
//   retail STLport), NextStopTimeIndex +0x38. No vtable; the destructor is implicit.
// Shape: boost `delete x;` (null-tested) with the implicit ~CGameTimeManager inlined: STLport ~_Rb_tree = clear()
//   (`if (_M_node_count != 0) { _M_erase(_M_root()); leftmost = header; root = 0; rightmost = header; count = 0; }`)
//   then ~_Rb_tree_base's `_M_header.deallocate(header, 1)` (`if (p) free(p)`).
// Label correction: catalogued as `GFCheckTreeNodes`; the real routine is boost::checked_delete<CGameTimeManager>.
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
class CMainGameComponent;

class CGameTimeManager {
public:
    CWorld*             World;                        // +0x00
    CMainGameComponent* GameComponent;                // +0x04
    float               Time;                         // +0x08
    float               PrevTime;                     // +0x0C
    float               TimeStep;                     // +0x10
    long                DayCount;                     // +0x14
    bool                UsingVillageTime;             // +0x18
    float               VillageTime;                  // +0x1C
    bool                VillageDay;                   // +0x20
    bool                FastForwardingToTime;         // +0x21
    float               TimeFastForwardingTo;         // +0x24
    float               SecondsLeftToFastForwardTime; // +0x28
    _Rb_tree<long, long, less<long> > StopTimeIndexes;   // +0x2C set<long> (_M_erase 0x004A9CB0)
    long                NextStopTimeIndex;            // +0x38
};

namespace boost {
template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void)sizeof(type_must_be_complete);
    delete x;
}
}

template void boost::checked_delete<CGameTimeManager>(CGameTimeManager*);