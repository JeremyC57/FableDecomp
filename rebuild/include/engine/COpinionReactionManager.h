#ifndef FABLE_ENGINE_COPINIONREACTIONMANAGER_H
#define FABLE_ENGINE_COPINIONREACTIONMANAGER_H
// COpinionReactionManager (new, chunk 765): no RTTI (no virtuals). Ego_r.pdb members (struct_layouts_egor.tsv, 0x738 bytes) at retail
// offsets: the two STLport lists are 4 bytes (PDB 12), so every member after Reactions sits 8 lower (PThingOpinionSource +0x714:
// CGameScriptInterface::OpinionSourceSetAsExclusive 0x00894E9B reads it there; SetThingOpinionSource 0x00700770 sets it); sizeof 0x728.
// The EOpinion* enum arrays are modelled as long arrays (the enums are not declared in the engine headers). Name set (Ego_r / FableWin):
// ?SetThingOpinionSource@COpinionReactionManager@@QAEXPAVCThing@@@Z (Ego_r 0x00550510, then AttachOpinionSource 0x00550640, as retail
// 0x00700770 then 0x00700780). Only the methods rows use are declared.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CDefPointer.h"
#ifdef FABLE_RETAIL_GET_OPINIONS_PASS30
#include <math.h>
#endif
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
enum EOpinionAttitudeType;enum EOpinionReactionType;class CTCOpinionSource;
#endif
class CThing; class CWorld; class CThingAICreature; class CReactionMatch; class COpinionReactionManagerDef;
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_DEEDREACTIONS
enum EOpinionAttitudeType; enum EOpinionReactionType; enum EOpinionDeedType; class CTCOpinionSource;   // (opt-in below)
#endif
#if defined(FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYER) && !defined(FABLE_RETAIL_CTCWIFE_MARRY)
enum EOpinionAttitudeType;
#endif
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTACHOPINIONSOURCE
class CTCOpinionSource;
#endif
class COpinionReactionManager {
#ifdef FABLE_RETAIL_BERSERK_CLEANUP_PASS29
public: void ClearOpinionSource(CThing&, bool);
private:
#endif
#ifdef FABLE_RETAIL_OPINIONREACTIONROOT_ISRUNNABLE_PASS27
public:
    const CThing* PeekThingOpinionSource() const { return PThingOpinionSource.Get(); }
    const std::list<CIntelligentPointer<const CThing> >& PeekOpinionSourcesList() const { return OpinionSourcesList; }
    EOpinionTargetingConditionType GetTargetingCondition(EOpinionReactionType type) const {
        return (EOpinionTargetingConditionType)TargetingCondition[type];
    }
    bool GetAllowWhileCarrying(EOpinionReactionType type) const { return AllowWhileCarrying[type]; }
    EOpinionReactionType GetReactionType(const CThing& thing, const bool* flags, const CTCOpinionSource* source) const {
        return GetAttitudeOrReactionType(thing, false, flags, source);
    }
#endif

#ifdef FABLE_RETAIL_OPINION_STRENGTH_PASS26
private:
    void GetOpinions(const CTCCreatureOpinionOfHero* ptc_opinion,COpinionSet& opinions,const CTCOpinionSource* ptc_source) const;
#ifdef FABLE_RETAIL_GET_OPINIONS_PASS30
    float GetCombinedOpinion(float personal, float village, float scaling) const
    {
        return fabsf(personal) > fabsf(village * scaling) ? personal : village * scaling;
    }
#endif
#endif
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_CTOR
    // Opt-in (chunk 2051, row 0x004A6E30 CWorld::Init): Ego_r.pdb ??0COpinionReactionManager@@QAE@ABVCWorld@@@Z, retail 0x007004B0. Declaration only.
public:
    COpinionReactionManager(const CWorld& world);                        // 0x007004B0
#endif
public:
#if defined(FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYER) && !defined(FABLE_RETAIL_CTCWIFE_MARRY)
    // Retail opt-in (wf7, row 0x007735D0 CTCShop::GetAttitudeBuyPriceMultiplier): Ego_r.pdb
    // ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z alone (out of line, 0x00701840),
    // for rows that do not want the rest of FABLE_RETAIL_CTCWIFE_MARRY. Declaration only.
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing&)const;   // 0x00701840
#endif
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing&)const;
#ifdef FABLE_RETAIL_OPINION_SELECTION_INLINE_ATTITUDE_PASS30
    FABLE_FORCEINLINE
#endif
    EOpinionAttitudeType GetAttitudeType(const CThing& thing,const CTCOpinionSource* source)const {
        return source?(EOpinionAttitudeType)GetAttitudeOrReactionType(thing,true,AttitudeMask,source):GetThingAttitudeToPlayer(thing);
    }
