from pathlib import Path
import struct,re,runpy,sys,csv,json,hashlib,subprocess,ctypes
r=Path('work/codex-lane/w22');o=r/'coordinator_retry148';w=r/'retry200g_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];total=0
for a in ['00ce6b10']:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+'_coff_relocs.txt')).write_text(s);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);out=[]
 for off,typ,sym in rel:
  off=int(off,16);assert typ=='DISP32';target=struct.unpack('<i',rd(int(a,16)+off,4))[0]+int(a,16)+off+4
  expected=0xcb7940 if sym.startswith('?IsActiveThreadTerminating')else 0x99eae0 if sym.startswith('??1CCharString')else None;assert target==expected,(a,sym,hex(target));out.append(f'{off:04x}\t{target:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n');total+=len(rel)
vt=[]
for slot,target in [(7,0x6e7410),(28,0x893730),(29,0x8937d0),(305,0x8915b0),(306,0x8915e0)]:assert struct.unpack('<I',rd(0x1260f0c+slot*4,4))[0]==target;vt.append(f'{slot}\t{target:08x}')
(o/'native_virtual_slots.tsv').write_text('slot\ttarget\n'+'\n'.join(vt)+'\n')
# Native thread predicate: null active thread means terminating, otherwise bool byte+5.
(o/'thread_predicate_bytes.txt').write_text(rd(0xcb7940,32).hex()+'\n')
assert rd(0xcb7940,3)==bytes.fromhex('8b412c')
ledger=list(csv.DictReader((w/'ATTEMPTS.tsv').open(),delimiter='\t'));assert len(ledger)==7
for row in ledger:
 p=Path(row['snapshot']);meta=json.loads((p/'before.json').read_text());assert meta['command'] and meta['effective_flags'] and meta['sha256']==row['source_sha256'];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==row['source_sha256'];assert row['reserved_uncertain']=='4';assert int(row['new_grade'])<=6
# Metadata-only corrected VT comment must be the only difference from final grade snapshot.
def tokens(s):return re.sub(r'\s+','',re.sub(r'//[^\n]*|/\*.*?\*/','',s,flags=re.S))
for a in ['00ce6b10']:
 row=[x for x in ledger if x['address']==a][-1];assert tokens((Path(row['snapshot'])/'source.cpp').read_text())==tokens((w/'src'/(a+'.cpp')).read_text())
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.txt').write_text('Seven new grades across two rows: CE6B10 new1 ready, AF4E10 new6 capped. Eight historical UNCERTAIN reservations, five unused new allowances. AF4E10 additionally has an audited malloc-versus-new semantic error; ungraded correction preserved.\n')
print('PASS',total,'direct relocations,5 actual Game slots,thread field,7 source snapshots; all frozen hashes verified.')
