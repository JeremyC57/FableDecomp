from prove import *
from allocmap import regs_for_bindvt, vt_stores, BASEVT
import json
OUT=S+'/anniv/wave2/'
def mc_of(cls):
  if '<' in cls: return mangle_cls(cls)
  return '@'.join(reversed(cls.split('::')))+'@@'
def tmpl_norm(cls):
  # catalog spelling: no spaces in template args
  return cls.replace(', ',',').replace(' &','&')
LAYOUT={'SF':{0:'sdd',4:'SuspendableProcess'},'AES':{0:'sdd',4:'SuspendableProcess'},
        'BIND':{0:'GetEntityScriptName',1:'CreateActiveEntityScript'},'PCTC':{0:'TransferOut',1:'TransferIn'},
        'level':{0:'sdd',1:'RegisterMain',2:'Main',3:'Init'},
        'entity':{0:'sdd',1:'Main',2:'Init',3:'GetParentScript',4:'OnPersist',5:'OnPredicateFail'}}
def strip_mod(n):
  for p in ('_global::','_unattributed::','_gapscan::'):
    if n.startswith(p): return n[len(p):]
  return n
def classify_cur(curname, want):
  """SAME / RESPELL / PLACEHOLDER / WRONG"""
  if curname is None: return 'PLACEHOLDER'
  if curname==want: return 'SAME'
  if PH.search(curname) or curname.startswith('ExternalRetry_'): return 'PLACEHOLDER'
  c=strip_mod(curname); cm=c.split('::')[-1]; wm=want.split('::')[-1]
  # class part
  cc='::'.join(c.split('::')[:-1]); wc='::'.join(want.split('::')[:-1])
  if cm==wm and (wc.endswith(cc) or cc.endswith(wc.split('::')[-1])) and cc.split('::')[-1]==wc.split('::')[-1]: return 'RESPELL'
  if cm.replace(' ','')==wm.replace(' ','') and cc.replace(' ','')==wc.replace(' ',''): return 'RESPELL'
  return 'WRONG'
def slot_labels(va,cls,kind,why,tag):
  """returns list of (addr,name,evidence,status) for slots"""
  out=[]; mc=mc_of(cls)
  for slot,meth in LAYOUT[kind].items():
    fva=dw(va+4*slot); f='%08x'%fva; n=len(refs(fva))
    if meth=='sdd':
      pok=pdb_has_sdd(mc); name=cls+"::`scalar deleting destructor'"
    else:
      pok=pdb_has(meth,mc); name=cls+'::'+meth
    curname=cur(fva); st=classify_cur(curname,name)
    extra=''
    if kind=='level' and slot==0 and False: pass
    if not pok: out.append((f,name,'',f'SKIP(pdb lacks {meth})',curname,slot,n)); continue
    if n!=1: out.append((f,name,'',f'SKIP(refs={n})',curname,slot,n)); continue
    if st=='SAME': out.append((f,name,'','SAME',curname,slot,n)); continue
    ev=f'{tag}: vtable 0x{va:08X} slot {slot} = {meth} of {cls}; {why}; PDB has {meth} of this exact class; function dword-referenced once (this vtable); was {curname} ({st.lower()})'
    out.append((f,name,ev,'RENAME' if st in ('PLACEHOLDER','RESPELL','WRONG') else st,curname,slot,n))
  return out

def reg_proof(bindvt, vt):
  """entity vt proven by parent registration: returns (entity_name_string, allocfn) or None"""
  for nm,a,an,v,fn in regs_for_bindvt(bindvt):
    vs=vt_stores(a,2)
    nb=[x for x in vs if x!=BASEVT]
    if vt in nb[:1] or v==vt: return nm,a,fn
  return None
