// CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierListWithCoversBodyAreaFlags(long flags, EHeroAttachableAppearanceModifierType type) const -- the def indices of the worn modifiers of one type whose appearance-modifier def covers any of the given body-area flags
// Leaf: ?GetAppearanceModifierListWithCoversBodyAreaFlags@CTCHeroAttachableAppearanceModifiers@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00706B50, 315 bytes, __fastcall, hidden list<long> result first, `ret 0Ch`.
// PDB: ?GetAppearanceModifierListWithCoversBodyAreaFlags@CTCHeroAttachableAppearanceModifiers@@QBE?AV?$list@JV?$allocator@J
//   @std@@@std@@JW4EHeroAtta... (catalog "FilterAbilityClasses_ByFlags" was descriptive). Callers: CTCInventoryClothing
//   0x005B3F61 and CCreatureActionWearGameObject::PerformAction 0x008461FC (type 2).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef> 0x0057FF92,
//   the defs' slot 1 (Destroy, the CDefPointer release), list<long>::_M_insert_dispatch(iterator, const_iterator,
//   const_iterator, const __false_type&) 0x005794C3 (the copy constructor of the returned list).
// Layout: as the landed GetAppearanceModifierList 0x00706800: AppearanceModifiers +0x30 (a vector of 12-byte STLport
//   vectors of 8-byte CModifierEntry); the modifier def's covered body-area flags at +0x40.
// Shape: a local STLport list filled by push_back, returned by copy (list(const list&) -> insert(begin(), first, last),
//   its __false_type homed in the dead `type` slot), then destroyed (clear + free the head).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc> struct _STLP_alloc_proxy {
    _Tp _M_data;
    _STLP_alloc_proxy(const _Alloc&, _Tp __p) : _M_data(__p) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = (_Node*)malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        free(_M_node._M_data);
    }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            free(__tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    list() : _List_base<_Tp, _Alloc>(_Alloc()) {}
    list(const list& __x) : _List_base<_Tp, _Alloc>(_Alloc()) { insert(begin(), __x.begin(), __x.end()); }
    iterator begin() { return iterator((_Node*)this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator begin() const { return const_iterator((_Node*)this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    void _M_insert_dispatch(iterator __pos, const_iterator __first, const_iterator __last, const __false_type&);   // 0x005794C3
    void insert(iterator __pos, const_iterator __first, const_iterator __last) { _M_insert_dispatch(__pos, __first, __last, __false_type()); }
};
}

class CDefBase {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +4
};
class CAppearanceModifierDef : public CDefBase {
public:
    unsigned char _pad08[0x40 - 8];
    unsigned long CoversBodyAreaFlags;                  // +0x40
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CThingObjectDef : public CDefBase {
public:
    bool GetSubDef(CDefPointer<CAppearanceModifierDef>* out);   // 0x0057FF92
};
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<CThingObjectDef>* out);   // 0x0042B040
};
CThingObjectDef* FindThingObjectDef(long index);                       // stand-in: the def lookup (not graded here)
CAppearanceModifierDef* FindAppearanceModifierDef(const CThingObjectDef* def);   // stand-in: the sub def lookup
#pragma auto_inline(off)
// Same-TU bodies of the two def getters (template instantiations retail holds in this TU): they write only their out
// pointer, which lets VC7.1 keep the sub def in a register across the push_back malloc.
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingObjectDef>* out)
{
    CThingObjectDef* def = FindThingObjectDef(index);
    out->Object = def;
    return def != 0;
}
__declspec(noinline) bool CThingObjectDef::GetSubDef(CDefPointer<CAppearanceModifierDef>* out)
{
    CAppearanceModifierDef* def = FindAppearanceModifierDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

struct CModifierEntry {                                 // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                      // +0x00
    bool Rendered;                                      // +0x04
};
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};

class CTCHeroAttachableAppearanceModifiers {
public:
    virtual void v00();
    void* ParentThing;                                  // +0x04 (CTCBase)
    unsigned char _pad08[0x30 - 0x08];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    std::list<long> GetAppearanceModifierListWithCoversBodyAreaFlags(long flags, long type) const;
};

std::list<long> CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierListWithCoversBodyAreaFlags(long flags, long type) const
{
    const CRetailVector<CModifierEntry>& modifiers = AppearanceModifiers[type];
    std::list<long> result;
    for (const CModifierEntry* it = modifiers.begin(); it != modifiers.end(); ++it) {
        CDefPointer<CThingObjectDef> def;
        CDefPointer<CAppearanceModifierDef> modifierDef;
        if (GetDefinitionManager()->GetDef(it->DefIndex, &def) && def->GetSubDef(&modifierDef)
            && (modifierDef->CoversBodyAreaFlags & flags))
            result.push_back(it->DefIndex);
    }
    return result;
}