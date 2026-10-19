from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry136'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
def blocks(a):
 b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',(o/f'{a}_coff_relocs.txt').read_text())
 return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
common={'??0CBaseObject':0x99a380,'??_7CCreatureActionBase':0x1259d24,'??0CBaseIntelligentPointer':0xa01b10,'??_7CIntelligentPointer_CThingCreatureBase':0x123e7d8,'??_7CIntelligentPointer_CThing@@':0x1238c6c,'?SetPItem':0xa01b90,'_malloc':0xbfea0e,'?SetAnimation':0x693030,'?g_NextCreatureActionID':0x13ba8b4,'??2@':0xbfea1a,'??0CAnimationEntry':0x5ded00,'?Reset':0x696830,'?GetPItem':0xa01b50,'?LowerBound':0x40f020,'?GetThingInPrimarySlot':0x6a1440,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0}
extra={
'00693b30':{},'00693dc0':{},
'00693030':{'?g_GameFramesPerSecond':0x1375550,'?GetAnimationTransitionInTime':0x5dc1f0,'__real@00000000':0x122dedc,'?GetAnimDuration':0x6627f0,'__ftol2':0xbfea70,'?GetWorld':0x4c7ab0,'?PeekAnimation':0xa242c0,'?BeginLoading':0xa26730},
'00847ac0':{'?Initialise':0x694d10,'?SetInLimbo':0x4c8cf0,'?GetCarryable':0x5cc10e,'??_7?$CIntelligentPointer@UCThing':0x1238c6c,'??1?$CIntelligentPointer@UCThing':0xa01c10,'?RemoveThingFromPrimarySlot':0x6a1e00,'?PutThingInCarrySlot':0x6a1f80},
'008d5790':{'?SetProjectileWeaponLoaded':0x5dac50,'?GetMissileLoadCarrySlot':0x5da920,'?GetThingInCarrySlot':0x6a11f0,'?RemoveThing@':0x6a19b0,'??_C@':0x127f254,'?PlayAnimation':0x7f90b0,'?GetCreatedShotDefGlobalIndex':0x5da750}}
for a,needle,count in [('00693b30','?g_NextCreatureActionID',12),('00693dc0','?g_NextCreatureActionID',13),('00693030','?g_GameFramesPerSecond',13),('00847ac0','?PutThingInCarrySlot',19),('008d5790','?SetProjectileWeaponLoaded',14)]:
 targets=dict(common,**extra[a]);selected=max([b for section,b in blocks(a)if section=='.text'and needle in b],key=len);rel=relocs(selected);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
table=list(csv.DictReader((r/'luna233_whole/handoff/ACTION_BASE_70_VTABLE_AUDIT.tsv').open(),delimiter='\t'));assert len(table)==70
for a in ['00693b30','00693dc0']:
 vb=[b for sec,b in blocks(a)if sec=='.rdata'and '?WaitForMeshToLoad' in b];assert len(vb)==1;rel=relocs(vb[0]);assert len(rel)==70;out=[]
 for row,(offset,typ,sym) in zip(table,rel):
  slot=int(row['slot']);assert int(offset,16)==slot*4;target=int(row['native'],16);assert struct.unpack('<I',rd(0x1259d24+slot*4,4))[0]==target
  decl=row['declaration'];method=re.search(r'([~\w]+)\(',decl).group(1)
  if slot==0:assert sym.startswith('??_ECCreatureActionBase')
  elif '= 0' in decl:assert sym=='__purecall'
  else:assert sym.startswith('?'+method+'@'),(slot,sym,decl)
  out.append(f'{slot}\t{target:08x}\t{sym}\t{decl}')
 (o/(a+'_emitted_vtable_audit.tsv')).write_text('slot\tnative_target\tcompiler_symbol\tdeclaration\n'+'\n'.join(out)+'\n')
 for vt,method,token in [(0x1238c6c,0x4a9710,'??_ECIntelligentPointer_CThing@@'),(0x123e7d8,0x4d6915,'??_ECIntelligentPointer_CThingCreatureBase@@')]:
  assert struct.unpack('<I',rd(vt,4))[0]==method
  assert any(sec=='.rdata'and token in b and len(relocs(b))==1 for sec,b in blocks(a))
for a in ['00847ac0','008d5790']:
 table=list(csv.DictReader((r/f'luna234_whole/probe/{a}_vtable_audit.tsv').open(),delimiter='\t'));assert len(table)==70
 # Find table by the complete first four entries, then validate all seventy retail cells.
 sig=b''.join(struct.pack('<I',int(row['native'],16))for row in table[:4]);pe=rd(0x1200000,0x100000);matches=[i for i in range(0,len(pe)-len(sig)+1,4)if pe[i:i+len(sig)]==sig];assert len(matches)==1,(a,matches)
 vt=0x1200000+matches[0]
 for row in table:assert struct.unpack('<I',rd(vt+4*int(row['slot']),4))[0]==int(row['native'],16)
 (o/(a+'_native_vtable_audit.tsv')).write_text('vtable\tslot\tnative_target\tmethod\n'+''.join(f"{vt:08x}\t{x['slot']}\t{x['native']}\t{x['method']}\n"for x in table))
assert rd(0x127f254,23)==b'PROJECTILE_WEAPON_FIRE\0'
assert rd(0x1244070,14)==b'CCarryableDef\0'
assert rd(0x122dedc,4)==struct.pack('<f',0.0)
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
print('PASS 71 body relocations, 140 emitted action-base slots, 4 emitted typed-pointer slots, 140 native derived-action slots, two literals, zero constant, and all frozen source/fixture hashes.')
