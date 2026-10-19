from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry159';w=r/'retry201c_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'??0CInputEvent':0xa04410,'__real@0000000000000000':0x122ed70,'?SetAsKeyboardKeyReleased':0xa03c50,'?SetAsKeyboardKeyHeld':0xa03c20,'?SetAsKeyboardKeyPressed':0xa03bf0,'?IsEventInQueue':0x9f4d40,'??1CInputEvent':0xa03b30,'?GetChecksum':0xa04160,'__real@4f800000':0x122dcb4,'__ftol2':0xbfea70,'??0CBaseClass@@':0x99a2d0,'??_7CInputManager':0x129c79c,'??0CBaseClassNonCopyable':0x99a2f0,'??_7CDiskFileWin32':0x122d06c,'??0CWideString':0x99aed0,'?erase@':0x9f67e0,'?GetControlMovementFromGameActionEvent':0x687d00,'__real@38d1b717':0x129ba3c,'__real@3f800000':0x122ded8,'__real@3f733333':0x125c0b8}
count=0
for a,n in [('009f50a0',12),('009f5170',5),('009f5bd0',7),('00691c50',4)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=''),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 count+=n;(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
for a,h in [(0x122dcb4,'4f800000'),(0x129ba3c,'38d1b717'),(0x122ded8,'3f800000'),(0x125c0b8,'3f733333')]:assert rd(a,4)==struct.pack('<I',int(h,16))
assert rd(0x122ed70,8)==b'\0'*8
u=lambda a:struct.unpack('<I',rd(a,4))[0]
def hierarchy(vt):
 col=u(vt-4);ch=u(col+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 return out
expected={0x129c79c:['CInputManager','CBaseClass'],0x122d06c:['CDiskFileWin32','CAFile','CBaseClassNonCopyable','CBaseClass'],0x129dbc4:['CMouse','CBaseClassNonCopyable','CBaseClass'],0x129c0ec:['CJoystick'],0x1237f74:['CInputProcessCreatureMovementWatchForControlAngleChange','CInputProcessGameBase','CAInputProcess','CBaseClassNonCopyable','CBaseClass']}
hr={}
for vt,names in expected.items():
 h=hierarchy(vt);assert h==[['.?AV'+n+'@@',0]for n in names],(hex(vt),h);hr[hex(vt)]=h
(o/'native_hierarchy_audit.json').write_text(json.dumps(hr,indent=2))
slots=[(0x129c79c,1,0x9f4e70),(0x129c79c,2,0x9f57a0),(0x129ea14,1,0xab46e0),(0x129ea7c,3,0xab6ad0),(0x129ea7c,4,0xab6ae0)]
files=[0x4016b0,0x99a6a0,0x99a920,0x99a960,0x99aa90,0x99abf0,0x99ac90,0x99abd0,0x99a560,0x401670,0x401680,0x401690,0x401650,0x401660,0x99ae00,0x99a5a0]
slots +=[(0x122d06c,k,v)for k,v in enumerate(files)]
for vt,k,v in slots:assert u(vt+4*k)==v,(hex(vt),k,hex(u(vt+4*k)))
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{v:08x}\t{k}\t{t:08x}\n'for v,k,t in slots))
records=list(csv.DictReader((w/'ATTEMPTS.tsv').open(),delimiter='\t'));assert len(records)==14
for x in records:
 p=Path(x['snapshot']);m=json.loads(Path(x['pregrade_record']).read_text());assert hashlib.sha256(p.read_bytes()).hexdigest()==m['sha256']==x['sha256'];assert m['command']and m['effective_flags']and m['historical_reserved']==4
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS',count,'relocations,5 pools,21 native slots,5 hierarchies,14 new grades +16 UNCERTAIN reservations/final hashes')
