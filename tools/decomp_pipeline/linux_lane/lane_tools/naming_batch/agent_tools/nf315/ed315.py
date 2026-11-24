# ed315.py <group>: the exact NF315 edits to rows/<addr>.cpp (copied from orig/ first when missing); each old string must match once
# (4th field 2 = replace every occurrence). Groups: B (binder-only signature fixes; the owner defines the PDB form, or a stand-in under NF302 Q2).
import sys,os,shutil
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
G={}
N=' (NF315: the PDB signature, Ego_r + FableWin)'
G['B']=[
 # B1 CMessageEventFilter_IsOriginatedByThing(const CThing&) 0x006AF2B0 (owner defines the PDB form)
 ('00919a70','    CMessageEventFilter_IsOriginatedByThing(CThing* thing);     // 0x006AF2B0','    CMessageEventFilter_IsOriginatedByThing(const CThing& thing);     // 0x006AF2B0'+N,0),
 ('00919a70','CMessageEventFilter_IsOriginatedByThing((CThing*)seller),','CMessageEventFilter_IsOriginatedByThing(*(CThing*)seller),',0),
 ('00934920','    CMessageEventFilter_IsOriginatedByThing(CThing* thing);    // 0x006AF2B0','    CMessageEventFilter_IsOriginatedByThing(const CThing& thing);    // 0x006AF2B0'+N,0),
 ('00934920','CMessageEventFilter_IsOriginatedByThing(GetCreature())','CMessageEventFilter_IsOriginatedByThing(*GetCreature())',0),
 # B2 CWorldMap::GetHeightZBelowPos(const C3DVector&) const 0x0063E480 (owner defines the PDB form)
 ('008c9f40','    float GetHeightZBelowPos(const C3DVector* position);   // 0x0063E480','    float GetHeightZBelowPos(const C3DVector& position) const;   // 0x0063E480'+N,0),
 ('008c9f40','GetWorldMap()->GetHeightZBelowPos(&position)','GetWorldMap()->GetHeightZBelowPos(position)',0),
 ('008ca4a0','    float GetHeightZBelowPos(const C3DVector* position);   // 0x0063E480','    float GetHeightZBelowPos(const C3DVector& position) const;   // 0x0063E480'+N,0),
 ('008ca4a0','GetWorldMap()->GetHeightZBelowPos(&position)','GetWorldMap()->GetHeightZBelowPos(position)',0),
 # B3 CTCFollowed::RemoveEmoteIcon(CThing&) protected 0x006D5050 (owner defines the PDB form)
 ('006d56e0','    void RemoveEmoteIcon(CThingAICreature* creature);       // 0x006D5050','    protected: void RemoveEmoteIcon(CThing& thing); public:       // 0x006D5050'+N,0),
 ('006d56e0','        RemoveEmoteIcon(creature);','        RemoveEmoteIcon(*creature);',0),
 # B4 CAnimationEntry::GetComponent<CAnimComponentCombatComboChain>(const T**) const 0x00738980 (owner defines the PDB form)
 ('00738f90','    template <class T> void GetComponent(T** out) const;   // <CAnimComponentCombatComboChain>: 0x00738980','    template <class T> void GetComponent(const T** out) const;   // <CAnimComponentCombatComboChain>: 0x00738980'+N,0),
 ('00738f90','        CAnimComponentCombatComboChain* combo;\n','        const CAnimComponentCombatComboChain* combo;\n',0),
 # B5 CAIStateGroupBase::AIAction_CombatPerformActionAndWait(const CCombatActionBase&, bool) IAE, EAIActionReturn 0x0097DA40
 #    (the owner row defines a stand-in: unresolved before and after, NF302 Q2)
 ('0091e050','    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40','    protected: EAIActionReturn AIAction_CombatPerformActionAndWait(const CCombatActionBase& action, bool wait); public:   // 0x0097DA40'+N,0),
 ('0091e050','class CAIStateGroupBase { public:\n','#include "engine/EAIAction.h"\nclass CAIStateGroupBase { public:\n',0),
 ('0091e050','        if (AIAction_CombatPerformActionAndWait(action, false) != 2) {','        if (AIAction_CombatPerformActionAndWait(*action, false) != 2) {',0),
 ('009621f0','    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40','    protected: EAIActionReturn AIAction_CombatPerformActionAndWait(const CCombatActionBase& action, bool wait); public:   // 0x0097DA40'+N,0),
 ('009621f0','                &CCreatureAction_PlayCombatAnimation(','                CCreatureAction_PlayCombatAnimation(',0),
 ('009624d0','    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40','    protected: EAIActionReturn AIAction_CombatPerformActionAndWait(const CCombatActionBase& action, bool wait); public:   // 0x0097DA40'+N,0),
 ('009624d0','AIAction_CombatPerformActionAndWait(&CCreatureAction_RiseUp(*GetCreature()), true);','AIAction_CombatPerformActionAndWait(CCreatureAction_RiseUp(*GetCreature()), true);',0),
 ('009624d0','AIAction_CombatPerformActionAndWait(&CCombatAction_StabThing(GetCreature(), target), true);','AIAction_CombatPerformActionAndWait(CCombatAction_StabThing(GetCreature(), target), true);',0),
 # B6 CResourceBank::AddResource(CResource&) 0x009FC380 (the owner row defines a made-up-class stand-in: NF302 Q2)
 ('00b2c460','    void AddResource(CResource* resource);                  // 0x009FC380','    void AddResource(CResource& resource);                  // 0x009FC380'+N,0),
 ('00b2c460','        Bank().AddResource(info.Cache.Data->AsResource());','        Bank().AddResource(*info.Cache.Data->AsResource());',0),
]
INC='#define FABLE_RETAIL_TUTORIAL_CATEGORIES_PASS26   // NF315: the shared ETutorialCategory enum (Ego_d TPI 0xCE58) for the PDB parameter type\n#include "engine/ETutorialCategory.h"\n'
D='bool GiveTutorialForCategory(ETutorialCategory category, bool force);'
O='bool GiveTutorialForCategory(long category, bool force);'
C='   // 0x006C9FA0 (NF315: ?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z, Ego_r + FableWin)'
G['G']=[]
for a,lits in [('005be028',[('GiveTutorialForCategory(3, false)','TUTORIAL_CATEGORY_BASIC_OBJECTS'),('GiveTutorialForCategory(0x20, false)','TUTORIAL_CATEGORY_TRADE_ITEM')]),
               ('005ee418',[('GiveTutorialForCategory(2, false)','TUTORIAL_CATEGORY_ABILITY_CYCLING')]),
               ('006d77c0',[('GiveTutorialForCategory(0x18, false)','TUTORIAL_CATEGORY_LEVELLING_UP')]),
               ('0072e940',[('GiveTutorialForCategory(0x23, false)','TUTORIAL_CATEGORY_BUILDING_OWNERSHIP')]),
               ('0077d4b0',[('GiveTutorialForCategory(0xf, false)','TUTORIAL_CATEGORY_EXPERIENCE_SPENDING')]),
               ('00784600',[('GiveTutorialForCategory(5, false)','TUTORIAL_CATEGORY_BOASTING')]),
               ('007e00d0',[('GiveTutorialForCategory(0x1f, false)','TUTORIAL_CATEGORY_TELEPORTING')])]:
    G['G'].append((a,'class CTCHero { public:',INC+'class CTCHero { public:',0))
    for o,e in lits: G['G'].append((a,o,'GiveTutorialForCategory(%s, false)'%e,0))
