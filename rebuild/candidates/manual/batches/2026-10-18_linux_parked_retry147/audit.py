from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry147';w=r/'astra235_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];expected=json.loads((o/'observed_relocations.json').read_text());total=0
for a,items in expected.items():
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+'_coff_relocs.txt')).write_text(s);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==len(items);out=[]
 for (off,typ,sym),(eo,et,es,ev) in zip(rel,items):
  assert (off,typ,sym)==(eo,et,es),(a,off,sym);n=int(off,16);target=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0];assert target==int(ev,16);out.append(f'{off}\t{target:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n');total+=len(rel)
slots=[(0x1260f0c,7,0x6e7410),(0x1260f0c,8,0x89b5b0),(0x1260f0c,108,0x8910d0),(0x1260f0c,185,0x898e60),(0x1260f0c,425,0x895db0),(0x1260f0c,426,0x895de0),(0x12a398c,1,0x42d930),(0x12c32f0,0,0xcdf120),(0x12c32f0,1,0xcdf130),(0x12c32f0,2,0xcdf160)]
for vt,slot,target in slots:assert struct.unpack('<I',rd(vt+slot*4,4))[0]==target,(hex(vt),slot)
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{v:08x}\t{s}\t{t:08x}\n'for v,s,t in slots))
ledger=[json.loads(l)for l in(w/'ATTEMPTS.jsonl').read_text().splitlines()];selected=[x for x in ledger if x['address']in expected];assert len(selected)==12
for row in selected:
 p=Path(row['snapshot']);meta=json.loads((p/'before.json').read_text());assert meta['command'] and meta['base_flags'];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==row['sha256']
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
 final=[x for x in selected if x['address']==row['address']][-1];assert final['sha256']==row['src']['sha256']
(o/'author_attempts.json').write_text(json.dumps(selected,indent=2));print('PASS',total,'body relocation targets,10 native virtual slots,12 source grades and all final source/fixture hashes')
