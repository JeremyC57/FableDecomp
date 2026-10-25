// CTCObjectAugmentations::SetupAugmentationSounds() -- rebuild the component's sound tables: clear them, and unless the owner is in limbo copy each def sound whose augmentation type is fitted into InitSounds / LoopingSounds as a resolved sound name
// Leaf: ?SetupAugmentationSounds@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766C00, length 368 bytes (thiscall, plain `ret`; 0x168 bytes + int3 pad).
// Identity: Ego_r.pdb ?SetupAugmentationSounds@CTCObjectAugmentations@@AAEXXZ (private); the catalog label
//   "CTCObjectAugmentations_UpdateParticleMorphs" (and the landed OnSetInLimbo 0x00768310 comment that repeats it) is
//   descriptive: the body fills the two sound maps from the def. Callers: OnSetInLimbo 0x00768310,
//   CopyAugmentationDataFromThing 0x00768210, AttachAugmentationToSlot / RemoveAugmentationFromSlot and the init paths.
// Callees: ClearAllSounds 0x00766550, HasCombinationOfAugmentations(EObjectAugmentationType) const 0x00766050,
//   CDefStringTable::GetString(CDefString) 0x009D49B0 on g_DefStringTable 0x013CA828 (CCharString returned through the
//   hidden pointer), map<EObjectAugmentationType, CObjectAugmentationSoundSet>::operator[] 0x0076AFF0,
//   CCharString::operator= 0x0099EFB0 (the Sound member, +4 of the sound set), ~CCharString 0x0099EAE0.
// Layout: CTCBase PThing +0x04 (CThing state byte +0x91: InLimbo 0x20); CTCObjectAugmentations PDef +0x0C, InitSounds
//   +0x20, LoopingSounds +0x2C; retail CObjectAugmentationsDef InitSounds +0x58, LoopingSounds +0x64 (STLport maps of
//   EObjectAugmentationType -> CDefString; Ego_r.pdb +0x64/+0x74 with its 16-byte containers).
// Shape: `ClearAllSounds(); if (!PThing->InLimbo) { for each (type, name) in PDef->InitSounds { type copied to a local;
//   if (HasCombinationOfAugmentations(type)) { CCharString sound = g_DefStringTable.GetString(name); InitSounds[type].Sound
//   = sound; } } the same for
//   LoopingSounds }` -- STLport tree iterator, `++` through the inlined static `_Rb_global<bool>::_M_increment`; PDef is
//   re-read for every end() compare.
enum EObjectAugmentationType { OBJECT_AUGMENTATION_TYPE_NONE };

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    ~CCharString();                                         // 0x0099EAE0
};

struct CObjectAugmentationSoundSet {                        // Ego_r.pdb, 12 bytes
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
    void _M_erase(_Link_type __x);
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

class CTCBase;

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char _bit1 : 1;
    unsigned char IncludeInThingScans : 1;                  // bit 2
    unsigned char Activated : 1;                            // bit 3
    unsigned char _bit4 : 1;
    unsigned char InLimbo : 1;                              // bit 5 (0x20)
};

struct CDefString { long Index; };

class CDefStringTable {
public:
    CCharString GetString(CDefString name);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828

class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long m_refCount;                                // +0x04
};

class CObjectAugmentationsDef : public CDefPointeeBase {
public:
    typedef std::map<EObjectAugmentationType, CDefString> CDefSoundMap;
    unsigned char _pad08[0x58 - 0x08];
    CDefSoundMap  InitSounds;                               // +0x58
    CDefSoundMap  LoopingSounds;                            // +0x64
};

template <class T> class CDefPointer {
public:
    T* Object;                                              // +0x00
    T* operator->() const { return Object; }
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* PThing;                                         // +0x04
    bool    PendingRemoval;                                 // +0x08
};

class CTCObjectAugmentations : public CTCBase {
public:
    typedef std::map<EObjectAugmentationType, CObjectAugmentationSoundSet> CSoundMap;
    CDefPointer<const CObjectAugmentationsDef> PDef;        // +0x0C
    bool          LoadedFromSaveGame;                       // +0x10
    void*         Augmentations[3];                         // +0x14 (vector<CDefPointer<const CThingBaseDef> >)
    CSoundMap     InitSounds;                               // +0x20
    CSoundMap     LoopingSounds;                            // +0x2C
    bool HasCombinationOfAugmentations(EObjectAugmentationType type) const;    // 0x00766050
private:
    void ClearAllSounds();                                  // 0x00766550
    void SetupAugmentationSounds();
};

void CTCObjectAugmentations::SetupAugmentationSounds()
{
    ClearAllSounds();
    if (!PThing->InLimbo) {
        CObjectAugmentationsDef::CDefSoundMap::const_iterator it;
        for (it = PDef->InitSounds.begin(); it != PDef->InitSounds.end(); ++it) {
            EObjectAugmentationType type = (*it).first;
            if (HasCombinationOfAugmentations(type)) {
                CCharString sound = g_DefStringTable.GetString((*it).second);
                InitSounds[type].Sound = sound;
            }
        }
        for (it = PDef->LoopingSounds.begin(); it != PDef->LoopingSounds.end(); ++it) {
            EObjectAugmentationType type = (*it).first;
            if (HasCombinationOfAugmentations(type)) {
                CCharString sound = g_DefStringTable.GetString((*it).second);
                LoopingSounds[type].Sound = sound;
            }
        }
    }
}