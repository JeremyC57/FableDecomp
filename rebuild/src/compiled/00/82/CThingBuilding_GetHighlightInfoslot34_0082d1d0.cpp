// CThingBuilding::GetHighlightInfo_slot34(SThingHighlightInfo* info) -- fills the highlight record: the owning player's def colour when the player is local, else opaque black; alpha from the building's normalised time clamped to 0..1; scale 1; flagged valid; reports false
// Leaf: GetHighlightInfo_slot34
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D1D0, 187 bytes (+5 int3). `ret 4`, bool in al. CThingBuilding vtable 0x0124509C slot 34 (+0x88).
//         Catalog name CThingBuilding::vfunc_34; the creature twin is 0x00830CB0 (which also tests PlayerNumber and uses 0.7).
// Callees: CThing::GetPlayerManager() 0x004C79F0 (manifest "CViewBase::GetDisplayEngine" propagated), CPlayerManager::GetPlayer(long)
//          0x00449910, the player's bool query 0x0099A330 (manifest "CTCInventoryBase::GetCanItemBeQuickAssigned" propagated; IsLocal
//          here), CDefClassBase virtual slot 1 (Delete), the building's normalised time 0x006A4E40 (manifest "CParticleSystem::
//          GetNormalisedTime" propagated). Floats: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Shape: the local colour is the def's four-byte colour copied whole (kept in esi) or the frame record of opaque black; the
//        1.0f constant is shared by the scale store and the clamp (edx); the def copy adds a reference under a null test and
//        releases through a register decrement without one.
// Layout: CThing PlayerNumber +0x90; CPlayer::PlayerDef +0x21C; CPlayerDef::Colour +0x40 (def classes pack to 1).
struct CRGBColour {                                  // retail byte order B, G, R, A (landed twin 0x006A9D10)
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CPlayerDef : CDefPointeeBase {
    unsigned char _pad08[0x40 - 0x08];
    CRGBColour    Colour;                            // +0x40
};
struct CPlayer {
    unsigned char          _pad00[0x21c];
    CDefPointer<CPlayerDef> PlayerDef;               // +0x21c
    bool IsLocal();                                  // 0x0099A330 (manifest label propagated; landed twin names it CInitBaseClass::IsInitialised)
};
struct CPlayerManager {
    CPlayer* GetPlayer(long playerNumber);           // 0x00449910
};
struct SThingHighlightInfo {                         // descriptive
    long       _unused;
    float      Scale;                                // +0x04
    float      Alpha;                                // +0x08
    CRGBColour Colour;                               // +0x0c
    bool       Valid;                                // +0x10
};
struct CThingBuilding {
    unsigned char _pad00[0x90];
    signed char   PlayerNumber;                      // +0x90
    CPlayerManager* GetPlayerManager();              // 0x004C79F0
    float GetNormalisedTime();                       // 0x006A4E40 (manifest label propagated)
    bool GetHighlightInfo_slot34(SThingHighlightInfo* info);
};
bool CThingBuilding::GetHighlightInfo_slot34(SThingHighlightInfo* info)
{
    CRGBColour colour;
    CPlayer* player = GetPlayerManager()->GetPlayer(PlayerNumber);
    if (player->IsLocal() == true) {
        CDefPointer<CPlayerDef> def(player->PlayerDef);
        colour = def->Colour;
    } else {
        colour = CRGBColour(0, 0, 0, 255);
    }
    float time = GetNormalisedTime();
    info->Alpha = time;
    if (time < 0.0f)
        info->Alpha = 0.0f;
    else if (time > 1.0f)
        info->Alpha = 1.0f;
    info->Colour = colour;
    info->Valid = true;
    info->Scale = 1.0f;
    return false;
}