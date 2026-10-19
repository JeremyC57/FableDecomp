from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry176';w=r/'luna236_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?g_pShaderRenderManager_01436e14':0x1436e14,'?CreateResource@CShaderRenderManager':0x98b860,'??3@':0xbfe9bc,'?OpenShaderBank@CShaderResource':0xa5d7c0,'?SetVertexShaderDeclaration':0xa5d4e0,'?SetVertexFormat':0xa5d4f0,'??0CEnginePrimitiveManager@@':0xbacdb0,'??_7CEnginePrimitiveManagerSpriteTrails':0x12a6a10,'??0CVertexShader':0xa62fb0,'?CEngineDynamic3DResourceManagerNativeVTable':0x12a69dc,'??0CBase@@':0xbf2880,'?Initialise@CEngineTreeFilerBase':0xbf3180,'?CEngineVertexBufferPoolNativeVTable':0x12a69ec,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?AddEnableConsoleVariable':0xbacd80,'?g_pEngineData':0x1436e98,'?GetVertexShader@CShaderResource':0xa5d5f0,'??_7CVertexShader':0x129b3d0,'??_7CShaderBase':0x129a030,'__ehhandler$':0x121728c,'__except_list':0,'?GetNextI@CBaseList':0xcb1380,'?RemoveAll@CBaseList':0xcb1340,'??1CAMEvent':0xcb01b0,'??1CBaseList':0xcb1890,'__imp__DeleteCriticalSection@4':0x143fde4}
strings={0x12a69f8:'EnableSpriteTrails',0x12a6964:'VSHADER_TRANSFORM_COL_TEX0'}
for a,s in strings.items():assert rd(a,len(s)+1)==s.encode()+b'\0'
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32' else u(int(a,16)+k)
  assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings and strings[v] in sym
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==37
# Reference-record allocation and fastcall deleting callback, actual resource extent and hidden-result convention.
assert rd(0x98b866,2)==bytes.fromhex('6a3c') and rd(0x98b88b,2)==bytes.fromhex('6a0c')
assert u(0x98b8a2)==0x98ba10 and rd(0x98ba10,8)==bytes.fromhex('85c974068b016a01')
assert rd(0xa5d60e,7)==bytes.fromhex('8bc75f5ec20800')
assert u(0x129b54c+4)==u(0x129d644+4)==0x9d06f0
# Full known manager and shader virtual signatures checked against PDB, plus pool ABI data tables.
tables={0x129a030:[0x987d10,0x987cf0],0x129b3d0:[0x9c1ae0,0x987cf0,0x9be210],0x12a6a10:[0xbcbe80,0xbca8b0,0xbca910,0xbacd90,0xbacda0,0xbcb800,0xb4a460,0xbca6b0,0xb38810,0xb38820,0xb38830,0xb38840,0xb38850],0x12a53e8:[0xbacde0,0xbfea08,0xbfea08,0xbacd90,0xbacda0,0xb4a450,0xb4a460,0xbfea08,0xb38810,0xb38820,0xb38830,0xb38840,0xb38850],0x12a69dc:[0xbca100,0xbca110,0xbca120],0x12a69ec:[0xbca100,0xbca110,0xbca120]}
for vt,targets in tables.items():assert [u(vt+4*i)for i in range(len(targets))]==targets
h=runpy.run_path(str(o/'coff_helpers.py'));syms,secs=h['coff'](next((o/'dry_work').rglob('00bca770.obj')));slots=[]
vtmap={'??_7CShaderBase@@6B@':0x129a030,'??_7CVertexShader@@6B@':0x129b3d0,'??_7CEnginePrimitiveManagerSpriteTrails@@6B@':0x12a6a10}
for name,value,sec in syms.values():
 if name not in vtmap:continue
 vt=vtmap[name];size,rels=secs[sec-1];ss=[]
 for off in range(value,size,4):
  if off not in rels:break
  ss.append(rels[off])
 assert len(ss)==len(tables[vt])
 for i,s in enumerate(ss):slots.append({'vtable':hex(vt),'slot':i,'target':hex(tables[vt][i]),'symbol':s,'signature':h['und'](s)})
assert len(slots)==18
hier=[]
for vt in vtmap.values():
 col=u(vt-4);chd=u(col+16);names=[]
 for i in range(u(chd+8)):
  bc=u(u(chd+12)+4*i);td=u(bc);names.append(rd(td+8,180).split(b'\0')[0].decode())
 hier.append({'vtable':hex(vt),'offset':u(col+4),'classes':names})
assert hier[2]['classes']==['.?AVCEnginePrimitiveManagerSpriteTrails@@','.?AVCEnginePrimitiveManager@@','.?AVCEnginePrimitiveRenderUtil@@','.?AVCBaseClassNonCopyable@@','.?AVCBaseClass@@']
assert u(0x121728d)==0x1371a18 and u(0x1371a18)==0x19930520 and u(0x1371a1c)==4
unwind=u(0x1371a20)
for i,(cleanup,offset) in enumerate([(0x1217260,8),(0x121726b,0x20),(0x1217276,0x38),(0x1217281,0x50)]):
 assert u(unwind+8*i+4)==cleanup and rd(cleanup,6)==bytes.fromhex('8b4df083c1')+bytes([offset])
(o/'native_hierarchy_audit.json').write_text(json.dumps({'emitted_slots':slots,'native_tables':{hex(k):list(map(hex,v))for k,v in tables.items()},'hierarchies':hier,'queue':'Full SDK declarations reviewed. Native COM slots: Release2 and Unadvise6. Native ABI +4 prefix unknown, ctor CAF500 proves offsets; native fixture asserts actual class and member layout. Four cleanup states for lock8, presentation20, stream38,event50.','shader_bank':'Two-slot verified interface prefix, not complete reconstructed native class. Factory98B860 allocates60-byte resource and12-byte refs0/fastcallcallback4/data8 record.','hidden_result':'A5D5F0 const by-value shader getter returns provided hidden storage in EAX and RET8; explicit adapter preserves ABI.'},indent=2))
records=[]
for a,n in [('00b3b5d0',6),('00bca770',9),('00caf580',5)]:
 for i in range(1,n+1):
  d=w/'attempts'/a/f'grade{i:02d}';sha=hashlib.sha256((d/'source.cpp').read_bytes()).hexdigest();assert sha==(d/'snapshot.sha256').read_text().split()[0];assert (d/'command.txt').read_text().strip();assert (d/'stdout.txt').exists() and (d/'stderr.txt').exists();records.append({'address':a,'grade':i,'source_sha256':sha,'command':(d/'command.txt').read_text(),'output_sha256':hashlib.sha256((d/'stdout.txt').read_bytes()+(d/'stderr.txt').read_bytes()).hexdigest(),'path':str(d)})
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS37relocations/2fullstrings/37nativeVTtargets/18emittedsignatures/3RTTIhierarchies/factory+fastcall+hiddenreturn/EH4states/20immutablegrades')
