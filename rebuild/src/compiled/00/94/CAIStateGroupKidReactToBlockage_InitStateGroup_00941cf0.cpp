// CAIStateGroup_KidReactToBlockage::InitStateGroup(const CWorld&, CThingAICreature&) -- stamp the current frame and pick a random minimum re-activation interval of 5..15 seconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941CF0, length 127 bytes (+ int3 padding), `ret 8`. Vtable slot 13 of
// CAIStateGroup_KidReactToBlockage (0x0128BBCC).
// Callees: GFGetCurrentFrame @ 0x0049D870 (__fastcall, no arguments), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0,
// __ftol2 @ 0x00BFEA70 (x2), GFROR13 @ 0x00497890 (__fastcall). Globals: the frame-rate long @ 0x01375550; float
// constants 0x0129287C = 10.0f, 0x01292878 = 5.0f, 0x0122DCB4 = 2^32 (the unsigned-to-float fix-up).
//
// Shape: the landed CAIStateGroup_MinionKidGroupTag (0x00934090) random helper with a float span: the frame rate
// is read into a local BEFORE the seed pointer is fetched (spilled to the `push ecx` slot for the `fild`), the
// range is `(long)(fps * seconds)`, then the seed step `seed = GFROR13(seed*0x24A1 + 0x24DF)` and the unsigned
// `range ? seed % range : 0`. The interval is `(long)(g_framesPerSecond * 5.0f + (float)random)` (the random
// operand, which holds the calls, is evaluated first; `fild ; test ; jge ; fadd 2^32` converts the unsigned).
// PDB names (struct_layouts_egor.tsv): LastEndState +0x14, CurrentActivationMinInterval +0x18. The engine
// header CAIStateGroup_KidReactToBlockage is not generated: local model.
class CWorld;
class CThingAICreature;

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                        // 0x0049D870
long __fastcall GFROR13(unsigned long);      // 0x00497890
extern long g_framesPerSecond;                              // 0x01375550

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long RandomFramesWithin(float seconds)
    {
        long framesPerSecond = g_framesPerSecond;
        unsigned long* seed = GetPWorldSeed();
        unsigned long range = (long)(framesPerSecond * seconds);
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        if (range != 0)
            return rotated % range;
        return 0;
    }
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

struct CAIStateGroup_KidReactToBlockage : CAIStateGroupBase {
    long LastEndState;                          // +0x14
    long CurrentActivationMinInterval;          // +0x18
    long LastActivation;                        // +0x1c
    long CurrentPersistMaxInterval;             // +0x20
    float our_radius;                           // +0x24
    float their_radius;                         // +0x28
    unsigned char PThingCreature[8];            // +0x2c CIntelligentPointer<CThingCreatureBase>
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_KidReactToBlockage::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    LastEndState = CWorld::GetFrame();
    CurrentActivationMinInterval = (long)(g_framesPerSecond * 5.0f + (float)RandomFramesWithin(10.0f));
}