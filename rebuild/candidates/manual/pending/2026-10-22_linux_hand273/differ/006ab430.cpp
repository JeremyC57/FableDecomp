// CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude(float magnitude) const -- classify the stick magnitude into a movement band (0 stationary, 1 slow, 2 walking, 3/4 the two fast bands) from the player movement def's band thresholds, honouring sneaking, the hero run option and a zero running speed
// Grade: DIFFER(404v408) -- return-block placement: retail keeps the FIRST copy of the merged `return 0` / `return 2` exits inline (fabs and run-option tests fall through), ours keeps the last (fallback chain); the sneak component lands in ecx (retail eax)
// Leaf: GetMovementBandTypeFromJoytickMagnitude
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB430, length 406 bytes (+ int3 padding; `ret 4`).
// Mangled (Ego_r.pdb): ?GetMovementBandTypeFromJoytickMagnitude@CThingPlayerCreature@@IBE?AW4EMovementBandType@@M@Z.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (const instantiation), CThing::GetPosition
//   0x004C73D0, virtual GetMaxRunningSpeed(const C3DVector&) (slot 81, +0x144).
// Data: g_bHeroRunOption 0x013756EA (landed name); float pool 0.01f 0x01230A5C, 0.97f 0x0125D1DC.
// Layout (retail, local model; the CThingCreatureBase::GetMaxSneakingSpeed 0x00664CB0 model): component bitset +0x20, component
//   map +0x44; PPlayerMovementDef +0x1EC (Ego_r.pdb +0x218 minus 0x10 for CThing, 0x14 for the 16->12-byte RelativeMovement
//   map and earlier containers); CPlayerMovementDef = PDB - 4: JoystickMovementStationaryBand +0x3C, SlowBand +0x40,
//   WalkingBand +0x44, JoggingBand +0x48. Component 0x36 (sneak movement) flag +0x0F, component 0xDF flag +0x0D.
// Shape: the sneaking flag is a named bool (bl) from HasTC && the component byte; every early exit written out in full.
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase* Find(long kind) const
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTCSneakMovement {                       // component 0x36
    unsigned char _pad00[0x0f];
    bool          m_bSneaking;                  // +0x0f
};
struct CTCMovementModifier {                    // component 0xDF
    unsigned char _pad00[0x0d];
    bool          m_bJogOnly;                   // +0x0d
};
struct CThing {
    virtual void v000();
    unsigned char  _pad04[0x1c];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CPlayerMovementDef {
    unsigned char _pad00[0x3c];
    float JoystickMovementStationaryBand;       // +0x3c
    float JoystickMovementSlowBand;             // +0x40
    float JoystickMovementWalkingBand;          // +0x44
    float JoystickMovementJoggingBand;          // +0x48
};
struct CPlayerMovementDefPointer {
    const CPlayerMovementDef* Object;
    const CPlayerMovementDef* operator->() const { return Object; }
};
extern bool g_bHeroRunOption;                   // 0x013756EA
enum EMovementBandType { MOVEMENT_BAND_STATIONARY, MOVEMENT_BAND_SLOW, MOVEMENT_BAND_WALKING, MOVEMENT_BAND_3, MOVEMENT_BAND_4 };

struct CThingPlayerCreature : CThing {
    unsigned char _pad50[0x1ec - 0x50];
    CPlayerMovementDefPointer PPlayerMovementDef;   // +0x1ec
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078();
    virtual float GetMaxWalkingSpeed(const C3DVector& direction) const;     // slot 79 (+0x13c)
    virtual float GetMaxJoggingSpeed(const C3DVector& direction) const;     // slot 80 (+0x140)
    virtual float GetMaxRunningSpeed(const C3DVector& direction) const;     // slot 81 (+0x144)
    virtual float GetMaxSprintingSpeed(const C3DVector& direction) const;   // slot 82 (+0x148)
    EMovementBandType GetMovementBandTypeFromJoytickMagnitude(float magnitude) const;
};

EMovementBandType CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude(float magnitude) const
{
    bool sneaking = HasTC(0x36) && ((CTCSneakMovement*)GetTC(0x36))->m_bSneaking;
    if (fabs(GetMaxRunningSpeed(GetPosition())) < 0.01f)
        return MOVEMENT_BAND_STATIONARY;
    if (g_bHeroRunOption)
    {
        if (magnitude < PPlayerMovementDef->JoystickMovementStationaryBand)
            return MOVEMENT_BAND_STATIONARY;
        if (magnitude < PPlayerMovementDef->JoystickMovementWalkingBand)
            return MOVEMENT_BAND_WALKING;
        if (HasTC(0xdf) && ((CTCMovementModifier*)GetTC(0xdf))->m_bJogOnly)
            return MOVEMENT_BAND_3;
        return MOVEMENT_BAND_4;
    }
    const CPlayerMovementDef* def = PPlayerMovementDef.Object;
    if (magnitude < def->JoystickMovementSlowBand && !sneaking)
        return MOVEMENT_BAND_SLOW;
    if (magnitude < def->JoystickMovementWalkingBand)
        return MOVEMENT_BAND_WALKING;
    if (magnitude < 0.97f && sneaking)
        return MOVEMENT_BAND_WALKING;
    if (magnitude < def->JoystickMovementJoggingBand)
        return MOVEMENT_BAND_4;
    if (magnitude < def->JoystickMovementStationaryBand)
        return MOVEMENT_BAND_STATIONARY;
    if (magnitude < def->JoystickMovementSlowBand)
        return MOVEMENT_BAND_SLOW;
    if (magnitude < def->JoystickMovementWalkingBand)
        return MOVEMENT_BAND_WALKING;
    if (magnitude < def->JoystickMovementJoggingBand)
        return MOVEMENT_BAND_4;
    return MOVEMENT_BAND_3;
}
