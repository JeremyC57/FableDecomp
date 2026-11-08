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
#ifdef FABLE_RETAIL_CTHING_LEVELSCRIPT
class CWorldMap;                                                 // chunk 771: GetWorldMap return type
#endif
#ifdef FABLE_RETAIL_CTHING_MANAGERACCESS
class CThingManager; class CThingSearchTools;                   // chunk 765: the manager accessors' types
#endif
#ifdef FABLE_RETAIL_CTHING
class CMainGameComponent; class CPlayer;                        // chunk 732: PeekComponent / GetPlayer return types
#endif
#ifdef FABLE_RETAIL_CTHING_MOREACCESS
class CGameDefinitionManager; class CASoundBank; class CWorldMap;   // plan phase 7: FABLE_RETAIL_CTHING_MOREACCESS return types
#endif
#ifdef FABLE_RETAIL_CTHING_LOADSINGLE
class CPersistContext; class CMemoryDataInputStream;            // chunk 846: the serialise virtuals' parameter types
#ifdef FABLE_RETAIL_CTHING_VIRTUALS
class CInterpolationInfo; class CThingInfo; class CEditEngineDrawInfo; class CThingDrawInformation; class CThingPhysicalDebugInfo;
class CEnginePrimitivePrepareInfo; enum ECollideType;             // chunk 868: the slot 17..45 virtuals' parameter types (forward enum as NThingShot_CTCShotDefinitionBase.h)
#endif
#endif
#ifdef FABLE_RETAIL_CTHING_STATEACCESS
class CScriptThing; class CDefString;                           // plan phase 6: GetScriptThing / GetDefName return types
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
#ifdef FABLE_RETAIL_CTHING_TCDRIVER
    // Retail opt-in (chunk 864, row 0x0050A650 via the CThingSearchTools TCD rough-area body; kept outside the FABLE_RETAIL_CTHING
    // method block so the body opt-in does not need it): FableWin.pdb
    // ?HasTCDriver@CThing@@QBE_NXZ (0x01CCC3F0), ?GetTCDriver@CThing@@QAEAAVCTCDriverBase@@XZ (0x01CCC420) and
    // ?GetTCDriver@CThing@@QBEABVCTCDriverBase@@XZ (0x01CF9F90): the PTCDriver (+0x5C) null test and the driver reference,
    // inline in retail (0x00518A54 `mov ecx,[edi+5Ch] ; test ecx,ecx` then the slot-28 virtual on it). No layout change.
    bool HasTCDriver() const { return PTCDriver!=0; }
    CTCDriverBase& GetTCDriver() { return *PTCDriver; }
    const CTCDriverBase& GetTCDriver() const { return *PTCDriver; }
#endif
#ifdef FABLE_RETAIL_CTHING_LOADSINGLE
    // Retail opt-in (chunk 846, row 0x0051FD80 CThingManager::LoadSingleThing): the CThing virtuals of vtable 0x01239ABC slots 3..16 in
    // retail slot order (catalog / rtti-vtables: 3 SerialiseGameState 0x004CD1A0, 4 ConstructFromLevelScript 0x004CAA00, 8 OnCreate
    // 0x004C9560, 9 InitialActivate 0x004C9690, 10 OnInitialActivate 0x004C97B0, 11 OnDie 0x004C7D50, 12 OnKill 0x004C7CF0,
    // 15 CreateFromDef 0x004CA930, 16 ConstructFromBinarySave 0x004CA390), spelled as the Ego_r.pdb name set (U = public virtual,
    // M = protected virtual); LoadSingleThing calls slots 4 (+0x10), 8 (+0x20), 9 (+0x24), 10 (+0x28) and 16 (+0x40). The nested
    // CSerialiseInfo (Ego_r.pdb, 16 bytes: UID +0, DefinitionType +8, Player +0xC; landed 0x004C81F0 reads those offsets), the two
    // static __fastcall SerialiseThingInfo overloads (0x004C81F0 / 0x004C88E0, landed under these spellings) and the inline
    // FableWin.pdb accessors ?IsToKillOnLevelUnload@CThing@@QBE_NXZ (`test byte ptr [ebp+93h],4`, bit 18 of the +0x91 run) and
    // ?SetSerialisationSectionName@CThing@@QAEXABVCCharString@@@Z (`lea ecx,[ebp+58h] ; call CCharString::operator=`).
    // Not to be combined with FABLE_RETAIL_CTHING_LEVELSCRIPT (which declares slot 4 alone). No layout change: CThing has its vptr.
    class CSerialiseInfo {
    public:
        unsigned __int64 UID;                                    // +0x00
        CCharString DefinitionType;                              // +0x08
        signed char Player;                                      // +0x0c
    };
    virtual bool SerialiseGameState(CPersistContext&);           // slot 3
    virtual bool ConstructFromLevelScript(const char*);          // slot 4  0x004CAA00
    virtual bool ConstructAlreadyExistingThingFromLevelScript(const char*);   // slot 5
    virtual bool ConstructFromGameStateScript(const char*);      // slot 6
    virtual void CopyGameState(const CThing&);                   // slot 7
    virtual bool OnCreate();                                     // slot 8  0x004C9560
    virtual void InitialActivate();                              // slot 9  0x004C9690
    virtual void OnInitialActivate();                            // slot 10 0x004C97B0
