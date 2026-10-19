from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'repair00541390';w=r/'astra235_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?FindIndex':0x535f5a,'?InsertOverflow':0x485dbc};obj=next((o/'dry_work').rglob('00541390.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==2;out=[]
for off,typ,sym in rel:
 n=int(off,16);assert typ=='DISP32';v=struct.unpack('<i',rd(0x541390+n,4))[0]+0x541390+n+4;assert [v0 for p,v0 in M.items()if sym.startswith(p)]==[v];out.append([off,sym,hex(v)])
assert u(0x1246e44+108*4)==0x530ec0;assert rd(0x530f1b,7)==bytes.fromhex('8bc65e59c20400');assert rd(0x42d930,11)==bytes.fromhex('85c974068b016a01ff10c3')
pdb=Path('ghidra_out/fablewin_pdb_names.tsv').read_text();assert '?GetRefCount@CIVCountedPointeeBase@@QBEJXZ'in pdb and '?Delete@CIVCountedPointeeBase@@UBEXXZ'in pdb
rows=[json.loads(l)for l in(w/'ATTEMPTS.jsonl').read_text().splitlines()if json.loads(l)['address']=='00541390'];assert len(rows)==9
for x in rows:assert x['command']and x['base_flags']and hashlib.sha256((Path(x['snapshot'])/'source.cpp').read_bytes()).hexdigest()==x['sha256']
(o/'attempt_audit.json').write_text(json.dumps(rows,indent=2));(o/'relocations.json').write_text(json.dumps(out,indent=2))
for x in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(x[k]['path']).read_bytes()).hexdigest()==x[k]['sha256']
print('PASS2relocs/loaddefslot108/constDelete+signedgetter/hiddenreturn/9grades/frozenhashes')