G['G'].append(('005be028','    '+O+'   // 0x006C9FA0','    '+D+C,0))
G['G'].append(('005be028','CTCHero::GiveTutorialForCategory(long, bool) 0x006C9FA0.','CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) 0x006C9FA0.',0))
G['G'].append(('005be028','            long category = CTCInventoryItem::GetTutorialCategory(itemDefIndex);','            ETutorialCategory category = CTCInventoryItem::GetTutorialCategory(itemDefIndex);   // NF315: the PDB return type (NF304 Q2)',0))
G['G'].append(('005ee418','    '+O+'   // 0x006C9FA0','    '+D+C,0))
G['G'].append(('005ee418','CTCHero::GiveTutorialForCategory(long, bool) @ 0x006C9FA0 (result dropped).','CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0 (result dropped).',0))
G['G'].append(('006d77c0','class CTCHero { public: '+O+' };   // 0x006C9FA0','class CTCHero { public: '+D+' };'+C,0))
G['G'].append(('0072e940','    '+O+'   // 0x006C9FA0','    '+D+C,0))
G['G'].append(('0077d4b0','    '+O+'   // 0x006C9FA0 (ETutorialCategory)','    '+D+C,0))
G['G'].append(('00784600','    '+O+'   // 0x006C9FA0','    '+D+C,0))
G['G'].append(('007e00d0','    '+O+'   // 0x006C9FA0','    '+D+C,0))
# Q: owner unit 0x00843F50 (NF304 Q6): the owner self (CThingCreatureBase*, CThing*, ...) -> the PDB (CThingCreatureBase&, CThing*, ...)
#    with every landed binder of the old self spelling (0x007F03D0) and of the (CThingCreatureBase&, CThing&) stand-in (0x007EF600, 0x007F01F0)
QL='// Leaf: ??0CCreatureAction_ActivateQuest@@QAE@AAVCThingCreatureBase@@PAVCThing@@ABVCCharString@@_N@Z\n'
QC='   // 0x00843F50 (NF315: ??0CCreatureAction_ActivateQuest@@QAE@AAVCThingCreatureBase@@PAVCThing@@ABVCCharString@@_N@Z, Ego_r + FableWin; engine/CCreatureAction_ActivateQuest.h)'
G['Q']=[
 ('00843f50','// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843F50','QL'+'// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843F50',0),
 ('00843f50','    CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources);\n','    CCreatureAction_ActivateQuest(CThingCreatureBase& owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources);'+QC+'\n',0),
 ('00843f50','CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources)\n    : CCreatureActionBase(*owner, CSoundPair(), 0, 0)','CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase& owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources)\n    : CCreatureActionBase(owner, CSoundPair(), 0, 0)',0),
 ('00843f50','// CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase*, CThing*,','// CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase&, CThing*,',0),
 ('007f03d0','    CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources);   // 0x00843F50','    CCreatureAction_ActivateQuest(CThingCreatureBase& owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources);'+QC,0),
 ('007f03d0','user->SetCurrentAction(CCreatureAction_ActivateQuest(user, 0, name, false));','user->SetCurrentAction(CCreatureAction_ActivateQuest(*user, 0, name, false));',0),
 ('007ef600','    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing& objectToUse,\n                                  const CCharString& scriptName, bool loadResources);   // 0x00843F50','    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing* objectToUse,\n                                  const CCharString& scriptName, bool loadResources);'+QC,0),
 ('007ef600','CCreatureAction_ActivateQuest(*creature, *parent, scriptName, def->LoadResources)','CCreatureAction_ActivateQuest(*creature, parent, scriptName, def->LoadResources)',0),
 ('007f01f0','    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing& objectToUse,\n                                  const CCharString& scriptName, bool loadResources);   // 0x00843F50','    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing* objectToUse,\n                                  const CCharString& scriptName, bool loadResources);'+QC,0),
 ('007f01f0','CCreatureAction_ActivateQuest(creature, GetParentThing(), expression, false)','CCreatureAction_ActivateQuest(creature, &GetParentThing(), expression, false)',0),
]
G['Q'][0]=(G['Q'][0][0],G['Q'][0][1],QL+G['Q'][0][1],0)
# W: owner unit 0x006EBDD0 (NF304 Q6 + NF300 Q1 / NF307 D3): owner self (CThingCreatureBase*, long) -> the PDB
#    (CThingCreatureBase&, EUnsheatheWeaponType) with every landed binder of the old self spelling (0x0092B3E0, 0x006ED510) and of the
#    pointer + enum stand-in (0x006EBEE0, whose local enum had made-up enumerator names for 4 / 5)
WP='?CreatureUnsheathWeapon@CCombatManager@@QAEXAAVCThingCreatureBase@@W4EUnsheatheWeaponType@@@Z'
WC='   // 0x006EBDD0 (NF315: '+WP+', Ego_r + FableWin; engine/CCombatManager.h)'
WI='#include "engine/EUnsheatheWeaponType.h"   // NF315: the PDB parameter enum of CreatureUnsheathWeapon\n'
OB='''class CCombatManager { public:void CreatureUnsheathWeapon(CThingCreatureBase*,long);};
void CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase* creature,long weapon)
{
    bool animate;
    bool secondary=false;
    if(!ShouldPlayUnsheatheWeaponAction(creature,weapon,&animate,&secondary)) return;
    if(IsIncapacitated(creature)) return;
    if(((signed char)creature->UsingComponents[4] & 0x80) != 0) return;
    if(NCreatureMode::IsKnockedDown(*creature)) return;
    if(!(creature->UsingComponents[1]&0x10000)) return;
    CInterfaceMap* map=&creature->Components;'''