ENT={ # vt: (class, parent binding vt)
 0x012cc48c:('NScript::CQ_EndGameFocalSitesScript::CPermanentBarrier',0x012cc260),
 0x012d29b8:('NScript::CQ_HangingTreeGoodScript::CHTG_Executioner',0x012d2704),
 0x012e70c8:('NScript::CV_BodyGuardScript::CHenchmanComment',0x012e7004),
 0x012e70e4:('NScript::CV_BodyGuardScript::CHenchmanRegionEntranceComment',0x012e7004),
 0x012e7100:('NScript::CV_BodyGuardScript::CHenchmanRegionExitComment',0x012e7004),
 0x012e7648:('NScript::CV_ChickenKickingScript::CChickenSign',0x012e73c8),
 0x012ea620:('NScript::CV_FishingCompetitionScript::CFishCompOrganiser',0x012ea57c),
 0x012cd06c:('NScript::CQ_FireHeartScript::CFH_MemorialCutsceneMarker',0x012cce90),
 0x012ccf10:('NScript::CQ_FireHeartScript::CFH_Prisoner',0x012cce90),
 0x012ccf2c:('NScript::CQ_FireHeartScript::CProphetCage',0x012cce90),
 0x012ccf48:('NScript::CQ_FireHeartScript::CFH_GuildMaster',0x012cce90),
 0x012ccfe4:('NScript::CQ_FireHeartScript::CFH_Scythe',0x012cce90),
 0x012cd000:('NScript::CQ_FireHeartScript::CFH_BriarRose',0x012cce90),
 0x012cd01c:('NScript::CQ_FireHeartScript::CFH_Villager',0x012cce90),
 0x012c32a4:('NScript::CPersonalScript_GlobalThingsScript::CClickableDoor',0x012c31f0),
}
LEVEL={0x012db86c:('NScript::CQ_PrisonEscapeRescueMotherScript','Q_PrisonEscapeRescueMother',0xddacb0,0xcd71ef),
       0x012f7300:('NScript::CQ_CinemaTestScript','Q_CinemaTest',0xf02460,0xcdb269)}
PCTC={0x012e149c:('CPersistContext_TransferableComponent<NScript::CQR_EscortTraderScript::EscortTraderQuestStage>','NScript::CQR_EscortTraderScript::OnPersist key "QuestStage" -> Transfer fn 00e1df60 builds this vtable; PDB field CQR_EscortTraderScript::QuestStage is EscortTraderQuestStage'),
      0x012e1da4:('CPersistContext_TransferableComponent<NScript::CQR_EscortTrader_ManagerScript::EscortTraderManagerPhase>','NScript::CQR_EscortTrader_ManagerScript::OnPersist key "OrchardFarmTraderEscortPhase" -> Transfer fn 00e257c0 builds this vtable; PDB field CQR_EscortTrader_ManagerScript::OrchardFarmTraderEscortPhase is EscortTraderManagerPhase'),
      0x012e9764:('CPersistContext_TransferableComponent<NScript::CV_ExposeMayorScript::STAGE_IN_PATH>','NScript::CV_ExposeMayorScript::OnPersist key "StageNumber" (field +0x4c) -> Transfer fn 00e75840 builds this vtable; PDB field CV_ExposeMayorScript::StageNumber is STAGE_IN_PATH'),
      0x012e976c:('CPersistContext_TransferableComponent<NScript::CV_ExposeMayorScript::PATH_ID>','NScript::CV_ExposeMayorScript::OnPersist key "Path" (field +0x48) -> Transfer fn 00e759f0 builds this vtable; PDB field CV_ExposeMayorScript::Path is PATH_ID')}
