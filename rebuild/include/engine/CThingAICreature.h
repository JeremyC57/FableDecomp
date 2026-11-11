#ifndef FABLE_ENGINE_CTHINGAICREATURE_H
#define FABLE_ENGINE_CTHINGAICREATURE_H
#ifdef FABLE_RETAIL_CTHINGAICREATURE
// Retail opt-in (2026-11-03, row 0x005EB9E0): the retail class chain. RTTI .?AVCThingAICreature@@ : CThingCreatureBase (ghidra_out/
// rtti_bases.tsv); sizeof 0x1D8 in retail (the default branch's STLport layout below). The members after the base (Ego_r.pdb
// CThingAICreature, RelationshipsInit .. Suspenders) stay opaque here. Needs the retail CThingCreatureBase chain
// (FABLE_RETAIL_CTHINGCREATUREBASE with FABLE_RETAIL_CTHINGGAMEOBJECT, FABLE_RETAIL_CTHINGPHYSICAL and the CThing chain).
// CTCTavernGame::FrameUpdate tests its game master through CIntelligentPointer<CThingAICreature> (IS_THING_ALIVE: GetPItem 0x00A01B50,
// then CThing::Dead, bit 0 of +0x91).
#include "engine/CThingCreatureBase.h"
#ifdef FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#include "engine/CIntelligentPointer.h"
class CThingBuilding;
#endif
class C3DVector; class CThingAICreatureInit; class CBrainDef;
#ifdef FABLE_RETAIL_CTHINGAICREATURE_SCRIPTPRIORITY
#if defined(_MSC_VER)
enum EScriptAIPriority;                                                  // chunk 1010b: MS forward enum
#else
enum EScriptAIPriority { };
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CEnginePrimitiveHandle.h"
class CAIBrain; class CEnginePrimitivePrepareInfo;   // chunk 801
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_MESSAGERANGE
class CMessageEvent;                                                     // chunk 765
#endif
class CThingAICreature : public CThingCreatureBase {
public:
#ifdef FABLE_RETAIL_CTHINGAICREATURE_ANIMEXISTS
    // Retail opt-in (agentNF9, row 0x00830F20): Ego_r.pdb / FableWin.pdb ?AnimExists@CThingAICreature@@MBE_NABVCCharString@@_N@Z (protected
    // const virtual, the override of CThingCreatureBase's slot 73: CThingAICreature vtable 0x0127293C slot 73 = 0x00830F20). Declaration only.
protected:
    virtual bool AnimExists(const CCharString& name, bool useModeFilterOnly) const;   // 0x00830F20
public:
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_DRAINED
    void SuspendAI(bool suspend,void* suspender);
    void ForceAIReset();
#endif
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingAICreature@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
    // Nested opt-in (chunk 1016, row 0x00934E50 via CMessageEvent::GetPOriginator<CThingAICreature>): FableWin.pdb
    // ?StatGetTypeFlags@CThingAICreature@@SIKXZ (static fastcall, inline in retail: the type test `not eax ; test al,1Fh`, 0x00934E9D).
    static unsigned long __fastcall StatGetTypeFlags() { return 0x1f; }
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE
    // Nested opt-in (chunk 853, row 0x0050A650): FableWin.pdb ?StatGetType@CThingAICreature@@SIJXZ (inline); retail
    // GetAllLeafThings<CThingAICreature> 0x00516830 scans type list 1 (`push 1 ; call PeekTypeList`). No layout change.
    static long __fastcall StatGetType() { return 1; }
#endif
    // chunk 719 (row 0x004FF750): Ego_r.pdb ?Create@CThingAICreature@@SIPAV1@JABVC3DVector@@JABVCThingAICreatureInit@@@Z (retail
    // 0x00833800), ?AutoSetupSexuality@CThingAICreature@@QAEXM@Z (0x008330D0) and FableWin's inline
    // ?SetAsAllowedToFollowHero@CThingAICreature@@QAEX_N@Z (emitted beside the CWorldMap TU's statics in FableWin; retail expands it:
    // bit 0 of +0x1C9, as CGameScriptInterface::EntitySetAsAllowedToFollowHero 0x0088DE90 shows).
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& pos, long, const CThingAICreatureInit& init);   // 0x00833800
    void AutoSetupSexuality(float probabilityOfHomosexuality);           // 0x008330D0
    void SetAsAllowedToFollowHero(bool allowed) { AllowedToFollowHero = allowed; }
