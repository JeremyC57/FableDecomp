// CAIStateGroup_BanditSitRoundCampfire::Idle() -- do nothing for a random 5..15 seconds (or until the idle action stops); true when the action was still running at the end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A540, length 125 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFA0 (twice, result unused: the world is only the object
// expression of static calls), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFFloatRandom(float, ulong*)
// @ 0x004978A0 (__fastcall: the float on the stack, the seed in ecx), CWorld::GetFrame @ 0x0049D870
// (__fastcall), __ftol2 @ 0x00BFEA70, CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200.
// Globals/constants: the frames-per-second long @ 0x01375550, 10.0f (pushed), 5.0f @ 0x01292458.
// Shape: the frame rate is read into a local BEFORE the seed call (it feeds `fimul`), the delay
// `(GFFloatRandom(10, seed) + 5) * fps` is a named float local (spilled across the frame call), the end
// frame `frame + delay` is converted with __ftol2 into ebx (unsigned: the loop test is `jae`); the loop is a
// 16-aligned `do/while` (`nop` before the head) running DoNothing while the frame is before the end and the
// action status is 4 (running); the result is `status == 4` (`xor eax,eax ; cmp ; sete`, an int-width result).
// `GetPWorld()->Static()` still emits the GetPWorld call (wave 25 lesson), which is the unused call here.
// Model: local -- the class has no engine header (layout from struct_layouts_egor.tsv as in the landed 0x0093A500).
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern long g_framesPerSecond;                                      // 0x01375550

class CWorld { public:
    static long GetFramesPerSecond() { return g_framesPerSecond; }
    static long __fastcall GetFrame();
};

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0 (PBrain->+0x1c->+0x24)
public:
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
    protected: EAIActionReturn AIAction_DoNothing(); public:                  // 0x0097E200 (EAIActionReturn)
};

struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupBase {
    unsigned char PCampfire[8];                 // +0x14 CIntelligentPointer<CThingObject>
    long          SeatPosIndex;                 // +0x1c
    bool          GotSeatPos;                   // +0x20
    bool          Sitting;                      // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
    unsigned long FrameToNextCheckIsRunnable;   // +0x28
    bool          WasCarryingWeapon;            // +0x2c
    long Idle();
};

long CAIStateGroup_BanditSitRoundCampfire::Idle()
{
    long framesPerSecond = PeekWorld().GetFramesPerSecond();
    float delay = (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * framesPerSecond;
    unsigned long endFrame = (unsigned long)(CWorld::GetFrame() + delay);
    long result;
    do {
        result = AIAction_DoNothing();
    } while ((unsigned long)PeekWorld().GetFrame() < endFrame && result == 4);
    return result == 4;
}