SFMAN={0x012ddd2c:('CEntityScriptBinding<NScript::CQ_SecretPassageScript>','BIND','TLC: slot1 CEntityScriptBinding<NScript::CQ_SecretPassageScript>::CreateActiveEntityScript; 10 registrations of NScript::CQ_SecretPassageScript::C*::Alloc (SP_AmbushGuard..DoorToMothersTortureChamber) in 00decad0 = slot 2 (Main) of level vtable 012ddc30 whose RegisterMain/Init are CQ_SecretPassageScript (00decad0 is mis-named CV_TravellingHeroesScript::Main in the manifest)'),
       0x012df40c:('CEntityScriptBinding<NScript::CQ_TraderEscortScript>','BIND','TLC: slot1 CEntityScriptBinding<NScript::CQ_TraderEscortScript>::CreateActiveEntityScript; 14 registrations of NScript::CQ_TraderEscortScript::C*::Alloc (DarkwoodTrader..EndTrader) in 00e00db0 = slot 2 (Main) of level vtable 012df16c whose RegisterMain/Init/OnPersist are CQ_TraderEscortScript (00e00db0 is mis-named CV_TravellingHeroesScript::Main in the manifest)'),
       0x012d1d30:('CSpawnedFunc<NScript::CQ_GuildTrainingWoodsMeleeScript>','SF','TLC: alloc 0x3c, sdd no member dtor; stored by NScript::CQ_GuildTrainingWoodsMeleeScript::RegisterMain ("Main" via vcall thunk) and its Main/DoMission spawning WatchForTermination, DoMission, TeleportOutHero (PDB: void(), CQ_GuildTrainingWoodsMeleeScript); 00d66e90 spawned as "WatchForLeaving" is mis-named CV_AmbushScamScript::WatchForQuestFinished'),
       0x012c9918:('CSpawnedFunc2<NScript::CQ_BanditCampBossBattleScript,float,CCharString>','SF','TLC: alloc 0x44 = PDB sizeof 68; sdd destroys CCharString at +0x40; +0x3c = 0x40000000 (2.0f); slot4 pushes CCharString copy(+0x40) and float(+0x3c) = 2 args; site is slot1 (Main) of NScript::CQ_BanditCampBossBattleScript::CGate2Guard1 vtable 012c8e80, thread "OpenGate"; PDB ?OpenGate@CQ_BanditCampBossBattleScript@NScript@@QAEXMVCCharString@@@Z'),
       0x012cc444:('CSpawnedFunc1<NScript::CQ_EndGameFocalSitesScript,CScriptThing>','SF','TLC: alloc 0x48 = PDB sizeof 72; sdd destroys CScriptThing at +0x3c; slot4 copies CScriptThing (+0x3c) and pushes it = 1 arg by value; site NScript::CQ_EndGameFocalSitesScript::CBattleMinion::Main, thread "FadeCreatureUp"; PDB ?FadeCreatureUp@CQ_EndGameFocalSitesScript@NScript@@QAEXVCScriptThing@@@Z'),
}
def catclass(r):
  m=re.search(r"catalog class '([^']*)'",r['likely_tlc_match']); return m.group(1) if m else None
def prove_row(va):
  """-> (cls, kind, why) or (None, fam, reason)"""
  if va in ENT:
    cls,bv=ENT[va]; rp=reg_proof(bv,va)
    if not rp: return None,'entity','registration not found'
    nm,a,fn=rp; exp=cls.split('::')[-1]
    if 'C'+nm!=exp: return None,'entity','registration string %s != %s'%(nm,exp)
    return cls,'entity','TLC: parent %s registers "%s" with Alloc %08x which builds this vtable (rule 5)'%(fn,nm,a)
  if va==0x012c3224:
    return 'CEntityScriptBase','base','TLC: slots 1 and 3 are _purecall (Main, GetParentScript pure); vtable stored first by >100 *::Alloc functions of every quest before the derived vtable = CEntityScriptBase base vtable'
  if va in LEVEL:
    cls,s,a,site=LEVEL[va]
    if va not in vt_stores(a,1): return None,'level','alloc does not store vt'
    return cls,'level','TLC: CScriptInfoManager::RegisterScripts registers "%s" with Alloc %08x (site %x) which builds this vtable; slot1 RegisterMain spawns "Main" in %s'%(s,a,site,cls)
  if va in PCTC:
    cls,why=PCTC[va]; return cls,'PCTC','TLC: '+why+'; slot layout 0 TransferOut / 1 TransferIn from named PCTC<long> 0122df38, <EHeroTrainingStatus> 0124f2f4, <bool> 0122d6c4'
  if va in SFMAN: return SFMAN[va]
  cls,f,ev=prove(va)
  return (cls,f,ev) if cls else (None,f,ev)

