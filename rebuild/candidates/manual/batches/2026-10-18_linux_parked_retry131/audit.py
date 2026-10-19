from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry131'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
guard={'??_7CIsThingAlive':0x12c32f0,'??0ScriptThingCopy':0x4abe90,'?SetPredicate':0xf35b10,'??1ScriptThingCopy':0x4aa840,'?IsActiveThreadTerminating':0xf35b30,'_rand':0xbfeb16,'??0CBaseObject':0x99a380,'??_C@':0x125eab4,'??_7CScriptGameResourceObjectScriptedThingBase':0x127094c,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?IsDistanceBetweenThingsUnder':0xcbe2ff,'?FadeOutAndKillEntity':0xcbe716,'??1CScriptGameResourceObjectScriptedThingBase':0x7e74d0}
rush={'?Initialise':0x857520,'?GetPItem':0xa01b50,'?RemoveTC':0x4c9840,'??_C@':0x123c4d0,'??0CCharString':0x99ebf0,'?AddTC':0x4c9d60,'??1CCharString':0x99eae0,'?SetPTarget':0x7a9ab0,'?SetFinalAction':0x8ceae0}
def blocks(a):
 t=(o/f'{a}_coff_relocs.txt').read_text();b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',t)
 return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
for a,needle,targets,count in [('00d2c530','?SetPredicate',guard,18),('008cd470','?SetFinalAction',rush,13)]:
 selected=max([b for section,b in blocks(a)if section=='.text'and needle in b],key=len);rel=relocs(selected);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
names=['??_ECScriptGameResourceObjectScriptedThingBase','?Validate@','?SummonerLightningOrbAttackTarget@','?FireProjectileWeaponAtTarget@','?MoveToPosition@','?MoveToThing@','?FollowPreCalculatedRoute@','?FollowThing@','?StopFollowingThing@','?IsFollowActionRunning@','?ClearCommands@','?WaitWhilePerformingTasks@','?GetScriptThing@','?Speak@','?Speak@','?AskHeroQuestion@','?Converse@','?PerformExpression@','?PlayAnimation@','?PlayCombatAnimation@','?PlayLoopingAnimation@','?ClearAllActions@','?ClearAllActionsIncludingLoopingAnimations@','?MoveToAndPickUpGenericBox@','?DropGenericBox@','?UnsheatheWeapons@','?IsPerformingScriptTask@','?IsFollowingThing@','?WaitForEntityToFinishPerformingTasks@','?Wait@']
addresses=[0x7e7520,0x7e72c0,0x7e72d0,0x7e72e0,0x7e72f0,0x7e7300,0x7e7310,0x7e7320,0x7e7330,0x7e7340,0x7e7360,0x7e7370,0x7e7490,0x7e7390,0x7e7380,0x7e73a0,0x7e73b0,0x7e73c0,0x7e73d0,0x7e73e0,0x7e73f0,0x7e7400,0x7e7410,0x7e7420,0x7e7430,0x7e7440,0x7e7450,0x7e7460,0x7e7470,0x7e7480]
out=['vtable\tslot\tnative_target\tcompiler_symbol']
for va,needle,expected,labels in [(0x127094c,'?Validate@',addresses,names),(0x12c32f0,'?Clone@CIsThingAlive',[0xcdf120,0xcdf130,0xcdf160],['??RCIsThingAlive','?Clone@CIsThingAlive','??_ECIsThingAlive'])]:
 b=next(b for section,b in blocks('00d2c530')if section=='.rdata'and needle in b);rel=relocs(b);assert len(rel)==len(expected)
 assert list(struct.unpack('<'+'I'*len(expected),rd(va,4*len(expected))))==expected
 for i,((off,typ,sym),address,label)in enumerate(zip(rel,expected,labels)):
  assert int(off,16)==4*i and sym.startswith(label)
  if va==0x127094c and i==13:assert '@@PBDW4' in sym
  if va==0x127094c and i==14:assert '@@KW4' in sym
  out.append(f'{va:08x}\t{i}\t{address:08x}\t{sym}')
for va,slot,target,label in [(0x1260f0c,7,0x6e7410,'NewScriptFrame'),(0x1260f0c,70,0x891ca0,'GetHero'),(0x1260f0c,601,0x89c190,'EntitySetInFaction'),(0x1265f84,74,0x7238e0,'SetFacingAnglesFromRHSet')]:
 assert struct.unpack('<I',rd(va+4*slot,4))[0]==target
 out.append(f'{va:08x}\t{slot}\t{target:08x}\t{label}')
(o/'virtual_audit.tsv').write_text('\n'.join(out)+'\n')
for va,s in [(0x125eab4,b'FACTION_VILLAGERS\0'),(0x123c4d0,b'CTCAssassinRush\0')]:assert rd(va,len(s))==s
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
rows=list(csv.DictReader((r/'luna232_whole/STATUS.tsv').open(),delimiter='\t'))
assert len(rows)==40 and sum(int(x['authored_grades'])for x in rows)==289
assert sum(x['status']=='DIFFER'for x in rows)==23
assert all(int(x['authored_grades'])==10 for x in rows if x['status']=='DIFFER')
(o/'chunk232_review.txt').write_text('Whole40 inventory reviewed:17 accepted after this landing,23 DIFFER at10first-phase grades;289 total author grades preserved. No untouched/partial rows. FinalD2C530 at4 incl corrected overload slots and actual base. Older initial attempts without immutable snapshots remain recorded as historical evidence limitations, not invented. No root source grades.\n')
print('PASS:31 body relocations,33 emitted vtable slots,4 called virtual slots,2 literals,frozen hashes,whole40 inventory/counts')
