// CTCLook::UpdateHeadCreatureAnim(const CCharString& name, ECreatureAnim anim, CAnimateThing* animate) -- keeps the head-turn combination animation of `anim` (11 horizontal, 12 vertical) on the creature: adds it from the cached head-turn anim entry when missing (a 50/50 blend), then refreshes its playback frame, remembering the previous one; without an entry nothing is touched, and when the animation cannot be found the head angles and velocities are zeroed
// Grade: DIFFER(226v226) -- this/anim ebx/edi swap + ebp saved late (shrink-wrapped) + previous ratio homed in the dead name param slot where retail uses a frame word (dl 25, 226v226; best try b)
// Leaf: ?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752F30, length 240 bytes (`ret 0Ch`, + int3 padding); speed TU (no frame pointer).
// Callees: CAnimateThing::FindCombinationAnim(long) @ 0x0070C4E0 (twice), CAnimInit::Set @ 0x0070C050, CAnimateThing::AddCombinationAnim
// (const CAnimInit&, long, long, bool, bool) @ 0x0070D780, CTCLook::ComputeAnimationPlaybackRatio(long) @ 0x00752510 (float in st0).
// Layout: retail CTCLook HeadAngleRelToBodyXY +0x2C, HeadAngleRelToBodyYZ +0x30, HeadVelocityXY +0x5C, HeadVelocityYZ +0x60, PHeadTurnAnimH
// +0xBC and PHeadTurnAnimV +0xC4 (PDB +0xC0/+0xC8; counted pointers whose entry starts with the anim index, -1 when none); CAnimInit
// is the landed 0x2C-byte record with the blend pair at +0x18; the combination anim's playback ratio at +0x1C and its previous value
// at +0x20 (descriptive names). The first argument is unused.
// Shape: the entry pointer is picked by a two-case `if`; the init record is built by the landed ctor (the blend pair zeroed before Set),
// then given a CBlendPair(0.5f, 0.5f) whose `const float&` arguments go through two frame words; the previous ratio is parked in a
// frame word across the compute call.
class CCharString;
enum ECreatureAnim { CREATURE_ANIM_HEAD_HORIZONTAL = 11, CREATURE_ANIM_HEAD_VERTICAL = 12 };
class CAnimateThing;
struct CBlendPair {
    float In;                                        // +0x00
    float Out;                                       // +0x04
    CBlendPair() : In(0.0f), Out(0.0f) { }
    CBlendPair(const float& in, const float& out) : In(in), Out(out) { }
};
struct CAnimInit {                                   // 0x2c bytes
    long          Mode;                              // +0x00
    unsigned char AnimType;                          // +0x04
    bool          Flag05;                            // +0x05
    long          AnimIndex;                         // +0x08
    float         Scales[3];                         // +0x0c
    CBlendPair    Blend;                             // +0x18
    unsigned long Flags;                             // +0x20
    long          Arg24;                             // +0x24
    long          Percent;                           // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent) : Blend()
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
class CAnimSet {
public:
    unsigned char _pad00[0x1c];
    float PlaybackRatio;                             // +0x1c
    float PreviousPlaybackRatio;                     // +0x20
};
class CAnimateThing {
public:
    CAnimSet* GetFirstCombinationAnimFromType(long type);   // 0x0070C4E0
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
};
struct CAnimEntryLite {                              // NAnimationEntryBodge::CAnimEntryLite
    long AnimIndex;                                  // +0x00 (-1: none)
};
struct CAnimEntryPointer {                           // CCountedPointer<CAnimEntryLite> (8 bytes)
    CAnimEntryLite* Data;
    void*           Info;
};

class CTCLook {
public:
    unsigned char     _pad00[0x2c];
    float             HeadAngleRelToBodyXY;          // +0x2c
    float             HeadAngleRelToBodyYZ;          // +0x30
    unsigned char     _pad34[0x5c - 0x34];
    float             HeadVelocityXY;                // +0x5c
    float             HeadVelocityYZ;                // +0x60
    unsigned char     _pad64[0xbc - 0x64];
    CAnimEntryPointer PHeadTurnAnimH;                // +0xbc
    CAnimEntryPointer PHeadTurnAnimV;                // +0xc4
private:
    float GetHeadCreatureAnimFrame(ECreatureAnim anim) const;   // 0x00752510
    void UpdateHeadCreatureAnim(const CCharString& name, ECreatureAnim anim, CAnimateThing* animate);
};
void CTCLook::UpdateHeadCreatureAnim(const CCharString& name, ECreatureAnim anim, CAnimateThing* animate)
{
    CAnimEntryLite* entry = 0;
    if (anim == CREATURE_ANIM_HEAD_HORIZONTAL)
        entry = PHeadTurnAnimH.Data;
    else if (anim == CREATURE_ANIM_HEAD_VERTICAL)
        entry = PHeadTurnAnimV.Data;
    if (entry) {
        long animIndex = entry->AnimIndex;
        if (animIndex != -1) {
            if (!animate->GetFirstCombinationAnimFromType(anim)) {
                CAnimInit init(anim, 0, animIndex, 0, false, 0, 100);
                init.Blend = CBlendPair(0.5f, 0.5f);
                animate->AddCombinationAnim(init, 0, 0, false, false);
            }
            CAnimSet* existing = animate->GetFirstCombinationAnimFromType(anim);
            if (existing) {
                float previous = existing->PlaybackRatio;
                existing->PlaybackRatio = GetHeadCreatureAnimFrame(anim);
                existing->PreviousPlaybackRatio = previous;
            } else {
                HeadAngleRelToBodyXY = 0.0f;
                HeadAngleRelToBodyYZ = 0.0f;
                HeadVelocityXY = 0.0f;
                HeadVelocityYZ = 0.0f;
            }
        }
    }
}
