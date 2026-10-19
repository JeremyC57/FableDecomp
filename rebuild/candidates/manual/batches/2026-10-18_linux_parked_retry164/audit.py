from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry164';w=r/'retry201d_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?GetCurrentServerFrame@':0x416225,'?g_GameFramesPerSecond':0x1375550,'__real@4f800000':0x122dcb4}
for a,n in [('006893d0',5),('00a0d6b0',0)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=''),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert u(0x122dcb4)==0x4f800000;assert rd(0x416225,4)==bytes.fromhex('8b4148c3');assert u(0x1375550)==15
slots=[(0x1253dbc,k,t)for k,t in zip(range(42,46),[0x5bd404,0x5bd427,0x5bd44a,0x5bd46d])]
for vt,k,v in slots:assert u(vt+4*k)==v
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{v:08x}\t{k}\t{t:08x}\n'for v,k,t in slots))
out={}
for vt in [0x1253dbc,0x129ca38,0x129ca10]:
 col=u(vt-4);ch=u(col+16);h=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);h.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 assert h[-2:]==[['.?AVCBaseClassNonCopyable@@',0],['.?AVCBaseClass@@',0]],h;out[hex(vt)]=h
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
records=list(csv.DictReader((w/'ATTEMPTS.tsv').open(),delimiter='\t'));assert len(records)==21
for x in records:
 p=Path(x['snapshot']);m=json.loads(Path(x['pregrade_record']).read_text());assert hashlib.sha256(p.read_bytes()).hexdigest()==m['sha256']==x['sha256'];assert m['command']and m['effective_flags']and m['historical_reserved']==4
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS5relocations/zero managerrelocs,4nativeinventoryslots,3hierarchies,framegetter/FPS/pool,21newgrades+16UNCERTAINreservations')
