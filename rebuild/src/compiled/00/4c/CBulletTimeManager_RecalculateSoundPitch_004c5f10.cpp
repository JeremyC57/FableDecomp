// CBulletTimeManager::RecalculateSoundPitch() -- with a sound system present, hand it the lowest SoundPitch over every registered bullet-time entry (1.0 when none is lower)
// Leaf: ?RecalculateSoundPitch@CBulletTimeManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5F10, length 144 bytes (138 + int3 padding), plain `ret` (thiscall, no arguments).
// Identity: FableWin.pdb / Ego_r.pdb `?RecalculateSoundPitch@CBulletTimeManager@@AAEXXZ` (private); the body walks the
// BulletTimeInfoMap (map<void*, CBulletTimeInfo>, CBulletTimeInfo = {float Multiplier, float SoundPitch} -> SoundPitch at
// node +0x18) and sits between the landed CBulletTimeManager members 0x004C5EA0 and 0x004C6010. The catalog
// "BulletTimeController_UpdateMinMultiplier" is behavioural.
// Callees: the sound system's virtual slot 45 (+0xB4) with (0, pitch); global 0x013B8394 = g_pSoundSystem (landed name).
// Constant 0x3F800000 = 1.0f.
// Layout: Ego_r.pdb CBulletTimeManager with the 12-byte retail STLport map (as the landed PrepareForLoad 0x004C6080):
// BulletTimeInfoMap +0 (header pointer, count, compare), Override +0x0C, UseDefaultMultiplier +0x0D,
// LastStrikeActionTick +0x10, World +0x14, PlayerManager +0x18.
// Shape: the global is read once into ecx (no call in the loop); the running minimum lives in the push-ecx frame slot and
// is updated by an integer copy (`if (p < pitch) pitch = p`). `++it` is the STLport 4.x form `_M_node =
// _Rb_global<bool>::_M_increment(_M_node)` (static, by value, inlined): the walk runs in eax and is copied back to esi
// after the end test; an in-place member `_M_increment` (no copy) gives 127 bytes against retail 138.
class CSoundSystem {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void SetPitch(long category, float pitch);  // slot 45 (+0xB4); name by effect
};
extern CSoundSystem* g_pSoundSystem;                    // 0x013B8394

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
    __forceinline iterator begin() { return iterator(_M_header->_M_left); }
    __forceinline iterator end() { return iterator(_M_header); }
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
private:
    void RecalculateSoundPitch();
};

void CBulletTimeManager::RecalculateSoundPitch()
{
    if (g_pSoundSystem) {
        float pitch = 1.0f;
        for (std::map<void*, CBulletTimeInfo>::iterator it = BulletTimeInfoMap.begin(); it != BulletTimeInfoMap.end(); ++it)
            if (it->second.SoundPitch < pitch)
                pitch = it->second.SoundPitch;
        g_pSoundSystem->SetPitch(0, pitch);
    }
}