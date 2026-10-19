from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry163';w=r/'astra237_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'??_7CFontManager':0x129ba38,'??3@':0xbfe9bc,'_free':0xbfea14,'??1CBaseClassNonCopyable':0x99a300,'??_7CGameCameraBase':0x1264a8c,'??0CCameraNative':0xa0c130,'_malloc':0xbfea0e,'?gCameraFramesPerSecond':0x1375550,'__real@3fc00000':0x125aadc,'__ftol2':0xbfea70,'?InterpolateRadialBlurParams':0x8854a0,'?InterpolateRadialBlurPositionParams':0x885680,'??0CIDrawEnvironmentRadialBlurParameters':0x885a80};count=0
for a,n in [('009e2d00',8),('008857e0',3)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 count+=n;(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
u=lambda a:struct.unpack('<I',rd(a,4))[0]
assert u(0x125aadc)==0x3fc00000
expected={0x129ba38:['CFontManager','CBaseClassNonCopyable','CBaseClass'],};out={}
for vt,names in expected.items():
 col=u(vt-4);ch=u(col+16);h=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);h.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 assert h==[['.?AV'+n+'@@',0]for n in names],(hex(vt),h);out[hex(vt)]=h
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
slots=[(0x129ba38,0,0x9e3560),(0x129da84,5,0xa635c0),(0x129ba20,0,0x9e2890)]
for vt,k,v in slots:assert u(vt+4*k)==v
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{v:08x}\t{k}\t{t:08x}\n'for v,k,t in slots))
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe')
for thunk,iat,name in [(0xbfea14,0x1440154,b'free'),(0xbfe9bc,0x1440164,b'??3@YAXPAX@Z'),(0xbfea0e,0x1440158,b'malloc')]:
 assert rd(thunk,6)==b'\xff\x25'+struct.pack('<I',iat);assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==iat]==[(b'MSVCR71.dll',name)]
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(rd(0x6fd8c0,294),0x6fd8c0));sizes=[]
for i,x in enumerate(ins):
 if x.mnemonic=='call'and x.op_str=='0xbfea0e':sizes.append(int([y for y in ins[max(0,i-4):i]if y.mnemonic=='push'][-1].op_str,0))
assert sizes==[20,20]
assert rd(0x8854a0,4)==bytes.fromhex('d9442404');assert rd(0x885680,14)==bytes.fromhex('83ec4453568b721c8bc18b4c2454');assert rd(0x885a80,6)==bytes.fromhex('8bc18b4c2404')
(o/'native_auxiliary_evidence.txt').write_text('Two malloc20sentinels; camera native vectors00/111/001/FOV3E471B48/0 and documented donor shifts. Radial helper hiddenECXout/EDXprevious, fraction/currentstack, realcopy885A80; fullhelperdisassembly retainedinworkerlogs. Genuinedefaultmember/owner/copyconstructors backedPDB; noinventedfixturedefaults.\n')
records=[]
for line in(o/'attempts.tsv').read_text().splitlines()[1:]:
 a,g,_=line.split('\t');ps=sorted((w/'snapshots').glob(a+'_??'));assert len(ps)==int(g),(a,len(ps),g)
 for p in ps:
  m=json.loads((p/'manifest.json').read_text());assert m['command']and m['compiler_flags'];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==m['sha256'];records.append(dict(snapshot=str(p),manifest=m))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS11relocs,font hierarchy/3VTslots,imports/hiddenreturnABI,',len(records),'immutablegrades/finalhashes')
