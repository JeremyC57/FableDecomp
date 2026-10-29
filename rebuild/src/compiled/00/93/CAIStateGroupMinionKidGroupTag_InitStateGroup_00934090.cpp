// CAIStateGroup_MinionKidGroupTag::InitStateGroup(const CWorld&, CThingAICreature&) -- pretend the kid last
// played tag a random 0..7 seconds ago (never in the future).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00934090, length 90 bytes (+ int3 padding), `ret 8` (both
// reference arguments unused).
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0 (ulong* into the main game component), GFROR13
// @ 0x00497890 (__fastcall), GFGetCurrentFrame @ 0x0049D870 (__fastcall, three times). Global: the frame-rate
// long @ 0x01375550, read BEFORE the seed call (`imul esi,esi,7` = 7 seconds of frames).
// Shape: an inline `RandomFramesWithin(seconds)` helper -- frame rate read into a local, the seed pointer
// fetched, the range multiplied (retail's `imul esi,esi,7` sits right after the GetPWorldSeed call, so the
// multiply follows the fetch in program order), then the landed CObjectFamilyDef::GetRandomObject random
// step (`seed = seed*0x24A1 + 0x24DF; seed = GFROR13(seed); r = range ? seed % range : 0`, unsigned `div`);
// then `FrameLastPlayedIn = frame - r` and a clamp `if (FrameLastPlayedIn > GFGetCurrentFrame())
// FrameLastPlayedIn = GFGetCurrentFrame();` with the frame re-read for each use (no local). FrameLastPlayedIn is +0x2c (engine header name); the header flattens
// the base, so the layout is local.
class CWorld;
class CThingAICreature;

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};        // 0x0049D870
long __fastcall GFROR13(unsigned long);   // 0x00497890
extern long g_framesPerSecond;              // 0x01375550

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
    protected:
    unsigned long* GetPWorldSeed() const;         // 0x008FCFD0
    public:
    // a random frame count within `seconds`: the frame rate is read BEFORE the seed pointer is fetched (retail
    // loads the global first and multiplies after the call), the seed step is the landed GetRandomObject one
    unsigned long RandomFramesWithin(long seconds)
    {
        long framesPerSecond = g_framesPerSecond;
        unsigned long* seed = GetPWorldSeed();
        unsigned long range = framesPerSecond * seconds;
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        if (range != 0)
            return rotated % range;
        return 0;
    }
};

struct CAIStateGroup_MinionKidGroupTag : CAIStateGroupBase {
    bool InitiatedTag;                      // +0x14
    unsigned char _pad15[3];
    void* PTCGroupTag;                      // +0x18
    float InitialTagPos_X, InitialTagPos_Y, InitialTagPos_Z;    // +0x1c
    bool StandModeAddedToCreature;          // +0x28
    unsigned char _pad29[3];
    long FrameLastPlayedIn;                 // +0x2c
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_MinionKidGroupTag::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    FrameLastPlayedIn = CWorld::GetFrame() - RandomFramesWithin(7);
    if (FrameLastPlayedIn > CWorld::GetFrame())
        FrameLastPlayedIn = CWorld::GetFrame();
}