NB='''#include "engine/EUnsheatheWeaponType.h"   // NF315: the PDB parameter enum
class CCombatManager { public:void CreatureUnsheathWeapon(CThingCreatureBase&,EUnsheatheWeaponType);};'+WC+'
void CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase& creature,EUnsheatheWeaponType weapon)
{
    bool animate;
    bool secondary=false;
    if(!ShouldPlayUnsheatheWeaponAction(&creature,weapon,&animate,&secondary)) return;
    if(IsIncapacitated(&creature)) return;
    if(((signed char)creature.UsingComponents[4] & 0x80) != 0) return;
    if(NCreatureMode::IsKnockedDown(creature)) return;
    if(!(creature.UsingComponents[1]&0x10000)) return;
    CInterfaceMap* map=&creature.Components;'''
G['W']=[
 ('006ebdd0','// CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase*,long) -- ','// Leaf: '+WP+'\n// CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase&,EUnsheatheWeaponType) -- ',0),
 ('006ebdd0',OB,NB.replace("'+WC+'",WC),0),
 ('006ebdd0','    CCreatureAction_UnsheatheWeapons action(*creature,weapon);\n    if(creature->IsFreeToPerformAction(action)) {','    CCreatureAction_UnsheatheWeapons action(creature,weapon);\n    if(creature.IsFreeToPerformAction(action)) {',0),
 ('006ebdd0','        else if(!creature->SetCurrentAction(action)) {','        else if(!creature.SetCurrentAction(action)) {',0),
 ('006ebee0','enum EUnsheatheWeaponType { UNSHEATHE_INNER_RING = 4, UNSHEATHE_OUTER_RING = 5 };\n',WI,0),
 ('006ebee0','    void CreatureUnsheathWeapon(CThingCreatureBase* creature, EUnsheatheWeaponType type);   // 0x006EBDD0','    void CreatureUnsheathWeapon(CThingCreatureBase& creature, EUnsheatheWeaponType type);'+WC,0),
 ('006ebee0','CreatureUnsheathWeapon(attacker, UNSHEATHE_OUTER_RING);','CreatureUnsheathWeapon(*attacker, UNSHEATHE_RANGED_WEAPON);',0),
 ('006ebee0','CreatureUnsheathWeapon(attacker, UNSHEATHE_INNER_RING);','CreatureUnsheathWeapon(*attacker, UNSHEATHE_MELEE_WEAPON);',0),
 ('006ed510','class CCombatManager {\n',WI+'class CCombatManager {\n',0),
 ('006ed510','    void CreatureUnsheathWeapon(CThingCreatureBase* creature, long weapon);        // 0x006EBDD0','    void CreatureUnsheathWeapon(CThingCreatureBase& creature, EUnsheatheWeaponType weapon);'+WC,0),
 ('006ed510','CreatureUnsheathWeapon(&attacker, 3);','CreatureUnsheathWeapon(attacker, UNSHEATHE_DEFAULT_WEAPON);',0),
 ('0092b3e0','#include "engine/EMovementType.h"','#include "engine/EUnsheatheWeaponType.h"   // NF315: the PDB parameter enum of CreatureUnsheathWeapon\n#include "engine/EMovementType.h"',0),
 ('0092b3e0','    void CreatureUnsheathWeapon(CThingCreatureBase* creature, long mode);   // 0x006EBDD0','    void CreatureUnsheathWeapon(CThingCreatureBase& creature, EUnsheatheWeaponType mode);'+WC,0),
 ('0092b3e0','CreatureUnsheathWeapon(GetCreature(), 4);','CreatureUnsheathWeapon(*GetCreature(), UNSHEATHE_MELEE_WEAPON);',0),
 ('0092b3e0','CreatureUnsheathWeapon(GetCreature(), 5);','CreatureUnsheathWeapon(*GetCreature(), UNSHEATHE_RANGED_WEAPON);',0),
 ('0092b3e0','CreatureUnsheathWeapon(GetCreature(), 1);','CreatureUnsheathWeapon(*GetCreature(), UNSHEATHE_NO_WEAPON);',0),
]
def run(g):
    for a,o,n,first in G[g]:
        p=A+'/rows/%s.cpp'%a
        if not os.path.exists(p): shutil.copy(A+'/orig/%s.cpp'%a,p)
        t=open(p,encoding='utf-8').read(); c=t.count(o)
        if first==2 and c>=1: open(p,'w',encoding='utf-8').write(t.replace(o,n)); print('OK',a,'x%d'%c); continue
        if c!=1: print('MISS',c,a,repr(o[:70])); continue
        open(p,'w',encoding='utf-8').write(t.replace(o,n)); print('OK',a)
if __name__=='__main__': run(sys.argv[1])
