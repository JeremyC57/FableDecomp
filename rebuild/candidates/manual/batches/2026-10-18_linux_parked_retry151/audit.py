from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry151';w=r/'luna236_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];rows=json.loads((o/'payload.json').read_text())['result']['authored'];count=0
M={'?SetFadeDistanceWithGamePrimitiveDefaults':0xb8fd40,'?g_pEngineGlobal':0x1436ea8,'?ClearTexturePool':0xbd9ce0,'?g_pEngineSceneState':0x1436e8c,'__real@':0x12a1b28,'?UpdateBackgroundLOD':0xbdc180,'?UpdateForeground':0xbdc510,'?UpdateBackgroundTesselation':0xbdc020,'?LoadForegroundTextures':0xb68070}
for row in rows:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+'_coff_relocs.txt')).write_text(s);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);out=[]
 for off,typ,sym in rel:
  n=int(off,16);native=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0]
  if sym.startswith('?resize@'):expected={0x70:0xbcde70,0x7d:0x75c040,0x8d:0x75c040,0xa3:0x442770}[n]
  else:es=[v for k,v in M.items()if sym.startswith(k)];assert len(es)==1,(a,sym);expected=es[0]
  assert expected==native,(a,sym,hex(native))
  if sym.startswith('?LoadForegroundTextures'):assert '@@QAE_N_N@'in sym
  out.append(f'{off}\t{native:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert struct.unpack('<I',rd(0x12a6b24+5*4,4))[0]==0xbcd660;assert struct.unpack('<I',rd(0x12a2b54+3*4,4))[0]==0xb684f0
assert rd(0x12a1b28,8)==struct.pack('<d',struct.unpack('<f',struct.pack('<f',1e30))[0])
(o/'native_auxiliary_evidence.txt').write_text('WeaponTrailVT12A6B24slot5=BCD660;LandscapeRendererVT12A2B54slot3=B684F0. Constant12A1B28 is double widened from float1e30, not literal double1e30. Actual LoadNewScene/LoadForegroundTextures bool return types PDB egor_pdb_names.tsv27119/27120.\n')
selected=[]
for a,g in [('00bcd660',7),('00b684f0',6)]:
 ps=sorted((w/'attempts'/a).glob('grade??'));assert len(ps)==g
 for p in ps:
  h=hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest();assert h in(p/'input.sha256').read_text();cmd=(p/'command.txt').read_text();assert 'timeout 200 python3' in cmd and a in cmd;selected.append(dict(snapshot=str(p),sha256=h,command=cmd))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS',count,'bodyrelocs,2nativeVTs,widenedconstant,13 immutablegrades and finalhashes')