#ifdef FABLE_RETAIL_CTHINGAICREATURE_IS_ALLOWED_TO_FOLLOW
    // Original FableWin02496dd0 and native006d4ebc: existing original flag.
    bool IsAllowedToFollowHero()const{return AllowedToFollowHero;}
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_SCRIPTPRIORITY
    // Nested opt-in (chunk 1010b, row 0x0089B5B0 StartScriptingEntity): Ego_r.pdb
    // ?GetCurrentStateGroupScriptPriority@CThingAICreature@@QBE?AW4EScriptAIPriority@@XZ, out of line in retail at 0x006D5AD0
    // (`mov ecx,[ecx+1A8h] ; jmp 0x0088B760` = CAIBrain::GetCurrentStateGroupScriptPriority; landing 2026-11-04x label). Declaration only.
    EScriptAIPriority GetCurrentStateGroupScriptPriority() const;        // 0x006D5AD0
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_INFORMATION
    // Nested opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb
    // ?GetHasInformation@CThingAICreature@@QBE_NXZ, out of line at 0x008300D0 (`mov al,[ecx+1C8h] ; shr al,2 ; and al,1`: bit 2 of
    // +0x1C8 = HasInformation below, the bit the landed SetHasInformation 0x00830EB0 writes; the manifest's "CTCPhysicsStandard::
    // CanCollideWithLandscape" there is a propagated label of a byte-identical getter). Declaration only.
    bool GetHasInformation() const;                                      // 0x008300D0
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_IMMUNETOATTACK
    // Nested opt-in (chunk 854, row 0x00924C70 CAIStateGroup_KrakenAttack::Update): FableWin.pdb ?SetAsImmuneToAttack@CThingAICreature@@QAEX_N@Z
    // (Ego_r.pdb has the ?IsImmuneToAttack@..QBE_NXZ twin), inline in retail: `or cl,40h` on +0x1C8 = bit 6 ImmuneToAttack below.
    void SetAsImmuneToAttack(bool immune) { ImmuneToAttack = immune; }
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_INITFROMDEF
    // Nested opt-in (agentNF12, 2026-11-11, row 0x008338D0): Ego_r.pdb / FableWin.pdb ?InitialiseFromDef@CThingAICreature@@IAEXXZ (protected)
    // and ?InitialiseMovementSpeeds@CThingAICreature@@QAEXXZ (retail 0x00830DD0, out of line). Needs FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT
    // (OverridingBrainName) for the speed members it names. Declarations only.
    void InitialiseMovementSpeeds();                                     // 0x00830DD0
protected:
    void InitialiseFromDef();                                            // 0x008338D0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_SETNEWBRAIN
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): Ego_r.pdb / FableWin.pdb
    // ?SetNewBrain@CThingAICreature@@QAEXV?$CDefPointer@$$CBVCBrainDef@@@@@Z (retail 0x00833010, out of line; the CDefPointer is passed by
    // value: `push ecx ; mov ecx,esp ; mov [ecx],eax ; inc [eax+4]`). Declaration only: no layout change.
    void SetNewBrain(CDefPointer<const CBrainDef> brain);                // 0x00833010
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_MESSAGERANGE
    // Nested opt-in (chunk 765, row 0x006AF270 CMessageEventFilter_CanBeHeard::operator()): Ego_r.pdb / FableWin.pdb
    // ?IsMessageEventWithinSightRange@CThingAICreature@@QBE_NABVCMessageEvent@@@Z (0x00833230) and
    // ?IsMessageEventWithinHearingRange@CThingAICreature@@QBE_NABVCMessageEvent@@@Z (0x008332E0), out of line in retail (the message
    // filters CanBeSeenOrHeard 0x006AF220 / CanBeSeen 0x006AF260 / CanBeHeard 0x006AF270 call them). Declarations only.
    bool IsMessageEventWithinSightRange(const CMessageEvent& event) const;     // 0x00833230
    bool IsMessageEventWithinHearingRange(const CMessageEvent& event) const;   // 0x008332E0
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
    // Nested opt-in (chunk 760, row 0x00946400 CAIStateGroup_PrepareMeal::IsRunnable): Ego_r.pdb HomeBuilding
    // (CIntelligentPointer<CThingBuilding>, retail +0x168: `lea ecx,[eax+168h] ; call 0x00A01B50`) and FableWin.pdb
    // ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ (inline in retail). Needs FABLE_RETAIL_CINTELLIGENTPOINTER.
    CThingBuilding* GetHomeBuilding() const;                            // body in the includer once CThingBuilding is complete
