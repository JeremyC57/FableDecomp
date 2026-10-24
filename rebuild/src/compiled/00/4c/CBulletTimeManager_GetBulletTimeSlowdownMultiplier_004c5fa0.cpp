// CBulletTimeManager::GetBulletTimeSlowdownMultiplier() const -- the default slowdown (the global long, as float) while UseDefaultMultiplier is set or no entry is registered, else the sum of every entry's Multiplier
// Leaf: ?GetBulletTimeSlowdownMultiplier@CBulletTimeManager@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5FA0, length 112 bytes (99 + int3 padding), plain `ret` (thiscall, float in st(0)).
// Identity: FableWin.pdb / Ego_r.pdb `?GetBulletTimeSlowdownMultiplier@CBulletTimeManager@@QBEMXZ`; the landed
// ConvertInterpolateToBulletTimeInterpolate 0x004C6010 calls it on its own `this` (named there "GetTimeScale"). The
// catalog "CAnimationList_GetTotalTime" is a propagated label.
// Globals: 0x01378D00 (long 4, the default slowdown: `fild`), 0x0122DEDC = 0.0f (g_fZero, landed name).
// Layout: Ego_r.pdb CBulletTimeManager with the 12-byte retail STLport map (as the landed PrepareForLoad 0x004C6080):
// BulletTimeInfoMap +0 (header pointer, count +4, compare), Override +0x0C, UseDefaultMultiplier +0x0D,
// LastStrikeActionTick +0x10, World +0x14, PlayerManager +0x18; CBulletTimeInfo {Multiplier +0, SoundPitch +4} at node +0x14.
// Shape: `UseDefaultMultiplier || empty()` returns the converted global; the running sum stays in st(0) from g_fZero;
// `++it` is STLport `_M_node = _Rb_global<bool>::_M_increment(_M_node)` inlined (one register here: no pressure).
extern long g_DefaultBulletTimeSlowdown;                // 0x01378D00 (4)

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                   // +0x10
};

template <class _Dummy> struct _Rb_global {
    static __forceinline _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
        return _M_node;
    }
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    __forceinline _Rb_tree_base_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    __forceinline void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
};

template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    __forceinline _Rb_tree_iterator(_Rb_tree_node_base* x) : _Rb_tree_base_iterator(x) {}
    __forceinline _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    __forceinline V* operator->() const { return &static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
    __forceinline bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
};

template <class K, class T> class map {
public:
    typedef pair<K const, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00 (_STLP_alloc_proxy::_M_data)
    unsigned int        _M_node_count;                  // +0x04
    unsigned int        _M_key_compare;                 // +0x08
    typedef _Rb_tree_iterator<const value_type> const_iterator;
    __forceinline const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    __forceinline const_iterator end() const { return const_iterator(_M_header); }
    __forceinline bool empty() const { return _M_node_count == 0; }
};
}

class CWorld;
class CPlayerManager;

class CBulletTimeManager {
public:
    class CBulletTimeInfo {                             // Ego_r.pdb, 8 bytes
    public:
        float Multiplier;                               // +0x0
        float SoundPitch;                               // +0x4
    };
    std::map<void*, CBulletTimeInfo> BulletTimeInfoMap; // +0x00
    bool            Override;                           // +0x0c
    bool            UseDefaultMultiplier;               // +0x0d
    long            LastStrikeActionTick;               // +0x10
    CWorld*         World;                              // +0x14
    CPlayerManager* PlayerManager;                      // +0x18
    float GetBulletTimeSlowdownMultiplier() const;
};

float CBulletTimeManager::GetBulletTimeSlowdownMultiplier() const
{
    if (UseDefaultMultiplier || BulletTimeInfoMap.empty())
        return (float)g_DefaultBulletTimeSlowdown;
    float total = 0.0f;
    for (std::map<void*, CBulletTimeInfo>::const_iterator it = BulletTimeInfoMap.begin(); it != BulletTimeInfoMap.end(); ++it)
        total += it->second.Multiplier;
    return total;
}