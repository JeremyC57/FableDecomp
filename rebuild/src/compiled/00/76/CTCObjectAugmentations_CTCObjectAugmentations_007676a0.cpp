// CTCObjectAugmentations::~CTCObjectAugmentations() -- kill the running particle effects, then tear down the members: trail pointer, running-effect pointers, both particle tables, both sound maps, the slot defs and the def pointer, then the CTCBase part
// Leaf: ??1CTCObjectAugmentations@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007676A0, length 320 bytes (thiscall, plain `ret`; 0x135 bytes + int3 pad).
// Identity: Ego_r.pdb ??1CTCObjectAugmentations@@UAE@XZ (the vptr store of vtable 0x0126A3A4 first); its only caller is
//   the landed scalar deleting destructor 0x0076C790. The catalog label "CTCObjectAugmentations_Destructor" is right.
// Callees: ClearAllParticleEffects 0x007674A0 (the body), ~CBaseIntelligentPointer 0x00A01C10 (ProjectileTrailEffect: the
//   implicit CIntelligentPointer<CThing> destructor only calls its base), STLport __destroy_aux(first, last, const
//   __false_type&) for AttackParticleEffects 0x0076BDD0, ParticleEffects 0x0076BD70 and Augmentations 0x005B26E9 (the
//   CurrentRunningAugEffect walk is inlined: virtual slot-0 destructor calls with 0), the shared
//   _Rb_tree<EObjectAugmentationType, ...>::_M_erase 0x00769650 (both sound maps), free 0x00BFEA14 (each vector block and
//   each map header), CTCBase::~CTCBase 0x00686830.
// Layout: as the landed ctor 0x007673C0: PDef +0x0C, Augmentations +0x14, InitSounds +0x20, LoopingSounds +0x2C,
//   ParticleEffects +0x38, AttackParticleEffects +0x44, CurrentRunningAugEffect +0x50, ProjectileTrailEffect +0x65 (the
//   intelligent-pointer classes are pack(1)); AttackParticleAndEndPoints is 0x18 with ParticleEffect at +0x10 (the
//   names of its first four words are descriptive).
// Shape: `~CTCObjectAugmentations() { ClearAllParticleEffects(); }` -- everything else is the compiler's member
//   teardown through the STLport 4.x destructors (`~vector` = `_Destroy(start, finish)` then `~_Vector_base`'s
//   deallocate; `~_Rb_tree` = inline clear(), then `~_Rb_tree_base` frees the header) and CDefPointer's inline release
//   (`if (Object) { Object->ReleaseReference(); Object = 0; }`, the pointee's inline `if (--m_refCount == 0)
//   Destroy();` keeping the pointer in ecx; spelled with `--Object->m_refCount` the member is re-read after the decrement).
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

enum EObjectAugmentationType { OBJECT_AUGMENTATION_TYPE_NONE };

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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

template <class _Tp> class _Node_alloc {
public:
    _Node_alloc() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, _Node_alloc<_Rb_tree_node<_Tp> > > _M_header;   // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    unsigned int _M_node_count;                                       // +0x04
    _Compare     _M_key_compare;                                      // +0x08

    protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
private:
    void _M_erase(_Link_type __x);
public:
    iterator begin() { return iterator((_Link_type)_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator begin() const { return const_iterator((_Link_type)_M_leftmost()); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    ~_Rb_tree() { clear(); }
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


template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct __false_type { __false_type() { } };

template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}

template <class _Tp> class _Vector_alloc {
public:
    _Vector_alloc() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Vector_alloc<_Tp> > _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}

class CThing;

class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long m_refCount;                                // +0x04
    void ReleaseReference() const
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

class CThingBaseDef;
class CObjectAugmentationsDef;

template <class T> class CDefPointer {
public:
    const CDefPointeeBase* Object;                          // +0x00
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseReference();
            Object = 0;
        }
    }
};

#pragma pack(push, 1)                                       // ProjectileTrailEffect sits at the odd offset +0x65
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* PItem;                                          // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
};
#pragma pack(pop)

struct ParticleAndEndPoints {                               // Ego_r.pdb, 20 bytes
    CCharString ParticleName;                               // +0x00
    CCharString HelperName;                                 // +0x04
    long        StartPoint;                                 // +0x08
    long        EndPoint;                                   // +0x0C
    long        PeriodicFrames;                             // +0x10
};

struct AttackParticleAndEndPoints {                         // Ego_r.pdb, 24 bytes (first four names descriptive)
    CCharString                 ParticleName;               // +0x00
    CCharString                 HelperName;                 // +0x04
    long                        StartPoint;                 // +0x08
    long                        EndPoint;                   // +0x0C
    CIntelligentPointer<CThing> ParticleEffect;             // +0x10
};

class CTCBase {
public:
    virtual ~CTCBase();                                     // 0x00686830
    CThing* PThing;                                         // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCObjectAugmentations : public CTCBase {             // vtable 0x0126A3A4
public:
    typedef std::map<EObjectAugmentationType, CObjectAugmentationSoundSet> CSoundMap;
    CDefPointer<const CObjectAugmentationsDef>               PDef;                          // +0x0C
    bool                                                     LoadedFromSaveGame;            // +0x10
    std::vector<CDefPointer<const CThingBaseDef> >           Augmentations;                 // +0x14
    CSoundMap                                                InitSounds;                    // +0x20
    CSoundMap                                                LoopingSounds;                 // +0x2C
    std::vector<std::vector<ParticleAndEndPoints> >          ParticleEffects;               // +0x38
    std::vector<std::vector<AttackParticleAndEndPoints> >    AttackParticleEffects;         // +0x44
    std::vector<CIntelligentPointer<CThing> >                CurrentRunningAugEffect;       // +0x50
    long                                                     CurrentRunningAugEffectIndex;  // +0x5C
    long                                                     CurPeriod;                     // +0x60
    bool                                                     InStrikeAction;                // +0x64
    CIntelligentPointer<CThing>                              ProjectileTrailEffect;         // +0x65
    virtual ~CTCObjectAugmentations();
private:
    void ClearAllParticleEffects();                         // 0x007674A0
};

CTCObjectAugmentations::~CTCObjectAugmentations()
{
    ClearAllParticleEffects();
}