protected:
    virtual void OnDie();                                        // slot 11 0x004C7D50
    virtual void OnKill();                                       // slot 12 0x004C7CF0
    virtual bool PreOnKill();                                    // slot 13
    virtual void OnSetInLimbo(bool);                             // slot 14
    virtual void CreateFromDef(const CThingBaseDef&);            // slot 15 0x004CA930
public:
    virtual void ConstructFromBinarySave(CMemoryDataInputStream&,const CSerialiseInfo&);   // slot 16 0x004CA390
    static void __fastcall SerialiseThingInfo(CPersistContext&,CSerialiseInfo&);           // 0x004C81F0
    static void __fastcall SerialiseThingInfo(bool,CCharString&,CSerialiseInfo&);         // 0x004C88E0
    bool IsToKillOnLevelUnload() const { return KillOnLevelUnload!=0; }
    void SetSerialisationSectionName(const CCharString& name) { SerialisationSectionName=name; }
#endif
#ifdef FABLE_RETAIL_CTHING_VIRTUALS
    // Retail opt-in (chunk 868, row 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate): the CThing virtuals of vtable
    // 0x01239ABC slots 17..45 in retail slot order, spelled as the Ego_r.pdb name set (the landed local models of
    // 0x00835FD0 CThingDeadCreature::OnCreate carry the same list; CThingGameObject.h's STANDINGON slots 38 / 39 agree), so that a
    // derived header's virtual (CThingPhysical slots 46..59, CThingGameObject 60..65: OnHit = +0x104) lands on its retail slot.
    // Needs FABLE_RETAIL_CTHING_LOADSINGLE (slots 3..16). Declarations only: no layout change, nothing defined.
    virtual bool IsToBePlacedInMap() const;                      // slot 17
    virtual bool ValidateThing(CCharString&) const;              // slot 18
    virtual bool WorldUpdate();                                  // slot 19
    virtual void WorldUpdateBulletTime();                        // slot 20
    virtual bool RenderUpdate(const CInterpolationInfo*);        // slot 21
    virtual bool IsEntitySerialisable() const;                   // slot 22
    virtual unsigned long GetChecksum() const;                   // slot 23
    virtual void GetInfo(CThingInfo*) const;                     // slot 24
    virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const;   // slot 25
    virtual void DrawUpdate(const CInterpolationInfo*);          // slot 26
    virtual bool DrawIsDrawable() const;                         // slot 27
    virtual void SetAsDrawable(bool);                            // slot 28
    virtual bool DrawIsEditDrawable() const;                     // slot 29
    virtual bool DrawIsInDrawList() const;                       // slot 30
    virtual unsigned long DrawGetPhysicsMesh() const;            // slot 31
    virtual bool DrawIsShowingFacingDirection() const;           // slot 32
    virtual bool DrawIsDisplayingInformation() const;            // slot 33
    virtual bool DrawGetThingDisplayInformation(CThingDrawInformation*) const;   // slot 34
    virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&) const;        // slot 35
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);   // slot 36
    virtual void SetThingStandingOn(CThing*);                    // slot 37
    virtual CThing* GetThingStandingOn();                        // slot 38
    virtual const CThing* GetThingStandingOn() const;            // slot 39
    virtual long GetFloorMaterial() const;                       // slot 40
    virtual void OnCollision(ECollideType,const C3DVector&,const C3DVector&,CThing*);   // slot 41
    virtual void OnNonKillingLevelUnload();                      // slot 42
    virtual void OnLevelUnload();                                // slot 43
    virtual bool IsFrameUpdated() const;                         // slot 44
    virtual void SetPositionUsingBrain();                        // slot 45
