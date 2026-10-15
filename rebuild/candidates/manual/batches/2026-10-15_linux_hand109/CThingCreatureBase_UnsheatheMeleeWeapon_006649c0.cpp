// CThingCreatureBase::UnsheatheMeleeWeapon() -- unless the creature's weapons are already out or it already carries a melee weapon, sheathe everything and draw the creature definition's default melee weapons (primary, then secondary)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006649C0, length 79 bytes (+ int3 padding).
// Callees: the creature's virtual slot 77 (+0x134, named as in the landed UnsheatheAllWeapons 0x00666830),
// IsThingCarryingMeleeWeapon(const CThing&) @ 0x006A15A0 (a __fastcall free function, the thing in ecx; the manifest
// gives both 0x006A15A0 and 0x006A1620 the melee name), CThingCreatureBase::SheatheAllWeapons @ 0x006647F0,
// CThingCreatureBase::UnsheatheWeapon(long, bool) @ 0x006648D0 (Ego_r.pdb `?UnsheatheWeapon@CThingCreatureBase@@IAEXJ_N@Z`).
// Layout (retail, local model): the creature definition pointer at +0xe0 (engine/CThingCreatureBase.h lists +0xe0 only as
// a retail-only long); in the definition the default weapon def indices at +0xd8 / +0xdc (melee) and +0xe0 (ranged), as
// the landed GetDefaultWeaponDef 0x00662860 reads them.
// Shape: two early returns on the bool tests (`jne` to the shared `pop esi ; ret`), the definition pointer re-read from
// `this` for each UnsheatheWeapon call (a call may change it).
struct CThingCreatureBase;

struct CThingCreatureDef {
    unsigned char _pad00[0xd8];
    long          DefaultMeleeWeapon;           // +0xd8
    long          DefaultSecondaryMeleeWeapon;  // +0xdc
    long          DefaultRangedWeapon;          // +0xe0
};

bool __fastcall IsThingCarryingMeleeWeapon(const CThingCreatureBase& creature);   // 0x006A15A0

struct CThingCreatureBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual bool AreWeaponsUnsheathed();        // slot 77 (+0x134)
    unsigned char      _pad04[0xdc];
    CThingCreatureDef* m_pDef;                  // +0xe0
    void SheatheAllWeapons();                   // 0x006647F0
    void UnsheatheWeapon(long weaponDef, bool primary);     // 0x006648D0
    void UnsheatheMeleeWeapon();
};

void CThingCreatureBase::UnsheatheMeleeWeapon()
{
    if (AreWeaponsUnsheathed())
        return;
    if (IsThingCarryingMeleeWeapon(*this))
        return;
    SheatheAllWeapons();
    UnsheatheWeapon(m_pDef->DefaultMeleeWeapon, true);
    UnsheatheWeapon(m_pDef->DefaultSecondaryMeleeWeapon, false);
}
