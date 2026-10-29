// CThing::~CThing() -- thing destructor: drop the thing count, purge this thing's pending component removals, delete every component (clearing its presence bit), empty the component map, unregister from the thing manager's class factory, then the member and base destructors
// Leaf: ??1CThing@@MAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8310, 412 bytes (the bitset range-error block at 0x004C849A ends it); Ego_r.pdb
//   `??1CThing@@MAE@XZ` (protected virtual); vtable store 0x01239ABC (CThing, as the landed CThing::CThing 0x004C9030).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 and ~CBaseIntelligentPointer 0x00A01C10 (the pending-removal
//   entries), free 0x00BFEA14 (STLport node allocator), CTCBase virtual slots 21 (+0x54, GetInterfaceType, as the landed
//   CThing::WorldUpdate 0x004C8050) and 0 (scalar deleting destructor, flag 1); CClass::Unregister(CClassFactory&) 0x00A37160
//   (Ego_r.pdb `?Unregister@CClass@@QAEXAAVCClassFactory@@@Z`; landed under the descriptive UnregisterFromFactory) on the thing
//   manager 0x013B8A1C (the factory the CThing constructor registers with); ~CCharString 0x0099EAE0 (x3);
//   CBaseObject::~CBaseObject 0x0099A430 (tail jump; Ego_r.pdb `??1CBaseObject@@MAE@XZ`); puts 0x00BFEAF2 / abort 0x00BFEAEC
//   (MSVCR71 import thunks: STLport's no-exceptions __stl_throw_out_of_range("bitset") from bitset::reset).
// Data: CThing::TCsToBeRemoved 0x013B8A24 (static list<pair<CIntelligentPointer<CThing>, CTCBase*> >, Ego_r.pdb names its
//   dynamic initializer/atexit destructor; the landed clear 0x004CC370 proves the element type); the thing counter 0x013B8A20
//   and the thing manager pointer 0x013B8A1C keep their landed descriptive names (no PDB data symbols).
// Inlined: the implicit CClass::~CClass (Ego_r.pdb has no out-of-line copy; a user-declared one stores the CClass vtable
//   before the tail jump, which retail does not), the members' destructors: CDefPointer<const
//   CThingBaseDef> PBaseDef (release through the pointee's slot 1 when the count drops to zero, then null), STLport list
//   (clear, relink, free the header node) and vector (free the block) for UpdateComponentsList and Components.
// Shape: the pending-removal sweep is `it = erase(it)` on a match else `++it`, end() re-read each pass; the component loop
//   re-reads Components each pass (bitset::reset with STLport's range check, the cold puts/abort block placed last;
//   delete; null the slot); clear() is STLport erase(begin(), end()) (`copy(end, finish, begin)`, count 0); the def
//   pointer is read once into a local before the release test.
// Layout: retail CThing (the landed constructor's view): UsingComponents bitset<274> +0x20, Components +0x44,
//   UpdateComponentsList +0x54, SerialisationSectionName +0x58, PBaseDef +0x70, ScriptName +0x74, ScriptData +0x78,
//   AddingTCsPermitted +0x93 bit 6. engine/CClass.h and engine/CBaseObject.h flatten the vptr into data, so the polymorphic
//   chain is modelled here (header debt; no CThing header exists); engine/CTCBase.h likewise, and the destructor needs the
//   component's virtual slots.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC (MSVCR71 import thunk)

namespace std {
inline void __cdecl __stl_throw_out_of_range(const char* __msg)   // STLport 4.6, exceptions off: _STLP_THROW_MSG
{
    puts(__msg);
    abort();
}

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x0: the header node
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy(__last, this->_M_finish, __first);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
    static iterator __copy(iterator __first, iterator __last, iterator __result)
    {
        for (int __n = int(__last - __first); __n > 0; --__n) {
            *__result = *__first;
            ++__first;
            ++__result;
        }
        return __result;
    }
};

template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
    _WordT& _M_getword(unsigned int __pos) { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    typedef _Base_bitset<(_Nb + 31) / 32> _Base;
    bitset<_Nb>& _Unchecked_reset(unsigned int __pos)
    {
        this->_M_getword(__pos) &= ~_Base::_S_maskbit(__pos);
        return *this;
    }
    bitset<_Nb>& reset(unsigned int __pos)
    {
        if (__pos >= _Nb)
            __stl_throw_out_of_range("bitset");
        return _Unchecked_reset(__pos);
    }
};
}   // namespace std

class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObjectPointer* PData;                              // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

class CTCBase { public:
    virtual ~CTCBase();                                     // slot 0 (scalar deleting destructor)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20();
    virtual long GetInterfaceType() const;                  // slot 21 (+0x54)
};
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
template <class K, class V> class CKeyPairCompareLess { };
template <class T> class CArray : public std::vector<T> { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                // +0x00
    TCompare           Compare;                             // +0x0C
    bool               Dirty;                               // +0x0D
};

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