#ifdef FABLE_RETAIL_CTHINGAICREATURE_SAVE
    // Retail opt-in (chunk 794, row 0x008304A0 CThingAICreature::Save): Ego_r.pdb / FableWin.pdb ?Save@CThingAICreature@@UAEXAAVCCharString@@@Z
    // (public virtual; it calls ?Save@CThingCreatureBase@@UAEXAAVCCharString@@@Z 0x00661A60 by name). The Ego_r.pdb members after
    // HomeBuilding keep their order and sizes in retail: WorkBuilding / FatherCreature / MotherCreature / SpouseCreature
    // (CIntelligentPointer<CThingBuilding|CThingAICreature>, 8 bytes each) at +0x170 / +0x178 / +0x180 / +0x188 and
    // OverridingBrainName (CCharString) at +0x190 -- Save reads them at exactly those offsets (`lea esi,[ebp+170h] ; call 0x00A01B50`,
    // ..., `mov ecx,[ebp+190h]`), the generated default branch below lists the same offsets from the PDB. No size change.
    virtual void Save(CCharString& out);                                // 0x008304A0
#ifdef FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
    // Nested opt-in (chunk 794, row 0x006C6040 CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed): FableWin.pdb
    // ?GetWorkBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ, inline in retail (`lea ecx,[esi+170h] ; call 0x00A01B50`).
    // Body in the includer once CThingBuilding is complete, like GetHomeBuilding.
    CThingBuilding* GetWorkBuilding() const;
#ifdef FABLE_RETAIL_CTHINGAICREATURE_PEEKWORKBUILDING
    // Original debug 02032620 and native 0090bbfa: WorkBuilding.Get().
    const CThingBuilding* PeekWorkBuilding() const;
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT
    // Nested opt-in (chunk 796, row 0x008315C0): Ego_r.pdb / FableWin.pdb ?ConstructFromLevelScript@CThingAICreature@@UAE_NPBD@Z (vtable
    // 0x0127293C slot 4, public virtual) and the nested CRelationshipsInit (Ego_r.pdb: five unsigned __int64 UIDs, sizeof 0x28;
    // ??0CRelationshipsInit@CThingAICreature@@QAE@XZ inline: retail `new(0x28)` then ten zero dword stores) held in
    // RelationshipsInit (CCountedPointer<CRelationshipsInit>, +0x160; Reset out of line at 0x00834D30,
    // ?Reset@?$CCountedPointer@VCRelationshipsInit@CThingAICreature@@@@QAEXPAVCRelationshipsInit@CThingAICreature@@@Z).
    // Same offsets as the opaque pad it replaces: no layout change.
    class CRelationshipsInit {
    public:
        unsigned __int64 HomeBuildingUID;                                // +0x00
        unsigned __int64 WorkBuildingUID;                                // +0x08
        unsigned __int64 FatherCreatureUID;                              // +0x10
        unsigned __int64 MotherCreatureUID;                              // +0x18
        unsigned __int64 SpouseCreatureUID;                              // +0x20
        CRelationshipsInit() : HomeBuildingUID(0), WorkBuildingUID(0), FatherCreatureUID(0), MotherCreatureUID(0), SpouseCreatureUID(0) { }
    };
    virtual bool ConstructFromLevelScript(const char* script);          // 0x008315C0
private:
    CCountedPointer<CRelationshipsInit> RelationshipsInit;               // +0x160
#else
private:
    unsigned char _pad_0x160[0x8];                                       // +0x160 RelationshipsInit (opaque)
#endif
    CIntelligentPointer<CThingBuilding> HomeBuilding;                    // +0x168
    CIntelligentPointer<CThingBuilding> WorkBuilding;                    // +0x170
    CIntelligentPointer<CThingAICreature> FatherCreature;                // +0x178
    CIntelligentPointer<CThingAICreature> MotherCreature;                // +0x180
    CIntelligentPointer<CThingAICreature> SpouseCreature;                // +0x188
    CCharString OverridingBrainName;                                     // +0x190