#endif
    void Kill(bool); // Ego_r original public member; native004c9b80.
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    const CThingBaseDef& PeekBaseDef() const;                    // inline in retail: body `return *PBaseDef;` defined after CThingBaseDef is complete
    unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}
    unsigned __int64 GetUniqueID() const { return UniqueID; }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    bool IsLevelLess() const;
    bool IsDead() const { return Dead!=0; }
#ifdef FABLE_RETAIL_CTHING_SCANINCLUSION_OUTOFLINE
    // Plan phase 6 (2026-11-05): the same member called out of line, as retail does in the TUs of the 4 landed binders of 0x0048BC40
    // (catalog CThing::IsIncludedInThingScans; Ego_r.pdb / FableWin.pdb ?IsIncludedInThingScans@CThing@@QBE_NXZ). Everywhere else
    // retail expands the body below.
    bool IsIncludedInThingScans() const;                         // 0x0048BC40
#else
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
#endif
    bool IsAlive() const { return !Dead; }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    // GetTC<T> / PeekTC<T>: __forceinline (owner decision 2026-11-01, EXTRA_RULES): Ego_r.pdb (the release build) has no
    // ??$GetTC@ / ??$PeekTC@ instantiation (FableWin.pdb, debug: 267 / 159), Fable.exe has no out-of-line copy, and plain
    // `inline` leaves calls under VC7.1's budget (0x00667450 ReactToGenericStrike: GetTC<CTCDExplosion> and
    // PeekTC<const CTCCreatureModeManager> called out of line, 1625 v 1755 bytes).
    // GetTC<T> body (Codex pass 11 consolidation 2): HasTC then Components.Find, like PeekTC. 0x00667D30 OnHit tests the
    // UsingComponents bit in place (`mov eax,[edi+20h]; test ah,ah; jns`); a `const bool&` local stores it first.
