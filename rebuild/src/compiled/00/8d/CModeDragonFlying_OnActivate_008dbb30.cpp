// NCreatureMode::CModeDragonFlying::OnActivate() -- start a flying leg: suspend a player creature's animation, clear the interface-0x3a flag, make the dragon un-willable, measure the leg, prime the flight animation and its playback rate
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DBB30, length 331 bytes (padded to 336), `ret`; vtable 0x0126868C slot 5.
// Callees: CModeBase::OnActivate @ 0x0073AAC0, CThingAnimatable::SetSuspendAnimation @ 0x00833A10, CVectorMap::LowerBound
//          @ 0x0040F020 (inlined GetInterface, types 0x3a and 0x5a), SetThingAsNotBloodyWillableAtAll(CThing*, bool) @ 0x007E4040
//          (`__fastcall`, bool in dl), CTCAnimationComplex::GetAnimateThing @ 0x0070B460 (returns +0xc; manifest label
//          "CWorld::DrawGetEnvironment" is propagated), GetAnimIndex @ 0x0070B490, GetAnimDuration(long) @ 0x0070B4A0, virtual
//          AnimUpdate(CAnimateThing&) (slot 2, CModeDragonFlyingBase 0x008D70A0) on `this`.
// Globals: game frames per second (long) @ 0x01375550, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout: landed CModeDragonFlying::OnDeactivate (0x008DC590) model: CModeBase ParentThing +4, Active +0x18; the thing's type
//   byte +0x10, presence bitset +0x20, interface map +0x44. PDB CModeDragonFlying members sit 4 bytes lower in retail:
//   Destination +0x44, StartPos +0x50, DistanceToTravel +0x68, Speed +0x6c, StartSpeed +0x70, MovementDuringAnimationLoop +0x74,
//   Acceleration +0x78, AnimLength +0x7c, FirstFrame +0x90; +0x34 is a CModeDragonFlyingBase counter. CAnimateThing playback
//   speeds +0x1c/+0x20.
// Shape: the 0x5a component pointer is zero-initialised and GetInterface's result is not tested; the playback rate is
//   1 / (AnimLength * frames per second) clamped to [0, 1] and written to both speeds.
#include <math.h>

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CModeBase;
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                   // +0x10 (1 = player creature)
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
};
struct CTCFlagged {                         // the type-0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_flag : 1;               // +0x50 bit 0
};
struct CAnimateThing {
    unsigned char _pad00[0x1c];
    float         PlaybackSpeed;            // +0x1c
    float         TargetPlaybackSpeed;      // +0x20
};
struct CTCAnimationComplex {                // the type-0x5a component
    CAnimateThing* GetAnimateThing();       // 0x0070B460
    long GetAnimIndex();                    // 0x0070B490
    float GetAnimDuration(long animIndex);  // 0x0070B4A0
};
void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);   // 0x007E4040

extern long g_GameFramesPerSecond;          // 0x01375550

struct C3DVector {
    float X, Y, Z;
};

struct CModeBase {
    virtual void v00();
    virtual void v01();
    virtual void AnimUpdate(CAnimateThing& anim);   // slot 2
    CThing*       ParentThing;              // +0x04
    unsigned char _pad08[0x10];
    bool          Active;                   // +0x18
    void OnActivate();                      // 0x0073AAC0
};

namespace NCreatureMode {

struct CModeDragonFlying : CModeBase {
    unsigned char _pad1c[0x18];
    long          FlapCounter;                      // +0x34
    unsigned char Filter[0x0c];                     // +0x38 CAnimFilterOfGroup
    C3DVector     Destination;                      // +0x44
    C3DVector     StartPos;                         // +0x50
    C3DVector     PreviousPos;                      // +0x5c
    float         DistanceToTravel;                 // +0x68
    float         Speed;                            // +0x6c
    float         StartSpeed;                       // +0x70
    float         MovementDuringAnimationLoop;      // +0x74
    float         Acceleration;                     // +0x78
    float         AnimLength;                       // +0x7c
    unsigned char _pad80[0x10];
    bool          FirstFrame;                       // +0x90
    void OnActivate();
};

__forceinline float Distance(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return (float)sqrt(dx * dx + dy * dy + dz * dz);
}
void CModeDragonFlying::OnActivate()
{
    CModeBase::OnActivate();
    if (ParentThing->m_type == 1)
        ParentThing->SetSuspendAnimation(true, this);
    CTCFlagged* flagged;
    if (ParentThing->GetInterface(&flagged, 0x3a))
        flagged->m_flag = 0;
    SetThingAsNotBloodyWillableAtAll(ParentThing, true);
    DistanceToTravel = Distance(Destination, StartPos);
    Acceleration = 0.0f;
    StartSpeed = Speed;
    CTCAnimationComplex* animation = 0;
    ParentThing->GetInterface(&animation, 0x5a);
    CAnimateThing* anim = animation->GetAnimateThing();
    AnimUpdate(*anim);
    AnimLength = animation->GetAnimDuration(animation->GetAnimIndex());
    MovementDuringAnimationLoop = AnimLength * Speed;
    float frames = (float)g_GameFramesPerSecond;
    float rate = 1.0f / (AnimLength * frames);
    if (rate < 0.0f)
        rate = 0.0f;
    else if (rate > 1.0f)
        rate = 1.0f;
    anim->PlaybackSpeed = rate;
    anim->TargetPlaybackSpeed = rate;
    FirstFrame = true;
    FlapCounter = 0;
}

}