// CTCDExperienceOrb::Create(long defIndex, const C3DVector& position) -- create a level-persistent experience-orb thing for player 4 on the map under the position, construct its orb driver there and activate it; a thing without the orb driver is killed; null on any failure
// Leaf: ?Create@CTCDExperienceOrb
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00779E90, 93 bytes (+3 int3), static `__fastcall` (defIndex in ecx,
//   position in edx), plain `ret`.
// Name: FableWin.pdb `?Create@CTCDExperienceOrb@@SIPAVCThing@@JABVC3DVector@@@Z`; callers CTCDExperienceOrb::Create
//   0x0077A890 (the other, CThing&-taking overload) and CGameScriptInterface::CreateExperienceOrb 0x0089FBB0 (landed, which
//   declares this address as `CThing* __fastcall (long, const C3DVector&)`). Label correction: catalog "Create" ->
//   CTCDExperienceOrb::Create(long, const C3DVector&).
// Callees: CWorldMap::GetMapNumberAt (virtual slot 16, +0x40) on the thing manager's world map (g_pThingManager
//   0x013B8A1C, +0x28); CThing::Create(long, long, long, bool) 0x004CA1D0 (landed); CTCDExperienceOrb::Construct(const
//   C3DVector&) 0x00779D20; CThing::DoActivationSequence() 0x004C9CA0; CThing::Kill(bool) 0x004C9B80.
// Layout (retail CThing): UsingComponents +0x20 (word 3 bit 9 = the experience-orb driver), PTCDriver +0x5c (the landed
//   CombineWithExperienceOrb 0x0077A130 probe).
// Shape: the `true` argument is pushed before the map lookup is evaluated; GetTCDriver<CTCDExperienceOrb>(&orb) is the
//   inlined bit test with no null test of the driver. A `result` local (null, set to the thing on success) is what keeps
//   the null in ebx across the calls (`xor ebx,ebx` ... `mov eax,ebx`); `return thing` / `return 0` arms give `xor eax,eax`
//   and early returns give a `neg al ; sbb ; and` select.
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
    CWorldMap* WorldMap;                                          // +0x28 (retail; PDB +0x34)
};
extern CThingManager* g_pThingManager;                            // 0x013B8A1C

class CTCDExperienceOrb;

class CThing {
public:
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                             // +0x20 bitset
    unsigned char _pad_0x44[0x5c - 0x44];
    void* PTCDriver;                                              // +0x5c
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                                  // 0x004C9CA0
    void Kill(bool checkLocked);                                  // 0x004C9B80
    bool GetTCDriver(CTCDExperienceOrb** out)
    {
        if (UsingComponents[3] & 0x200) {
            *out = (CTCDExperienceOrb*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDExperienceOrb {
public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);
    void Construct(const C3DVector& position);                    // 0x00779D20
};

CThing* __fastcall CTCDExperienceOrb::Create(long defIndex, const C3DVector& position)
{
    CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(position), true);
    CThing* result = 0;
    if (thing) {
        CTCDExperienceOrb* orb;
        if (thing->GetTCDriver(&orb)) {
            orb->Construct(position);
            if (thing->DoActivationSequence())
                result = thing;
        }
        else
            thing->Kill(false);
    }
    return result;
}