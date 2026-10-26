// NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::Update() -- track the bar's thing: refresh value/max and a red-to-green colour from its health, or re-acquire it by UID once its map's region is the current one
// Leaf: Update
// Retail 0x006463FF, 360 bytes, size-optimised TU (#pragma optimize("s"): ebp frame, `leave`, unaligned entry).
// Callees: IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const> > 0x00655D04 (__fastcall), GetPItem 0x00A01B50,
// the game global 0x013B86A0 (+0x24 world: GetWorldMap slot 13, GetThingManager slot 3), CWorldMap::
// GetRegionNumberMapIsIn 0x004FC190, the current region 0x004FB150, CThingManager::GetThingWithUID 0x0051ED80,
// IS_THING_ALIVE<CThing*> 0x0040DCE0 (__fastcall), CIntelligentPointer(CThing*) 0x00655145, operator= 0x00655180,
// ~CBaseIntelligentPointer 0x00A01C10. Pool: 1.0f 0x0122DED8, 255.0f 0x01230014.
// Layout: Ego_r.pdb CQuestInfoElementBar shifted by -4 up to ThingUID: Value +0x1C, MaxValue +0x20, MinValue +0x24,
// Colour +0x28 (B,G,R,A), ThingValid +0x38, PThing +0x39 (unaligned: pack(1)), ThingUID +0x48, ThingMapNumber +0x50.
// Shape: the current-region call is the LEFT operand of `==` (VC keeps the source order of the compare and calls the
// right side first); the re-acquired pointer is a named local assigned with operator= (lea/push); a same-TU
// auto_inline(off) body of the raw IS_THING_ALIVE tells VC the address-taken `thing` does not escape, so it shares
// its frame word with the region compare's spill (0xC frame instead of 0x10).
#pragma optimize("s", on)
class CThing;
class CThingPhysical { public:
    unsigned char _pad00[0x8e];
    short         MapNumber;                       // +0x8e
    unsigned char _pad90;
    unsigned char Flags91;                         // +0x91 (0x20: hidden)
    unsigned char _pad92[0xb0 - 0x92];
    float         MaxHealth;                       // +0xb0
    float         Health;                          // +0xb4
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(CThing* thing);            // 0x00655145
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x00655180

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingPhysical>* p);   // 0x00655D04
#pragma auto_inline(off)
bool __fastcall IS_THING_ALIVE_RAW(CThing* const* p)                            // 0x0040DCE0 (same-TU stand-in)
{
    const CThingPhysical* thing = (const CThingPhysical*)*p;
    return thing && !(thing->Flags91 & 1);
}
#pragma auto_inline(on)
class CThingManager { public:
    CThing* GetThingWithUID(unsigned __int64 uid);  // 0x0051ED80
};
class CWorldMap { public:
    long GetCurrentRegionNumber();                  // 0x004FB150
public:
    long GetRegionNumberMapIsIn(long) const;   // 0x004FC190
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();      // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};
class CGame { public: unsigned char _pad[0x24]; CWorld* World; };   // +0x24
extern CGame* g_pGame;                             // 0x013B86A0
class CRGBColour { public: unsigned char B, G, R, A; };
#pragma pack(push, 1)
struct CQuestInfoElementBar {
    void*         __vftable;                       // +0x00
    unsigned char _pad04[0x1c - 0x04];
    float         Value;                           // +0x1c
    float         MaxValue;                        // +0x20
    float         MinValue;                        // +0x24
    CRGBColour    Colour;                          // +0x28
    CRGBColour    FadeColour;                      // +0x2c
    long          Graphic1;                        // +0x30
    long          Graphic2;                        // +0x34
    bool          ThingValid;                      // +0x38
    CIntelligentPointer<CThingPhysical> PThing;    // +0x39
    unsigned char _pad41[7];
    unsigned __int64 ThingUID;                     // +0x48
    long          ThingMapNumber;                  // +0x50
    bool Update();
};
#pragma pack(pop)

bool CQuestInfoElementBar::Update()
{
    if (IS_THING_ALIVE(&PThing)) {
        if (PThing.GetPItem()->Flags91 & 0x20)
            return true;
        ThingMapNumber = PThing.GetPItem()->MapNumber;
        Value = PThing.GetPItem()->Health;
        MinValue = 0.0f;
        MaxValue = PThing.GetPItem()->MaxHealth;
        if (MaxValue > MinValue && Value >= MinValue && MaxValue >= Value) {
            float ratio = Value / (MaxValue - MinValue);
            if (ratio > 1.0f)
                ratio = 1.0f;
            Colour.R = (unsigned char)((1.0f - ratio) * 255.0f);
            Colour.G = (unsigned char)(ratio * 255.0f);
            Colour.B = 0;
        }
    } else if (ThingValid && ThingUID && ThingMapNumber) {
        CWorld* world = g_pGame->World;
        CWorldMap* map = world->GetWorldMap();
        if (map->GetCurrentRegionNumber() == map->GetRegionNumberMapIsIn(ThingMapNumber)) {
            CThing* thing = world->GetThingManager()->GetThingWithUID(ThingUID);
            if (IS_THING_ALIVE_RAW(&thing)) {
                CIntelligentPointer<CThingPhysical> found(thing);
                PThing = found;
            }
        }
    }
    return true;
}