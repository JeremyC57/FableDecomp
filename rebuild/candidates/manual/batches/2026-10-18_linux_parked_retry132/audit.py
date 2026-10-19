from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry132'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
move={'?GetPItem':0xa01b50,'?GetAnimMovementVector':0x6626e0,'__real@3f800000':0x122ded8,'?GetWorld':0x4c7ab0,'?PeekAnimation':0xa242c0,'?GetEntrySymbol':0x9cc1d0,'??_C@':0x1239e98,'??0CRetailString@@QAE@PBDJ':0x99ebf0,'??0CRetailString@@QAE@ABU':0x99ec30,'??1CCharString':0x99eae0}
events={'?GetPItem':0xa01b50,'?QuestManager':0x13b89fc,'?LowerBound':0x4365b0,'?GetFlourishEvent':0x8d4d90,'?GetSuccessfulFlourishEvent':0x8d4e30}
summon={'?GetPItem':0xa01b50,'?LowerBound':0x4365b0,'??_C@':0x125d2a4,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?GetDefinitionManager':0x4c7990,'?GetDefGlobalIndexFromName':0x9ad410}
def blocks(a):
 t=(o/f'{a}_coff_relocs.txt').read_text();b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',t)
 return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
for a,needle,targets,count in [('00692c80','?PeekAnimation',move,13),('008d16d0','?GetFlourishEvent',events,6),('008d17b0','?GetSuccessfulFlourishEvent',events,6),('008d08a0','?GetDefinitionManager',summon,11)]:
 selected=max([b for section,b in blocks(a)if section=='.text'and needle in b],key=len);rel=relocs(selected);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
expected=[0x62ed20,0x99a400,0x692db0,0xbfea08,0x694430,0x692850,0x692460,0x692870]
assert list(struct.unpack('<8I',rd(0x1259d24,32)))==expected
source=(r/'luna233_whole/src/00692c80.cpp').read_text();body=source[source.index('struct CCreatureActionBase {'):source.index(' void* IntelligentPointer;')];decl=re.findall(r'virtual\s+([^;]+);',body)
labels=['~CCreatureActionBase','Validate','EditUpdate','PerformAction','FinishAction','IsPlayingAnimation','GetAnimDelayTime','GetAnimationIndex']
assert len(decl)==8
for d,n in zip(decl,labels):assert n+'(' in d
(o/'virtual_audit.tsv').write_text('vtable\tslot\tnative_target\tmethod\n'+''.join(f'01259d24\t{i}\t{a:08x}\t{n}\n'for i,(a,n)in enumerate(zip(expected,labels))))
for va,t in [(0x1239e98,b'UNKNOWN\0'),(0x125d2a4,b'GHOSTLY_SWORD\0'),(0x122ded8,struct.pack('<f',1.0))]:assert rd(va,len(t))==t
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
rows=[json.loads(x)for x in(r/'retry199h_sol/ATTEMPTS.jsonl').read_text().splitlines()];assert len(rows)==19
counts={}
for x in rows:
 cmd=x['command'];p=Path(cmd[5]);assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert cmd[4]==x['address'];assert cmd[:3]==['timeout','200','python3'];assert x['uncertain_reserved']==4;counts[x['address']]=counts.get(x['address'],0)+1
assert counts=={'008d16d0':4,'008d17b0':4,'008d08a0':5,'008d0730':6},counts
(o/'attempt_audit.txt').write_text('19 immutable Sol pregrade snapshots/hashes/full commands verified, counts16D0=4,17B0=4,08A0=5,0730=6 plus4UNCERTAINeach. Original reservation does not reset. 0730capped6; accepted rows stop atmatch withunusedallowances2/2/1. Astra692C80g7, source+fixture frozen. No root sourcegrades.\n')
print('PASS36 body relocations,8 actual Action virtual declarations,2 strings,float1,frozen hashes,19Sol attempts')
