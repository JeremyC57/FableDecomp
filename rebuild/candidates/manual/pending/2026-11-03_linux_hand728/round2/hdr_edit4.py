import re
H='hdr/engine/'
def wrap(name, guard, macro, body):
    p=H+name+'.h'; s=open(p).read()
    d='#define '+guard+'\n'
    assert s.count(d)==1
    s=s.replace(d, d+'#ifdef '+macro+'\n'+body+'#else // generated default (unchanged)\n',1)
    e='#endif // '+guard
    i=s.rindex(e)
    s=s[:i]+'#endif // '+macro+'\n'+s[i:]
    open(p,'w').write(s)

wrap('CHeroMarriageInfo','FABLE_ENGINE_CHEROMARRIAGEINFO_H','FABLE_RETAIL_CHEROMARRIAGEINFO','''// Retail opt-in (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): Ego_r.pdb CHeroMarriageInfo (0x18, no containers, so the
// retail offsets are Ego_r's), typed; the default constructor zeroes the five dwords and the bool (FrameUpdate 0x006D268E builds a
// local this way before ?PeekMarriageInfo@CTCHeroStats@@QBE_N_KAAVCHeroMarriageInfo@@@Z). Constructor body inferred from those
// stores (no out-of-line copy in either PDB name set).
#include <stddef.h>
#include "rebuild_abi.h"
enum EMarriageState;                                                     // MS forward enum (rows name the values they use)
class CHeroMarriageInfo {
public:
    CHeroMarriageInfo() : WifeUID(0), MaritalHomeUID(0), State((EMarriageState)0), GayMarriage(false) {}
    unsigned __int64 WifeUID;                                            // +0x00
    unsigned __int64 MaritalHomeUID;                                     // +0x08
    EMarriageState   State;                                              // +0x10
    bool             GayMarriage;                                        // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CHeroMarriageInfo) == 0x18);
''')

wrap('CTCHeroStats','FABLE_ENGINE_CTCHEROSTATS_H','FABLE_RETAIL_CTCHEROSTATS','''// Retail opt-in (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): the polymorphic retail class over FABLE_RETAIL_CTCBASE.
// Members are the generated default's (retail STLport offsets, sizeof 0x1DC); only NumLoversAcquired (+0x140, FrameUpdate
// 0x006D2902 `inc [eax+140h]`) is typed out. Ego_r.pdb / FableWin.pdb spellings: ?StatGetInterfaceType@CTCHeroStats@@SI?AW4ETCInterfaceType@@XZ
// (inline; GetTC key 4), ?PeekMarriageInfo@CTCHeroStats@@QBE_N_KAAVCHeroMarriageInfo@@@Z (0x0057AC89) and the private
// ?SetMarriageState@CTCHeroStats@@AAEXPBVCThingAICreature@@W4EMarriageState@@@Z (0x0057BC6E), which CTCWife::FrameUpdate calls
// (so CTCWife is a friend).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CHeroMarriageInfo;
class CThingAICreature;
class CTCWife;
enum EMarriageState;
class CTCHeroStats : public CTCBase {
    friend class CTCWife;
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_STATS; }
    bool PeekMarriageInfo(unsigned __int64 wifeUID, CHeroMarriageInfo& info) const;   // 0x0057AC89
private:
    void SetMarriageState(const CThingAICreature* wife, EMarriageState state);      // 0x0057BC6E
public:
    unsigned char    _pad_0x0c[0x140 - 0x0c];                // +0x0c SunTan .. TotalSpentExperience (generated default, opaque)
    long             NumLoversAcquired;                      // +0x140
    unsigned char    _pad_0x144[0x1dc - 0x144];              // +0x144 NumWeddings .. (opaque)
};
FABLE_STATIC_ASSERT(sizeof(CTCHeroStats) == 0x1dc);
''')