private:
    EOpinionReactionType GetAttitudeOrReactionType(const CThing&,bool,const bool*,const CTCOpinionSource*)const;
public:
#endif
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_REACTIONMATCHSTRENGTH
    // Overlay opt-in (wfD4, row 0x006D1E30): Ego_r.pdb 0x005515E0 ?GetReactionMatchStrength@COpinionReactionManager@@QBEMW4EOpinionReactionType@@ABVCThingAICreature@@@Z
    // (Ego_r order 0x005515E0 .. GetThingAttitudeToPlayer 0x00551920 = retail 0x00701590 .. 0x00701840), out of line, called at 0x006D2794
    // (`push ebx ; push 0Bh ; mov ecx,<manager>`, result `fstp`). Declaration only.
    float GetReactionMatchStrength(EOpinionReactionType type, const class CThingAICreature& creature) const;   // 0x00701590
#endif
#ifdef FABLE_RETAIL_GREETING_PASS23
    // Both PDBs plus original donor call graph/native00683d75..00683e00.
    EOpinionAttitudeType GetAttitudeType(const CThing&,const CTCOpinionSource*) const;
    EOpinionAttitudeType GetBlurredAttitude(EOpinionAttitudeType) const;
    float GetThingOpinionOfPlayer(const CThing&,EOpinion) const;
    const CCharString& GetAttitudeString(EOpinionAttitudeType attitude) const { return AttitudeStrings[attitude]; }
private:
    // Exact private static const array spelling in both PDBs; native013babd0.
    static const CCharString AttitudeStrings[18];
public:
#endif
#ifdef FABLE_RETAIL_CREATURE_ONCREATE_PASS27
    bool AttachOpinionSource(CThing&,long,class CTCOpinionSource**) const;
#endif
#ifdef FABLE_RETAIL_OPINION_FILTER_PASS27
    bool GetAllowWhileFollowingPlayer(EOpinionReactionType type) const { return AllowWhileFollowingPlayer[type]; }
    EOpinionReactionType GetReactionType(const CThing& thing,const bool* flags,const CTCOpinionSource* source) const {
        return GetAttitudeOrReactionType(thing,false,flags,source);
    }
#endif
    void SetThingOpinionSource(CThing* thing);                           // 0x00700770
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYERASSTRING
    // Opt-in (NF52, row 0x007018B0): Ego_r.pdb 0x00551D70 / FableWin.pdb 0x01F16020
    // ?GetThingAttitudeToPlayerAsString@COpinionReactionManager@@QBEABVCCharString@@ABVCThing@@@Z (public const), retail 0x007018B0, and
    // the private ?GetAttitudeOrReactionType@COpinionReactionManager@@ABE?AW4EOpinionReactionType@@ABVCThing@@_NPB_NPBVCTCOpinionSource@@@Z
    // it calls (out of line, 0x00701010; FABLE_RETAIL_CTCWIFE_MARRY declares the same member). The includer declares the enums and
    // CTCOpinionSource. Declarations only, no layout change.
    const CCharString& GetThingAttitudeToPlayerAsString(const CThing& thing) const;   // 0x007018B0
#ifndef FABLE_RETAIL_CTCWIFE_MARRY
private:
    EOpinionReactionType GetAttitudeOrReactionType(const CThing&,bool,const bool*,const CTCOpinionSource*)const;   // 0x00701010
public:
#endif
#endif
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTACHOPINIONSOURCE
    // Retail opt-in (acctC wfA1, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): Ego_r.pdb
    // ?AttachOpinionSource@COpinionReactionManager@@QBE_NAAVCThing@@ABVCCharString@@PAPAVCTCOpinionSource@@@Z, out of line in retail
    // at 0x00700780 (call 0x0068350E). Declaration only.
    bool AttachOpinionSource(CThing& thing, const CCharString& name, CTCOpinionSource** source) const;   // 0x00700780
#endif
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_PEEKSOURCE
    // Nested opt-in (chunk 2004, row 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable): Ego_r.pdb / FableWin.pdb
    // ?PeekThingOpinionSource@COpinionReactionManager@@QBEPBVCThing@@XZ, inline in retail (the caller calls GetPItem 0x00A01B50 on
    // `lea ecx,[eax+714h]` itself, 0x009609CA). No layout change.
    const CThing* PeekThingOpinionSource() const { return PThingOpinionSource.Get(); }
