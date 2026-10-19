from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry166';w=r/'astra240_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?NewChunk@':0xa663d0,'?UnlinkChunk@':0xa65e70,'?AppendChunk@':0xa65f00,'?PrependChunk@':0xa65ec0,'?DeleteEmptyChunk@':0xa66160}
for a in ['00a66450','00a662a0']:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==3;out=[]
 for off,typ,sym in rel:
  assert typ=='DISP32';k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4;assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v];out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
# Genuine chunk helpers preserve ECX and operate links8/C and allocatorheadsC1C/C20.
aux={}
for a,n in [(0xa65e70,0x45),(0xa65ec0,0x3a),(0xa65f00,0x3a),(0xa66160,0x69)]:
 b=rd(a,n);assert b.endswith(bytes.fromhex('c20400'));assert bytes.fromhex('1c0c0000')in b and bytes.fromhex('200c0000')in b;aux[f'{a:08x}']=b.hex()
(o/'native_auxiliary_evidence.txt').write_text(json.dumps(aux,indent=2)+'\n')
pdb=Path('ghidra_out/egor_pdb_names.tsv').read_text();assert '00c08370\t?NewChunk@CMemoryAllocatorFixedSize@@IAEPAVCMemoryAreaChunk@1@XZ'in pdb;assert '00c08420\t?Allocate@CMemoryAllocatorFixedSize@@UAEPAXK@Z'in pdb;assert '00c081c0\t?Deallocate@CMemoryAllocatorFixedSize@@UAEXPAX@Z'in pdb
records=[json.loads(l)for l in (w/'ATTEMPTS.jsonl').read_text().splitlines()if json.loads(l)['address']in ['00a66450','00a662a0']];assert len(records)==7
for x in records:
 p=Path(x['command'][5]);assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['effective_flags']and x['prior']==0
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS6relocations/4nativehelperbodies/realNewChunkreturn+targets/7immutablegrades+frozenhashes')