LAYOUT['base']={0:'sdd'}
def run():
  L={'medium':[], 'low':[], 'medium_shifted':[], 'low_shifted':[], 'extra':[]}
  review=[]; detail=[]
  seen=set()
  for r in rows:
    conf='medium' if r['confidence']=='medium' else 'low'
    va=rowva(r); A=tmpl_norm(r['name']); cc=catclass(r)
    cls,kind,why=prove_row(va)
    if cls: cls=tmpl_norm(cls)
    if not cls:
      review.append((va,A,conf,cc,'',f'unsettled:{why}',0,'')); continue
    def emit(bucket,tag):
      labs=slot_labels(va,cls,kind,why,tag); n=0
      for f,name,ev,st,curname,slot,nr in labs:
        detail.append((bucket,'%08x'%va,slot,f,st,curname,name))
        if st=='RENAME' and f not in seen:
          seen.add(f); L[bucket].append((f,name,ev)); n+=1
      return n,labs
    if cls==A:
      n,labs=emit(conf,f'Anniversary wave2 ({conf}, strict): Anniversary RTTI {A} matched to this vtable; TLC proof agrees')
      v='landed-ok' if n else 'catalog-right'  # all slots already carry the name
      review.append((va,A,conf,cc,cls,v,n,why)); continue
    # Anniversary name does not fit this vtable
    inann=ann_has(mc_of(cls)) if '<' in cls or '::' in cls else ann_has(mc_of(cls))
    if conf=='low' and cc and tmpl_norm(cc)==cls:
      n,_=emit('extra','TLC-only (catalog class confirmed, Anniversary row was shifted)')
      review.append((va,A,conf,cc,cls,'catalog-right',n,f'Anniversary name is the neighbouring class; {why}')); continue
    if inann:
      b=conf+'_shifted'
      n,_=emit(b,f'Anniversary wave2 ({conf}, shift-corrected): Anniversary row put {A} here, TLC proves {cls}, which Anniversary RTTI also has (neighbouring vtable)')
      reason='shifted:TLC vtable is %s'%cls
      review.append((va,A,conf,cc,cls,('rejected:%s'%reason) if conf=='medium' else 'catalog-wrong:%s'%cls,n,why)); continue
    n,_=emit('extra','TLC-only (class not in Anniversary RTTI)')
    review.append((va,A,conf,cc,cls,'rejected:TLC vtable is %s (not an Anniversary RTTI class)'%cls,n,why))
  return L,review,detail

ADDL=[(0x012e6228,'CSpawnedFunc1<NScript::CV_BookCollectingScript,long>','SF','TLC: alloc 0x40 = PDB sizeof CSpawnedFunc1<T,long>; stored by CBS_Teacher::AskForBook (CV_BookCollectingScript entity) spawning "BookReaction" = NScript::CV_BookCollectingScript::BookReaction, PDB ?BookReaction@CV_BookCollectingScript@NScript@@QAEXJ@Z (one long arg); slot4 pushes [esi+0x3c] = 1 arg'),
      (0x012ddc30,'NScript::CQ_SecretPassageScript','level','TLC: slots 1/3 are NScript::CQ_SecretPassageScript::RegisterMain/Init; vtable built by NScript::CQ_SecretPassageScript::CQ_SecretPassageScript; slot 2 registers the CEntityScriptBinding<NScript::CQ_SecretPassageScript> entity classes'),
      (0x012df16c,'NScript::CQ_TraderEscortScript','level','TLC: slots 1/3/4 are NScript::CQ_TraderEscortScript::RegisterMain/Init/OnPersist; vtable built by NScript::CQ_TraderEscortScript::Alloc; slot 2 registers the CEntityScriptBinding<NScript::CQ_TraderEscortScript> entity classes')]
