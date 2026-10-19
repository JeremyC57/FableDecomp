from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry169';w=r/'retry202a_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'__real@4f800000':0x122dcb4,'__real@c1c80000':0x12462a4,'??0CChangingStateComponent':0x52cc50,'?CListPrimaryVTable':0x12462e4,'?CListObserverVTable':0x12462bc,'?CListCountedObserverVTable':0x12462b4,'?GetDataFromDef@CList':0x53822b}
for a,n in [('0053633e',2),('0053664c',2),('0053b63e',5)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v];out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert u(0x122dcb4)==0x4f800000 and u(0x12462a4)==0xc1c80000
slots=[(0x12462e4,33,0x52eaf0),(0x12462e4,35,0x52ebb0),(0x12462e4,185,0x5379f5)]
for vt,k,v in slots:assert u(vt+k*4)==v
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{vt:08x}\t{k}\t{v:08x}\n'for vt,k,v in slots))
out={}
for vt,offset in [(0x12462e4,0),(0x12462bc,4),(0x12462b4,24)]:
 col=u(vt-4);assert u(col+4)==offset;ch=u(col+16);h=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);h.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 assert h[0]==['.?AVCList@NUISystem@@',0]and h[1]==['.?AVCChangingStateComponent@NUISystem@@',0];assert h[-2:]==[['.?AVCObserver@NUISystem@@',4],['.?AVCActionParam@NUISystem@@',24]];out[hex(vt)]={'object_offset':offset,'hierarchy':h}
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
records=list(csv.DictReader((w/'ATTEMPTS.tsv').open(),delimiter='\t'));assert len(records)==16
for x in records:
 p=Path(x['snapshot']);m=json.loads(Path(x['pregrade_record']).read_text());assert hashlib.sha256(p.read_bytes()).hexdigest()==m['sha256']==x['sha256'];assert m['command']and m['effective_flags']and m['historical_reserved']==4;assert int(x['cumulative'])==4+int(x['new_grade'])
assert {a:sum(x['address']==a for x in records)for a in ['0053633e','0053664c','0053b63e','00bf7f70']}=={'0053633e':2,'0053664c':5,'0053b63e':3,'00bf7f70':6}
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS9relocations/3UIvirtualslots/3RTTIoffsets+hierarchies/2floatpools/16observedgrades+16UNCERTAINreservations')
