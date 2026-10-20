#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CThing { int id; };
struct CThingPhysical;
struct CThingBuilding : CThing { };
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
    CBuildingPointer() : CBaseIntelligentPointer() { }   // test-only default ctor
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
// ---- test doubles ----
CCharString& CCharString::operator=(const CCharString& o) { PData = o.PData; return *this; }
static int g_ipLive = 0;
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : PItem(0) { ++g_ipLive; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_ipLive; }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CThing* p) { PItem = p; }
void CBuildingPointer::v00() {}
static bool g_baseOk = true; static long g_baseType; static const CCharString* g_baseName; static bool g_baseEnd;
bool CThingCreatureBase::ConstructFromParams(long, const C3DVector&, long t, CThingPhysical*, CThingPhysical*, const CCharString& n, bool e)
{ g_baseType = t; g_baseName = &n; g_baseEnd = e; return g_baseOk; }
int main()
{
    CThingCreatureDef def; def.Health = 75.0f; def.DefaultOwner = 3;
    CThingBuilding home, work;
    CThingAICreatureInit init; std::memset(&init, 0, sizeof init);
    init.OverridingBrainName.PData = (void*)&def; init.Drawable = true; init.PlaceAtEndOfList = true;
    init.EnableCreatureAutoPlacing = true; init.HomeBuilding = &home; init.WorkBuilding = &work;
    C3DVector pos = { 1, 2, 3 };
    {
        CThingAICreature c; c.PDef = &def; c.PlayerNumber = -1; c.Drawable = 0; c.DeletedOnOwnerLevelOutOfScope = 1;
        c.MaxHealth = 0; c.Health = 0; c.OverridingBrainName.PData = 0; c.EnableCreatureAutoPlacing = 0; c.ImmuneToAttack = 1;
        int liveBefore = g_ipLive;
        bool ok = c.ConstructFromParams(9, pos, 5, init) && g_baseType == 5 && g_baseName == &init.ScriptName && g_baseEnd
            && c.PlayerNumber == 3 && c.MaxHealth == 75.0f && c.Health == 75.0f && c.Drawable && c.DeletedOnOwnerLevelOutOfScope
            && c.OverridingBrainName.PData == (void*)&def && c.HomeBuilding.PItem == &home && c.WorkBuilding.PItem == &work
            && c.EnableCreatureAutoPlacing && c.ImmuneToAttack && g_ipLive == liveBefore;
        c.PlayerNumber = -1;
        ok = ok && c.ConstructFromParams(9, pos, 2, init) && c.PlayerNumber == -1;
        g_baseOk = false; c.Health = 1.0f;
        ok = ok && !c.ConstructFromParams(9, pos, 5, init) && c.Health == 1.0f;
        if (!ok) { std::printf("00830360_TEST FAIL\n"); return 1; }
    }
    std::printf("00830360_TEST PASS\n");
    return 0;
}
