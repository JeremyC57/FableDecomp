// CTCDNone::Create(long, const C3DVector&, bool) -- create a bare thing (player 4, level persistent) on the map under the position (map 0 when the flag is set), check it carries the no-op driver, and activate it; null on any failure
// Leaf: ?Create@CTCDNone
// Retail: Fable.exe (TLC, VC7.1) @ 0x00835A20, 77 bytes, static __fastcall (ecx = def index, edx = position), `ret 4`.
// Identity: ?Create@CTCDNone@@SIPAVCThing@@JABVC3DVector@@_N@Z (Ego_r.pdb / FableWin.pdb, where it is followed by its
//   inline helper CThing::GetTCDriver<CTCDNone>); the only (long, const C3DVector&, bool) thing factory whose body is a
//   plain CThing. Callers: CTCInventoryClothing/Stats::InitialiseMannequin, CTCInventoryAbilities::OpenInventory, the
//   CIntelligentPointer<CThing> persist traits and GFSerialisePointer. The manifest label is the bare "Create".
// Callees: CWorldMap::GetMapNumberAt(const C3DVector&) const = slot 16 (+0x40) of the thing manager's world map
//   (g_pThingManager 0x013B8A1C +0x28), CThing::Create(long, long, long, bool) 0x004CA1D0 (landed, __fastcall),
//   CThing::DoActivationSequence() 0x004C9CA0.
// Layout: retail CThing UsingComponents bitset at +0x20 (the CTCDNone interface bit is bit 0 of +0x2c, interface 96),
//   PTCDriver at +0x5c (PDB +0x6c); the inline GetTCDriver<T> tests the bit and copies the cached driver (the landed
//   0x008C4810 template); its out value is dead here. Local model (no CThing header).
// Shape: the map is a zero-initialised local overwritten when the flag is clear (`xor eax,eax` before the flag test; a
//   `?:` argument puts the zero in the taken arm); a zero result declared after the create
//   call (edi), then `if (thing && GetTCDriver(&driver) && Activate) result = thing; return result;`.
class C3DVector { public: float X, Y, Z; };
class CTCDNone;
class CTCDriverBase;

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
};

class CThingManager { public:
    unsigned char _pad00[0x28];
    CWorldMap*    WorldMap;                              // +0x28 (PDB +0x34)
};
extern CThingManager* g_pThingManager;                   // 0x013B8A1C

class CThing {
public:
    virtual void v000();
    unsigned char  _pad04[0x1c];
    unsigned long  UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char  _pad44[0x18];
    CTCDriverBase* PTCDriver;                            // +0x5c (PDB +0x6c)
    unsigned char  _pad60[0x38];
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                         // 0x004C9CA0
    bool GetTCDriver(CTCDNone** out)                     // GetTCDriver<CTCDNone>
    {
        if (UsingComponents[3] & 1) {                    // interface 96
            *out = (CTCDNone*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDNone {
public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& pos, bool noMap);
};

CThing* __fastcall CTCDNone::Create(long defIndex, const C3DVector& pos, bool noMap)
{
    long map = 0;
    if (!noMap)
        map = g_pThingManager->WorldMap->GetMapNumberAt(pos);
    CThing* thing = CThing::Create(defIndex, 4, map, true);
    CThing* result = 0;
    CTCDNone* driver;
    if (thing && thing->GetTCDriver(&driver) && thing->DoActivationSequence())
        result = thing;
    return result;
}