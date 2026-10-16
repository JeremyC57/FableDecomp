// CTCFlashOnHit::FrameUpdate() -- keep the parent self-illuminated while the flash lasts, tint it red each frame, and when the timer runs out remove the tint and the flash component
// Leaf: FrameUpdate
//
// Retail 0x007E33B0, 104 bytes (plain `ret`; vtable 0x0123E034 slot 9, the CTCBase FrameUpdate slot;
// the catalog "CTCFlashOnHit::vfunc_9" is the slot name). Callees (Ego_r.pdb names on NGraphicAppearance::
// CTCGraphicAppearance): SetSelfIlluminating(bool) @ 0x004BFA20, RemoveTintColour(void*) @ 0x004BFAE0
// (manifest "CParticleSystem::Kill", propagated), SetTintColour(const CRGBColour&, void*) @ 0x004C00B0;
// CThing::RemoveTC(long) @ 0x004C9840 (component 0xfb, this one).
// Layout = Ego_r.pdb CTCFlashOnHit (0x1c): Timer +0x18; retail CThing keeps its graphic appearance at +0x64.
// Shape: `Timer > 0` pushed straight from `setg al`; `Timer-- == 0` (old value tested, decrement stored
// before the branch); the `this` owner argument is pushed once above the branch for both appearance
// calls; the red tint is a CRGBColour(255, 0, 0, 255) temporary whose ctor assigns R,G,B,A in its body
// (byte stores +2/+1/+0/+3).
struct CRGBColour {
    unsigned char B;                                        // +0
    unsigned char G;                                        // +1
    unsigned char R;                                        // +2
    unsigned char A;                                        // +3
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CTCGraphicAppearance {
    void SetSelfIlluminating(bool on);                      // 0x004BFA20
    void RemoveTintColour(void* owner);                     // 0x004BFAE0
    void SetTintColour(const CRGBColour& colour, void* owner);   // 0x004C00B0
};

struct CThing {
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    void RemoveTC(long type);                               // 0x004C9840
};

struct CTCFlashOnHit {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    long    FadeInTime;                                     // +0x0c
    long    FadeOutTime;                                    // +0x10
    long    LifeTime;                                       // +0x14
    long    Timer;                                          // +0x18
    void FrameUpdate();
};

void CTCFlashOnHit::FrameUpdate()
{
    ParentThing->PGraphicAppearance->SetSelfIlluminating(Timer > 0);
    if (Timer-- == 0)
    {
        ParentThing->PGraphicAppearance->RemoveTintColour(this);
        ParentThing->RemoveTC(0xfb);
    }
    else
        ParentThing->PGraphicAppearance->SetTintColour(CRGBColour(255, 0, 0, 255), this);
}