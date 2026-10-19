// CHitParameters::CHitParameters(CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon, const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter) -- a localised, directional hit: position/direction copied, the four caller flags, play-response / move-back / particle defaults, empty damage map and augmentation list, the hit location, no material, the hitting thing and weapon pointers
// Leaf: ??0CHitParameters
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789238, 178 bytes, `ret 24h` (nine arguments).
// Callees: map<EDamageAttribute, float>::map() @ 0x00453554 (out of line; the propagated label "CopyBackBufferToTexture"
//   is wrong), CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0 (x2).
// Layout (Ego_r.pdb CHitParameters names; retail 0x60 bytes, the map and the vector 12 bytes instead of 16):
//   Localised +0, Directional +1, Position +2, Direction +0x0e, KnockDown..CauseRecoil +0x1a..+0x28, ShotBuildupTime
//   +0x2c, DamageMap +0x30, Augmentations +0x3c, PHitLocation +0x48, PHitMaterialDef +0x4c, PThingHitting +0x50,
//   PWeapon +0x58. The vectors sit at the unaligned +2/+0x0e, so the record is `#pragma pack(1)` with an explicit pad
//   before ShotBuildupTime.
// Shape: every member is an initialiser in declaration order; the float zero is `fldz ; fstp`, the byte constants are
//   immediates or the shared `bl` zero, the vector is three null words, the intelligent pointers are out-of-line ctors.
// Pragma: frame-pointer size TU (`push dword ptr [ebp+0Ch]`, fldz zero): `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;
struct CHitLocation;
struct CMaterialDef;

#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)

struct CDamageMap {                                // map<EDamageAttribute, float> (12 bytes in retail)
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CDamageMap();                                  // 0x00453554
};

template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

struct CDefPointer {                               // CDefPointer<const CMaterialDef>
    const CMaterialDef* Object;
    CDefPointer() : Object(0) { }
};

struct CIntelligentPointer {                       // CIntelligentPointer<CThing>: vptr + item
    void* __vftable;
    void* m_pItem;
    CIntelligentPointer(CThing* thing);            // 0x004A96F0
};

#pragma pack(push, 1)
struct CHitParameters {
    bool          Localised;                       // +0x00
    bool          Directional;                     // +0x01
    C3DVector     Position;                        // +0x02
    C3DVector     Direction;                       // +0x0e
    bool          KnockDown;                       // +0x1a
    bool          Decapitate;                      // +0x1b
    bool          Blockable;                       // +0x1c
    bool          Flourish;                        // +0x1d
    bool          EpicSpell;                       // +0x1e
    bool          BlockCounter;                    // +0x1f
    bool          PlayHitResponse;                 // +0x20
    bool          PlayHitResponseOverrideSet;      // +0x21
    bool          MoveBack;                        // +0x22
    bool          CreateParticleEffectOnHit;       // +0x23
    bool          CreateDustParticleEffectOnHit;   // +0x24
    bool          GuaranteeHit;                    // +0x25
    bool          Blocked;                         // +0x26
    bool          HitNegated;                      // +0x27
    bool          CauseRecoil;                     // +0x28
    unsigned char _pad29[3];
    float         ShotBuildupTime;                 // +0x2c
    CDamageMap    DamageMap;                       // +0x30
    CRetailVector<long> Augmentations;             // +0x3c vector<EObjectAugmentationType>
    CHitLocation* PHitLocation;                    // +0x48
    CDefPointer   PHitMaterialDef;                 // +0x4c
    CIntelligentPointer PThingHitting;             // +0x50
    CIntelligentPointer PWeapon;                   // +0x58
    CHitParameters(CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter);
};
#pragma pack(pop)

CHitParameters::CHitParameters(CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon, const C3DVector& position,
                               const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter)
    : Localised(true)
    , Directional(true)
    , Position(position)
    , Direction(direction)
    , KnockDown(knockDown)
    , Decapitate(false)
    , Blockable(blockable)
    , Flourish(flourish)
    , EpicSpell(false)
    , BlockCounter(blockCounter)
    , PlayHitResponse(true)
    , PlayHitResponseOverrideSet(false)
    , MoveBack(true)
    , CreateParticleEffectOnHit(true)
    , CreateDustParticleEffectOnHit(true)
    , GuaranteeHit(false)
    , Blocked(false)
    , HitNegated(false)
    , CauseRecoil(false)
    , ShotBuildupTime(0.0f)
    , PHitLocation(hitLocation)
    , PThingHitting(thingHitting)
    , PWeapon(weapon)
{
}
