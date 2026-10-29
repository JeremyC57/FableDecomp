// CAIStateGroup_MinionSleep::InitStateGroup(const CWorld&, CThingAICreature&) -- reset the bed state and pick the bedtime offset.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C440, length 73 bytes (+ int3 padding). Vtable slot 13
// of CAIStateGroup_MinionSleep (0x0128B140).
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (one inlined step of the world
// random generator; the stored seed forwards straight into the unsigned `div` by 10).
// Shape: the GoToBed-style members are reset (state +0x2c = -1, flag +0x34, delay +0x20 = 0), then the bedtime
// delay at +0x24 becomes `seed % 10` and WasCarryingWeapon (+0x60, engine header) is cleared -- VC schedules the
// byte store before the division result's store. Both arguments are unused (`ret 8`). The engine header keeps
// everything but +0x60 as pads, so the intermediate members are modelled locally (same slots as the landed
// CAIStateGroup_GoToBed::InitStateGroup @ 0x0092C490).
class CWorld;
class CThingAICreature;

long __fastcall GFROR13(unsigned long);   // 0x00497890

// One step of the world random generator on a shared seed; returns the new seed.
inline unsigned long WorldRandomStep(unsigned long& seed)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return seed;
}

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
};

struct CAIStateGroup_MinionSleep : CAIStateGroupBase {
    unsigned char PBed[8];                      // +0x14 CIntelligentPointer<CThing>
    long m_sleepTimeOffset;                     // +0x1c
    unsigned long m_bedTimeDelay;               // +0x20
    unsigned long m_wakeTimeDelay;              // +0x24 minutes, 0..9
    long m_pad28;                               // +0x28
    long m_state;                               // +0x2c
    long m_pad30;                               // +0x30
    bool m_flag34;                              // +0x34
    unsigned char _pad35[0x2b];                 // +0x35..+0x5f
    bool WasCarryingWeapon;                     // +0x60
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_MinionSleep::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    m_state = -1;
    m_flag34 = false;
    m_bedTimeDelay = 0;
    m_wakeTimeDelay = WorldRandomStep(*GetPWorldSeed()) % 10;
    WasCarryingWeapon = false;
}