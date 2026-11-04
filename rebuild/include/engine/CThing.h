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
#ifdef FABLE_RETAIL_CTHING
class CMainGameComponent; class CPlayer;                        // chunk 732: PeekComponent / GetPlayer return types
#endif
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
#ifdef FABLE_RETAIL_CTHING
    // Retail (chunk 732, row 0x006AA2E0): GetTC(ETCInterfaceType) is __forceinline like GetTC<T> / PeekTC<T> (owner decision
    // 2026-11-01: no Ego_r.pdb copy, no out-of-line copy in Fable.exe; FableWin.pdb ?GetTC@CThing@@QAEAAVCTCBase@@W4ETCInterfaceType@@@Z
    // is the debug copy). Plain inline is called out of line in the large CThingPlayerCreature draw pass (814 v 1080 bytes).
    __forceinline CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    __forceinline const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#else
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#endif
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
    // Retail opt-in (2026-11-03, chunk 718 row 0x008A93A0): FableWin.pdb ?GetTypeFlags@CThing@@QBEKXZ (inline in retail:
    // EntityWillTeleportToArea tests the +0x6C flags in place). A declaration with its body only: no layout change.
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
    // Retail opt-in (2026-11-03, chunk 720 rows 0x005C552C / 0x005C36C2): Ego_r.pdb / FableWin.pdb
    // ?GetTCGraphicAppearance@CThing@@QAEPAVCTCGraphicAppearance@NGraphicAppearance@@XZ (and the QBE const twin), inline in
    // retail (`mov ecx,[esi+64h]` straight off the thing before SetScale). Declarations with bodies only: no layout change.
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }
    const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() const { return PTCGraphicAppearance; }
    // ?GetPlayerNumber@CThing@@QBEKXZ (Ego_r.pdb / FableWin.pdb), inline in retail (`movsx eax,byte ptr [ecx+90h]`, 0x005C37CC).
    unsigned long GetPlayerNumber() const { return PlayerNumber; }
    void SetInLimbo(bool);                                       // 0x004C8CF0 ?SetInLimbo@CThing@@QAEX_N@Z (Ego_r.pdb / FableWin.pdb)
    // Retail (2026-11-03, chunk 720 row 0x0065B854 CTCVillage::AddThingToVillage; was FABLE_RETAIL_CTHING_VILLAGE, folded into this class branch at landing): ?HasScriptThing@CThing@@QBE_NXZ (Ego_r.pdb
    // 0x004C9A30; out of line in retail at 0x004C7E70, `mov ecx,esi; call`) and FableWin.pdb's inline
    // ?SetPersistOnUnloadWhenInLimbo@CThing@@QAEX_N@Z (0x0215EDB0, emitted in the CTCVillage TU right after AddThingToVillage;
    // retail `or byte ptr [esi+92h],40h` = bit 14 of the +0x91 bitfield run). Declarations with bodies only: no layout change.
    bool HasScriptThing() const;                                 // 0x004C7E70
#ifdef FABLE_RETAIL_CTHING_SCRIPTSUPERVISION
    // Nested opt-in (chunk 755, row 0x00938280 CAIStateGroup_UsesSeat::AllocChairSittingMode): Ego_r.pdb 0x004C9A40
    // ?IsUnderScriptSupervision@CThing@@QBE_NXZ, out of line in retail at 0x004C7E90 (after HasScriptThing 0x004C7E70).
    bool IsUnderScriptSupervision() const;                       // 0x004C7E90
#endif
    // Chunk 732 (row 0x006AA2E0): Ego_r.pdb / FableWin.pdb ?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ, out of line in
    // retail at 0x004C7A70 (the only PDB spelling; the landed CMainGameComponentFontView / AUCMainGameComponent forms are invented).
    const CMainGameComponent& PeekComponent() const;             // 0x004C7A70
    // Chunk 732 (row 0x006AABE0): Ego_r.pdb ?GetPlayer@CThing@@QAEAAVCPlayer@@XZ, out of line in retail (0x004C7A30).
    CPlayer& GetPlayer();                                        // 0x004C7A30
#ifdef FABLE_RETAIL_CTHING_PEEKPLAYER
    // Chunk 756 (row 0x008031E0): Ego_r.pdb / FableWin.pdb ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ, out of line in retail (0x004C7A10).
    const CPlayer& PeekPlayer() const;                           // 0x004C7A10
#endif
    void SetPersistOnUnloadWhenInLimbo(bool persist) { PersistOnUnloadWhenInLimbo = persist; }
    // Chunk 751 (row 0x0082A3E0 CMiniMapDisplay::CMarkerInfo::IsReversedOnMap): FableWin.pdb
    // ??$PeekTCDriver@$$CBVCTCDRegionExit@@@CThing@@QBE_NPAPBVCTCDRegionExit@@@Z (inline in retail: the interface bit test, then
    // PTCDriver +0x5C read straight off the thing). __forceinline like GetTC<T> / PeekTC<T> (owner decision 2026-11-01). The driver
    // class must be complete (derived from CTCDriverBase) where it is instantiated.
    template<class T> __forceinline bool PeekTCDriver(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=static_cast<T*>(PTCDriver);
            return true;
        }
        return false;
    }
#ifdef FABLE_RETAIL_CTHING_ADDANDCOPYTC
    // Chunk 756 (row 0x004C9F60): Ego_r.pdb ?AddAndCopyTC@CThing@@QAEXABVCTCBase@@_N@Z (out of line) and the FableWin.pdb member template
    // ??$GetTC@VCTCBase@@@CThing@@QAE_NW4ETCInterfaceType@@PAPAVCTCBase@@@Z (the non-const twin of PeekTC(id, ptc); __forceinline like it).
    void AddAndCopyTC(const CTCBase& other,bool flag);
    template<class T> __forceinline bool GetTC(ETCInterfaceType id,T** ptc) {
        if(HasTC(id)) {
            CTCBase& base=GetTC(id);
            *ptc=GFDebugPDynamicCast(&base,(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_CTHING_GETTCDRIVER
    // Chunk 756 (row 0x007BE05C): FableWin.pdb / Ego_r.pdb ??$GetTCDriver@VCTCDParticleEmitter@@@CThing@@QAE_NPAPAVCTCDParticleEmitter@@@Z,
    // out of line in retail (0x004ACB50: mov eax,[ecx+28h] ; test ah,8 = HasTC(TCI_PARTICLE_EMITTER 75), then PTCDriver +0x5C
    // stored through the out pointer). Plain inline: the size-optimised caller calls it. The driver class must be complete.
    template<class T> bool GetTCDriver(T** result) {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=static_cast<T*>(PTCDriver);
            return true;
        }
        return false;
    }
#endif
#endif
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

#endif