#ifdef FABLE_RETAIL_CTHING_GETTC_EXPANDED
    // Plan phase 6 (2026-11-05, the CThing header wave): the same member templates with their bodies written out as the migrated
    // rows' local models had them and as retail expands them (0x0069F4F0, 0x00782390, 0x00634890 ...): the UsingComponents test,
    // CVectorMap::LowerBound (out of line in retail: 0x0040F020 non-const, 0x004365B0 const), the end-or-greater test, the store
    // through the out pointer. The default branch's HasTC + Components.Find gives the same bytes only where VC7.1 expands Find in
    // place; this branch does not depend on that. GetTC(id, ptc) is FableWin.pdb ??$GetTC@VCTCBase@@@CThing@@QAE_NW4ETCInterfaceType@@PAPAVCTCBase@@@Z
    // (also under FABLE_RETAIL_CTHING_ADDANDCOPYTC below, which yields to this branch). The cast is written in the C style so that a
    // TU whose T is a flat stand-in model (not derived from a complete CTCBase) compiles; for a complete T derived from CTCBase it is
    // exactly the static_cast of GFDebugPDynamicCast (single inheritance, no adjustment).
    // Shape (the 1,502 local models of this body the plan migrated, e.g. 0x00435000 / 0x0056FF3D / 0x004BFB40, and retail's bytes
    // there): the key is copied into a local inside the test (the `mov [ebp-4],<id>` store sits after the presence test, not
    // before it); the map is reached through a pointer local (`add esi,44h ; mov ecx,esi ... mov esi,[esi+4]`: the end read
    // through the same register -- written straight on Components VC7.1 hoists the key store above the test and reads end() as
    // `[esi+48h]`); the end-or-greater choice is the conditional expression, not an `if` re-assignment (0x004BFB40 loads the
    // component straight into ecx for the next call, `mov ecx,[eax+4]`; the if form goes through eax, 276 v 272 bytes). Gate
    // results over the first 140 migrated rows: the if form 106 passing, this form 115, a superset.
    template<class T> __forceinline bool GetTC(ETCInterfaceType id,T** ptc) {
        if(HasTC(id)) {
            ETCInterfaceType type=id;
            CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >* map=&Components;
            CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator it=map->LowerBound(type);
            *ptc=(T*)((it==map->end()||type<it->first)?map->end():it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id,T** ptc) const {
        if(HasTC(id)) {
            ETCInterfaceType type=id;
            const CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >* map=&Components;
            CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::const_iterator it=map->LowerBound(type);
            *ptc=(const T*)((it==map->end()||type<it->first)?map->end():it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetTC(T** result) { return GetTC(T::StatGetInterfaceType(),result); }
    template<class T> __forceinline bool PeekTC(T** result) const { return PeekTC(T::StatGetInterfaceType(),result); }
#else
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
#endif
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
#ifdef FABLE_RETAIL_CTHING_STATTYPEFLAGS
    // Nested opt-in (chunk 768, row 0x00969E70 via the CThingSearchTools rough-area body): FableWin.pdb ?StatGetTypeFlags@CThing@@SIKXZ
    // (static fastcall; inline in retail, where the IsThingOfType<CThing> test folds: CThing's mask is 0).
    static unsigned long __fastcall StatGetTypeFlags() { return 0; }
#endif
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
#ifdef FABLE_RETAIL_CTHING_STATEACCESS
    // Nested opt-in (plan phase 6, 2026-11-05, the CThing header wave): the public members the migrated rows called out of line.
    // Evidence per line: Ego_r.pdb + FableWin.pdb spelling, the catalog row at the retail address, and the rows' local
    // declarations citing that address (26 / 39 / 24 / 6 / 3 / 22 / 2 / 3 / 15 / 15 / 58 rows). Declarations only: no layout change.
    void AddToRenderList();                                      // 0x004C7AC0 ?AddToRenderList@CThing@@QAEXXZ
    CScriptThing GetScriptThing() const;                         // 0x004C7E50 ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ
    void SetAsGameStatePersisted(bool);                          // 0x004C7880 ?SetAsGameStatePersisted@CThing@@QAEX_N@Z
    bool IsGameStatePersisted() const;                           // 0x004C8280 ?IsGameStatePersisted@CThing@@QBE_NXZ
    bool IsLevelPersistent() const;                              // 0x004C82A0 ?IsLevelPersistent@CThing@@QBE_NXZ
    CDefString GetDefName() const;                               // 0x004C7CC0 ?GetDefName@CThing@@QBE?AVCDefString@@XZ
    void SetPlayerNumber(long);                                  // 0x004C7440 ?SetPlayerNumber@CThing@@QAEXJ@Z
    void SetCurrentMapNumber(long);                              // 0x004C7580 ?SetCurrentMapNumber@CThing@@QAEXJ@Z
    void SetForceFrameUpdate(bool);                              // 0x004C7C60 ?SetForceFrameUpdate@CThing@@QAEX_N@Z
    bool DoActivationSequence();                                 // 0x004C9CA0 ?DoActivationSequence@CThing@@QAE_NXZ
    void RemoveTC(ETCInterfaceType);                             // 0x004C9840 ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z
    // 0x004C9920 is not a catalog row; both PDBs spell it ?IsAffectedByBulletTime@CThing@@QBE_NXZ and 15 landed rows call it there
    // (8 of them with a non-const QAE stand-in spelling, respelled by the wave).
    bool IsAffectedByBulletTime() const;                         // 0x004C9920
#ifdef FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): FableWin.pdb ?SetIsAffectedByBulletTimeOverride@CThing@@QAEX_N@Z,
    // inline in retail (`mov bl,[eax+93h] ; and bl,0DFh ; mov [eax+93h],bl` = bit 21 of the +0x91 run). Body only: no layout change.
    void SetIsAffectedByBulletTimeOverride(bool value) { IsAffectedByBulletTimeOverride = value; }
    // and FableWin.pdb ?SetScriptName@CThing@@QAEXABVCCharString@@@Z, inline in retail (`lea ecx,[eax+74h] ; call CCharString::operator=`).
    void SetScriptName(const CCharString& name) { ScriptName = name; }
#endif
    // FableWin.pdb-only inline flag accessors (?IsInLimbo@CThing@@QBE_NXZ, ?IsActivated@, ?IsDying@, ?IsKillLocked@,
    // ?IsGloballyLoaded@, ?IsDeathPersistent@; inline in retail, which tests the +0x91..+0x93 bits in place in every caller --
    // the same evidence as GetTypeFlags above).
    bool IsInLimbo() const { return InLimbo != 0; }
    bool IsActivated() const { return Activated != 0; }
    bool IsDying() const { return Dying != 0; }
    bool IsKillLocked() const { return KillLocked != 0; }
    bool IsGloballyLoaded() const { return GloballyLoaded != 0; }
    bool IsDeathPersistent() const { return DeathPersistent != 0; }
protected:
    void UpdateThingScanInclusion();                             // 0x004C8C00 ?UpdateThingScanInclusion@CThing@@IAEXXZ (3 rows)
    const CPlayerManager& PeekPlayerManager() const;             // 0x004C79F0 ?PeekPlayerManager@CThing@@IBEABVCPlayerManager@@XZ (3 rows)
public:
#endif
#ifdef FABLE_RETAIL_CTHING_MOREACCESS
    // Nested opt-in (plan phase 7, 2026-11-06): more public / protected non-virtual members the CThing rows call out of line. Each
    // is in both PDB name sets (Ego_r.pdb + FableWin.pdb, the spelling below) and is the catalog row at the retail address; the
    // rows' local CThing models declared them at those addresses. Declarations only: no layout change.
    void SetAsFrameUpdated();                                    // 0x004C7C40 ?SetAsFrameUpdated@CThing@@QAEXXZ
    static CThing* __fastcall Create(long,long,long,bool);       // 0x004CA1D0 ?Create@CThing@@SIPAV1@JJJ_N@Z
    const CGameDefinitionManager& PeekDefinitionManager() const; // 0x004C7990 ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ
    CASoundBank& GetSampleBank() const;                          // 0x004C7A80 ?GetSampleBank@CThing@@QBEAAVCASoundBank@@XZ
    long GetJoystickDeviceNumber() const;                        // 0x004C7CA0 ?GetJoystickDeviceNumber@CThing@@QBEJXZ
    bool IsRenderedThisFrame() const;                            // 0x004C78F0 ?IsRenderedThisFrame@CThing@@QBE_NXZ
    bool IsForceNoSerialise() const;                             // 0x004C7BD0 ?IsForceNoSerialise@CThing@@QBE_NXZ
    bool IsFreeForDeletion() const;                              // 0x004C7500 ?IsFreeForDeletion@CThing@@QBE_NXZ
    const CCharString& GetTypeName() const;                      // 0x004C7E10 ?GetTypeName@CThing@@QBEABVCCharString@@XZ
    void CreateUID();                                            // 0x004C7FF0 ?CreateUID@CThing@@QAEXXZ
    bool IsUnderScriptControl() const;                           // 0x004C8C90 ?IsUnderScriptControl@CThing@@QBE_NXZ
    void SetToKillOnLevelUnload(bool);                           // 0x004C9C40 ?SetToKillOnLevelUnload@CThing@@QAEX_N@Z
    void FrameUpdate();                                          // 0x004C9D20 ?FrameUpdate@CThing@@QAEXXZ
protected:
    const CWorldMap& PeekWorldMap() const;                       // 0x004C79E0 ?PeekWorldMap@CThing@@IBEABVCWorldMap@@XZ
public:
#endif
#ifdef FABLE_RETAIL_CTHING_OPERATORNEW
    // Nested opt-in (plan phase 7, 2026-11-06): CThing::operator new(size_t, bool) 0x004C7380, Ego_r.pdb / FableWin.pdb
    // ??2CThing@@KIPAXI_N@Z (protected static __fastcall: ecx = size, dl = the flag the thing manager is asked about; the catalog's
    // CThing::operator new). Every derived class's Alloc / Create calls it as `new (flag) CThingX` (13 landed rows modelled it
    // locally). A declaration only: no layout change. It hides the global operator new for CThing and its derived classes, as
    // in retail, so only the TUs that allocate things opt in.
protected:
    static void* __fastcall operator new(unsigned int size, bool flag);  // 0x004C7380
public:
#endif
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
#ifdef FABLE_RETAIL_CTHING_MANAGERACCESS
    // Nested opt-in (chunk 765, rows 0x004C7A50 / 0x004C7A60 / 0x004C7BC0): the current thing manager, a private static at 0x013B8A1C
    // (the established spelling ?PCurrentThingManager@CThing@@0PAVCThingManager@@A of 65 landed rows), and its Ego_r.pdb / FableWin.pdb
    // public static __fastcall accessors ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ /
    // ?PeekPCurrentThingManager@CThing@@SIPBVCThingManager@@XZ (inline in retail: every caller loads [0x013B8A1C] in place); the
    // protected out-of-line forwarders ?GetThingSearchTools@CThing@@IAEAAVCThingSearchTools@@XZ (0x004C7A50),
    // ?PeekThingSearchTools@CThing@@IBEABVCThingSearchTools@@XZ (0x004C7A60) and ?CheckUID@CThing@@IBEXXZ (0x004C7BC0): retail keeps
    // Ego_r's donor order PeekPlayer / GetPlayer / GetThingSearchTools / PeekThingSearchTools / PeekComponent (0x004C7A10..0x004C7A70)
    // and IsToBePlacedInMap / CheckUID / IsForceNoSerialise (0x004C7B80..0x004C7BD0). No layout change.
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }
    static const CThingManager* __fastcall PeekPCurrentThingManager() { return PCurrentThingManager; }
protected:
    CThingSearchTools& GetThingSearchTools();                    // 0x004C7A50
    const CThingSearchTools& PeekThingSearchTools() const;       // 0x004C7A60
    void CheckUID() const;                                       // 0x004C7BC0
private:
    static CThingManager* PCurrentThingManager;                  // 0x013B8A1C
public:
#endif
#ifdef FABLE_RETAIL_CTHING_ADDANDCOPYTC
    // Chunk 756 (row 0x004C9F60): Ego_r.pdb ?AddAndCopyTC@CThing@@QAEXABVCTCBase@@_N@Z (out of line) and the FableWin.pdb member template
    // ??$GetTC@VCTCBase@@@CThing@@QAE_NW4ETCInterfaceType@@PAPAVCTCBase@@@Z (the non-const twin of PeekTC(id, ptc); __forceinline like it).
    void AddAndCopyTC(const CTCBase& other,bool flag);
#ifndef FABLE_RETAIL_CTHING_GETTC_EXPANDED
    template<class T> __forceinline bool GetTC(ETCInterfaceType id,T** ptc) {
        if(HasTC(id)) {
            CTCBase& base=GetTC(id);
            *ptc=GFDebugPDynamicCast(&base,(T*)0);
            return true;
        }
        return false;
    }
#endif
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
#ifdef FABLE_RETAIL_CTHING_LEVELSCRIPT
    // Nested opt-in (chunk 771, row 0x006A5D90 CThingPhysical::ConstructFromLevelScript): Ego_r.pdb
    // ?ConstructFromLevelScript@CThing@@UAE_NPBD@Z (public virtual; 0x004CAA00, called by name from the CThingPhysical override,
    // engine_api.tsv `CThing *,char *`) and ?GetWorldMap@CThing@@IAEAAVCWorldMap@@XZ (protected; out of line at 0x004C79D0, the
    // catalog's CThing::GetWorldMap). Declarations only: CThing already has its vptr (CClass), so no layout change.
public:
    virtual bool ConstructFromLevelScript(const char* script);   // 0x004CAA00
protected:
    CWorldMap& GetWorldMap();                                    // 0x004C79D0
public:
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
