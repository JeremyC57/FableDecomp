from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry158';w=r/'astra237_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'??0CTCBase':0x686800,'??_7CTCGraphicAppearance':0x123986c,'_malloc':0xbfea0e,'?Clear@':0xa0d300,'?GetMainPlayerNumber@':0x449700,'?SetPlayer@':0xa0d3a0,'??_7CIsThingAlive':0x12c32f0,'??0CAliveScriptThing':0x4abe90,'?SetPredicate':0xf35b10,'??1CIsThingAlive':0xe241b0,'?IsComplete':0xf35b30,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'??_C@_0BB@PABDMMLB':0x125d1c8,'??_C@_0BK@JGNHKIMI':0x12dcc9c}
for a,n in [('004c00e0',6),('00446220',3),('00eed6b0',16)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=''),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
slots=[(0x1260f0c,7,0x6e7410),(0x1260f0c,40,0x894370),(0x1260f0c,115,0x8929d0),(0x12c32f0,0,0xcdf120),(0x12c32f0,1,0xcdf130),(0x12c32f0,2,0xcdf160),(0x123986c,0,0x4c5d80),(0x125be0c,0,0x687050),(0x1238c8c,20,0x4aab10),(0x129ca10,6,0x486390)]
for vt,k,v in slots:assert struct.unpack('<I',rd(vt+k*4,4))[0]==v,(hex(vt),k)
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{v:08x}\t{k}\t{t:08x}\n'for v,k,t in slots))
assert rd(0x449700,4)==bytes.fromhex('8b411cc3')
for a,s in [(0x125d1c8,'SCRIPT_NAME_HERO'),(0x12dcc9c,'TEXT_QST_055_DIARY_LOCKED')]:assert rd(a,len(s)+1)==s.encode()+b'\0'
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');assert rd(0xbfea0e,6)==bytes.fromhex('ff2558014401');assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==0x1440158]==[(b'MSVCR71.dll',b'malloc')]
# Constructor's four actual immediate malloc sizes.
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(rd(0x4c00e0,300),0x4c00e0));sizes=[]
for i,x in enumerate(ins):
 if x.mnemonic=='call'and x.op_str=='0xbfea0e':
  recent=ins[max(0,i-4):i];push=[y for y in recent if y.mnemonic=='push'];sizes.append(int(push[-1].op_str,0))
assert sizes==[32,24,24,12],sizes
(o/'allocation_sizes.json').write_text(json.dumps(sizes))
records=[]
for line in(o/'attempts.tsv').read_text().splitlines()[1:]:
 a,g,_=line.split('\t');ps=sorted((w/'snapshots').glob(a+'_??'));assert len(ps)==int(g),(a,len(ps),g)
 for p in ps:
  m=json.loads((p/'manifest.json').read_text());assert m['command']and m['compiler_flags'];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==m['sha256'];records.append(dict(snapshot=str(p),manifest=m))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS25relocs,10nativeVTslots,4allocation sizes/mallocimport,2literals,playergetter and',len(records),'immutablegrades/finalhashes')