#ifdef FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
    // Nested opt-in (chunk 801, row 0x00833C00): Ego_r.pdb / FableWin.pdb
    // ?DrawPreparePrimitivesForRendering@CThingAICreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z (vtable 0x0127293C slot 36, public
    // virtual; it calls the CThingCreatureBase one 0x00663C40 first) and the Ego_r.pdb members DebugRenderHandles
    // (list<CEnginePrimitiveHandle>, retail +0x194: `lea edi,[esi+194h]` then clear 0x00834F90), MoodSprite / MoodSpriteCounter
    // (+0x198 / +0x19C), PCurrentBrainOutput (+0x1A0, opaque) and PBrain (CCountedPointer<CAIBrain>, +0x1A8: `cmp [esi+1A8h],0`
    // then `mov ecx,[esi+1A8h] ; call 0x0088B7E0`). The offsets agree with the landed ~CThingAICreature 0x008320C0. No size change.
    // Needs FABLE_RETAIL_STLP_LIST (back()); the includes above bring the list, CCountedPointer and CEnginePrimitiveHandle (whose
    // retail branch needs FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE).
public:
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x00833C00
private:
    std::list<CEnginePrimitiveHandle> DebugRenderHandles;                // +0x194
    unsigned long MoodSprite;                                            // +0x198
    unsigned long MoodSpriteCounter;                                     // +0x19C
    unsigned char _pad_0x1a0[0x8];                                       // +0x1A0 PCurrentBrainOutput (opaque)
    CCountedPointer<CAIBrain> PBrain;                                    // +0x1A8
    unsigned char _pad_0x1b0[0x18];                                      // +0x1B0 WFTimeLastDamaged .. MaxFlyingSpeed (opaque)
#else
#ifdef FABLE_RETAIL_CTHINGAICREATURE_INITFROMDEF
    // The Ego_r.pdb members after DebugRenderHandles .. PBrain (opaque here) in order and spelling: WFTimeLastDamaged,
    // VillageTaskPerforming, then the four speeds InitialiseFromDef stores at +0x1B8 / +0x1BC / +0x1C0 / +0x1C4 (0x008338E0..0x0083390A;
    // Ego_r +488..+500, 0x30 higher). Same offsets as the pad it names.
    unsigned char _pad_0x194[0x1c];                                      // +0x194 DebugRenderHandles .. PBrain (opaque)
    float WFTimeLastDamaged;                                             // +0x1B0
    class CVillageTask* VillageTaskPerforming;                           // +0x1B4
    float MaxWalkingSpeed;                                               // +0x1B8
    float MaxJoggingSpeed;                                               // +0x1BC
    float MaxRunningSpeed;                                               // +0x1C0
    float MaxFlyingSpeed;                                                // +0x1C4
#else
    unsigned char _pad_0x194[0x34];                                      // +0x194 DebugRenderHandles .. MaxFlyingSpeed (opaque)
#endif
#endif
#else
private:
    unsigned char _pad_0x160[0x8];                                       // +0x160 RelationshipsInit (opaque)
    CIntelligentPointer<CThingBuilding> HomeBuilding;                    // +0x168
    unsigned char _pad_0x170[0x58];                                      // +0x170 .. MaxFlyingSpeed (Ego_r.pdb members, opaque)
#endif
#else
private:
    unsigned char _pad_0x160[0x68];                                      // +0x160 RelationshipsInit .. MaxFlyingSpeed (Ego_r.pdb members, opaque)
#endif
    bool PerformingVillageTaskFlag:1, AISuspended:1, HasInformation:1, WanderWithInformation:1,
        WaveWithInformation:1, ContinueAIWithInformation:1, ImmuneToAttack:1, EnableCreatureAutoPlacing:1;   // +0x1C8 (Ego_r 0x1F8)
    bool AllowedToFollowHero:1, RegionFollowingOverriddenFromScript:1, RespondingToFollowAndWait:1,
        CanBeCourted:1, CanBeMarried:1;                                  // +0x1C9 (Ego_r 0x1F9; CanBeCourted bit 3 / CanBeMarried bit 4 landed)
    unsigned char _pad_0x1ca[0xe];                                       // +0x1CA Suspenders (opaque)
};
FABLE_STATIC_ASSERT(sizeof(CThingAICreature) == 0x1d8);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingAICreature = 0x210.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingAICreature = 0x1d8.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CAIBrain;
class CBrainOutput;
class CCPPointerInfo;
class CCharStringData;
struct CRelationshipsInit;
struct CVillageTask;

