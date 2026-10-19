from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry135'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
resize={'?g_LandscapeRenderer':0x1436ea8,'?AllocVirtual':0xbd9af0,'?FreeVirtual':0xbd9b90}
gamma={'___security_cookie':0x139c8a8,'__real@3e99999a':0x1238170,'__real@40c00000':0x125a2a0,'?g_RampZero':0x122dedc,'?g_RampMaximum':0x129b3f0,'__ftol2':0xbfea70,'@__security_check_cookie@4':0xbfe9f9}
def blocks(a):
 t=(o/f'{a}_coff_relocs.txt').read_text();b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',t)
 return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
for a,needle,targets,count in [(a,'?AllocVirtual',resize,6)for a in ['00bfbf40','00bfbe40','00bfc040']]+[('009beaa0','___security_cookie',gamma,9)]:
 selected=max([b for section,b in blocks(a)if section=='.text'and needle in b],key=len);rel=relocs(selected);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
for va,value in [(0x1238170,0.3),(0x125a2a0,6.0),(0x122dedc,0.0),(0x129b3f0,65280.0)]:assert rd(va,4)==struct.pack('<f',value)
for a,n,gap in [('00bfbf40',185,7),('00bfbe40',186,6),('00bfc040',185,7)]:
 assert rd(int(a,16)+n-3,3)==b'\xc2\x08\x00';assert rd(int(a,16)+n,gap)==b'\xcc'*gap
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
(o/'constants_and_abi.txt').write_text('Gamma nativefloatpools:1238170=.3,125A2A0=6,122DEDC=0,129B3F0=65280. D3D9 COM SetGammaRamp slot21, nativecallpushes ramp/flags/swapchain0/this, stdcallcallee stackcleanup. Actual-source VC fixture uses stdcall member PMF withnative4byteassert; host fullrepresentation. Nativecookie139C8A8/checkBFE9F9,ftol2BFEA70 audited. Resize exactends+ret8/CCgap independentoriginalspan preserved; allocatorBD9AF0/FreeBD9B90 identity andnullablebehavior independentlydisassembled.\n')
print('PASS27 body relocations,4 floatpools,3 preciseboundaries,frozenhashes')
