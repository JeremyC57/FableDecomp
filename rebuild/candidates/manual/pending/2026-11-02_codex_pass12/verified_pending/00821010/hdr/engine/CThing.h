#ifdef FABLE_RETAIL_UI_ORDINARY_INLINE
#ifdef FABLE_RETAIL_CTHING
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H
#ifdef FABLE_RETAIL_UI_ORDINARY_INLINE
#define FABLE_THING_INLINE inline
#else
#define FABLE_THING_INLINE __forceinline
#endif
// Original CThing hierarchy and members from Ego_r TPI 0xbbaf; native
// AutoPlace/OnHit confirm PhysicsTC+60, PBaseDef+70, Dead+91; ABI size98.
#ifndef FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASS
#endif
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CClass.h"
#include "engine/CCharString.h"
#include "engine/CDefPointer.h"
#include "engine/CVectorMap.h"
#include "engine/ETCInterfaceType.h"
class CPersistContext;class CMemoryDataInputStream;
class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }
#if defined(FABLE_RETAIL_TYPED_GETTC) || defined(FABLE_RETAIL_QUICK_ACCESS)
// FableWin GetTC<T> debug instantiations name this checked-cast template;
// the release path is the identity-adjusted static cast observed in native lookups.
template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
#endif
class CThing : public CClass {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
protected:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThing(unsigned long);
#endif
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
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    static CClass* __fastcall Alloc();
#endif
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
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOAD
    class CSerialiseInfo {public:unsigned __int64 UID;CCharString DefinitionType;signed char Player;};
    virtual bool SerialiseGameState(CPersistContext&);
    virtual bool ConstructFromLevelScript(const char*);
    virtual bool ConstructAlreadyExistingThingFromLevelScript(const char*);
    virtual bool ConstructFromGameStateScript(const char*);
    virtual void CopyGameState(const CThing&);
    virtual bool OnCreate();
    virtual void InitialActivate();
    virtual void OnInitialActivate();
protected:
    virtual void OnDie();
    virtual void OnKill();
    virtual bool PreOnKill();
    virtual void OnSetInLimbo(bool);
    virtual void CreateFromDef(const CThingBaseDef&);
public:
    virtual void ConstructFromBinarySave(CMemoryDataInputStream&,const CSerialiseInfo&);
    static void __fastcall SerialiseThingInfo(CPersistContext&,CSerialiseInfo&);
    static void __fastcall SerialiseThingInfo(bool,CCharString&,CSerialiseInfo&);
    bool IsToKillOnLevelUnload()const{return KillOnLevelUnload!=0;}
    bool IsInLimbo()const{return InLimbo!=0;}
    void SetInLimbo(bool);
    bool IsLevelPersistent()const;
    void SetSerialisationSectionName(const CCharString& name){SerialisationSectionName=name;}
#endif
    void Kill(bool); // Ego_r original public member; native004c9b80.
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
#ifdef FABLE_RETAIL_SHOP
    const CThingBaseDef& PeekBaseDef()const{return *PBaseDef;}
    unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_TARGET_RANDOM_WALK) || defined(FABLE_RETAIL_HIT_EFFECT)
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    unsigned __int64 GetUniqueID() const { return UniqueID; }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    bool IsLevelLess() const;
    template<class T> bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=static_cast<T*>(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second));
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    unsigned __int64 GetUniqueID()const{return UniqueID;}
    const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance()const{return PTCGraphicAppearance;}
#endif
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
#ifdef FABLE_RETAIL_FORCE_HASTC
    FABLE_THING_INLINE bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#else
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#endif
#ifdef FABLE_RETAIL_TYPED_GETTC
#ifdef FABLE_RETAIL_REACT_STRIKE
    template<class T> FABLE_THING_INLINE bool GetTC(T** ptc) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) {
            CTCBase& base=GetTC(T::StatGetInterfaceType());
            *ptc=GFDebugPDynamicCast(&base,(T*)0);
        }
        return present;
    }
#else
    template<class T> FABLE_THING_INLINE bool GetTC(T** result) {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(Components.Find(T::StatGetInterfaceType())->second,(T*)0);
            return true;
        }
        return false;
    }