wrap('CTCPerceiveThing','FABLE_ENGINE_CTCPERCEIVETHING_H','FABLE_RETAIL_CTCPERCEIVETHING','''// Retail opt-in (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): the polymorphic retail class over FABLE_RETAIL_CTCBASE,
// sizeof 0x58 as the generated default; members after the base opaque. Ego_r.pdb / FableWin.pdb spellings: StatGetInterfaceType
// (inline; GetTC key 0x34), ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (0x007446A0), ?IsAwareOfThing@CTCPerceiveThing@@QBE_NXZ
// (0x00643B3B), ?GetExtendedSightRadius@CTCPerceiveThing@@QBEMXZ (0x00744950). 0x00661600 (`LineOfSight || Smelt`, +0x2D / +0x31)
// keeps the landed spelling CanSeeOrSmellPerceivedThing: neither PDB names a const bool query with that body (naming queue; the
// Ego_r candidate is ?IsThingPerceived@CTCPerceiveThing@@QBE_NXZ 0x0053C6B0, unproven).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCPerceiveThing : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PERCEIVE_THING; }
    const CThing* GetPerceivedThing() const;                 // 0x007446A0
    bool IsAwareOfThing() const;                             // 0x00643B3B
    bool CanSeeOrSmellPerceivedThing() const;                // 0x00661600 (landed spelling, see above)
    float GetExtendedSightRadius() const;                    // 0x00744950
    unsigned char    _pad_0x0c[0x58 - 0x0c];                 // +0x0c PerceivedThing .. FrameHitByPerceivedThing (opaque)
};
FABLE_STATIC_ASSERT(sizeof(CTCPerceiveThing) == 0x58);
''')

# new headers
open(H+'CTCVolumeContainmentTracker.h','w').write('''#ifndef FABLE_ENGINE_CTCVOLUMECONTAINMENTTRACKER_H
#define FABLE_ENGINE_CTCVOLUMECONTAINMENTTRACKER_H
// CTCVolumeContainmentTracker (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): the retail class over FABLE_RETAIL_CTCBASE.
// Ego_r.pdb: CIntelligentPointer<CThing> PThingInside at +0x0C (no container before it, so the retail offset is Ego_r's); FableWin.pdb
// ?StatGetInterfaceType@CTCVolumeContainmentTracker@@SI?AW4ETCInterfaceType@@XZ (GetTC key 0x75) and ?GetPThingInside@CTCVolumeContainmentTracker@@QBEPAVCThing@@XZ,
// both inline in retail (FrameUpdate: `lea ecx,[eax+0Ch] ; call GetPItem`). The members after PThingInside are not modelled here.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CTCVolumeContainmentTracker : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_VOLUME_CONTAINMENT_TRACKER; }
    CThing* GetPThingInside() const { return PThingInside.Get(); }
    CIntelligentPointer<CThing> PThingInside;                // +0x0c
};
#endif
''')
open(H+'COpinionReactionManager.h','w').write('''#ifndef FABLE_ENGINE_COPINIONREACTIONMANAGER_H
#define FABLE_ENGINE_COPINIONREACTIONMANAGER_H
// COpinionReactionManager (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): declarations only (layout not modelled).
// Ego_r.pdb / FableWin.pdb ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z (0x00701840)
// and ?GetReactionMatchStrength@COpinionReactionManager@@QBEMW4EOpinionReactionType@@ABVCThingAICreature@@@Z (0x00701590).
class CThing;
class CThingAICreature;
enum EOpinionAttitudeType;
enum EOpinionReactionType;
class COpinionReactionManager {
public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;                              // 0x00701840
    float GetReactionMatchStrength(EOpinionReactionType reaction, const CThingAICreature& creature) const; // 0x00701590
};
#endif
''')
open(H+'CTCHeroOnlineScoreboard.h','w').write('''#ifndef FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
#define FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
// CTCHeroOnlineScoreboard (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): the retail class over FABLE_RETAIL_CTCBASE,
// declarations only (members after the base not modelled). FableWin.pdb ?StatGetInterfaceType@CTCHeroOnlineScoreboard@@SI?AW4ETCInterfaceType@@XZ
// (inline; GetTC key 0x10D), Ego_r.pdb / FableWin.pdb ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z
// (0x0055F3B0; the activity enum is nested).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCHeroOnlineScoreboard : public CTCBase {
public:
    enum EOnlineScoreboardActivity;                          // MS forward enum (rows name the values they use)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_ONLINE_SCOREBOARD; }
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float value, bool);   // 0x0055F3B0
};
#endif
''')
open(H+'COpinionSet.h','w').write('''#ifndef FABLE_ENGINE_COPINIONSET_H
#define FABLE_ENGINE_COPINIONSET_H
// COpinionSet (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): Ego_r.pdb CArray<float> OpinionValues at +0 (retail CArray 0xC);
// FrameUpdate reads OpinionValues[3] / [4] through the begin pointer (`mov ebx,[ebx+48h] ; mov eax,[ebx+0Ch]`).
#include "engine/CArray.h"
class COpinionSet {
public:
    CArray<float> OpinionValues;                             // +0x00
};
#endif
''')

