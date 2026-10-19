from pathlib import Path
import sys,runpy,struct,re,subprocess,json,hashlib
D=Path(__file__).resolve().parent;sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
native={hex(vt):[hex(u(vt+4*k))for k in range(25)]for vt in [0x125c1f4,0x125c754]}
assert u(0x125c754+9*4)==0x69e1e0 and u(0x125c754+10*4)==0x69e1d0
assert rd(0x691ccf,3)==bytes.fromhex('ff5224') and rd(0x691d06,3)==bytes.fromhex('ff5020')
assert rd(0x69e1d0,11)==rd(0x69e1e0,11)==bytes.fromhex('8b4424048908b001c20400')
text=subprocess.check_output(['objdump','-r',str(next((D/'dry_work').rglob('00691c50.t.obj')))],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',text);chunks=[b[i+1]for i in range(1,len(b),2)if b[i]=='.rdata'];v=next(c for c in chunks if '00000000 dir32             ?Update@TestCamera' in c);rows=re.findall(r'^([0-9a-f]{8})\s+dir32\s+(\S+)',v,re.M);assert len(rows)==25
assert rows[9][1].startswith('?GetCameraManager@TestCamera@@UBE') and rows[10][1].startswith('?GetCameraManager@TestCamera@@UAE')
assert rows[24][1].startswith(('??_GTestCamera','??_ETestCamera'))
(D/'logs/00691c50.camera_repair.native25slots.json').write_text(json.dumps({'native_tables':native,'emitted_TestCamera':rows},indent=2))
M={'?GetControlMovementFromGameActionEvent':0x687d00,'__real@38d1b717':0x129ba3c,'__real@3f800000':0x122ded8,'__real@3f733333':0x125c0b8}
obj=next((D/'dry_work').rglob('00691c50.obj'));out=subprocess.check_output(['objdump','-r',str(obj)],text=True);(D/'logs/00691c50.camera_repair.relocations.txt').write_text(out);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',out);chunks=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];r=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(chunks,key=len),re.M);assert len(r)==4
for off,kind,sym in r:
 k=int(off,16);target=u(0x691c50+k) if kind=='dir32' else struct.unpack('<i',rd(0x691c50+k,4))[0]+0x691c50+k+4
 assert [v for p,v in M.items()if sym.startswith(p)]==[target]
for a,bits in [(0x129ba3c,0x38d1b717),(0x122ded8,0x3f800000),(0x125c0b8,0x3f733333)]:assert u(a)==bits
pre=json.loads((Path('work/codex-lane/w22/retry201c_sol/logs/00691c50_03.pregrade.json')).read_text());src=Path('work/codex-lane/w22/retry201c_sol/src/00691c50.cpp');assert hashlib.sha256(src.read_bytes()).hexdigest()==pre['sha256'];print('PASS native slots9/10, both native getter bodies, emitted25slots/constness,4targetrelocations,3floatpools,sourcehash/pregrade')
