#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H
// Original CThing hierarchy and members from Ego_r TPI 0xbbaf; native
// AutoPlace/OnHit confirm PhysicsTC+60, PBaseDef+70, Dead+91; ABI size98.
#ifndef FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASS
#endif
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CClass.h"
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CVectorMap.h"
#include "engine/ETCInterfaceType.h"
class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }
// FableWin GetTC<T> debug instantiations name this checked-cast template;
// the release path is the identity-adjusted static cast observed in native lookups.
template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
class CThing : public CClass {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
protected:
    unsigned __int64 UniqueID;
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Components;
    std::list<CTCBase*> UpdateComponentsList;
    CCharString SerialisationSectionName;
    CTCDriverBase* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CDefPointer<const CThingBaseDef> PBaseDef;
    CCharString ScriptName;
    CCharString ScriptData;
public:
    CThing* RenderListPrev;
    CThing* RenderListNext;
    CThing* UpdateListPrev;
    CThing* UpdateListNext;
protected:
    unsigned short DefGlobalIndex;
    short CurrentMapNumber;
    signed char PlayerNumber;

protected:
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1;
    unsigned char IsAffectedByBulletTimeOverride : 1;
    unsigned char AddingTCsPermitted : 1;

private:
    unsigned char InitialisationStage : 3;
public:
    void Kill(bool); // Ego_r original public member; native004c9b80.
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    const CThingBaseDef& PeekBaseDef() const;                    // inline in retail: body `return *PBaseDef;` defined after CThingBaseDef is complete
    unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}
    unsigned __int64 GetUniqueID() const { return UniqueID; }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    bool IsLevelLess() const;
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    // GetTC<T> / PeekTC<T>: __forceinline (owner decision 2026-11-01, EXTRA_RULES): Ego_r.pdb (the release build) has no
    // ??$GetTC@ / ??$PeekTC@ instantiation (FableWin.pdb, debug: 267 / 159), Fable.exe has no out-of-line copy, and plain
    // `inline` leaves calls under VC7.1's budget (0x00667450 ReactToGenericStrike: GetTC<CTCDExplosion> and
    // PeekTC<const CTCCreatureModeManager> called out of line, 1625 v 1755 bytes).
    // GetTC<T> body (Codex pass 11 consolidation 2): HasTC then Components.Find, like PeekTC. 0x00667D30 OnHit tests the
    // UsingComponents bit in place (`mov eax,[edi+20h]; test ah,ah; jns`); a `const bool&` local stores it first.
    template<class T> __forceinline bool GetTC(T** result) {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(Components.Find(T::StatGetInterfaceType())->second,(T*)0);
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id,T** ptc) const {
        if(HasTC(id)) {
            const CTCBase& base=GetTC(id);
            *ptc=GFDebugPDynamicCast(&base,(T*)0);
            return true;
        }
        return false;
    }
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThing,UniqueID)==0x18);
        FABLE_STATIC_ASSERT(offsetof(CThing,UsingComponents)==0x20);
        FABLE_STATIC_ASSERT(offsetof(CThing,Components)==0x44);
        FABLE_STATIC_ASSERT(offsetof(CThing,PhysicsTC)==0x60);
        FABLE_STATIC_ASSERT(offsetof(CThing,PBaseDef)==0x70);
    }
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const;
    C3DVector GetPhysPos() const;
#ifdef FABLE_RETAIL_CTHING
    // Retail opt-in (2026-11-03, row 0x005EB9E0): the public non-virtual SetAsRenderedThisFrame 0x004C7590 (Ego_r.pdb / FableWin.pdb
    // ?SetAsRenderedThisFrame@CThing@@QAEXK@Z, the only spelling bound to 0x004C7590; `push 4; call` with ecx = the TC's ParentThing
    // at 0x005EBF44). A declaration only: no layout change.
    void SetAsRenderedThisFrame(unsigned long);                  // 0x004C7590
    void SetAsActivated(bool);                                   // Ego_r.pdb ?SetAsActivated@CThing@@QAEX_N@Z (row 0x00821010)
    // Header wave 2026-11-03 (row 0x00522A20 InitThingTypes registers it): Ego_r.pdb ?Alloc@CThing@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

#endif
