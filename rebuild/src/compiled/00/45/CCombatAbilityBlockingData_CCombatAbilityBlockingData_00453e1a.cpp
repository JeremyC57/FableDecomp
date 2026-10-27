// CCombatAbilityBlockingData::~CCombatAbilityBlockingData() -- destroy the set member, then the CCombatAbilityData base (which stores its own vtable)
// Leaf: ??1CCombatAbilityBlockingData@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453E1A, 19 bytes, thiscall, plain `ret`.
// Identity (proven): its caller 0x004549B2 is slot 0 (the scalar deleting destructor) of vtable 0x01232DEC (RTTI .?AVCCombatAbilityBlockingData@@); it destroys the member at +0x20 = Ego_r.pdb CCombatAbilityBlockingData::ValidBlockWeaponTypes (set<EWeaponClass>), then stores the base CCombatAbilityData vtable 0x01232484; also destroyed as the +0x28 member by 0x00453E51.
// Callees: the member destructor 0x00451B07.
// Shape (family M034, representative 0x00453E1A): an empty destructor of a `__declspec(novtable)` class (no own vtable store,
//   as in retail) over a base whose inline destructor stores the base vtable; the member destructor runs first.
// Header debt: the class chain is modelled locally (no engine/ header carries the destructors).
enum EWeaponClass { WEAPON_CLASS_NONE = 0 };
namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    ~set();                                               // 0x00451B07 (clear + free the header node)
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
class CDefString { public: long TablePos; };
enum ECombatAbilityInterfaceType { COMBAT_ABILITY_INTERFACE_NONE = 0 };
class CCombatAbilityData {                                // Ego_r.pdb: vptr + five members (0x18 bytes)
public:
    virtual ~CCombatAbilityData() { }
    long PercentageChance;
    float SecondsDuration;
    float SecondsToWaitBeforeRepeat;
    CDefString Anim;
    ECombatAbilityInterfaceType InterfaceType;
};
class __declspec(novtable) CCombatAbilityBlockingData : public CCombatAbilityData {
public:
    virtual ~CCombatAbilityBlockingData();
    float BlockAngle;                                     // +0x18
    bool BlockCanBeBroken;                                // +0x1c
    bool ForceRecoilWhenBlocking;                         // +0x1d
    std::set<EWeaponClass> ValidBlockWeaponTypes;         // +0x20
    EWeaponClass WeaponClass;                             // +0x2c
};
CCombatAbilityBlockingData::~CCombatAbilityBlockingData()
{
}