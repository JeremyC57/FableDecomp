// CBountyHuntActivationChecker::RespawnFishermen() -- recreate the fishing-competition organiser at MK_FC_ORG and, unless the competitors were attacked, the two competitors at MK_FI_COMP1 / MK_FI_COMP2 (data strings "1" / "2", appearance morph seeds 9 / 36)
// Leaf: RespawnFishermen
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7F7F0, 908 bytes (912 with padding); called from Main 0x00E7F410.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC
//   (the CScriptThing destructor is expanded inline at every site), CScriptThing slot 6 GetPos (virtual on the lookup
//   temporaries) and slot 4 SetDataString (inline on the named locals: the implementation's slot 4 when PImp_Data is set);
//   CGameScriptInterface slots 72 GetThingWithScriptName, 91 CreateCreature, 479 EntitySetAppearanceMorphSeed.
//   Literals "FishCompOrganiser" 0x012EA554, "MK_FC_ORG" 0x012EA67C, "CREATURE_FISHERMAN" 0x012EA668,
//   "FishCompCompetitor" 0x012EA568, "MK_FI_COMP1" 0x012EA764, "MK_FI_COMP2" 0x012EA71C,
//   "CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_WAREHOUSE_WORKER" 0x012EA728,
//   "CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_UNEMPLOYED" 0x012EA6E8, "1" 0x012EA6E4, "2" 0x012EA6E0.
// Layout: Ego_r.pdb CBountyHuntActivationChecker: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CV_FishingCompetitionScript, retail = PDB - 0x14: CompetitorsAttacked +0x4f).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl {                               // the game-side thing behind PImp_Data
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003();
    virtual void SetDataString(const CCharString& text);   // slot 4 (+0x10)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003();
    virtual void SetDataString(const CCharString& text) { if (PImp_Data) PImp_Data->SetDataString(text); }   // slot 4 (+0x10)
    virtual void t005();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    CScriptThingImpl* PImp_Data;                        // +0x04
    CCPPointerInfo* PImp_Info;                          // +0x08
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing()                                     // inline here; out-of-line copy 0x004AA840
    {
        if (PImp_Info) {
            --PImp_Info->RefCount;
            if (PImp_Info->RefCount == 0) {
                PImp_Info->DeleteFunc(PImp_Info->Data);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(void* predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CV_FishingCompetitionScript {
    unsigned char _pad[0x4f];
    bool CompetitorsAttacked;                          // +0x4f
};
struct CBountyHuntActivationChecker : CEntityScriptBase {
    CV_FishingCompetitionScript* ParentClass;          // +0x14
    void*                        PMasterData;          // +0x18 CQ_SunnyvaleMasterData*
    void RespawnFishermen();
};

void CBountyHuntActivationChecker::RespawnFishermen()
{
    Game->CreateCreature("CREATURE_FISHERMAN", Game->GetThingWithScriptName("MK_FC_ORG").GetPos(), "FishCompOrganiser", false);
    if (!ParentClass->CompetitorsAttacked) {
        CScriptThing competitor1 = Game->CreateCreature("CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_WAREHOUSE_WORKER", Game->GetThingWithScriptName("MK_FI_COMP1").GetPos(), "FishCompCompetitor", false);
        CScriptThing competitor2 = Game->CreateCreature("CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("MK_FI_COMP2").GetPos(), "FishCompCompetitor", false);
        competitor1.SetDataString("1");
        competitor2.SetDataString("2");
        Game->EntitySetAppearanceMorphSeed(competitor1, 9);
        Game->EntitySetAppearanceMorphSeed(competitor2, 36);
    }
}