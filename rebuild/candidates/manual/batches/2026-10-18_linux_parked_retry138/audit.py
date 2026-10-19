from pathlib import Path
import struct,re,runpy,sys,os,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry138';w=r/'retry200c_sol'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
def blocks(a):
 b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',(o/f'{a}_coff_relocs.txt').read_text());return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
targets={'??_7CIsThingAlive':0x12c32f0,'??0ScriptThingCopy':0x4abe90,'?SetPredicate':0xf35b10,'??1CIsThingAlive':0xe241b0,'?IsActiveThreadTerminating':0xf35b30,'??_C@':0x125d1c8,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'??2@':0xbfea1a,'??0CFrameData':0x9fe380,'?Uninitialise':0x9fcba0,'??3@':0xbfe9bc,'??4CManagedTexture':0xa69b20,'?ClearHandle':0xa69ad0}
for a,needle,n in [('00ef29e0','?SetPredicate',13),('009fe550','??0CFrameData',7)]:
 rel=relocs(max([b for sec,b in blocks(a)if sec=='.text'and needle in b],key=len));assert len(rel)==n;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v;assert native==match[0],(name,hex(native))
  out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
rel=relocs(next(b for sec,b in blocks('00ef29e0')if sec=='.rdata'and '?Clone@CIsThingAlive' in b));assert len(rel)==3
for i,(name,target)in enumerate([('??RCIsThingAlive',0xcdf120),('?Clone@CIsThingAlive',0xcdf130),('??_ECIsThingAlive',0xcdf160)]):
 assert rel[i][2].startswith(name);assert struct.unpack('<I',rd(0x12c32f0+4*i,4))[0]==target
for vt,slot,target in [(0x1238c8c,20,0x4aab10),(0x1260f0c,7,0x6e7410)]:assert struct.unpack('<I',rd(vt+4*slot,4))[0]==target
assert rd(0x125d1c8,17)==b'SCRIPT_NAME_HERO\0'
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
g=[json.loads(x)for x in(w/'ATTEMPTS.jsonl').read_text().splitlines()];assert len(g)==17
for x in g:
 assert hashlib.sha256(Path(x['command'][5]).read_bytes()).hexdigest()==x['sha256'];assert x['uncertain_reserved']==4 and int(x['new_attempt'])<=6
(o/'attempt_audit.txt').write_text('All17 pregrade source hashes/full commands verified;20 UNCERTAIN reservations preserved. Ready2+2new; capped950/6CD890 six each; FISTP9FCA40 one new, five unused. Root review virtual-ABI correction spent grade2 on9FE550; EF29E0 lifetime fixture-only correction did not change production. No root source grade.\n')
print('PASS20 body relocations, three emitted predicate slots, two native script slots, literal, frozen hashes and17 preserved source grades.')