class CDefPointeeBase { public:                             // counted def object: vptr, reference count +4
    virtual void CUnprovenSlot0() const;
    virtual void CUnprovenSlot1() const;                    // slot 1: called when the last reference goes
    mutable long RefCount;                                  // +0x04
};
class CThingBaseDef;
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00 (Ego_r.pdb: CDefPointeeBase*)
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->CUnprovenSlot1();
            Object = 0;
        }
    }
};

class CBaseObject { public:
    CBaseObjectPointer* IntelligentPointer;                 // +0x04
protected:
    virtual ~CBaseObject();                                 // 0x0099A430
};
class CClassFactory;
class CClass : public CBaseObject { public:
    std::_List_node_base* ClassListPos;                     // +0x08
    std::_List_node_base* TypeListPos;                      // +0x0C
    unsigned char         Type;                             // +0x10
    void Unregister(CClassFactory& factory);                // 0x00A37160
};

class CThingManager;
extern CThingManager* g_pThingManager;                      // 0x013B8A1C
extern long g_thingCount;                                   // 0x013B8A20

class CTCDriverBase; class CTCPhysicsBase; class CTCGraphicAppearance;
class CThing : public CClass {                              // vtable 0x01239ABC
public:
    unsigned __int64       UniqueID;                        // +0x18
    std::bitset<274>       UsingComponents;                 // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    std::list<CTCBase*>    UpdateComponentsList;            // +0x54
    CCharString            SerialisationSectionName;        // +0x58
    CTCDriverBase*         PTCDriver;                       // +0x5c
    CTCPhysicsBase*        PhysicsTC;                       // +0x60
    CTCGraphicAppearance*  PTCGraphicAppearance;            // +0x64
    long                   WorldFrameCreatedAt;             // +0x68
    unsigned long          ThingTypeFlags;                  // +0x6c
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
    CCharString            ScriptName;                      // +0x74
    CCharString            ScriptData;                      // +0x78
    CThing*                RenderListPrev;                  // +0x7c
    CThing*                RenderListNext;                  // +0x80
    CThing*                UpdateListPrev;                  // +0x84
    CThing*                UpdateListNext;                  // +0x88
    unsigned short         DefGlobalIndex;                  // +0x8c
    short                  CurrentMapNumber;                // +0x8e
    signed char            PlayerNumber;                    // +0x90
    unsigned char          Dead : 1;                        // +0x91
    unsigned char          CreatedByOwnerThingCreation : 1;
    unsigned char          IncludeInThingScans : 1;
    unsigned char          Activated : 1;
    unsigned char          GloballyLoaded : 1;
    unsigned char          InLimbo : 1;
    unsigned char          KillLocked : 1;
    unsigned char          CreatedBySerialisation : 1;
    unsigned char          DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char          Drawable : 1;
    unsigned char          Dying : 1;
    unsigned char          ForceFrameUpdate : 1;
    unsigned char          LevelPersistent : 1;
    unsigned char          GameStatePersisted : 1;
    unsigned char          PersistOnUnloadWhenInLimbo : 1;
    unsigned char          PausedDuringCutscenes : 1;
    unsigned char          NotPausedDuringCutscenes : 1;        // +0x93
    unsigned char          DeathPersistent : 1;
    unsigned char          KillOnLevelUnload : 1;
    unsigned char          ActivatedScript : 1;
    unsigned char          LevelIsUnloading : 1;
    unsigned char          IsAffectedByBulletTimeOverride : 1;
    unsigned char          AddingTCsPermitted : 1;
    unsigned char          InitialisationStage : 3;             // +0x94
    static std::list<std::pair<CIntelligentPointer<CThing>, CTCBase*> > TCsToBeRemoved;   // 0x013B8A24
protected:
    virtual ~CThing();
};

CThing::~CThing()
{
    --g_thingCount;
    AddingTCsPermitted = false;
    std::list<std::pair<CIntelligentPointer<CThing>, CTCBase*> >::iterator it = TCsToBeRemoved.begin();
    while (it != TCsToBeRemoved.end()) {
        if (it->first.GetPItem() == this)
            it = TCsToBeRemoved.erase(it);
        else
            ++it;
    }
    for (unsigned int i = 0; i < Components.Data.size(); ++i) {
        UsingComponents.reset(Components.Data[i].second->GetInterfaceType());
        delete Components.Data[i].second;
        Components.Data[i].second = 0;
    }
    AddingTCsPermitted = true;
    Components.Data.clear();
    Unregister(*(CClassFactory*)g_pThingManager);
}