p=H+'CTCCreatureOpinionOfHero.h'; s=open(p).read()
old='    unsigned long        RelativeOpinion_[3];    // +0x48 COpinionSet RelativeOpinion (CArray<float> OpinionValues)\n'
assert old in s
s=s.replace(old,'    COpinionSet          RelativeOpinion;        // +0x48 (chunk 728 round 2: typed; CArray<float> OpinionValues)\n')
old='    bool IsWillingToContinueFollowingHero() const;                       // 0x006818F0\n'
assert old in s
s=s.replace(old,old+'''    // Chunk 728 round 2 (row 0x006D1E30 CTCWife::FrameUpdate): FableWin.pdb ?StatGetInterfaceType@CTCCreatureOpinionOfHero@@SI?AW4ETCInterfaceType@@XZ
    // (inline; GetTC key 0x4F) and Ego_r.pdb / FableWin.pdb ?SetPersonalityOverride@CTCCreatureOpinionOfHero@@QAEXJ_N@Z (0x00681E60).
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATURE_OPINION_OF_HERO; }
    void SetPersonalityOverride(long personalityDef, bool);              // 0x00681E60
''')
old='#include "engine/CDefPointer.h"\nclass COpinionOfHeroDef;'
assert old in s
s=s.replace(old,'#include "engine/CDefPointer.h"\n#include "engine/ETCInterfaceType.h"\n#include "engine/COpinionSet.h"\nclass COpinionOfHeroDef;')
open(p,'w').write(s)

p=H+'CTCSoundPlayer.h'; s=open(p).read()
old='    void StopAttachedSound(unsigned long sampleID);                      // 0x0066ADD0\n'
assert s.count(old)==1
s=s.replace(old,old+'''    // Chunk 728 round 2 (row 0x006D1E30): Ego_r.pdb / FableWin.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z
    // (0x0066BD00) and ?SetSpecificSampleVolume@CTCSoundPlayer@@QAEXKM@Z (0x0066AFF0). Declarations only.
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool, bool, bool, long);   // 0x0066BD00
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);  // 0x0066AFF0
''')
open(p,'w').write(s)

p=H+'CThingAICreature.h'; s=open(p).read()
old='    CThingBuilding* GetHomeBuilding() const { return HomeBuilding.Get(); }\n'
assert s.count(old)==1
s=s.replace(old,old+'''    // Chunk 728 round 2 (row 0x006D1E30 CTCWife::FrameUpdate): Ego_r.pdb / FableWin.pdb ?SetNewBrain@CThingAICreature@@QAEXV?$CDefPointer@$$CBVCBrainDef@@@@@Z
    // (0x00833010; the def pointer by value), ?IsBrainUpdateZoneActiveThisFrame@CThingAICreature@@QBE_NXZ (Ego_r 0x00536F40; retail
    // 0x00830130 forwards [this+1A8h] to CAIBrain::IsUpdateZoneActiveThisFrame 0x0088B810) and FableWin's inline
    // ?IsCourtable@CThingAICreature@@QBE_NXZ (retail tests bit 3 of +0x1C9, CanBeCourted, in place).
    void SetNewBrain(CDefPointer<const CBrainDef> brain);    // 0x00833010
    bool IsBrainUpdateZoneActiveThisFrame() const;           // 0x00830130
    bool IsCourtable() const { return CanBeCourted; }
''')
old='class C3DVector; class CThingAICreatureInit;\n'
assert s.count(old)==1
s=s.replace(old,'class C3DVector; class CThingAICreatureInit; class CBrainDef;\n#include "engine/CDefPointer.h"\n')
open(p,'w').write(s)

p=H+'CDefinitionManager.h'; s=open(p).read()
old='    template<class T> bool GetDef(long,CDefPointer<const T>*) const;\n'
assert s.count(old)==1
s=s.replace(old,old+'    template<class T> bool GetDef(const CCharString&,CDefPointer<const T>*) const;   // chunk 728 round 2: ??$GetDef@VCBrainDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCBrainDef@@@@@Z 0x006D3E80\n')
open(p,'w').write(s)

p=H+'CWorld.h'; s=open(p).read()
old='    CGameScriptInterface& GetGameScriptInterface() const { return *PGameScriptInterface_ptr; }\n'
assert s.count(old)==1
s=s.replace(old,old+'''    // FableWin.pdb ?GetOpinionReactionManager@CWorld@@QAEAAVCOpinionReactionManager@@XZ / ?PeekOpinionReactionManager@CWorld@@QBEABVCOpinionReactionManager@@XZ,
    // inline in retail (FrameUpdate 0x006D1EDA / 0x006D278C read the CCountedPointer data +0x74).
    COpinionReactionManager& GetOpinionReactionManager() { return *POpinionReactionManager_Data; }
    const COpinionReactionManager& PeekOpinionReactionManager() const { return *POpinionReactionManager_Data; }
''')
open(p,'w').write(s)
print("ok")
