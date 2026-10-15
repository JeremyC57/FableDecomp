// CTCWeaponTrail::OnCreate() -- choose the trail type from the owner (an object: the 0x20000000 property gives type 3; anything else: flag bit 3 at +0x6c gives type 2), reset the trail counts and timer, build the trails
// Leaf: OnCreate
// Grade: DIFFER(92v91) -- only the byte widening differs: retail `movzx ecx,byte ptr [eax+6Ch]`, the lane
// `xor ecx,ecx ; mov cl,byte ptr [eax+6Ch]` (everything else, the one-shot guard and the neg/sbb select included, matches).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076EF40, length 91 bytes (+ int3 padding).
// CTCWeaponTrail vtable 0x0126A5B4 slot 4 (rtti-vtables.tsv; chunk label "CTCWeaponTrail::vfunc_4"); slot 4 of the
// CTCBase-derived vtables is OnCreate (Ego_r ?OnCreate@CTCWeaponTrail@@EAEXXZ).
// Callees: CTCWeaponTrail::InitialiseTrails @ 0x0076EB00 (manifest label; Ego_r ?InitialiseTrails@CTCWeaponTrail@@AAEXXZ).
// Global 0x013BADA0: a one-shot bool set on the first call (nothing else hangs off it in this build).
// Layout (Ego_r.pdb CTCWeaponTrail, retail 4-byte STLport lists): NoTrails +0x0c, NoProjectileTrails +0x10, TrailType
// +0x2c, MinProjectileTrailTimer +0x64. CThing: type byte +0x10 (5 here), property word +0x2c, flag byte +0x6c.
// Tried: (1) this form (92v91, xor/mov widening); (2) bit 3 as an `unsigned char` bitfield: `shr 3 ; and 1` (94v91);
// (3) /G7: `cmp byte ptr [g],0` and `mov cl` without the clear (90v91); (4) /G6: unchanged (92v91).
// Shape: `x ? 3 : 0` on a masked word lowers to `and ; neg ; sbb ; and 3`, `(b & 8) ? 2 : 0` on a byte to
// `movzx ; shr 2 ; and 2`; the two -1 counts precede the InitialiseTrails call, the timer follows it.
struct CThing {
    unsigned char _pad_00[0x10];
    unsigned char m_thingType;                              // +0x10
    unsigned char _pad_11[0x2c - 0x11];
    unsigned long m_properties;                             // +0x2c
    unsigned char _pad_30[0x6c - 0x30];
    unsigned char m_flags6c;                                // +0x6c (bit 3: wants a type-2 trail)
};

enum { THING_TYPE_OBJECT = 5 };

enum EWeaponTrailType {
    WEAPON_TRAIL_TYPE_NONE = 0,
    WEAPON_TRAIL_TYPE_2    = 2,
    WEAPON_TRAIL_TYPE_3    = 3
};

static bool s_weaponTrailCreated = false;                   // 0x013BADA0

struct CTCWeaponTrail {
    void*            __vftable;                             // +0x00
    CThing*          ParentThing;                           // +0x04
    bool             ToBeRemoved;                           // +0x08
    bool             IsInGlobalUpdateTCs;                   // +0x09
    long             NoTrails;                              // +0x0c
    long             NoProjectileTrails;                    // +0x10
    unsigned char    _pad_0x14[0x2c - 0x14];
    EWeaponTrailType TrailType;                             // +0x2c
    unsigned char    _pad_0x30[0x64 - 0x30];
    long             MinProjectileTrailTimer;               // +0x64
    void InitialiseTrails();                                // 0x0076EB00
    void OnCreate();
};

void CTCWeaponTrail::OnCreate()
{
    if (!s_weaponTrailCreated)
        s_weaponTrailCreated = true;
    if (ParentThing->m_thingType == THING_TYPE_OBJECT)
        TrailType = (ParentThing->m_properties & 0x20000000) ? WEAPON_TRAIL_TYPE_3 : WEAPON_TRAIL_TYPE_NONE;
    else
        TrailType = (ParentThing->m_flags6c & 8) ? WEAPON_TRAIL_TYPE_2 : WEAPON_TRAIL_TYPE_NONE;
    NoTrails = -1;
    NoProjectileTrails = -1;
    InitialiseTrails();
    MinProjectileTrailTimer = -1;
}
