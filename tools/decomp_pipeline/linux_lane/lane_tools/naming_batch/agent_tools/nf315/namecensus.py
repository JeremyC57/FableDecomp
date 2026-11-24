# namecensus.py (NF315 list): for each new (Class, Member) a changed row now binds, list landed sources/tests that DEFINE `Class::Member(...)` (a
# definition line: return type before the qualified name, not a call); the owner row is expected, anything else is a conflict to read.
import os,re
R='/home/user/FableDecomp/rebuild/'
N=[('CAIStateGroupBase','AIAction_DoNothing'),('CAIStateGroup_Gaze','FindFreeGazePos'),('CBase','GetCamTools'),('CBasicString<T>','ExtendData'),
   ('CEngineCamera','ClipSphere'),('CEngineSceneGridCell','InvalidateAll'),('CFadeInFadeOutBase','IsOpen'),('CQuestManager','IsQuestActive'),
   ('CTCDoor','Open'),('CTCHeroExperience','UpdateAbilitiesForStat'),('CTCHeroExperience','OnStatLevelChange'),
   ('CTCInventoryAbilities','IsAbilityPotentiallyAvailable'),('CTCPhysicalShieldSpell','StopSound'),('CTCScriptedControl','IsActionListEmpty'),
   ('CTCTrapBase','Reset'),('CThingCreatureBase','GetAnimFromName'),('CThingCreatureBase','GetCombatTypeDef')]   # NF315 batch 1-3
rx=[(c,m,re.compile(r'^[\w:<>\*&\s]+?\b%s::%s\s*\('%(c,m),re.M)) for c,m in N]
hits={(c,m):[] for c,m,_ in rx}
for d in ('src/compiled','tests'):
    for root,_,fs in os.walk(R+d):
        for f in fs:
            p=os.path.join(root,f)
            try: t=open(p,encoding='utf-8',errors='replace').read()
            except Exception: continue
            for c,m,r in rx:
                if m in t and r.search(t): hits[(c,m)].append(os.path.relpath(p,R))
for k,v in hits.items(): print('%s::%s\t%d\t%s'%(k[0],k[1],len(v),' '.join(v[:6])))
