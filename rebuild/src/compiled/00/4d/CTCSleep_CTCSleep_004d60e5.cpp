// CTCSleep::CTCSleep(CThing*) -- base CTCBase(thing), own vtable, IsReadyToSleep/FastTrack false, LastFrameWokeUp = -1000000
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D60E5, length 39 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (stores the parent thing at +4, clears the two flags
// at +8/+9, vtable 0x0125BE0C = CTCBase; the manifest's "CEngineSubPrimitive2DClockSprite" label is
// propagated). Vtable 0x0123DD94 = CTCSleep (rtti-vtables.tsv).
// PDB CTCSleep (0x20 bytes): IsReadyToSleep +0xc, FastTrack +0xd, LastFrameWokeUp +0x10,
// PosAtFastTrackSetting (C3DVector) +0x14 -- the vector is left uninitialised. 0xFFF0BDC0 is
// -1000000 ("woke up a long time ago"). The engine header for CTCBase is a flat POD, hence `_Calls`.
// The pass-through `push dword ptr [esp+8]` and immediate byte stores mark a size-optimised TU.
#pragma optimize("s", on)
class CThing;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

class C3DVector { public: float X, Y, Z; };

class CTCSleep : public CTCBase_Calls { public:
    bool      IsReadyToSleep;                               // +0x0c
    bool      FastTrack;                                    // +0x0d
    long      LastFrameWokeUp;                              // +0x10
    C3DVector PosAtFastTrackSetting;                        // +0x14 (not initialised)
    virtual void v00();                                     // own vtable 0x0123DD94
    CTCSleep(CThing* parent);
};

CTCSleep::CTCSleep(CThing* parent)
    : CTCBase_Calls(parent)
    , IsReadyToSleep(false)
    , FastTrack(false)
    , LastFrameWokeUp(-1000000)
{
}