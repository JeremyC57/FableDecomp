// CTCDInternalMarker::Create(long, const C3DVector&) -- create an internal-marker thing for player 4 on the map under the position, construct its driver there and activate it; null on failure
// Leaf: ?Create@CTCDInternalMarker@@SIPAVCThing@@JABVC3DVector@@@Z
// Retail 0x00870BB0, 0x4E bytes (+ int3 padding), static __fastcall (def index in ecx, position in edx, plain `ret`).
// Catalog label `Create` (_global); Ego_r names ?Create@CTCDInternalMarker@@SIPAVCThing@@JABVC3DVector@@@Z and the driver test
// is interface type 0xA2 = CTCDInternalMarker (GetInterfaceType, vtable slot 21: presence word 5 bit 2, `test byte ptr
// [esi+34h],4`). Callers 0x006834E0, 0x00783B1E, 0x00934B2A.
// Callees: CWorldMap::GetMapNumberAt (slot 16, +0x40) on g_pThingManager (0x013B8A1C) +0x28, CThing::Create(long, long, long,
// bool) @ 0x004CA1D0, CTCDInternalMarker::Construct(const C3DVector&) @ 0x00870B30 (manifest "CTCDCameraPoint::Construct" is
// a propagation; Ego_r ?Construct@CTCDInternalMarker@@AAEXABVC3DVector@@@Z), CThing::DoActivationSequence @ 0x004C9CA0.
// Shape: the landed CTCDExperienceOrb::Create 0x00779E90 without the Kill branch: `push 1` (the last Create argument) goes
// before the GetMapNumberAt call that yields the third, the result local lives in ebx (`xor ebx,ebx`), the driver pointer
// is PTCDriver +0x5C read through the GetTCDriver out-pointer helper.
class C3DVector { public: float X, Y, Z; };

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;      // slot 16 (+0x40)
};

class CThingManager {
public:
    unsigned char _pad_0x00[0x28];
    CWorldMap* WorldMap;                                          // +0x28
};
extern CThingManager* g_pThingManager;                            // 0x013B8A1C

class CTCDInternalMarker;

class CThing {
public:
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                             // +0x20 bitset
    unsigned char _pad_0x44[0x5c - 0x44];
    void* PTCDriver;                                              // +0x5c
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                                  // 0x004C9CA0
    bool GetTCDriver(CTCDInternalMarker** out)
    {
        if (UsingComponents[5] & 0x4) {
            *out = (CTCDInternalMarker*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDInternalMarker {
public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);
private:
    void Construct(const C3DVector& position);                    // 0x00870B30
};

CThing* __fastcall CTCDInternalMarker::Create(long defIndex, const C3DVector& position)
{
    CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(position), true);
    CThing* result = 0;
    if (thing) {
        CTCDInternalMarker* marker;
        if (thing->GetTCDriver(&marker)) {
            marker->Construct(position);
            if (thing->DoActivationSequence())
                result = thing;
        }
    }
    return result;
}