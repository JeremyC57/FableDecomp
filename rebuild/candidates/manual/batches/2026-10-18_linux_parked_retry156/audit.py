from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry156';w=r/'astra235_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'?FindChild':0x535fe4,'_memmove':0xbfeae6,'?RemoveBaseChild':0x533bc0,'?FindIndex':0x535f5a,'?InsertOverflow':0x485dbc}
for a,n in [('00541120',3),('00541230',2),('00541390',2),('00705040',0)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=''),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
slots=[(0x1246e44,64,0x541120),(0x1246e44,148,0x541230),(0x1246e44,142,0x5416b0),(0x1246e44,108,0x530ec0)]
for vt,k,v in slots:assert struct.unpack('<I',rd(vt+k*4,4))[0]==v
assert rd(0x530f1b,7)==bytes.fromhex('8bc65e59c20400')
assert rd(0x42d930,11)==bytes.fromhex('85c974068b016a01ff10c3')
assert rd(0x70514b,5)==bytes.fromhex('c20400cccc')
# Proven unaligned CUIDef positions are real PDB fields, not a packing trick.
pdb=Path('ghidra_out/struct_layouts_egor.tsv').read_text().splitlines();matches=[l for l in pdb if l.startswith('CUIDef\t')and any(x in l for x in ['PositionOffset','Wrapping','Inverted','NonScrollingChildren','UpLimit','DownLimit'])];assert len(matches)>=6;(o/'CUIDef_pdb_fields.tsv').write_text('\n'.join(matches)+'\n')
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');b=rd(0xbfeae6,6);assert b[:2]==b'\xff\x25';iat=struct.unpack('<I',b[2:])[0];assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==iat]==[(b'MSVCR71.dll',b'memmove')]
records=[];ledger=[json.loads(l)for l in(w/'ATTEMPTS.jsonl').read_text().splitlines()]
for a,g in [('00541120',10),('00541230',3),('00541390',8),('00705040',5)]:
 rows=[x for x in ledger if x['address']==a and x['kind']=='grade'];assert len(rows)==g
 for m in rows:
  assert m['command']and m['base_flags'];assert hashlib.sha256((Path(m['snapshot'])/'source.cpp').read_bytes()).hexdigest()==m['sha256'];records.append(m)
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS7 relocations,4 nativeUIslots,hidden-result/constDelete/matrixRET,unaligned PDBfields,memmove import,26 immutable grades/finalhashes')
