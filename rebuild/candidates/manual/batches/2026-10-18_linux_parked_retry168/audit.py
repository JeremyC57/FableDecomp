from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry168';w=r/'astra239_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?GetPItem@CBaseIntelligentPointer@@QBE':0xa01b50,'?LowerBound@ComponentMap@@QBE':0x4365b0,'?QuestManager':0x13b89fc,'??_C@':0x122d70e,'??4CCharString@@QAEABV0@PBD@Z':0x99efe0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0};tables={};records=[];total=0
for row in csv.DictReader((w/'READY_CTORS_FINAL.tsv').open(),delimiter='\t'):
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==(8 if a=='008d3bf0'else 5);out=[];total+=len(rel)
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert [v0 for p,v0 in M.items()if sym.startswith(p)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
 t=next((o/'dry_work').rglob(a+'.t.obj'));s=subprocess.check_output(['objdump','-r',str(t)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);v=next(b[i+1]for i in range(1,len(b),2)if b[i]=='.rdata'and'?GetType@TestHit'in b[i+1]);vv=re.findall(r'^([0-9a-f]{8})\s+dir32\s+(\S+)',v,re.M);assert len(vv)==5;assert vv[0][1].startswith(('??_ETestHit','??_GTestHit'));assert vv[1][1].startswith('?GetType@TestHit@@UBE?AW4EEventType');assert vv[2][1].startswith('?OnAdd@EventBase@@UAEXABU');assert vv[3][1].startswith('?GetLifeTime@EventBase@@UBEJ');assert vv[4][1].startswith('?IsReadyToDie@EventBase@@UBE_N');tables[a]=vv
 n=0
 for p in sorted((w/'snapshots').glob(a+'_[0-9]*')):
  x=json.loads((p/'grade.json').read_text());assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['effective_flags'];counted=not(p/'COUNT_CORRECTION.json').exists()
  if not counted:
   c=json.loads((p/'COUNT_CORRECTION.json').read_text());assert c['counted']==False and c['identical_flags'];prior=p.parent/(a+'_01');strip=lambda t:re.sub(r'//[^\n]*','',t);assert strip((p/'source.cpp').read_text())==strip((prior/'source.cpp').read_text());x['count_correction']=c
  else:n+=1
  x['counted']=counted;x['snapshot_path']=str(p);records.append(x)
 assert n==int(row['source_grades']),(a,n)
assert total==43;assert len(records)==34 and sum(x['counted']for x in records)==33
native={}
for vt,expected in [(0x124f2c4,[0x57a502,0xbfea08,0x57a4fc,0x810e60,0x57a4ff]),(0x1272390,[0x811d10,0x811d00,0x57a4fc,0x810e60,0x57a4ff]),(0x124f2dc,[0x57f36f,0x57a535,0x57a4fc,0x810e60,0x57a4ff])]:
 assert [u(vt+4*i)for i in range(5)]==expected;native[hex(vt)]=[hex(x)for x in expected]
assert rd(0x811d00,6)==bytes.fromhex('b81d000000c3');assert rd(0x57a535,4)==bytes.fromhex('6a2358c3');assert rd(0x57a4fc,3)==bytes.fromhex('c20400');assert rd(0x57a4ff,3)==bytes.fromhex('b001c3');assert rd(0x810e60,6)==bytes.fromhex('a150553701c3');assert rd(0x122d70e,1)==b'\0';assert rd(0x4365d0,2)==bytes.fromhex('7d0d')
(o/'native_hierarchy_audit.json').write_text(json.dumps({'native_event_vtables':native,'emitted_TestHit_vtables':tables},indent=2));(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for x in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(x[k]['path']).read_bytes()).hexdigest()==x[k]['sha256']
print('PASS43relocations/3nativeeventVTs+8emitted5slotVTs/typevalues+constsignatures/signedlookup/33grades+1metadataverification')
