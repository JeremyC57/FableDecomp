// CTCDExplosion::Create(long defIndex, const C3DVector& position) -- create an explosion thing for player 4 at a position inside the world map: set both ends of the blast to the position, construct and activate it; a thing without the explosion driver is killed; null on any failure
// Leaf: ?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@@Z
// Retail: Fable.exe @ 0x0080C460, 154 bytes (+ int3 padding), static `__fastcall` (defIndex in ecx, position in edx),
//   plain `ret`.
// Name: FableWin.pdb `?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@@Z` (the one-vector overload; the two-vector one is
//   0x0080C500). 20 callers (CTCExplodingObject::Explode 0x00750DE5, CTCLightningOrb::FrameUpdate 0x007F25DC, the creature
//   actions 0x008C4856.., all landed with a `CThing* __fastcall (long, const C3DVector&)` declaration).
// Callees: CWorldMap::IsPosInMap(const C3DVector&) 0x004FC600 and CWorldMap::GetMapNumberAt (virtual slot 16, +0x40) on
//   the thing manager's world map (g_pThingManager 0x013B8A1C, +0x28); CThing::Create(long, long, long, bool)
//   0x004CA1D0; CTCDExplosion::Construct(const C3DVector&) 0x0080C250 (landed as CTCDCameraPoint::Construct, a byte-twin
//   label); CThing::DoActivationSequence() 0x004C9CA0; CThing::Kill(bool) 0x004C9B80.
// Layout (retail CThing): UsingComponents +0x20 (word 2 bit 1 = the explosion driver), PTCDriver +0x5c; CTCDExplosion
//   StartPos +0x20, EndPos +0x2c (Ego_r.pdb).
// Shape: the landed CTCDExperienceOrb::Create 0x00779E90 model with the map-bounds test first: a `result` local (null, set
//   to the thing on success) keeps the null in ebx (`xor ebx,ebx`) and VC duplicates the success tail (`mov eax,edi`);
//   an early `return thing` drops the zero register. The C3DVector member assignments are the implicit operator= next to
//   C3DVector's user copy ctor (`lea eax,[ecx+20h] ; mov edx,esi`).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;      // slot 16 (+0x40)
    bool IsPosInMap(const C3DVector& pos) const;                  // 0x004FC600
};
class CThingManager {
public:
    unsigned char _pad_0x00[0x28];
    CWorldMap* WorldMap;                                          // +0x28 (retail; PDB +0x34)
};
extern CThingManager* g_pThingManager;                            // 0x013B8A1C

class CTCDExplosion;

class CThing {
public:
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                             // +0x20 bitset
    unsigned char _pad_0x44[0x5c - 0x44];
    void* PTCDriver;                                              // +0x5c
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                                  // 0x004C9CA0
    void Kill(bool checkLocked);                                  // 0x004C9B80
    bool GetTCDriver(CTCDExplosion** out)
    {
        if (UsingComponents[2] & 0x2) {
            *out = (CTCDExplosion*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDExplosion {
public:
    unsigned char _pad_0x00[0x20];
    C3DVector StartPos;                                           // +0x20
    C3DVector EndPos;                                             // +0x2c
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);
private:
    void Construct(const C3DVector& position);                    // 0x0080C250
};

CThing* __fastcall CTCDExplosion::Create(long defIndex, const C3DVector& position)
{
    CThing* result = 0;
    if (g_pThingManager->WorldMap->IsPosInMap(position)) {
        CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(position), true);
        if (thing) {
            CTCDExplosion* explosion;
            if (thing->GetTCDriver(&explosion)) {
                explosion->StartPos = position;
                explosion->EndPos = position;
                explosion->Construct(position);
                if (thing->DoActivationSequence())
                    result = thing;
            }
            else
                thing->Kill(false);
        }
    }
    return result;
}