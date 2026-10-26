// CAIStateGroup_GoToBed::InitStateGroup(const CWorld&, CThingAICreature&) -- pick this creature's bedtime offset.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C490, length 58 bytes.
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0 (the world's random seed, an unsigned long* into
// the brain's world), GFROR13 @ 0x00497890 (__fastcall rotate-right-13, landed).
//
// Shape: the state at +0x2c is reset to -1 first, then one step of the world's linear-congruential random
// generator runs inline on the seed (`seed = seed * 0x24A1 + 0x24DF; seed = ROR13(seed)`) -- the helper
// returns the freshly stored seed, which retail forwards straight into `div` -- and the bedtime offset at
// +0x20 becomes `seed % 10` (unsigned: `xor edx,edx ; div`). Both arguments are unused (`ret 8`). No engine
// header exists for CAIStateGroup_GoToBed; the layout is local (see the constructor @ 0x008FEB6B).
class CWorld;
class CThingAICreature;

extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

// One step of the world random generator on a shared seed; returns the new seed.
inline unsigned long WorldRandomStep(unsigned long& seed)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return seed;
}

class CAIStateGroupBase { public:
    virtual void v00();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    unsigned char PBed[8];                      // +0x14 CIntelligentPointer<CThing>
    long m_sleepTimeOffset;                     // +0x1c
    unsigned long m_bedTimeDelay;               // +0x20 minutes past sleep time, 0..9
    long m_wakeTimeDelay;                       // +0x24
    long m_pad28;                               // +0x28
    long m_state;                               // +0x2c
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_GoToBed::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    m_state = -1;
    m_bedTimeDelay = WorldRandomStep(*GetPWorldSeed()) % 10;
}