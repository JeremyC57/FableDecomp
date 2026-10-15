// CTCAtmosPlayer::CTCAtmosPlayer(CThing* thing) -- CTCBase(thing), PSoundEmitter and AtmosDefIndex cleared, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E35A0, length 33 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated).
// Vtable 0x01270484 = CTCAtmosPlayer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCAtmosPlayer (20 bytes): CTCBase 0xc, PSoundEmitter +0xc, AtmosDefIndex +0x10.
// engine/CTCBase.h holds the layout only, so the base is modelled as `CTCBase_Calls` (as the landed
// 0x007AEE10). Shape: both zeroes share one `xor eax,eax` register and are stored before the vtable.
struct CThing;
struct CASoundEmitter;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CTCAtmosPlayer : CTCBase_Calls {                     // vtable 0x01270484
    CASoundEmitter* PSoundEmitter;                          // +0x0c
    long            AtmosDefIndex;                          // +0x10
    virtual void v00();
    CTCAtmosPlayer(CThing* thing);
};

CTCAtmosPlayer::CTCAtmosPlayer(CThing* thing)
    : CTCBase_Calls(thing)
    , PSoundEmitter(0)
    , AtmosDefIndex(0)
{
}