#endif
    template<class T> FABLE_THING_INLINE bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_SHOP
    template<class T> FABLE_THING_INLINE bool PeekTC(ETCInterfaceType id,T** ptc) const {
        if(HasTC(id)) {
            const CTCBase& base=GetTC(id);
            *ptc=GFDebugPDynamicCast(&base,(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_QUICK_ACCESS
    template<class T> bool PeekTC(T** result) const {
        bool present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    template<class T> bool GetTCDriver(T**);
#endif
#ifdef FABLE_RETAIL_TYPED_GETTCDRIVER
    // Original Ego_r00762cd0/FableWin025a6d40 typed driver overload.
    // Native006677eb tests interface65 before the mutable Components lookup.
    template<class T> FABLE_THING_INLINE bool GetTCDriver(T** result) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(&GetTC(T::StatGetInterfaceType()),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_FORCE_GETTC
    FABLE_THING_INLINE CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    FABLE_THING_INLINE const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
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
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

#endif
#else
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThing = 0xa8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThing = 0x98.
// Retail evidence: 5 trusted landed function(s); reconciliation 3 conflict(s) in 3 file(s) vs 2 agreeing.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCharStringData;
class CDefPointeeBase;
class CTCDriverBase;
struct CTCGraphicAppearance;
class CTCPhysicsBase;

#pragma pack(push, 1)
class CThing {
public:
    void*                 __vftable;                         // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char         _pad_0x04[0x4];                    // +0x04
    unsigned char         _pad_0x08[0x4];                    // +0x08
    unsigned char         _pad_0x0c[0x4];                    // +0x0c
    unsigned char         _pad_0x10[0x1];                    // +0x10
    unsigned char         _pad_0x11[0x7];                    // +0x11
    unsigned __int64      UniqueID;                          // +0x18
    unsigned char         UsingComponents[0x24];             // +0x20 bitset<274>
    unsigned char         Components[0x10];                  // +0x44 CVectorMap<ETCInterfaceType,CTCBase_*,CKeyPairCompareLess<ETCInterfaceType,CTCBase_*>_>
    unsigned char         UpdateComponentsList[0x4];         // +0x54 list<CTCBase_*,std::allocator<CTCBase_*>_>
    CCharStringData*      SerialisationSectionName_PStringData; // +0x58 CCharString::PStringData
    CTCDriverBase*        PTCDriver;                         // +0x5c
    CTCPhysicsBase*       PhysicsTC;                         // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;              // +0x64
    long                  WorldFrameCreatedAt;               // +0x68
    unsigned long         ThingTypeFlags;                    // +0x6c
    CDefPointeeBase*      PBaseDef_Object;                   // +0x70 CDefPointer<CThingBaseDef_const_>::Object
    CCharStringData*      ScriptName_PStringData;            // +0x74 CCharString::PStringData
    CCharStringData*      ScriptData_PStringData;            // +0x78 CCharString::PStringData
    CThing*               RenderListPrev;                    // +0x7c
    CThing*               RenderListNext;                    // +0x80
    CThing*               UpdateListPrev;                    // +0x84
    CThing*               UpdateListNext;                    // +0x88
    unsigned short        DefGlobalIndex;                    // +0x8c
    short                 CurrentMapNumber;                  // +0x8e
    signed char           PlayerNumber;                      // +0x90
    unsigned char         CreatedBySerialisation[0x1];       // +0x91 bitfield group: uchar:1
    unsigned char         PausedDuringCutscenes[0x1];        // +0x92 bitfield group: uchar:1
    unsigned char         AddingTCsPermitted[0x1];           // +0x93 bitfield group: uchar:1
    unsigned char         InitialisationStage[0x4];          // +0x94 bitfield group: uchar:3
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);

#endif // FABLE_ENGINE_CTHING_H

#endif

#else
#ifdef FABLE_RETAIL_CTHING
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
#include "engine/CCharString.h"
#include "engine/CDefPointer.h"
#include "engine/CVectorMap.h"
#include "engine/ETCInterfaceType.h"
class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }
#if defined(FABLE_RETAIL_TYPED_GETTC) || defined(FABLE_RETAIL_QUICK_ACCESS)
// FableWin GetTC<T> debug instantiations name this checked-cast template;
// the release path is the identity-adjusted static cast observed in native lookups.
template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
#endif
class CThing : public CClass {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
protected:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThing(unsigned long);
#endif
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
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    static CClass* __fastcall Alloc();
#endif
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
#ifdef FABLE_RETAIL_MAP_BRUSHES
    void SetAsActivated(bool);
#endif
    void Kill(bool); // Ego_r original public member; native004c9b80.
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
#ifdef FABLE_RETAIL_SHOP
    const CThingBaseDef& PeekBaseDef()const{return *PBaseDef;}
    unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_TARGET_RANDOM_WALK) || defined(FABLE_RETAIL_HIT_EFFECT)
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    unsigned __int64 GetUniqueID() const { return UniqueID; }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    bool IsLevelLess() const;
    template<class T> bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=static_cast<T*>(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second));
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    unsigned __int64 GetUniqueID()const{return UniqueID;}
    const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance()const{return PTCGraphicAppearance;}
#endif
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
#ifdef FABLE_RETAIL_FORCE_HASTC
    __forceinline bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#else
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#endif
#ifdef FABLE_RETAIL_TYPED_GETTC
#ifdef FABLE_RETAIL_REACT_STRIKE
    template<class T> __forceinline bool GetTC(T** ptc) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) {
            CTCBase& base=GetTC(T::StatGetInterfaceType());
            *ptc=GFDebugPDynamicCast(&base,(T*)0);
        }
        return present;
    }
#else
    #ifdef FABLE_RETAIL_WILL_BUTTON_TIMER
    template<class T> inline bool GetTC(T** result) {
#else
    template<class T> __forceinline bool GetTC(T** result) {
#endif
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(Components.Find(T::StatGetInterfaceType())->second,(T*)0);
            return true;
        }
        return false;
    }
#endif
    #ifdef FABLE_RETAIL_WILL_BUTTON_TIMER
    template<class T> inline bool PeekTC(T** result) const {
#else
    template<class T> __forceinline bool PeekTC(T** result) const {
#endif
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_SHOP
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id,T** ptc) const {
        if(HasTC(id)) {
            const CTCBase& base=GetTC(id);
            *ptc=GFDebugPDynamicCast(&base,(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_QUICK_ACCESS
    template<class T> bool PeekTC(T** result) const {
        bool present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    template<class T> bool GetTCDriver(T**);
#endif
#ifdef FABLE_RETAIL_TYPED_GETTCDRIVER
    // Original Ego_r00762cd0/FableWin025a6d40 typed driver overload.
    // Native006677eb tests interface65 before the mutable Components lookup.
    template<class T> __forceinline bool GetTCDriver(T** result) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(&GetTC(T::StatGetInterfaceType()),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_FORCE_GETTC
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
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

#endif
#else
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThing = 0xa8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThing = 0x98.
// Retail evidence: 5 trusted landed function(s); reconciliation 3 conflict(s) in 3 file(s) vs 2 agreeing.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCharStringData;
class CDefPointeeBase;
class CTCDriverBase;
struct CTCGraphicAppearance;
class CTCPhysicsBase;

#pragma pack(push, 1)
class CThing {
public:
    void*                 __vftable;                         // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char         _pad_0x04[0x4];                    // +0x04
    unsigned char         _pad_0x08[0x4];                    // +0x08
    unsigned char         _pad_0x0c[0x4];                    // +0x0c
    unsigned char         _pad_0x10[0x1];                    // +0x10
    unsigned char         _pad_0x11[0x7];                    // +0x11
    unsigned __int64      UniqueID;                          // +0x18
    unsigned char         UsingComponents[0x24];             // +0x20 bitset<274>
    unsigned char         Components[0x10];                  // +0x44 CVectorMap<ETCInterfaceType,CTCBase_*,CKeyPairCompareLess<ETCInterfaceType,CTCBase_*>_>
    unsigned char         UpdateComponentsList[0x4];         // +0x54 list<CTCBase_*,std::allocator<CTCBase_*>_>
    CCharStringData*      SerialisationSectionName_PStringData; // +0x58 CCharString::PStringData
    CTCDriverBase*        PTCDriver;                         // +0x5c
    CTCPhysicsBase*       PhysicsTC;                         // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;              // +0x64
    long                  WorldFrameCreatedAt;               // +0x68
    unsigned long         ThingTypeFlags;                    // +0x6c
    CDefPointeeBase*      PBaseDef_Object;                   // +0x70 CDefPointer<CThingBaseDef_const_>::Object
    CCharStringData*      ScriptName_PStringData;            // +0x74 CCharString::PStringData
    CCharStringData*      ScriptData_PStringData;            // +0x78 CCharString::PStringData
    CThing*               RenderListPrev;                    // +0x7c
    CThing*               RenderListNext;                    // +0x80
    CThing*               UpdateListPrev;                    // +0x84
    CThing*               UpdateListNext;                    // +0x88
    unsigned short        DefGlobalIndex;                    // +0x8c
    short                 CurrentMapNumber;                  // +0x8e
    signed char           PlayerNumber;                      // +0x90
    unsigned char         CreatedBySerialisation[0x1];       // +0x91 bitfield group: uchar:1
    unsigned char         PausedDuringCutscenes[0x1];        // +0x92 bitfield group: uchar:1
    unsigned char         AddingTCsPermitted[0x1];           // +0x93 bitfield group: uchar:1
    unsigned char         InitialisationStage[0x4];          // +0x94 bitfield group: uchar:3
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);

#endif // FABLE_ENGINE_CTHING_H

#endif

#endif
