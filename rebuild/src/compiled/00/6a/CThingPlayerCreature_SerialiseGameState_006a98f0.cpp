// CThingPlayerCreature::SerialiseGameState(CPersistContext& context) -- base creature serialise; unless the context is in mode 1 or 3, reset MaxHealth from the creature def, top Health up when it holds the sentinel value, and set a CThing state bit
// Leaf: SerialiseGameState@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A98F0, length 112 bytes (`ret 4`). Vtable CThingPlayerCreature
// 0x012457FC slot 3 (manifest "vfunc_3"; slot 3 is SerialiseGameState as for CThingCreatureBase 0x006628E0).
// Callees: CThingCreatureBase::SerialiseGameState @ 0x006628E0 (direct base call); float pool 0x0122DEDC
// (0.0f: an unset Health is compared for equality).
// Layout (retail; Ego_r.pdb in brackets): CThing state byte +0x92 [PDB +0xa2] bit 1 (PDB order: Drawable);
// CThingPhysical MaxHealth +0xb0 [PDB +0xc8], Health +0xb4 [PDB +0xcc]; CThingCreatureBase PDef
// (CDefPointer<const CThingCreatureDef>) +0xe0 [PDB +0xfc]; the def's float at +0x50; CPersistContext mode +0x18.
// Shape: the body under `if (base call)` with a shared `mov al,1` exit (as 0x006628E0); the def value is
// loaded once and stored with `fst` into MaxHealth, kept on the x87 stack for the conditional Health store;
// the bit set is a 1-bit bitfield assignment (`or byte ptr [esi+92h],2`).
struct CThingCreatureDef {
    unsigned char _pad_0x00[0x50];
    float DefaultMaxHealth;                           // +0x50
};

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          m_mode;                             // +0x18
};

struct CThingCreatureBase {
    unsigned char _pad_0x00[0x92];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;  // +0x92 bit 0
    unsigned char Drawable : 1;                       // +0x92 bit 1
    unsigned char _bits_0x92 : 6;
    unsigned char _pad_0x93[0xb0 - 0x93];
    float MaxHealth;                                  // +0xb0
    float Health;                                     // +0xb4
    unsigned char _pad_0xb8[0xe0 - 0xb8];
    const CThingCreatureDef* PDef;                    // +0xe0 CDefPointer<const CThingCreatureDef>::Object
    bool SerialiseGameState(CPersistContext& context);    // 0x006628E0
};

static const float HEALTH_UNSET = 0.0f;               // 0x0122DEDC (0.0f)

struct CThingPlayerCreature : CThingCreatureBase {
    bool SerialiseGameState(CPersistContext& context);
};

bool CThingPlayerCreature::SerialiseGameState(CPersistContext& context)
{
    if (CThingCreatureBase::SerialiseGameState(context)) {
        if (context.m_mode != 1 && context.m_mode != 3) {
            float maxHealth = PDef->DefaultMaxHealth;
            MaxHealth = maxHealth;
            if (Health == HEALTH_UNSET)
                Health = maxHealth;
            Drawable = 1;
        }
        return true;
    }
    return false;
}