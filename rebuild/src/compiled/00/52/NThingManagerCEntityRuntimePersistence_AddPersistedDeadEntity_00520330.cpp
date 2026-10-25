// NThingManager::CEntityRuntimePersistence::AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC) -- record a dead entity by UID in the persisted-dead map (ignoring a zero UID): a CPersistedDeadEntity is filled in and inserted keyed by the UID
// Leaf: ?AddPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@QAEX_K_NK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00520330, length 95 bytes (the oracle row carries 1 byte of int3 padding).
// Callees: std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity>, ..>::insert_unique(const value_type&)
//   @ 0x0052A130 (STLport 4.6; the map's inline insert(const value_type&) forwards to it). FINDINGS chunk 474: the earlier
//   model called it a Dinkumware _Tree::insert; retail's STL is STLport, so the map is the real STLport 4.6 template here.
//   insert_unique returns pair<iterator,bool> through a hidden pointer that VC7.1 homes in the dead slot of the by-value
//   UID argument (`lea ecx,[esp+40h]`).
// Shape: `or ecx,edx` is the 64-bit zero test; the 24-byte value_type temporary is built at [esp+20h] (first = uid,
//   second = the 16-byte record copied as four words: the uid words straight from esi/edi, the bool as the whole padded
//   word `mov edx,[esp+1Ch]`, the CRC from ebx), so only the record's flag byte store survives.
// Names (Ego_r.pdb, name sets): class NThingManager::CEntityRuntimePersistence {CArray<CEntityScriptInfo>
//   LevelPersistentEntityScripts, GamePersistentEntityScripts; map<unsigned __int64, CPersistedDeadEntity>
//   PersistedDeadEntities; long SaveFormatVersion}; the nested class CPersistedDeadEntity {UID +0, GamePersistent +8,
//   SectionNameCRC +0xc}. The map sits at retail +0x18 (PDB +0x20: retail CArrays are 12-byte STLport vectors).
namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    unsigned int _M_node_count;                                                    // +0x04
    _Compare     _M_key_compare;                                                   // +0x08

    pair<iterator, bool> insert_unique(const value_type& __x);                      // 0x0052A130
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;

    _Rep_type _M_t;

    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
};

}   // namespace std

namespace NThingManager {

class CEntityRuntimePersistence {
public:
    class CPersistedDeadEntity {                          // 16 bytes
    public:
        unsigned __int64 UID;                             // +0x00
        bool             GamePersistent;                  // +0x08
        unsigned long    SectionNameCRC;                  // +0x0c
    };

    unsigned char _before_PersistedDeadEntities[0x18];    // LevelPersistentEntityScripts, GamePersistentEntityScripts (12-byte CArrays)
    std::map<unsigned __int64, CPersistedDeadEntity> PersistedDeadEntities;   // +0x18

    void AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC);
};

void CEntityRuntimePersistence::AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC)
{
    if (uid != 0) {
        CPersistedDeadEntity entity;
        entity.UID = uid;
        entity.GamePersistent = gamePersistent;
        entity.SectionNameCRC = sectionNameCRC;
        PersistedDeadEntities.insert(std::map<unsigned __int64, CPersistedDeadEntity>::value_type(uid, entity));
    }
}

}   // namespace NThingManager
