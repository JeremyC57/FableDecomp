// CThingPlayerCreature::~CThingPlayerCreature() -- player creature dtor: if it is in melee, deregister it from the combat manager as a destroyed combatant; then the member dtors (movement def and hero combat def pointers, the relative movement acceleration map) and ~CThingCreatureBase
// Leaf: ??1CThingPlayerCreature@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AA230, 161 bytes, tail jump to the base destructor.
// Identity: the vtable store 0x012457FC carries RTTI `.?AVCThingPlayerCreature@@` (rtti_bases: CThingPlayerCreature :
//   CThingCreatureBase); Ego_r.pdb has `??1CThingPlayerCreature@@UAE@XZ`. The catalog label is right.
// Callees: CThingCreatureBase::IsInMelee 0x00661E80 (Ego_r `?IsInMelee@CThingCreatureBase@@QBE_NXZ`, bound so: the body returns
//   MeleeAttacker || MeleeAttacked, bits 2/3 of +0x15C), CThingCreatureBase::GetCombatManager 0x00661ED0 (Ego_r
//   `?GetCombatManager@CThingCreatureBase@@QAEAAVCCombatManager@@XZ`: the non-const overload, its body goes through the non-const
//   world getter 0x004C7AA0 where the const one at 0x00661EC0 uses 0x004C7AB0; the bound spellings return a pointer),
//   CCombatManager::DeregisterDestroyedCombatant 0x006ED9E0 (Ego_r `QAEXAAVCThingCreatureBase@@@Z`), the map's out-of-line
//   std::_Rb_tree<EGameAction, pair<const EGameAction, pair<C3DVector, long> >, ...>::_M_erase 0x00527910 (private; frees each
//   node with free 0x00BFEA14, no element dtors), free 0x00BFEA14 (the map header), CThingCreatureBase::~CThingCreatureBase
//   0x006661B0 (tail jump; its first store is the CThingCreatureBase vtable 0x0125AC0C). Virtual call: each def's slot 1 on the
//   last reference.
// Layout: Ego_r.pdb CThingPlayerCreature (0x230) with retail STLport sizes; the base CThingCreatureBase is 0x160 in retail
//   (engine/CThingCreatureBase.h agrees, but flattens the vptr, so the base is modelled here: header debt).
//   RelativeMovementAccelerationComponents +0x1A8 (map 0xC), RelativeMovementAcceleration +0x1B4, ..., PHeroCombatDef +0x1E8,
//   PPlayerMovementDef +0x1EC, ImpulseVelocity +0x1F0, LeanAngle +0x1FC, MovementBandType +0x200.
// Shape: user body `if (IsInMelee()) GetCombatManager().DeregisterDestroyedCombatant(*this);`, then the implicit member dtors in
//   reverse order: the two CDefPointer releases (pointer read once into a local, `dec [ecx+4] ; jne`), the map's ~_Rb_tree
//   (clear: out-of-line _M_erase when node_count != 0) and ~_Rb_tree_base (header free).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x0
    _Rb_tree_node_base* _M_parent;                          // +0x4
    _Rb_tree_node_base* _M_left;                            // +0x8
    _Rb_tree_node_base* _M_right;                           // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, allocator<_Node> > _M_header;   // +0x0
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
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
private:
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                         // +0x0 (0xC bytes)
    void clear() { _M_t.clear(); }
};
} // namespace std

class C3DVector { public:
    float X;                                                // +0x0
    float Y;                                                // +0x4
    float Z;                                                // +0x8
};
enum EGameAction { };                                       // values not needed here
enum EControlledMovementType { };                           // values not needed here
enum EMovementBandType { };                                 // values not needed here

class CDefPointeeBase { public:                             // counted def object: vptr, reference count +4
    virtual void CUnprovenSlot0() const;
    virtual void CUnprovenSlot1() const;                    // slot 1: called when the last reference goes
    mutable long RefCount;                                  // +0x04
};
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
class CHeroCombatDef;
class CPlayerMovementDef;

class CThingCreatureBase;
class CCombatManager { public:
    void DeregisterDestroyedCombatant(CThingCreatureBase& creature);   // 0x006ED9E0
};
class CThingCreatureBase { public:                          // retail size 0x160
    virtual ~CThingCreatureBase();                          // 0x006661B0
    bool IsInMelee() const;                                 // 0x00661E80
    CCombatManager& GetCombatManager();                     // 0x00661ED0
    unsigned char _pad04[0x15C];
};

class CThingPlayerCreature : public CThingCreatureBase { public:
    C3DVector                                           ControlPos;                         // +0x160
    C3DVector                                           ControlMoveByVector;                // +0x16C
    unsigned char                                       MovedByPlayer : 1;                  // +0x178
    unsigned char                                       Jumping : 1;
    unsigned char                                       Pad1 : 1;
    EControlledMovementType                             ControlledMovementType;             // +0x17C
    C3DVector                                           MovementAcceleration;               // +0x180
    float                                               MaxSlowWalkingSpeed;                // +0x18C
    float                                               MaxWalkingSpeed;                    // +0x190
    float                                               MaxJoggingSpeed;                    // +0x194
    float                                               MaxRunningSpeed;                    // +0x198
    float                                               MaxSprintingSpeed;                  // +0x19C
    float                                               MaxFlyingSpeed;                     // +0x1A0
    float                                               MaxStrafingSpeed;                   // +0x1A4
    std::map<EGameAction, std::pair<C3DVector, long> >  RelativeMovementAccelerationComponents;   // +0x1A8
    C3DVector                                           RelativeMovementAcceleration;       // +0x1B4
    C3DVector                                           LastRelativeMovementAcceleration;   // +0x1C0
    bool                                                CollidedWithThing;                  // +0x1CC
    unsigned char                                       LastThingCollisionNormal[0xC];      // +0x1CD C3DVector (1-aligned in retail)
    unsigned char                                       LastThingCollisionPosition[0xC];    // +0x1D9 C3DVector (1-aligned in retail)
    CDefPointer<const CHeroCombatDef>                   PHeroCombatDef;                     // +0x1E8
    CDefPointer<const CPlayerMovementDef>               PPlayerMovementDef;                 // +0x1EC
    C3DVector                                           ImpulseVelocity;                    // +0x1F0
    float                                               LeanAngle;                          // +0x1FC
    EMovementBandType                                   MovementBandType;                   // +0x200
    virtual ~CThingPlayerCreature();
};

CThingPlayerCreature::~CThingPlayerCreature()
{
    if (IsInMelee())
        GetCombatManager().DeregisterDestroyedCombatant(*this);
}