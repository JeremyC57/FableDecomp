// CTCSleep::GetFastTrack() -- the fast-track flag stays set only while the thing has not moved (each axis within 0.0001 of PosAtFastTrackSetting); returns the flag
// Leaf: GetFastTrack
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe 0x0092C3B0, 99 bytes (plain `ret`).
// Callees: CThing::GetPos() const @ 0x004C73D0 (const C3DVector&). Global 0.0001f @ 0x0129BA3C.
// Layout = Ego_r.pdb CTCSleep (0x20): IsReadyToSleep +0x0C, FastTrack +0x0D, LastFrameWokeUp +0x10,
// PosAtFastTrackSetting +0x14.
// Shape: FastTrack = a three-way `(float)fabs(a - b) <= eps` && chain as `? true : false` (the bool in al, stored on both arms; VC
// tail-duplicates the true arm's return), then `return FastTrack` reloads.
#include <math.h>
class C3DVector { public: float X, Y, Z; };
extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (0.0001f)
class CThing { public:
    const C3DVector& GetPos() const;           // 0x004C73D0
};
class CTCSleep { public:
    virtual void v00();
    CThing*   ParentThing;                          // +0x04 (CTCBase)
    bool      ToBeRemoved;                          // +0x08
    bool      IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    bool      IsReadyToSleep;                       // +0x0c
    bool      FastTrack;                            // +0x0d
    long      LastFrameWokeUp;                      // +0x10
    C3DVector PosAtFastTrackSetting;                // +0x14
    bool GetFastTrack();
};

bool CTCSleep::GetFastTrack()
{
    if (FastTrack) {
        const C3DVector& pos = ParentThing->GetPos();
        FastTrack = ((float)fabs(PosAtFastTrackSetting.X - pos.X) <= DELTA_NOTIONAL_ZERO
            && (float)fabs(PosAtFastTrackSetting.Y - pos.Y) <= DELTA_NOTIONAL_ZERO
            && (float)fabs(PosAtFastTrackSetting.Z - pos.Z) <= DELTA_NOTIONAL_ZERO) ? true : false;
    }
    return FastTrack;
}