#endif
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_HEROSTATS
    // Opt-in (chunk 4006, rows 0x005D1F0C / 0x005CE0E6): Ego_r.pdb / FableWin.pdb
    // ?GetHeroStatsInOpinionFormat@COpinionReactionManager@@QBEMW4EOpinion@@@Z, out of line at 0x007009D0 (landed). The includer
    // declares `enum EOpinion` (no shared header carries its enumerators yet). Declaration only.
    float GetHeroStatsInOpinionFormat(EOpinion opinion) const;           // 0x007009D0
#endif
#ifdef FABLE_RETAIL_COPINIONREACTIONMANAGER_DEEDREACTIONS
    // Retail opt-in (p25O, row 0x00960120 CAIStateGroup_DeedReactionRoot::Update): Ego_r.pdb / FableWin.pdb
    // ?GetAttitudeType@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@PBVCTCOpinionSource@@@Z (inline here: the row
    // pushes source, the mask address, 1 and the thing for GetAttitudeOrReactionType 0x00701010 at 0x009601B2; body = the landed
    // out-of-line copy 0x00681500), ?CanPlayDeedReactionSpeech@COpinionReactionManager@@QBE_NW4EOpinionDeedType@@@Z (inline: the
    // DeedReactionSpeechPlaying[type] <= 0 test at 0x009604F1), and the out-of-line ?GetBlurredAttitude@..@@QBE?AW4EOpinionAttitudeType@@W42@@Z
    // (0x00700120), ?GetThingAttitudeToPlayer@ (0x00701840), ?LogDeedReactionPlayingSpeech@..@@QAEXW4EOpinionDeedType@@_N@Z (0x006FEAE0).
    // The includer declares the enums and CTCOpinionSource.
public:
#if !defined(FABLE_RETAIL_CTCWIFE_MARRY) && !defined(FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYER)
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840
#endif
#if !defined(FABLE_RETAIL_CTCWIFE_MARRY) && !defined(FABLE_RETAIL_GREETING_PASS23)
    EOpinionAttitudeType GetAttitudeType(const CThing& thing, const CTCOpinionSource* source) const {
        if (source)
            return (EOpinionAttitudeType)GetAttitudeOrReactionType(thing, true, AttitudeMask, source);
        return GetThingAttitudeToPlayer(thing);
    }
#endif
#ifndef FABLE_RETAIL_GREETING_PASS23
    EOpinionAttitudeType GetBlurredAttitude(EOpinionAttitudeType attitude) const;   // 0x00700120
#endif
    bool CanPlayDeedReactionSpeech(EOpinionDeedType deed) const { return DeedReactionSpeechPlaying[deed] <= 0; }
    void LogDeedReactionPlayingSpeech(EOpinionDeedType deed, bool playing);    // 0x006FEAE0
#if !defined(FABLE_RETAIL_CTCWIFE_MARRY) && !defined(FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYERASSTRING)
private:
    EOpinionReactionType GetAttitudeOrReactionType(const CThing&, bool, const bool*, const CTCOpinionSource*) const;   // 0x00701010
#endif
#endif
private:
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CTOR_PASS27
    friend class CTCHeroOpinionDeedLog;
#endif
    CDefPointer<const COpinionReactionManagerDef> PDef;                  // +0x000
    CWorld* World;                                                       // +0x004
#ifdef FABLE_RETAIL_OPINION_SELECTION_PASS30
    mutable
#endif
    bool ReactionFlagsTemp[79];                                          // +0x008
    std::list<CReactionMatch*> Reactions;                                // +0x058
#ifdef FABLE_RETAIL_OPINION_SELECTION_PASS30
    EOpinionAttitudeType AttitudeCondition[79];
#else
    long AttitudeCondition[79];                                          // +0x05c EOpinionAttitudeType
#endif
    long TargetingCondition[79];                                         // +0x198 EOpinionTargetingConditionType
    unsigned long PreReactionDelaySeconds[79];                           // +0x2d4
    bool BlockFurtherReactionsFlags[79];                                 // +0x410
    bool AllowSpeechOnNonPureAISpeaker[79];                              // +0x45f
    bool AllowWhileCarrying[79];                                         // +0x4ae
    bool AllowWhileFollowingPlayer[79];                                  // +0x4fd
    bool FurtherReactionsBlocked;                                        // +0x54c
    unsigned long ReactionReenableFrame;                                 // +0x550
    bool AttitudeMask[79];                                               // +0x554
    long DeedReactionSpeechPlaying[92];                                  // +0x5a4
    CIntelligentPointer<CThing> PThingOpinionSource;                     // +0x714
    std::list<CIntelligentPointer<const CThing> > OpinionSourcesList;    // +0x71c
    CIntelligentPointer<const CThingAICreature> DebugCreature;           // +0x720
};
FABLE_STATIC_ASSERT_AT(71, sizeof(COpinionReactionManager) == 0x728);
#endif
