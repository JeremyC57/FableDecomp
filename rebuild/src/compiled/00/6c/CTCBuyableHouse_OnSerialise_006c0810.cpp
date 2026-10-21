// CTCBuyableHouse::OnSerialise(CPersistContext* context, bool full) -- persist the house state (wife, ownership flags, rent day, dress level, money bags); on load/save of money bags fold the real bags into VirtualMoneyBags first
// Leaf: OnSerialise
// Retail 0x006C0810, 352 bytes, `ret 8`.
// Callees: GFSerialisePointer(CPersistContext*, const char*, CIntelligentPointer<CThing>*, ulong) 0x0088AE40 (`__fastcall`),
// CPersistContext::Transfer<signed char> 0x004045C0 (the bool members) and Transfer<long> 0x00410BE0,
// CTCBuyableHouse::CountMoneyBags 0x006C04D0, CTCBuyableHouse::DestroyMoneyBags(bool) 0x006C0580.
// Strings: "WifeLivingHere" 0x0125D814, "OwnedByPlayer" 0x0125D804, "IsScripted" 0x0125D7F8, "Rented" 0x0125D7F0,
// "DayNextRentIsDue" 0x0125D7DC, "CurrentDressLevel" 0x0125D7C8, "HasPreviouslyBeenBuyable" 0x0125D7AC,
// "VirtualMoneyBags" 0x0125D798, "IsResidential" 0x0125D788.
// Layout: engine/CTCBuyableHouse.h (= Ego_r.pdb, retail-confirmed: IsScripted +0x10 .. IsResidential +0x49); a local model
// is used for the CTCBase ParentThing (+4) and the members; CThing type word +0x8E; CPersistContext mode +0x18 (1, 3 = the
// money-bag folding modes).
// Shape: each default is a temporary homed in a dead argument slot (`context` for most, `full` after its test); the
// unset rent days become 0x7FFFFFFF outside modes 1/3; the money-bag fold tests a NAMED bool (`bool folding = Mode == 1 ||
// Mode == 3; if (folding) {...; Transfer} else Transfer`) which keeps the then-arm first with the cross-jumped Transfer.
struct CBaseObject;
struct CThing {
    unsigned char _pad00[0x8e];
    short         ThingType;                            // +0x8e
};
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PData;
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
};
void __fastcall GFSerialisePointer(CPersistContext* context, const char* name, CBaseIntelligentPointer* pointer,
                                   unsigned long thingType);   // 0x0088AE40
struct CDefPointeeBase;
class CTCBuyableHouse {
public:
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[4];
    CDefPointeeBase*        PDef;                       // +0x0c
    bool                    IsScripted;                 // +0x10
    bool                    OwnedByPlayer;              // +0x11
    bool                    Rented;                     // +0x12
    long                    DaySomeoneWillMoveIn;       // +0x14
    long                    DayNextRentIsDue;           // +0x18
    long                    CurrentDressLevel;          // +0x1c
    long                    MaxDressLevel;              // +0x20
    CBaseIntelligentPointer MainSignPost;               // +0x24
    CBaseIntelligentPointer DressingSignPost;           // +0x2c
    long                    FrameToNextCheckIfWeNeedToCreateAnything;   // +0x34
    CBaseIntelligentPointer WifeLivingHere;             // +0x38
    bool                    InitiallySetupDressLevel;   // +0x40
    long                    VirtualMoneyBags;           // +0x44
    bool                    HasPreviouslyBeenBuyable;   // +0x48
    bool                    IsResidential;              // +0x49
    long CountMoneyBags() const;                        // 0x006C04D0
    void DestroyMoneyBags(bool giveToHero);             // 0x006C0580
    void OnSerialise(CPersistContext* context, bool full);
};

void CTCBuyableHouse::OnSerialise(CPersistContext* context, bool full)
{
    GFSerialisePointer(context, "WifeLivingHere", &WifeLivingHere, ParentThing->ThingType);
    context->Transfer("OwnedByPlayer", OwnedByPlayer, false);
    context->Transfer("IsScripted", IsScripted, false);
    context->Transfer("Rented", Rented, false);
    context->Transfer("DayNextRentIsDue", DayNextRentIsDue, 0L);
    context->Transfer("CurrentDressLevel", CurrentDressLevel, 0L);
    if (full)
        context->Transfer("HasPreviouslyBeenBuyable", HasPreviouslyBeenBuyable, false);
    if (CurrentDressLevel == -1)
        CurrentDressLevel = 0;
    if (context->Mode != 1 && context->Mode != 3) {
        if (DaySomeoneWillMoveIn == 0)
            DaySomeoneWillMoveIn = 0x7fffffff;
        if (DayNextRentIsDue == 0)
            DayNextRentIsDue = 0x7fffffff;
    }
    bool folding = context->Mode == 1 || context->Mode == 3;
    if (folding) {
        if (Rented) {
            VirtualMoneyBags += CountMoneyBags();
            DestroyMoneyBags(false);
        }
        context->Transfer("VirtualMoneyBags", VirtualMoneyBags, 0L);
    } else {
        context->Transfer("VirtualMoneyBags", VirtualMoneyBags, 0L);
    }
    context->Transfer("IsResidential", IsResidential, true);
}