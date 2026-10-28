// CThingPlayerCreature::CThingPlayerCreature() -- player creature: CThingCreatureBase(2) base, vtable, the relative-movement acceleration map built empty, the two def pointers cleared
// Leaf: ??0CThingPlayerCreature@@QAE@XZ
// Retail: Fable.exe @ 0x0052AB20, 93 bytes (16-aligned /O2 entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x012457FC is RTTI .?AVCThingPlayerCreature@@ (base CThingCreatureBase); PDB name set
// `??0CThingPlayerCreature@@QAE@XZ`; callers the thing factory thunk 0x0052B892 (`jmp`) and 0x006AC933. The catalog
// label C3DMeshFileBoneChunk::C3DMeshFileBoneChunk is propagated.
// Callees: CThingCreatureBase::CThingCreatureBase(long) 0x00666090 (bound `??0CThingCreatureBase@@QAE@J@Z`) with 2;
// malloc 0x00BFEA0E (the 0x24-byte _Rb_tree header node: base 0x10 + pair<const EGameAction, pair<C3DVector,long> > 0x14).
// Layout: Ego_r.pdb CThingPlayerCreature, retail = Ego_r - 0x28 (the base's containers) and a further -4 after the map:
// RelativeMovementAccelerationComponents +0x1A8, PHeroCombatDef +0x1E8, PPlayerMovementDef +0x1EC (C3DVector is packed).
// Shape: STLport 4.6 map() inlined: _Rb_tree_base(a): _M_header(a, 0), header = allocate(1); then _M_node_count(0) and
// _M_empty_initialize (color red = 0, root 0, leftmost = rightmost = header; each store reloads the header after the byte
// store). The C3DVector members have empty default constructors.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    _Color_type         _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;      // _S_rb_tree_red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    map() : _M_t(_Compare(), _Alloc()) {}
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}
#pragma pack(push, 1)
class C3DVector { public:
    C3DVector() {}
    float X, Y, Z;
};
#pragma pack(pop)
template <class T> class CDefPointer { public:
    CDefPointer() : Data(0) {}
    T* Data;
};
enum EGameAction { };
enum EControlledMovementType { };
enum EMovementBandType { };
class CHeroCombatDef;
class CPlayerMovementDef;
class CThingCreatureBase { public:
    CThingCreatureBase(long thingType);                  // 0x00666090
    virtual ~CThingCreatureBase();
    unsigned char _pad04[0x15C];
};
class CThingPlayerCreature : public CThingCreatureBase { public:
    CThingPlayerCreature();
    C3DVector                                         ControlPos;                              // +0x160
    C3DVector                                         ControlMoveByVector;                     // +0x16C
    unsigned char                                     MovedByPlayer : 1;                       // +0x178
    unsigned char                                     Jumping : 1;
    unsigned char                                     Pad1 : 1;
    EControlledMovementType                           ControlledMovementType;                  // +0x17C
    C3DVector                                         MovementAcceleration;                    // +0x180
    float                                             MaxSlowWalkingSpeed;                     // +0x18C
    float                                             MaxWalkingSpeed;                         // +0x190
    float                                             MaxJoggingSpeed;                         // +0x194
    float                                             MaxRunningSpeed;                         // +0x198
    float                                             MaxSprintingSpeed;                       // +0x19C
    float                                             MaxFlyingSpeed;                          // +0x1A0
    float                                             MaxStrafingSpeed;                        // +0x1A4
    std::map<EGameAction, std::pair<C3DVector, long> > RelativeMovementAccelerationComponents; // +0x1A8
    C3DVector                                         RelativeMovementAcceleration;            // +0x1B4
    C3DVector                                         LastRelativeMovementAcceleration;        // +0x1C0
    bool                                              CollidedWithThing;                       // +0x1CC
    C3DVector                                         LastThingCollisionNormal;                // +0x1CD
    C3DVector                                         LastThingCollisionPosition;              // +0x1D9
    CDefPointer<const CHeroCombatDef>                 PHeroCombatDef;                          // +0x1E8
    CDefPointer<const CPlayerMovementDef>             PPlayerMovementDef;                      // +0x1EC
    C3DVector                                         ImpulseVelocity;                         // +0x1F0
    float                                             LeanAngle;                               // +0x1FC
    EMovementBandType                                 MovementBandType;                        // +0x200
};
CThingPlayerCreature::CThingPlayerCreature()
    : CThingCreatureBase(2)
{
}