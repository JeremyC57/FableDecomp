from pathlib import Path
import json,struct,runpy,sys,hashlib,re,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry165';w=r/'astra235_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?PeekGuiDef':0x438570,'?Resize@HandleArray':0x443f90,'?GFAnchorToTopRight':0x643ca1,'?GlobalPlayerGui':0x13b8790,'__real@437f0000':0x1230014,'__ftol2':0xbfea70,'??0CEnginePrimitive2DSprite':0x493ac2}
obj=next((o/'dry_work').rglob('0064f6dc.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rows=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rows)==7
out=[]
for off,kind,sym in rows:
 n=int(off,16);v=u(0x64f6dc+n)if kind=='dir32'else struct.unpack('<i',rd(0x64f6dc+n,4))[0]+0x64f6dc+n+4;assert [v0 for p,v0 in M.items()if sym.startswith(p)]==[v];out.append(f'{off}\t{v:08x}\t{sym}')
(o/'0064f6dc_relocation_audit.tsv').write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert u(0x12a0f3c+23*4)==0xb23bc0;assert u(0x1230014)==0x437f0000;assert u(0x1238168)==0x44200000
assert rd(0x643ca1+0x10,3)==bytes.fromhex('8d45f4');assert rd(0x643ccd,2)==bytes.fromhex('8bc6');assert rd(0x493b58,3)==bytes.fromhex('c24400')
# Native sprite writes B,G,R,A at34..37, 84byte final UV field50; exact ctor signature and stack68.
for addr,data in [(0x493b15,'884636'),(0x493b1b,'884635'),(0x493b21,'884634'),(0x493b27,'884637'),(0x493b51,'894650')]:assert rd(addr,len(bytes.fromhex(data)))==bytes.fromhex(data)
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n012a0f3c\t23\t00b23bc0\n')
records=[json.loads(l)for l in (w/'ATTEMPTS.jsonl').read_text().splitlines()if json.loads(l)['address']=='0064f6dc'];assert len(records)==3
for x in records:
 p=Path(x['snapshot']);assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==x['sha256'];before=json.loads((p/'before.json').read_text());assert before==x;assert x['command']and x['base_flags'];assert (p/'stdout.txt').exists()
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for x in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(x[k]['path']).read_bytes()).hexdigest()==x[k]['sha256']
print('PASS7relocations/nativeengine23/floatpools/hiddenresult+spriteABI/3immutablegrades+frozenhashes')
