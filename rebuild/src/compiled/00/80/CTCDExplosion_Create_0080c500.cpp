// CTCDExplosion::Create(long defIndex, const C3DVector& startPos, const C3DVector& endPos) -- create a swept explosion thing for player 4 running from startPos to endPos: set the blast ends, construct it at the start and activate it; a thing without the explosion driver is killed; null on any failure
// Leaf: ?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@0@Z
// Retail: Fable.exe @ 0x0080C500, 141 bytes (+ int3 padding), static `__fastcall` (defIndex in ecx, startPos in edx,
//   endPos on the stack), `ret 4`.
// Name: FableWin.pdb `?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@0@Z` (the two-vector overload; the one-vector one
//   is 0x0080C460). Callers 0x007FC09F, 0x0089FCA8, 0x008F51F0, 0x008F5403.
// Callees: CWorldMap::GetMapNumberAt (virtual slot 16, +0x40) on g_pThingManager (0x013B8A1C) +0x28; CThing::Create
//   0x004CA1D0; CTCDExplosion::Construct(const C3DVector&) 0x0080C250; CThing::DoActivationSequence() 0x004C9CA0;
//   CThing::Kill(bool) 0x004C9B80.
// Layout (retail CThing): UsingComponents +0x20 (word 2 bit 1 = the explosion driver), PTCDriver +0x5c; CTCDExplosion
//   StartPos +0x20, EndPos +0x2c (Ego_r.pdb).
// Shape: the 0x0080C460 / landed CTCDExperienceOrb::Create 0x00779E90 model without the map-bounds test: a `result`
//   local keeps the null in ebx (`xor ebx,ebx` after the create) and the success tail is duplicated (`mov eax,esi`).
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
    static CThing* __fastcall Create(long defIndex, const C3DVector& startPos, const C3DVector& endPos);
private:
    void Construct(const C3DVector& position);                    // 0x0080C250
};

CThing* __fastcall CTCDExplosion::Create(long defIndex, const C3DVector& startPos, const C3DVector& endPos)
{
    CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(startPos), true);
    CThing* result = 0;
    if (thing) {
        CTCDExplosion* explosion;
        if (thing->GetTCDriver(&explosion)) {
            explosion->StartPos = startPos;
            explosion->EndPos = endPos;
            explosion->Construct(startPos);
            if (thing->DoActivationSequence())
                result = thing;
        }
        else
            thing->Kill(false);
    }
    return result;
}