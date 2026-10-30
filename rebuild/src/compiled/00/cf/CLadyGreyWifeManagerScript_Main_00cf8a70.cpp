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
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing() {}                                  // out of line 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12();
    virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18();
    virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24();
    virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36();
    virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41(); virtual void s42();
    virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48();
    virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54();
    virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60();
    virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65(); virtual void s66();
    virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70(); virtual void s71(); virtual void s72();
    virtual void s73(); virtual void s74();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    virtual bool IsDead() const;                        // slot 76 (+0x130) 0x004AA890
};
static inline bool IsThingDead(const CScriptThing& thing) { return thing.IsDead(); }
static inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }
static inline const C3DVector& PositionOf(const CScriptThing& thing) { return thing.GetPos(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
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