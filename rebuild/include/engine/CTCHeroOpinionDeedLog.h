#ifndef FABLE_ENGINE_CTCHEROOPINIONDEEDLOG_H
#define FABLE_ENGINE_CTCHEROOPINIONDEEDLOG_H
// New header (chunk 794). Retail opt-in only: FABLE_RETAIL_CTCHEROOPINIONDEEDLOG. No default path exists (no repo header before).
// Base: Fable.exe RTTI .?AVCTCHeroOpinionDeedLog@@ : CTCBase. Interface TCI_HERO_OPINION_DEED_LOG (0x51; OnHit 0x007039A0 tests
// [thing+0x28] & 0x20000 and looks the component up with key 0x51). Methods: Ego_r.pdb / FableWin.pdb spellings
//   ?PostDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@ABVC3DVector@@PBVCThing@@PAVCThingAICreature@@W4EOpinionPostDeedType@@_N5PBVCFaction@@5J@Z (0x006C6990)
//   ?InformRelevantThingsOfDeed@CTCHeroOpinionDeedLog@@AAE_NJPAVCThingAICreature@@W4EOpinionPostDeedType@@PBVCFaction@@_N@Z (0x006C6040)
// Layout (Ego_r.pdb struct_layouts_egor.tsv, retail STLport map/list shifts): kept opaque past CTCBase here.
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#include "engine/CTCBase.h"
#include "engine/EOpinionDeedType.h"
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CTOR_PASS27
#include "engine/CWitnessesSearchingDeed.h"
#endif
class C3DVector;
class CThing;
class CThingAICreature;
class CFaction;
class COpinionDeed;
class COpinionDeedEffectsDef;
enum EOpinionPostDeedType;
class CTCHeroOpinionDeedLog : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CTOR_PASS27
    CTCHeroOpinionDeedLog(CThing&);
    virtual ~CTCHeroOpinionDeedLog();
    virtual void OnInitialActivate();
    virtual void FrameUpdate();
    virtual CCharString GetClassTypeName() const;
    virtual ETCInterfaceType GetInterfaceType() const;
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_OPINION_DEED_LOG; }
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_ISDEEDWITNESSED
    // Nested opt-in (NF435, row 0x0089C8A0 CGameScriptInterface::IsDeedWitnessed): Ego_r.pdb / FableWin.pdb
    // ?IsDeedWitnessed@CTCHeroOpinionDeedLog@@QAE_NJ_N@Z, out of line in retail at 0x006C5E90 (landed). Declaration only: no layout change.
    bool IsDeedWitnessed(long, bool);                                    // 0x006C5E90
#endif
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1);   // 0x006C6990
private:
    bool InformRelevantThingsOfDeed(long handle, CThingAICreature* creature, EOpinionPostDeedType postType,
                                    const CFaction* faction, bool b1);                                          // 0x006C6040
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CRIMES
    // e10 opt-in (row 0x0065CB6B CTCVillage::ReportCrime): Ego_r.pdb ?SetVillageForCrimes@CTCHeroOpinionDeedLog@@QAEXAAVCThing@@@Z (0x006C55E0),
    // ?SetCrimeDeedReportedToVillage@CTCHeroOpinionDeedLog@@QAEXJ@Z (0x006C5A50), ?FindCriminalDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@PBVCThing@@@Z
    // (0x006C5800), all public and out of line (calls at 0x0065CBD8 / 0x0065CBE2 / 0x0065CD07). Declarations only: no layout change.
public:
    void SetVillageForCrimes(CThing& village);                           // 0x006C55E0
    void SetCrimeDeedReportedToVillage(long deed_handle);                // 0x006C5A50
    long FindCriminalDeed(EOpinionDeedType deed_type, const CThing* target);   // 0x006C5800
private:
#endif
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
    // Nested opt-in (chunk 794, row 0x006C6040 InformRelevantThingsOfDeed): Ego_r.pdb OpinionDeedMap (std::map<long, COpinionDeed*>,
    // retail +0x0C: `lea esi,[edi+0Ch]` then the out-of-line find 0x006C7830) and
    // ?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z (out of line,
    // 0x006C5630: hidden result + the type). Needs stlport/fable_map.h and engine/CDefPointer.h. The members after the map stay unmodelled.
