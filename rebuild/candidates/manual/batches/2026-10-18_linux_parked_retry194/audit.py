from pathlib import Path
import struct,runpy,sys,json,hashlib,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry194';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?g_fOne_0122DED8':0x122ded8,'?g_fReciprocal255_01231724':0x1231724,'?g_pViewRecord_01436E80':0x1436e80,'?BeginLoading@C3DMeshInfoLoadingCallView':0xa256f0,'?UnknownRet4@':0xa23c50,'_memmove':0xbfeae6,'?g_OcclusionManager':0x1436e5c,'?GetResult@CEngineOcclusionManager_Methods':0xb8d0b0,'?ClearHandles@CEnginePrimitiveOcclusionInfo':0xb2f8b0}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert[val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==13
assert rd(0x122ded8,4)==struct.pack('<f',1.)and rd(0x1231724,4)==struct.pack('<f',1./255.)
assert rd(0xa23c50,3)==bytes.fromhex('c20400')
assert rd(0xb8d0b0,5)==bytes.fromhex('33c0c20800')
assert rd(0xb2f8b3,15)==bytes.fromhex('8b47103bc08b4f0c7505894f105fc3')
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');imports={x.address:(e.dll.decode(),x.name.decode()if x.name else x.ordinal)for e in pe.DIRECTORY_ENTRY_IMPORT for x in e.imports};assert rd(0xbfeae6,2)==bytes.fromhex('ff25');iat=u(0xbfeae8);assert imports[iat]==('MSVCR71.dll','memmove')
(o/'native_imports.json').write_text(json.dumps(dict(native='00bfeae6',iat=hex(iat),name=imports[iat]),indent=2))
pdb=[l.split('\t',1)[1]for l in(Path('ghidra_out/egor_pdb_names.tsv').read_text()+Path('ghidra_out/fablewin_pdb_names.tsv').read_text()).splitlines()if'\t'in l]
symbols=['?BeginLoading@C3DMeshInfo@@QBEXK_N0@Z','?GetResult@CEngineOcclusionManager@@QAEKK_N@Z','?ClearHandles@CEnginePrimitiveOcclusionInfo@@AAEXXZ','?CheckVisibilityResult@CEnginePrimitiveOcclusionManager@@QAEXAAVCEnginePrimitiveOcclusionInfo@@@Z','?UpdateRepeatedStaticMesh@CEnginePrimitiveManagerRepeatedStaticMeshes@@QAEXABVCEnginePrimitiveHandle@@ABUCAddMeshDesc@NEnginePrimitiveManagerRepeatedStaticMeshes@@@Z']
for s in symbols:assert s in pdb,s
(o/'original_signatures.json').write_text(json.dumps([dict(symbol=s,undecorated=H['und'](s))for s in symbols],indent=2))
sy,se=H['coff'](next((o/'dry_work_const').rglob('00b991d0.t.obj')));assert any(n=='?BeginLoading@C3DMeshInfoLoadingCallView@@QBEXK_N0@Z'and s>0 for n,v,s in sy.values())
# Descriptor field names/types from actual donor structure; native body and assertions agree with these offsets.
fields=[]
for line in Path('ghidra_out/struct_layouts_egor.tsv').read_text().splitlines():
 if line.startswith('CAddMeshDesc\t'):fields.append(line.split('\t'))
assert len(fields)==17 and all(x[1]=='104'for x in fields)
assert {x[-1]:int(x[2])for x in fields}=={'MeshInfo':0,'MaxScale':4,'VectorNormals':8,'ObjectCount':12,'SubSections':16,'BoundingBox':20,'BoundingSphere':44,'FadeDist':60,'ObjectOffsets':68,'ObjectMatricies':72,'EnginePrimitiveFlags':76,'AlphaRef':80,'AlphaMipBias':84,'Persistent':88,'EPFFlags':92,'WindDelayArray':96,'WindScaleFactor':100}
# The donor metadata declares17 entries; count actual field lines, not an inferred extra padding member.
assert len(fields)==17
(o/'descriptor_name_evidence.json').write_text(json.dumps(fields,indent=2))
hier={}
for vt,cl in [(0x12a3e9c,'CEnginePrimitiveManagerRepeatedStaticMeshes'),(0x12a3f4c,'CEngineInternalPrimitiveRepeatedStaticMesh'),(0x122f598,'CEnginePrimitiveHandle')]:
 col=u(vt-4);name=rd(u(col+12)+8,180).split(b'\0')[0].decode();assert name=='.?AV'+cl+'@@';ch=u(col+16);items=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);items.append([rd(u(bc)+8,180).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=items
assert rd(0xb31fe0,6)==bytes.fromhex('8b8990020000')and rd(0xb31fe6,1)==b'\xe9'and 0xb31feb+struct.unpack('<i',rd(0xb31fe7,4))[0]==0xb991d0
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2))
records=[]
for a,total in [('00b991d0',1),('00b2f8e0',3)]:
 for i in range(1,total+1):
  p=r/'luna236_whole/attempts'/a/f'grade{i:02d}';sha=(p/'input.sha256').read_text().split()[0];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==sha;cmd=(p/'command.txt').read_text();assert '--flags'not in cmd;records.append(dict(address=a,source=str(p/'source.cpp'),sha256=sha,command=cmd,effective_flags=['/O2','/Oy','/W3'],log=str(p/'stdout.txt'),status=(p/'stdout.txt').read_text().splitlines()[0]))
q=json.loads((o/'root_amendments/grade.json').read_text());assert hashlib.sha256(Path(q['source']).read_bytes()).hexdigest()==q['sha256'];assert 'RELOCATION_MATCH'in(o/'root_amendments/grade.log').read_text();records.append(q);assert len(records)==5;(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
assert(o/'dry_const.log').read_text().count('RELOCATION_MATCH PASS')==2
print('PASS13relocations/2floatpools/5PDBsignatures/3RTTIidentities/3nativehelpers/CRTmemmove/5immutablegrades')
