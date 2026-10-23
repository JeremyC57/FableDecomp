// CThingPlayerCreature::vfunc_34(CThingHighlightInfo* info) -- vtable slot 34: fill a highlight record from the owning player's def colour (opaque black when the player is not initialised), the creature's clamped [0, 1] value, enabled, 0.7 scale; returns false
// Leaf: vfunc_34
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9D10, length 172 bytes, `ret 4`, bool in al; slot 34 of vtable
//   0x012457FC = CThingPlayerCreature (rtti-vtables.tsv; the slot's PDB name is not recovered).
// Callees: the game object's player manager getter @ 0x004C79F0 (`[[0x013B8A1C]+0x30]`, a CThing member; manifest
//   "CViewBase::GetDisplayEngine" is a byte twin), CPlayerManager::GetPlayer(long) @ 0x00449910, CInitBaseClass::
//   IsInitialised @ 0x0099A330, the creature's normalised value getter @ 0x006A4E40 (manifest label propagated; name
//   unproven), the def's virtual Destroy (slot 1).
// Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f; 0.7f is stored as an immediate.
// Layout: retail CThing PlayerNumber +0x90; retail CPlayer PlayerDef +0x21c (PDB +0x224); CPlayerDef colour at +0x40;
//   the record: scale +4, value +8, colour +0xc, enabled +0x10 (class not identified).
// Shape: the player def is a by-value CDefPointer copy (add-ref with its null test, the def's inline Release on
//   destruction); the fallback CRGBColour(0, 0, 0, 255) is built byte by byte in the frame; the value clamp is
//   `v = x; if (v < 0) v = 0; else if (v > 1) v = 1;` on the record field.
struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CPlayerDef : CDefPointeeBase {
    unsigned char _pad08[0x40 - 0x08];
    CRGBColour Colour;                          // +0x40
};
struct CInitBaseClass {
    bool IsInitialised();                       // 0x0099A330
};
struct CPlayer : CInitBaseClass {
    unsigned char _pad00[0x21c];
    CDefPointer<CPlayerDef> PlayerDef;          // +0x21c
};
struct CPlayerManager {
    CPlayer* GetPlayer_00449910(long number);            // 0x00449910
};
struct CThingHighlightInfo {
    unsigned char _pad00[4];
    float Scale;                                // +0x04
    float Value;                                // +0x08
    CRGBColour Colour;                          // +0x0c
    bool Enabled;                               // +0x10
};

class CThingPlayerCreature {
public:
    unsigned char _pad00[0x90];
    signed char PlayerNumber;                   // +0x90
    CPlayerManager* GetPlayerManager_004c79f0();   // 0x004C79F0
    float GetNormalisedValue_006a4e40();        // 0x006A4E40
    bool vfunc_34(CThingHighlightInfo* info);
};

bool CThingPlayerCreature::vfunc_34(CThingHighlightInfo* info)
{
    CPlayer* player = GetPlayerManager_004c79f0()->GetPlayer_00449910(PlayerNumber);
    CRGBColour colour;
    if (player->IsInitialised() == true) {
        CDefPointer<CPlayerDef> def(player->PlayerDef);
        colour = def->Colour;
    } else {
        colour = CRGBColour(0, 0, 0, 255);
    }
    float value = GetNormalisedValue_006a4e40();
    info->Value = value;
    if (value < 0.0f)
        info->Value = 0.0f;
    else if (value > 1.0f)
        info->Value = 1.0f;
    info->Colour = colour;
    info->Enabled = true;
    info->Scale = 0.7f;
    return false;
}