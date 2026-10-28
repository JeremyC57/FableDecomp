// CTCObjectAugmentations::ClearAllSounds() -- stop every looping augmentation sound that was played (through the owner's sound player), then empty the looping and initial sound maps
// Leaf: ?ClearAllSounds@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766550, length 272 bytes (thiscall, plain `ret`; 0x108 bytes + int3 pad).
// Identity: Ego_r.pdb ?ClearAllSounds@CTCObjectAugmentations@@AAEXXZ (private); the catalog label
//   "CTCObjectAugmentations_UpdateTreeStructure" is descriptive. Its only caller is SetupAugmentationSounds 0x00766C00,
//   whose first act is to clear the sound maps it then refills.
// Callees: CInterfaceMap::LowerBound 0x0040F020 (inline GetInterface, 0x10 = CTCSoundPlayer: bitset word +0x20 bit 16),
//   CTCSoundPlayer::StopAttachedSound(unsigned long) 0x0066ADD0 (catalog "CGameScriptThing::SetToKillOnLevelUnload" is a
//   propagated twin), _Rb_tree<EObjectAugmentationType, pair<const ..., CObjectAugmentationSoundSet> >::_M_erase
//   0x00769650 (both maps share the instantiation; catalog "LTextTreeWalkThrough::BuildTreeArray" is propagated).
// Layout: CTCBase PThing +0x04; CTCObjectAugmentations InitSounds +0x20, LoopingSounds +0x2C (STLport maps, 12 bytes:
//   header pointer, node count, comparator); the sound-set node value at +0x14 (Played +0x14, Sound +0x18, Handle +0x1C).
// Shape: `for (it = LoopingSounds.begin(); it != end(); ++it) if (it->second.Played) { CTCSoundPlayer* player; if
//   (PThing->GetInterface(&player, 0x10)) player->StopSample(it->second.Handle); } LoopingSounds.clear();
//   InitSounds.clear();` -- STLport 4.x tree iterator whose `++` is `_M_node = _Rb_global<bool>::_M_increment(_M_node)`
//   (the static template body inlined by /Ob2: the walk runs in eax and is copied back to esi), inline `_Rb_tree::clear()` (root
//   erase, then leftmost/root/rightmost/count reset); `this` is spilled across the inlined GetInterface.
class CTCBase;

class CInterfaceMap {
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CTCSoundPlayer {                                      // interface 0x10
public:
public:
    void StopAttachedSound(unsigned long);   // 0x0066ADD0
};

enum EObjectAugmentationType { OBJECT_AUGMENTATION_TYPE_NONE };

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    ~CCharString();                                         // 0x0099EAE0
};

class CObjectAugmentationSoundSet { public:                        // Ego_r.pdb, 12 bytes
    bool          Played;                                   // +0x00
    CCharString   Sound;                                    // +0x04
    unsigned long Handle;                                   // +0x08
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                   // +0x00
    _Base_ptr   _M_parent;                                  // +0x04
    _Base_ptr   _M_left;                                    // +0x08
    _Base_ptr   _M_right;                                   // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr _M_node);
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    _STLP_alloc_proxy<_Link_type, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare     _M_key_compare;                                      // +0x08

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);
public:
    iterator begin() { return iterator((_Link_type)_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator begin() const { return const_iterator((_Link_type)_M_leftmost()); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
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

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef _Tp data_type;
    typedef _Tp mapped_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
    void clear() { _M_t.clear(); }
    _Tp& operator[](const key_type& __k);
};

}

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* PThing;                                         // +0x04
    bool    PendingRemoval;                                 // +0x08
};

class CTCObjectAugmentations : public CTCBase {
public:
    typedef std::map<EObjectAugmentationType, CObjectAugmentationSoundSet> CSoundMap;
    void*         PDef;                                     // +0x0C (CDefPointer<const CObjectAugmentationsDef>)
    bool          LoadedFromSaveGame;                       // +0x10
    void*         Augmentations[3];                         // +0x14 (vector<CDefPointer<const CThingBaseDef> >)
    CSoundMap     InitSounds;                               // +0x20
    CSoundMap     LoopingSounds;                            // +0x2C
private:
    void ClearAllSounds();
};

void CTCObjectAugmentations::ClearAllSounds()
{
    for (CSoundMap::iterator it = LoopingSounds.begin(); it != LoopingSounds.end(); ++it) {
        if ((*it).second.Played) {
            CTCSoundPlayer* player;
            if (PThing->GetInterface(&player, 0x10))
                player->StopAttachedSound((*it).second.Handle);
        }
    }
    LoopingSounds.clear();
    InitSounds.clear();
}