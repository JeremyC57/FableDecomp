// Retry 10: RELOCATION_MATCH -- the vector modelled as STLport _Vector_base (plain _M_start/_M_finish, _M_end_of_storage an _STLP_alloc_proxy with its own ctor); three plain pointers schedule the member-ctor `lea` two stores too early.
// CSpecialAbilitiesBattleChargeDef::CSpecialAbilitiesBattleChargeDef() -- base CParentDefClassBase() (0x00430370), this class's vtable, then the four retail vectors DashDistance / RadiusOfEffect / BattleChargeDamage (+0x40, +0x4c, +0x58) and NumPeopleHit (+0x90) built empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004570D3, length 61 bytes.
// Callees: 0x00430370 (the shared def root constructor). Vtable 0x012340C4 = CSpecialAbilitiesBattleChargeDef
// (rtti-vtables.tsv; slot 20 GetSizeofClass is 0x00457110, right behind this body).
// Layout from the PDB (Ego_r: RunSpeed +0x40, three vector<float> +0x44/+0x54/+0x64, eleven floats, the
// vector<long> NumPeopleHit +0xa0, two floats; 184 bytes) shifted to the retail build: the def root ends at
// +0x3c and retail vectors are 12 bytes (first/last/end), so RunSpeed +0x3c, the vectors +0x40/+0x4c/+0x58,
// the floats +0x64..+0x8c, NumPeopleHit +0x90, the two dragon floats +0x9c/+0xa0 (0xa4 bytes).
// Shape: the vtable store precedes the zero stores (the vectors are member objects with inline
// constructors); the size-optimised def module (`#pragma optimize("s", on)`) forms the address of the
// far vector once (`lea eax,[esi+90h]`, three short-displacement stores) while the near ones keep their
// disp8 stores; eax is busy with that address, so the zero register is ecx.
#pragma optimize("s", on)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
    CParentDefClassBase();                      // 0x00430370
};

template <class T>
struct CAllocProxy {                            // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
template <class T>
struct CRetailVector {                          // STLport _Vector_base: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    CAllocProxy<T> end;
    CRetailVector() : first(0), last(0), end(0) { }
};

class CSpecialAbilitiesBattleChargeDef : public CParentDefClassBase { public:
    float                RunSpeed;                              // +0x3c
    CRetailVector<float> DashDistance;                          // +0x40
    CRetailVector<float> RadiusOfEffect;                        // +0x4c
    CRetailVector<float> BattleChargeDamage;                    // +0x58
    float                BattleChargeCombatMultiplierFactor;    // +0x64
    float                BattleChargeRadialStartSeconds;        // +0x68
    float                BattleChargeRadialEndSeconds;          // +0x6c
    float                BattleChargeRadialThreshold;           // +0x70
    float                BattleChargeRadialLength;              // +0x74
    float                BattleChargeRadialIntensity;           // +0x78
    float                BattleChargeRadialFade;                // +0x7c
    float                BattleChargeRadialRange;               // +0x80
    float                BattleChargeRadialTwist;               // +0x84
    float                PoseDelaySecs;                         // +0x88
    float                ChargeDelay;                           // +0x8c
    CRetailVector<long>  NumPeopleHit;                          // +0x90
    float                DragonExtraGumphRadiusMultiplier;      // +0x9c
    float                DragonExtraGumphRadius;                // +0xa0
    virtual void v00();
    CSpecialAbilitiesBattleChargeDef();
};

CSpecialAbilitiesBattleChargeDef::CSpecialAbilitiesBattleChargeDef()
    : CParentDefClassBase()
{
}