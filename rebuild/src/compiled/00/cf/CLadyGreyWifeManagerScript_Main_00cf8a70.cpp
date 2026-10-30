// CLadyGreyWifeManagerScript::Main() -- once Lady Grey exists, every few frames: while her regions are loaded and she is
//   alive and married to the hero, bring her out of limbo next to the hero and (by time of day) to her Bowerstone appear
//   point or the mayor's bedroom, or send her back into limbo; if she is no longer the hero's wife deactivate the quest;
//   then wait for the hero's region to change
// Leaf: ?Main@CLadyGreyWifeManagerScript@
// Layout: CScriptBase (0x40 bytes), Game +0x40, the wife and her marker things +0x48 / +0x54 / +0x60 / +0x6c / +0x78,
//   time of day +0x84, flag +0x88, the two region names +0x8c / +0x90 (descriptive). CScriptThing / CCountedPointer as
//   the landed CHTE_AssaultGuard::Main 0x00D6AEA0 (PImp assignment 0x00704580).
// Shape: the per-function inline budget expands the first GetThingWithScriptName temporary's ~CScriptThing (vtable from
//   ebp) and calls 0x004AA840 for the later ones.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const char* text);          // 0x0099EFE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
static inline bool IsThingDead(const CScriptThing& thing) { return thing.IsDead(); }
static inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }
static inline const C3DVector& PositionOf(const CScriptThing& thing) { return thing.GetPos(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CLadyGreyWifeManagerScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    unsigned char         _pad44[4];
    CScriptThing          LadyGrey;                     // +0x48
    CScriptThing          PoshVillage;                  // +0x54
    CScriptThing          BedroomVillage;               // +0x60
    CScriptThing          Bedroom;                      // +0x6c
    CScriptThing          AppearPoint;                  // +0x78
    long                  TimeOfDay;                    // +0x84
    bool                  Flag88;                       // +0x88
    unsigned char         _pad89[3];
    CCharString           BedroomRegion;                // +0x8c
    CCharString           PoshRegion;                   // +0x90
    void Main();
};

void CLadyGreyWifeManagerScript::Main()
{
    while (IsThingDead(LadyGrey)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        LadyGrey = Game->GetThingWithScriptName("LadyGrey");
    }
    if (IsActiveThreadTerminating())
        return;
    TimeOfDay = 0;
    Flag88 = false;
    BedroomRegion = "MayorsHouseBedroom";
    PoshRegion = "BowerstonePosh";
    while (!IsActiveThreadTerminating()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if ((Game->IsLevelLoaded(PoshRegion) || Game->IsLevelLoaded(BedroomRegion)) && IsThingAlive(LadyGrey)) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsEntityMarriedToHero(LadyGrey)) {
                if (Game->IsEntityInLimbo(LadyGrey)) {
                    Game->EntityTeleportToPosition(LadyGrey, Game->GetHero().GetPos(), 0.0f, false, false);
                    Game->SetThingPersistent(LadyGrey, true);
                    if (Game->IsLevelLoaded("BowerstonePosh")) {
                        TimeOfDay = Game->GetTimeOfDay();
                        if (TimeOfDay > 800 && TimeOfDay < 2000) {
                            AppearPoint = Game->GetRandomThingWithScriptName("LadyGreyAppearPoint");
                            Game->EntityTeleportToPosition(LadyGrey, PositionOf(AppearPoint), 0.0f, false, false);
                            Game->EntitySetInLimbo(LadyGrey, false, true);
                            PoshVillage = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BOWERSTONE_POSH");
                            Game->EntityAttachToVillage(LadyGrey, PoshVillage);
                        }
                    } else if (Game->IsLevelLoaded("MayorsHouseBedroom")) {
                        TimeOfDay = Game->GetTimeOfDay();
                        if (TimeOfDay <= 800 || TimeOfDay >= 2000) {
                            AppearPoint = Game->GetRandomThingWithScriptName("LadyGreyAppearPoint");
                            Game->EntityTeleportToPosition(LadyGrey, PositionOf(AppearPoint), 0.0f, false, false);
                            Game->EntitySetInLimbo(LadyGrey, false, true);
                            BedroomVillage = Game->GetThingWithScriptName("MayorsBedroomVillage");
                            Bedroom = Game->GetThingWithScriptName("MayorsBedroom");
                            Game->EntityAttachToVillage(LadyGrey, BedroomVillage);
                            Game->SetThingHomeBuilding(LadyGrey, Bedroom);
                        }
                    }
                } else if (Game->IsLevelLoaded("BowerstonePosh")) {
                    TimeOfDay = Game->GetTimeOfDay();
                    if (TimeOfDay <= 800 || TimeOfDay >= 2000)
                        Game->EntitySetInLimbo(LadyGrey, true, false);
                } else if (Game->IsLevelLoaded("MayorsHouseBedroom")) {
                    TimeOfDay = Game->GetTimeOfDay();
                    if (TimeOfDay > 800 && TimeOfDay < 2000)
                        Game->EntitySetInLimbo(LadyGrey, true, false);
                }
            } else {
                Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
            }
        } else if (Game->IsLevelLoaded(PoshRegion) || Game->IsLevelLoaded(BedroomRegion)) {
            if (IsActiveThreadTerminating())
                return;
        } else if (Game->IsEntityInLimbo(LadyGrey)) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntityTeleportToPosition(LadyGrey, Game->GetHero().GetPos(), 0.0f, false, false);
            Game->SetThingPersistent(LadyGrey, true);
        }
        {
            CCharString region = Game->GetRegionName();
            while (Game->IsRegionLoaded(region)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}