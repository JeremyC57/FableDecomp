// CThingAICreature::GetHighlightInfo_slot34(SThingHighlightInfo* info) -- fills the highlight record: the owning player's def colour (opaque) when the creature belongs to a local player, else opaque black; alpha from the creature's normalised time clamped to 0..1; scale 0.7; flagged valid; reports false
// Grade: DIFFER(ours v retail) -- 206v201: with the landed twin model (CDefPointer copy + CRGBColour ctor) the def copy is re-read from the frame (`mov esi,[esp+8]`) and the branch layout shifts; the byte-wise form (201v201) differs only in the `fcom`/`fst` order after the time call
// Leaf: GetHighlightInfo_slot34
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830CB0, 199 bytes (+9 int3). `ret 4`, bool in al. CThingAICreature vtable 0x0127293C slot 34 (+0x88;
//         CThing's own slot is 0x004CD220). Catalog name CThingAICreature::vfunc_34; the twin CThingBuilding slot is 0x0082D1D0.
// Callees: CThing::GetPlayerManager() 0x004C79F0 (manifest "CViewBase::GetDisplayEngine" propagated: it is g_pThingManager->
//          PlayerManager), CPlayerManager::GetPlayer(long) 0x00449910, the player's bool query 0x0099A330 (manifest
//          "CTCInventoryBase::GetCanItemBeQuickAssigned" propagated; named IsLocal here), CDefClassBase virtual slot 1 (Delete)
//          when the def copy was the last reference, the creature's normalised time 0x006A4E40 (manifest "CParticleSystem::
//          GetNormalisedTime" propagated). Floats: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Shape: the colour is a four-byte frame record (B, G, R, A) written byte by byte by the CRGBColour constructor in both arms;
//        the player's def pointer is a by-value CDefPointer copy (add-ref under a null test, inline Release on destruction);
//        the time is stored (`fst`) before the two compares (`test ah,5 ; jp` for `< 0`, `test ah,41h ; jne` for `> 1`).
// Layout: CThing PlayerNumber +0x90; CPlayer::PlayerDef +0x21C (Ego_r.pdb +0x224); CPlayerDef::Colour +0x40 (PDB +0x44,
//         def classes pack to 1).
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
    long       _unused;                              // +0x00
    float      Scale;                                // +0x04
    float      Alpha;                                // +0x08
    CRGBColour Colour;                               // +0x0c
    bool       Valid;                                // +0x10
};
struct CThingAICreature {
    unsigned char _pad00[0x90];
    signed char   PlayerNumber;                      // +0x90
    CPlayerManager* GetPlayerManager();              // 0x004C79F0
    float GetNormalisedTime();                       // 0x006A4E40 (manifest label propagated)
    bool GetHighlightInfo_slot34(SThingHighlightInfo* info);
};
bool CThingAICreature::GetHighlightInfo_slot34(SThingHighlightInfo* info)
{
    CRGBColour colour;
    long playerNumber = PlayerNumber;
    CPlayer* player;
    if (playerNumber != -1 && (player = GetPlayerManager()->GetPlayer(playerNumber))->IsLocal() == true) {
        CDefPointer<CPlayerDef> def(player->PlayerDef);
        colour = CRGBColour(def->Colour.R, def->Colour.G, def->Colour.B, 255);
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
    info->Scale = 0.7f;
    return false;
}
