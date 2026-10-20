// CThingAICreature::ConstructFromParams(long defIndex, const C3DVector& position, long creationType, const CThingAICreatureInit& init) -- creature-base construction, then the owner (for creation type 5), full health from the def, drawable flag, overriding brain name, home and work buildings and auto-placing flag from the init block
// Leaf: ?ConstructFromParams@CThingAICreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830360, 305 bytes (+15 int3), `ret 10h`, bool in al.
// Callees: CThingCreatureBase::ConstructFromParams(long, const C3DVector&, long, CThingPhysical*, CThingPhysical*,
//   const CCharString&, bool) @ 0x00662880 (landed), CCharString::operator=(const CCharString&) @ 0x0099EFB0,
//   CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
//   (CIntelligentPointer<CThingBuilding> vtable 0x01244344).
// Layout: PDB CThingAICreatureInit (0x1C): OriginalThing +0, PreviousThing +4, ScriptName +8, OverridingBrainName +0xC,
//   PlayerCharacter +0x10, Drawable +0x11, PlaceAtEndOfList +0x12, EnableCreatureAutoPlacing +0x13, HomeBuilding +0x14,
//   WorkBuilding +0x18. Retail CThing PlayerNumber +0x90, Drawable bit 1 of +0x92; CThingPhysical MaxHealth +0xB0, Health
//   +0xB4 (PDB - 0x18); CThingCreatureBase PDef +0xE0; CThingCreatureDef Health +0x50 (CThingBaseDef, PDB - 4),
//   DefaultOwner +0x11C (PDB +0x124); CThingAICreature HomeBuilding +0x168, WorkBuilding +0x170, OverridingBrainName
//   +0x190, EnableCreatureAutoPlacing bit 7 of +0x1C8.
// Shape: `MaxHealth = Health = def->Health` (`fst` then `fstp`); bit fields merged from the init bools; each building is
//   the intelligent pointer's inline operator= from a temporary built from the raw pointer (the landed 0x00836A50 model).
struct C3DVector;
struct CThing;
struct CThingPhysical;
struct CThingBuilding;
struct CCharString {
    void* PData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CBuildingPointer : CBaseIntelligentPointer {          // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    CBuildingPointer(CThingBuilding* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    CBuildingPointer& operator=(const CBuildingPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CThingAICreatureInit {
    CThingPhysical* OriginalThing;              // +0x00
    CThingPhysical* PreviousThing;              // +0x04
    CCharString     ScriptName;                 // +0x08
    CCharString     OverridingBrainName;        // +0x0c
    bool            PlayerCharacter;            // +0x10
    bool            Drawable;                   // +0x11
    bool            PlaceAtEndOfList;           // +0x12
    bool            EnableCreatureAutoPlacing;  // +0x13
    CThingBuilding* HomeBuilding;               // +0x14
    CThingBuilding* WorkBuilding;               // +0x18
};
struct CThingCreatureDef {
    unsigned char _pad00[0x50];
    float Health;                               // +0x50
    unsigned char _pad54[0x11c - 0x54];
    long  DefaultOwner;                         // +0x11c
};
struct CThingCreatureBase {
    void*         __vftable;
    unsigned char _pad04[0x90 - 0x04];
    signed char   PlayerNumber;                 // +0x90
    unsigned char _pad91;                       // +0x91
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char Drawable : 1;
    unsigned char _pad93[0xb0 - 0x93];
    float         MaxHealth;                    // +0xb0
    float         Health;                       // +0xb4
    unsigned char _padb8[0xe0 - 0xb8];
    CThingCreatureDef* PDef;                    // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    unsigned char _pade4[0x160 - 0xe4];
    bool ConstructFromParams(long defIndex, const C3DVector& position, long creationType, CThingPhysical* originalThing,
                             CThingPhysical* previousThing, const CCharString& scriptName, bool placeAtEndOfList);   // 0x00662880
};
struct CThingAICreature : CThingCreatureBase {
    unsigned char    _pad160[0x168 - 0x160];
    CBuildingPointer HomeBuilding;              // +0x168
    CBuildingPointer WorkBuilding;              // +0x170
    unsigned char    _pad178[0x190 - 0x178];
    CCharString      OverridingBrainName;       // +0x190
    unsigned char    _pad194[0x1c8 - 0x194];
    bool PerformingVillageTaskFlag : 1;         // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;
    bool EnableCreatureAutoPlacing : 1;         // bit 7
    bool ConstructFromParams(long defIndex, const C3DVector& position, long creationType, const CThingAICreatureInit& init);
};

bool CThingAICreature::ConstructFromParams(long defIndex, const C3DVector& position, long creationType,
                                           const CThingAICreatureInit& init)
{
    if (CThingCreatureBase::ConstructFromParams(defIndex, position, creationType, init.OriginalThing, init.PreviousThing,
                                                init.ScriptName, init.PlaceAtEndOfList)) {
        if (creationType == 5)
            PlayerNumber = (signed char)PDef->DefaultOwner;
        MaxHealth = Health = PDef->Health;
        Drawable = init.Drawable;
        OverridingBrainName = init.OverridingBrainName;
        HomeBuilding = init.HomeBuilding;
        WorkBuilding = init.WorkBuilding;
        EnableCreatureAutoPlacing = init.EnableCreatureAutoPlacing;
        return true;
    }
    return false;
}