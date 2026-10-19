from pathlib import Path
import struct,re,runpy,sys,csv,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry144';w=r/'luna234_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
targets={'?GetPItem':0xa01b50,'?LowerBound':0x40f020,'?AllowReactionsOnHitFromAttacker':0x79e580,'??_C@_0BC@':0x123d1d8,'??0CCharString':0x99ebf0,'?AddTC':0x4c9d60,'??1CCharString':0x99eae0,'?GetScorpionBattle':0x8c5a50,'?Initialise':0x857520,'?GetTrollBattle':0x8c78f0,'?GetPosition':0x4c73d0,'?CreateExplosion':0x80c460,'?AddImmuneThing':0x80cbf0,'?FinishAction':0x859a20,'??0CCombatActionBase':0x858030,'??_7CCreatureAction_RockTrollShakeAndExplode':0x128154c,'?SetCurrentAction':0x6644f0,'??1CCombatActionBase':0x62e900,'??_C@_0BD@':0x1282cd0,'?SetAnimationName':0x8574d0,'?CreatePhysicalObstruction':0x7fd4b0,'??0CBaseIntelligentPointer':0xa01b10,'??_7?$CIntelligentPointer@UCThing':0x1238c6c,'?SetPItem':0xa01b90,'??1?$CIntelligentPointer@UCThing':0xa01c10,'?PushBack':0x4ad520}
for a,n in [('008c4680',15),('008c74f0',14),('008cbb90',23)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+'_coff_relocs.txt')).write_text(s);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[(b[i],b[i+1])for i in range(1,len(b),2)];t=max([x for sec,x in bs if sec=='.text'],key=len);rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',t,re.M);assert len(rel)==n,(a,len(rel));out=[]
 for off,typ,name in rel:
  off=int(off,16);expect=[v for k,v in targets.items()if name.startswith(k)];assert len(expect)==1,(a,name);v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v;assert expect==[native],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
 tables=list(csv.DictReader((w/'probe'/(a+'_vtable_audit.tsv')).open(),delimiter='\t'));expected={'008c4680':83,'008c74f0':162,'008cbb90':81};assert len(tables)==expected[a]
 for row in tables:
  vt=({'RockTrollFallToDeath':0x1262b04,'RockTrollShakeAndExplode':0x128154c}[row['class']]if a=='008c74f0'else {'008c4680':0x1281024,'008cbb90':0x1264214}[a]);assert struct.unpack('<I',rd(vt+int(row['slot'])*4,4))[0]==int(row['native'],16),(a,row)
 (o/(a+'_native_vtable_audit.tsv')).write_text((w/'probe'/(a+'_vtable_audit.tsv')).read_text())
 if a=='008c74f0':
  vr=[re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',x,re.M)for sec,x in bs if sec=='.rdata'and '?Clone@CCreatureAction_RockTrollShakeAndExplode' in x];assert len(vr)==1 and len(vr[0])==81
  for row,(off,typ,sym)in zip([x for x in tables if x['class']=='RockTrollShakeAndExplode'],vr[0]):
   assert int(off,16)==4*int(row['slot']);method=re.search(r'(\w+)\(',row['method']).group(1)
   if int(row['slot']):assert sym.startswith('?'+method+'@'),(row,sym)
for va,literal in [(0x123d1d8,b'CTCHitOnCollision'),(0x1282cd0,b'STRIKE_FLAME_SLICE')]:assert rd(va,len(literal)+1)==literal+b'\0'
# GetPosition directly returns the physics position and has no callback or side effect.
assert rd(0x4c73d0,10)==bytes.fromhex('8b416083c00cc3cccccc')
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS52 body relocations,326 native virtual slots,81 emitted ShakeAndExplode slots,two string literals,read-only GetPosition and frozen source/fixture hashes.')