ADDL.append((0x012ca58c,'NScript::CQ_BountyHuntScript::CBH_GangDeputy2','entity','TLC: parent CQ_BountyHuntScript Main (00d13580) registers "BH_GangDeputy2" with Alloc 00d161a0 which builds this vtable (rule 5); slot 2 spawns "WatchForDeputy2Killed" (PDB WatchForDeputy2Killed(CScriptThing) of CQ_BountyHuntScript)'))
MANUAL=[('00d66e90','NScript::CQ_GuildTrainingWoodsMeleeScript::WatchForLeaving','TLC-only: NScript::CQ_GuildTrainingWoodsMeleeScript::DoMission (00d66ca0) spawns thread "WatchForLeaving" as CSpawnedFunc<NScript::CQ_GuildTrainingWoodsMeleeScript> (vtable 012d1d30, +0x34 = this function, +0x38 = this); PDB ?WatchForLeaving@CQ_GuildTrainingWoodsMeleeScript@NScript@@QAEXXZ; was CV_AmbushScamScript::WatchForQuestFinished (wrong class)'),
        ('00e482f0','NScript::CV_BanditTollScript::CheckForLeaderDead','TLC-only: NScript::CV_BanditTollScript::Main (00e479b0) spawns thread "CheckForLeaderDead" as CSpawnedFunc<NScript::CV_BanditTollScript> (vtable 012e4b90, +0x34 = this function, +0x38 = this); PDB ?CheckForLeaderDead@CV_BanditTollScript@NScript@@QAEXXZ; was _global::CScriptGameResourceObjectScriptedThingBase_HandleBanditQuestEncounter (autoname)')]
def run_addl(L,detail,seen):
  for a,n,e in MANUAL:
    if a not in seen: seen.add(a); L['extra'].append((a,n,e)); detail.append(('tlc_extra','-','-',a,'RENAME',cur(int(a,16)),n))
  for va,cls,kind,why in ADDL:
    for f,name,ev,st,curname,slot,nr in slot_labels(va,cls,kind,why,'TLC-only (found while verifying; not an Anniversary row)'):
      detail.append(('tlc_extra','%08x'%va,slot,f,st,curname,name))
      if st=='RENAME' and f not in seen: seen.add(f); L['extra'].append((f,name,ev))

if __name__=='__main__':
  L,review,detail=run()
  run_addl(L,detail,set(a for v in L.values() for a,_,_ in v))
  L['tlc_extra']=L.pop('extra'); L.pop('low_shifted')
  for k,v in L.items():
    open(OUT+'labels_%s.tsv'%k,'w').write('address\tname\tevidence\n'+''.join('%s\t%s\t%s\n'%x for x in v))
    print(k,len(v))
  with open(OUT+'review.tsv','w') as fh:
    fh.write('tlc_vtable\tanniversary_name\tconfidence\tcatalog_class\ttlc_proven_class\tverdict\tlabels\tevidence\n')
    for va,A,conf,cc,cls,v,n,why in review: fh.write('%08x\t%s\t%s\t%s\t%s\t%s\t%d\t%s\n'%(va,A,conf,cc,cls,v,n,why))
  with open(OUT+'slot_detail.tsv','w') as fh:
    fh.write('bucket\tvtable\tslot\taddress\tstatus\tcurrent\tproposed\n')
    for x in detail: fh.write('\t'.join(map(str,x))+'\n')
  from collections import Counter
  print(Counter((c,v.split(':')[0]) for va,A,c,cc,cls,v,n,why in review))