public:
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;   // 0x006C5630
    // ?GetDeedFromHandle@CTCHeroOpinionDeedLog@@QBEPBVCOpinionDeed@@J@Z (Ego_r.pdb + FableWin.pdb), out of line at 0x006C58F0 (find
    // 0x006C7830, end -> NULL, else the mapped deed) and auto-inlined by its own TU (0x006C6040 copies the by-value handle first).
    const COpinionDeed* GetDeedFromHandle(long handle) const;                                     // 0x006C58F0
private:
    std::map<long, COpinionDeed*> OpinionDeedMap;                        // +0x0c
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
    // Retail opt-in (Codex pass18): Ego_r TPI19ce2 member order. Native004d3f51 allocates34a8;
    // dtor006c5d70 proves list+18, intelligent pointer+20 and92 reaction records of90 bytes.
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CTOR_PASS27
    std::list<CWitnessesSearchingDeed> OpinionDeedsSearchingForWitnesses;
#else
    unsigned char OpinionDeedsSearchingForWitnesses[4]; // std::list<CWitnessesSearchingDeed>,+18
#endif
    long NextHandle; // +1c
    CIntelligentPointer<CThing> VillageForCrimes; // +20
    long WeaponOutDeedHandle; // +28
    bool WeaponOutCrimeEnabled; // +2c
    bool GuardsIgnoreCrimes; // +2d
    unsigned short DeedEffectsDefIndices[92]; // +2e
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDREACTIONS
    // Nested opt-in (p25O, row 0x00960120 CAIStateGroup_DeedReactionRoot::Update): the member typed as Ego_r.pdb has it (needs
    // engine/COpinionDeedReaction.h) and its two accessors, Ego_r.pdb
    // ?GetDeedReactionAnimName@CTCHeroOpinionDeedLog@@QBEABVCCharString@@W4EOpinionDeedType@@W4EOpinionAttitudeType@@@Z and
    // ?GetDeedReactionDialogueTag@CTCHeroOpinionDeedLog@@QBEKW4EOpinionDeedType@@W4EOpinionAttitudeType@@@Z, inline in retail
    // (0x00960256..0x0096026D, 0x00960343 / 0x00960363: the element address and the +0x48 tag read in place).
public:
    const CCharString& GetDeedReactionAnimName(EOpinionDeedType deed, EOpinionAttitudeType attitude) const { return DeedReactions[deed].Animations[attitude]; }
    unsigned long GetDeedReactionDialogueTag(EOpinionDeedType deed, EOpinionAttitudeType attitude) const { return DeedReactions[deed].DialogueTags[attitude]; }
private:
    COpinionDeedReaction DeedReactions[92]; // +e8
#else
    unsigned char DeedReactions[13248]; // COpinionDeedReaction[92],+e8; each has18 CCharString then18 ulong.
#endif
#endif
#endif
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x006C8D20
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x006C8D30
public:
#endif
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_PUTWEAPONAWAY
public:
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): Ego_r.pdb ?PlayerPutWeaponAway@CTCHeroOpinionDeedLog@@QAEXXZ, inferred for 0x006C6010
    // (called 0x004A49A4 on TCI_HERO_OPINION_DEED_LOG; body clears the deed handle +0x28 when not -1; the only other QAEXXZ is PlayerTakenOutWeapon).
    void PlayerPutWeaponAway();                                          // 0x006C6010
#endif // FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_PUTWEAPONAWAY
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_TAKENOUTWEAPON
public:
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): Ego_r.pdb ?PlayerTakenOutWeapon@CTCHeroOpinionDeedLog@@QAEXXZ, inferred for 0x006C6E20
    // (called 0x004A54EE when CTCCarrying::IsThingCarryingWeapon(hero); its twin PlayerPutWeaponAway 0x006C6010 is called at region unload).
    void PlayerTakenOutWeapon();                                         // 0x006C6E20
#endif // FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_TAKENOUTWEAPON
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_ONSERIALISE
public:
    // NF62 opt-in (2026-11-17, row 0x006C7370): Ego_r.pdb ?OnSerialise@CTCHeroOpinionDeedLog@@UAEXAAVCPersistContext@@_N@Z (public
    // override of CTCBase slot 1, 0x00686960 in the base). Declaration only: no layout or vtable change.
    virtual void OnSerialise(CPersistContext& context, bool in_game);   // 0x006C7370
#endif
};
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
FABLE_STATIC_ASSERT_AT(64, sizeof(CTCHeroOpinionDeedLog)==0x34a8);
#endif
#endif
#endif