#pragma pack(push, 1)
class CThingAICreature {
public:
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char       _pad_0x04[0x4];                      // +0x04
    unsigned char       _pad_0x08[0x4];                      // +0x08
    unsigned char       _pad_0x0c[0x4];                      // +0x0c
    unsigned char       _pad_0x10[0x1];                      // +0x10
    unsigned char       _pad_0x11[0x7];                      // +0x11
    unsigned char       _pad_0x18[0x4];                      // +0x18
    unsigned char       _pad_0x1c[0x4];                      // +0x1c
    unsigned char       _pad_0x20[0x38];                     // +0x20
    unsigned char       _pad_0x58[0x4];                      // +0x58
    unsigned char       _pad_0x5c[0x4];                      // +0x5c
    unsigned char       _pad_0x60[0x4];                      // +0x60
    unsigned char       _pad_0x64[0x4];                      // +0x64
    unsigned char       _pad_0x68[0x4];                      // +0x68
    unsigned char       _pad_0x6c[0x4];                      // +0x6c
    unsigned char       _pad_0x70[0x4];                      // +0x70
    unsigned char       _pad_0x74[0x4];                      // +0x74
    unsigned char       _pad_0x78[0x4];                      // +0x78
    unsigned char       _pad_0x7c[0x4];                      // +0x7c
    unsigned char       _pad_0x80[0x4];                      // +0x80
    unsigned char       _pad_0x84[0x4];                      // +0x84
    unsigned char       _pad_0x88[0x4];                      // +0x88
    unsigned char       _pad_0x8c[0x4];                      // +0x8c
    unsigned char       _pad_0x90[0x4];                      // +0x90
    unsigned char       _pad_0x94[0x1];                      // +0x94
    unsigned char       _pad_0x95[0x7];                      // +0x95
    unsigned char       _pad_0x9c[0x4];                      // +0x9c
    unsigned char       _pad_0xa0[0x4];                      // +0xa0
    unsigned char       _pad_0xa4[0x4];                      // +0xa4
    unsigned char       _pad_0xa8[0x4];                      // +0xa8
    unsigned char       _pad_0xac[0x4];                      // +0xac
    unsigned char       _pad_0xb0[0x4];                      // +0xb0
    unsigned char       _pad_0xb4[0x4];                      // +0xb4
    unsigned char       _pad_0xb8[0x4];                      // +0xb8
    unsigned char       _pad_0xbc[0x1];                      // +0xbc
    unsigned char       _pad_0xbd[0x3];                      // +0xbd
    unsigned char       _pad_0xc0[0x4];                      // +0xc0
    unsigned char       _pad_0xc4[0x4];                      // +0xc4
    unsigned char       _pad_0xc8[0x1];                      // +0xc8
    unsigned char       _pad_0xc9[0x7];                      // +0xc9
    unsigned char       _pad_0xd0[0x4];                      // +0xd0
    unsigned char       _pad_0xd4[0xc];                      // +0xd4
    unsigned char       _pad_0xe0[0x4];                      // +0xe0
    unsigned char       _pad_0xe4[0x4];                      // +0xe4
    unsigned char       _pad_0xe8[0x4];                      // +0xe8
    unsigned char       _pad_0xec[0x4];                      // +0xec
    unsigned char       _pad_0xf0[0x4];                      // +0xf0
    unsigned char       _pad_0xf4[0x4];                      // +0xf4
    unsigned char       _pad_0xf8[0x4];                      // +0xf8
    unsigned char       _pad_0xfc[0x4];                      // +0xfc
    unsigned char       _pad_0x100[0x4];                     // +0x100
    unsigned char       _pad_0x104[0x4];                     // +0x104
    unsigned char       _pad_0x108[0x4];                     // +0x108
    unsigned char       _pad_0x10c[0x4];                     // +0x10c
    unsigned char       _pad_0x110[0x4];                     // +0x110
    unsigned char       _pad_0x114[0x4];                     // +0x114
    unsigned char       _pad_0x118[0x4];                     // +0x118
    unsigned char       _pad_0x11c[0x4];                     // +0x11c
    unsigned char       _pad_0x120[0x4];                     // +0x120
    unsigned char       _pad_0x124[0x4];                     // +0x124
    unsigned char       _pad_0x128[0x4];                     // +0x128
    unsigned char       _pad_0x12c[0x4];                     // +0x12c
    unsigned char       _pad_0x130[0x4];                     // +0x130
    unsigned char       _pad_0x134[0x4];                     // +0x134
    unsigned char       _pad_0x138[0x4];                     // +0x138
    unsigned char       _pad_0x13c[0x4];                     // +0x13c
    unsigned char       _pad_0x140[0x4];                     // +0x140
    unsigned char       _pad_0x144[0xc];                     // +0x144
    unsigned char       _pad_0x150[0x4];                     // +0x150
    unsigned char       _pad_0x154[0x4];                     // +0x154
    unsigned char       _pad_0x158[0x4];                     // +0x158
    unsigned char       _pad_0x15c[0x3];                     // +0x15c
    unsigned char       _pad_0x15f[0x1];                     // +0x15f
    CRelationshipsInit* RelationshipsInit_Data;              // +0x160 CCountedPointer<CThingAICreature::CRelationshipsInit>::Data
    CCPPointerInfo*     RelationshipsInit_Info;              // +0x164 CCountedPointer<CThingAICreature::CRelationshipsInit>::Info
    unsigned char       HomeBuilding[0x8];                   // +0x168 CIntelligentPointer<CThingBuilding>
    unsigned char       WorkBuilding[0x8];                   // +0x170 CIntelligentPointer<CThingBuilding>
    unsigned char       FatherCreature[0x8];                 // +0x178 CIntelligentPointer<CThingAICreature>
    unsigned char       MotherCreature[0x8];                 // +0x180 CIntelligentPointer<CThingAICreature>
    unsigned char       SpouseCreature[0x8];                 // +0x188 CIntelligentPointer<CThingAICreature>
    CCharStringData*    OverridingBrainName_PStringData;     // +0x190 CCharString::PStringData
    unsigned char       DebugRenderHandles[0x4];             // +0x194 list<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned long       MoodSprite;                          // +0x198
    unsigned long       MoodSpriteCounter;                   // +0x19c
    CBrainOutput*       PCurrentBrainOutput_Data;            // +0x1a0 CCountedPointer<CBrainOutput>::Data
    CCPPointerInfo*     PCurrentBrainOutput_Info;            // +0x1a4 CCountedPointer<CBrainOutput>::Info
    CAIBrain*           PBrain_Data;                         // +0x1a8 CCountedPointer<CAIBrain>::Data
    CCPPointerInfo*     PBrain_Info;                         // +0x1ac CCountedPointer<CAIBrain>::Info
    float               WFTimeLastDamaged;                   // +0x1b0
    CVillageTask*       VillageTaskPerforming;               // +0x1b4
    float               MaxWalkingSpeed;                     // +0x1b8
    float               MaxJoggingSpeed;                     // +0x1bc
    float               MaxRunningSpeed;                     // +0x1c0
    float               MaxFlyingSpeed;                      // +0x1c4
    unsigned char       EnableCreatureAutoPlacing[0x1];      // +0x1c8 bitfield group: bool:1
    unsigned char       CanBeMarried[0x1];                   // +0x1c9 bitfield group: bool:1
    unsigned char       _pad_0x1ca[0x2];                     // +0x1ca
    unsigned char       Suspenders[0xc];                     // +0x1cc set<void_*,std::less<void_*>,std::allocator<void_*>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingAICreature) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, RelationshipsInit_Data) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, RelationshipsInit_Info) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, HomeBuilding) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, WorkBuilding) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, FatherCreature) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, MotherCreature) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, SpouseCreature) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, OverridingBrainName_PStringData) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, DebugRenderHandles) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, MoodSprite) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, MoodSpriteCounter) == 0x19c);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, PCurrentBrainOutput_Data) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, PCurrentBrainOutput_Info) == 0x1a4);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, PBrain_Data) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, PBrain_Info) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, WFTimeLastDamaged) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, VillageTaskPerforming) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, MaxWalkingSpeed) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, MaxJoggingSpeed) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, MaxRunningSpeed) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, MaxFlyingSpeed) == 0x1c4);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, EnableCreatureAutoPlacing) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, CanBeMarried) == 0x1c9);
FABLE_STATIC_ASSERT(offsetof(CThingAICreature, Suspenders) == 0x1cc);

#endif // FABLE_RETAIL_CTHINGAICREATURE
#endif // FABLE_ENGINE_CTHINGAICREATURE_H
