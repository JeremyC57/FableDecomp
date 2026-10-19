from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry150';w=r/'astra237_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];rows=json.loads((o/'payload.json').read_text())['result']['authored'];count=0
M={'?gGlowResources':0x1436e98,'?ReleaseMemoryPage':0xf37030,'?Initialise@CGlowPixelFormat':0x9e3830,'?AllocateMemoryPage':0xf37660,'?AllocateTexture':0xf37440,'?gWeatherCamera':0x1436ea0,'?ClipBox@':0xb30610,'?GetScratchSystem':0x9a4ec0,'?PrepareShutdown':0xa0af80,'?AttachTextureToStage':0x9a0cf0,'?Uninitialise@CScratchTexture':0x9f9f70,'??3@':0xbfe9bc,'?FreeMemoryPage':0xf37030}
for row in rows:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+'_coff_relocs.txt')).write_text(s);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);out=[]
 for off,typ,sym in rel:
  n=int(off,16);native=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0];expected=[v for k,v in M.items()if sym.startswith(k)];assert expected==[native],(a,sym,hex(native))
  if sym.startswith('?ClipBox@'):assert '@@QBE' in sym
  out.append(f'{off}\t{native:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert struct.unpack('<I',rd(0x122f84c,4))[0]==0x41c9c0;assert rd(0xa0af80,1)==b'\xc3'
(o/'surface_destructor_bytes.txt').write_text(rd(0x41c9c0,40).hex()+'\n');(o/'native_auxiliary_evidence.txt').write_text('Surface VT122F84C slot0=41C9C0 actual deleting destructor;A0AF80 genuine no-op member. PDB ClipBox const at egor_pdb_names.tsv27646. Native arrays12/resources44/pages28/shutdown40 versus donor16/32/54. FreeMemoryPage int and AllocateTexture unsigned declarations are ABI-width projections of PDB long/unsignedlong on Win32.\n')
selected=[]
for a,g in [('00b5ece0',3),('00b51f60',8),('00f37170',3)]:
 ps=sorted((w/'snapshots').glob(a+'_??'));assert len(ps)==g
 for p in ps:
  m=json.loads((p/'manifest.json').read_text());assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==m['sha256'];assert m.get('command') and (m.get('compiler_flags')or m.get('compiler_command'));selected.append(dict(snapshot=str(p),manifest=m))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS',count,'bodyrelocs,surfaceVT/no-op,14 sourcegrade snapshots,allfrozenhashes')
