// CTCBoastingArea::CTCBoastingArea(CThing* thing) -- CTCBase(thing), own vtable, HeroWithinRadiusState = 0, Radius = 3.0, AnticipationRadius = 10.0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782770, length 46 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated).
// Identity: vtable 0x0126B314 = CTCBoastingArea (rtti-vtables.tsv; its slot 14 is 0x00782A90, the
// OnKill of this chunk); the manifest label "CTCBed::CTCBed" is propagated.
// Layout = Ego_r.pdb CTCBoastingArea (24 bytes): HeroWithinRadiusState +0xc, Radius +0x10,
// AnticipationRadius +0x14. Shape (as the landed 0x007AEE10): constant initialisers are immediate
// stores after the vtable store (0x40400000 = 3.0f, 0x41200000 = 10.0f).
struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

enum EHeroWithinRadiusState { HERO_OUTSIDE_RADIUS = 0 };

struct CTCBoastingArea : CTCBase_Calls {                    // vtable 0x0126B314
    EHeroWithinRadiusState HeroWithinRadiusState;           // +0x0c
    float                  Radius;                          // +0x10
    float                  AnticipationRadius;              // +0x14
    virtual void v00();
    CTCBoastingArea(CThing* thing);
};

CTCBoastingArea::CTCBoastingArea(CThing* thing)
    : CTCBase_Calls(thing)
    , HeroWithinRadiusState(HERO_OUTSIDE_RADIUS)
    , Radius(3.0f)
    , AnticipationRadius(10.0f)
{
}