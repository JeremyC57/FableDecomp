// CTCCombat::~CTCCombat() -- combat component destructor
// Leaf: ??1CTCCombat@@UAE@XZ
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void operator delete(void* block);                          // 0x00BFE9BC

class CCPPointerInfo { public:                              // the shared count block
    long RefCount;                                          // +0x0
    void (__fastcall *DeleteFunc)(void*);                   // +0x4
    void* Data;                                             // +0x8
};
template <class T> class CCountedPointer { public:         // 8 bytes: object, shared count block
    T* Data;                                                // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
enum ECombatAbilityDataType { };
class CCombatAbilityData;
class CCombatAbility { public:                              // 8 bytes (Ego_r.pdb), trivially destructible
    long RepeatTimer;                                       // +0x0
    long ActiveTimer;                                       // +0x4
};

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef unsigned int size_type;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
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
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
public:
    ~_Rb_tree() { clear(); }
private:
    void _M_erase(_Link_type __x);                          // 0x00738EB0
public:
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
class multimap { public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
    void clear() { _M_t.clear(); }
};
} // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CBaseObjectPointer;
class CBaseObject;
class CThing { public:                                      // retail flag byte +0x91 (Dead bit 0)
    unsigned char _pad_0x00[0x91];
    bool          Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    void Kill(bool immediately);                            // 0x004C9B80
};

#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // vtable 0x0129C95C, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return GetPointer(); }
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CResource : public CIVCountedPointeeBase { public:
};
class CDefPointeeBase : public CResource { public:
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CDefPointer() { Release(); }
};

class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};

class CCombatManager;
class CThingCreatureBase;
class CCombatTypeDef;
class CMeleeCombatAbilityDef;
enum EUnsheatheWeaponType { };
class CTCCombat : public CTCBase { public:                  // vtable 0x0126724C
    virtual ~CTCCombat();
    long                ComboChainPos;                      // +0x0C
    long                ComboChainID;                       // +0x10
    long                ComboChainGracePeriodCountdown;     // +0x14
    long                HandednessGracePeriodCountdown;     // +0x18
    long                UnarmedCombatIdleCountdown;         // +0x1C
    CCombatManager*     CombatManager;                      // +0x20
    CThingCreatureBase* ParentCreature;                     // +0x24
    CDefPointer<const CCombatTypeDef>                        PCombatTypeDef;         // +0x28
    std::vector<CDefPointer<const CMeleeCombatAbilityDef> >  AbilityList;            // +0x2C
    std::vector<CDefPointer<const CMeleeCombatAbilityDef> >  UsableAbilityList;      // +0x38
    long                                                     CurrentAbilityIndex;    // +0x44
    CDefPointer<const CMeleeCombatAbilityDef>                PBlockCounterAbility;   // +0x48
    CDefPointer<const CMeleeCombatAbilityDef>                PFlourish360Ability;    // +0x4C
    long                                                     BlockCounterTimer;      // +0x50
    std::vector<CIntelligentPointer<const CThing> >          AlwaysBlockThings;      // +0x54
    bool                PerformingAction;                   // +0x60
    bool                PerformingStrikeAction;             // +0x61
    bool                StartingToBlock;                    // +0x62
    bool                Blocking;                           // +0x63
    long                BlockHoldTimer;                     // +0x64
    long                BlockingNumFrames;                  // +0x68
    EUnsheatheWeaponType UnsheathingWeaponType;             // +0x6C
    std::multimap<ECombatAbilityDataType, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> > CombatAbilities;   // +0x70
    long                ShotOverrideType;                   // +0x7C
    bool                ReadyForMelee;                      // +0x80
    long                CurFlourishTimer;                   // +0x84
    std::vector<CIntelligentPointer<CThing> >                FlourishParticles;      // +0x88
    bool                IsInMiddleOfFlourishAction;         // +0x94
};

CTCCombat::~CTCCombat()
{
    CombatAbilities.clear();
    for (long i = 0; i < (long)FlourishParticles.size(); ++i) {
        if (IS_THING_ALIVE(FlourishParticles[i]))
            FlourishParticles[i]->Kill(true);
    }
}
