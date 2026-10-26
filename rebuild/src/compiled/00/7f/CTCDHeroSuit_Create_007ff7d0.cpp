// CTCDHeroSuit::Create(long defIndex, const C3DVector& position) -- create a level-persistent hero-suit thing for player 4 on map 0 and activate it; a thing without the hero-suit driver is killed; null on any failure
// Leaf: ?Create@CTCDHeroSuit@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FF7D0, 61 bytes, static `__fastcall` (defIndex in ecx, position in
//   edx -- unused, overwritten by the player constant), plain `ret`. Caller: CTCInventoryClothing 0x005B3781 (ecx = a
//   def index from its list, edx = the owner's GetPosition()).
// Name: Ego_r.pdb `?Create@CTCDHeroSuit@@SIPAVCThing@@JABVC3DVector@@@Z` (donor 0x007F2E80, followed by
//   CTCDHeroSuit::OnCreate, as retail 0x007FF7D0 is followed by CTCDHeroSuit vtable slot 4 0x007FF8A0); the catalog's
//   bare "Create" is the leaf.
// Callees: CThing::Create(long, long, long, bool) 0x004CA1D0 (landed, static __fastcall), CThing::DoActivationSequence()
//   0x004C9CA0, CThing::Kill(bool) 0x004C9B80.
// Layout (retail CThing): UsingComponents bitset +0x20 -- word 2 bit 29 (`test dword ptr [esi+28h],20000000h`) marks the
//   hero-suit driver; the driver pointer itself is never read (no Construct call), so the inline GetTCDriver reduces to
//   the bit test.
// Shape: the landed CTCDExperienceOrb::Create 0x00779E90 form without the map lookup and the Construct call: a `result`
//   local preloaded with 0 (`xor edi,edi`) is what the kill and failure paths return; the success path returns the
//   thing register directly.
class C3DVector { public: float X, Y, Z; };

class CTCDHeroSuit;

class CThing {
public:
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                             // +0x20 bitset
    unsigned char _pad_0x44[0x5c - 0x44];
    void* PTCDriver;                                              // +0x5c
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                                  // 0x004C9CA0
    void Kill(bool checkLocked);                                  // 0x004C9B80
    bool GetTCDriver(CTCDHeroSuit** out)
    {
        if (UsingComponents[2] & 0x20000000) {
            *out = (CTCDHeroSuit*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDHeroSuit {
public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);
};

CThing* __fastcall CTCDHeroSuit::Create(long defIndex, const C3DVector& position)
{
    CThing* thing = CThing::Create(defIndex, 4, 0, true);
    CThing* result = 0;
    if (thing) {
        CTCDHeroSuit* suit;
        if (thing->GetTCDriver(&suit)) {
            if (thing->DoActivationSequence())
                result = thing;
        }
        else
            thing->Kill(false);
    }
    return result;
}