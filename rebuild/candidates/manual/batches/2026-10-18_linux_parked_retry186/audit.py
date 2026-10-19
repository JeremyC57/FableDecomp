from pathlib import Path
import struct,runpy,sys,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry186';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'_free':0xbfea14,'??1CParentDefClassBase':0x430300,'??0CEnginePrimitiveManager@@':0xbacdb0,'??_7CEnginePrimitiveManagerWeaponTrails':0x12a6aec,'??0CVertexShader':0xa62fb0,'?CEngineDynamic3DResourceManagerNativeVTable':0x12a69dc,'??0CBase@@':0xbf2880,'?Initialise@CEngineTreeFilerBase':0xbf3180,'?CEngineVertexBufferPoolNativeVTable':0x12a69ec,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?AddEnableConsoleVariable':0xbacd80,'?g_pEngineData':0x1436e98,'?GetVertexShader@CShaderResource':0xa5d5f0,'??_7CVertexShader':0x129b3d0,'??_7CShaderBase':0x129a030}
strings={0x12a6ad4:'EnableWeaponTrails',0x12a6964:'VSHADER_TRANSFORM_COL_TEX0'}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings and rd(v,len(strings[v])+1)==strings[v].encode()+b'\0'
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==54
# Three pool entries are explicitly ABI data, not fabricated virtual declarations; same original table targets as reviewed176.
for vt in [0x12a69dc,0x12a69ec]:assert [u(vt+i*4)for i in range(3)]==[0xbca100,0xbca110,0xbca120]
assert rd(0xa5d60e,7)==bytes.fromhex('8bc75f5ec20800')
# Const/nonconst subdefinition helpers are byte-identical except two relative calls.
x=bytearray(rd(0x9ad4a0,0x62));y=bytearray(rd(0x9ad510,0x62))
for off in [0x29,0x3e]:x[off:off+4]=y[off:off+4]=b'\0'*4
assert x==y and rd(0x9b2130,0x52)==rd(0x9b20d0,0x52)
hier={}
for vt in [0x12341fc,0x123460c,0x12a6aec,0x129a030,0x129b3d0]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert [x[0]for x in hier['0x12a6aec']]==['.?AVCEnginePrimitiveManagerWeaponTrails@@','.?AVCEnginePrimitiveManager@@','.?AVCEnginePrimitiveRenderUtil@@','.?AVCBaseClassNonCopyable@@','.?AVCBaseClass@@']
for vt in ['0x12341fc','0x123460c']:assert [x[0]for x in hier[vt][1:]]==['.?AVCParentDefClassBase@@','.?AVCDefClassBase@@','.?AVCDefPointeeBase@@','.?AVCResource@@','.?AVCIVCountedPointeeBase@@']
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2));sig=json.loads((o/'native_virtual_signatures.json').read_text());assert len(sig['slots'])==86 and not sig['mismatches']
records=[]
for a in ['004573fd','00457ca0']:
 ss=sorted((r/'astra242_whole/snapshots'/a).glob('a[0-9][0-9]'));assert len(ss)==2
 for p in ss:
  x=json.loads((p/'manifest.json').read_text());assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['base_flags']==['/O2','/Oy','/W3'];records.append(x)
for i in [1,2]:
 p=r/'luna236_whole/attempts/00bcc5f0'/f'grade{i:02d}';sha=hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest();assert sha==(p/'snapshot.sha256').read_text().split()[0];assert(p/'stdout.txt').exists();records.append({'address':'00bcc5f0','attempt':i,'sha256':sha,'command':(p/'command.txt').read_text(),'path':str(p)})
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS54relocations/2fullstrings/86signatures including50supplementalABI-only/5RTTIhierarchies/6immutablegrades')
