from pathlib import Path
import struct,runpy,sys,json,hashlib,re,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry197';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
obs=json.loads((o/'observed_relocations.json').read_text());M={'?GetCellBoundingBoxAndDrawDistance@':0xb919c0,'?ClipWorldSpaceBoundingBox@':0xb308e0,'?g_pEnginePrimitiveTypeInfo':0x1436e84,'?IntersectsSolid@':0xadecc0,'?GetCellRenderMask@':0xb91dc0,'?ClipBoundingBox@':0xb4ced0,'?RenderCellClippedShadowBuffer@':0xb92660,'?PreloadPrimitives@CEngineSceneGrid@@QAE_NKKABUCEngineCamera@@K@Z':0xb91f50,'?PreloadPrimitives@CEngineSceneGrid@@QAE_NKKABUC3DBoundingSphere@@K@Z':0xb92d30}
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16);assert[val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==19
pdb={}
for fn in ['ghidra_out/egor_pdb_names.tsv','ghidra_out/fablewin_pdb_names.tsv']:
 for i,l in enumerate(Path(fn).read_text().splitlines(),1):
  if '\t'in l:pdb.setdefault(l.split('\t',1)[1],[]).append([fn,i,l.split('\t')[0]])
def norm(s):return re.sub(r'\s+','',re.sub(r'public: |protected: |private: |class |struct |enum ','',s)).replace('CEngineCameraMethods::','CEngineCamera::')
# Original full signatures for directly called boundary helpers, target functions and real constructors.
originals=[]
for sym in sorted(set(x[2]for rows in obs.values()for x in rows if not x[2].startswith('?g_'))):
 comp=H['und'](sym);hits=[s for s in pdb if s.startswith('?'+sym[1:].split('@')[0]+'@')and norm(H['und'](s))==norm(comp)];assert hits,(sym,comp);originals.append(dict(compiled=sym,signature=comp,original_symbols=hits,records=[pdb[s]for s in hits]))
for a in ['00b92f80','00b92e50']:
 sy,se=H['coff'](next((o/'dry_work_extended').rglob(a+'.obj')));sym=next(n for n,v,s in sy.values()if n.startswith('?PreloadPrimitives@CEngineScene@@')and s>0);hits=[s for s in pdb if s.startswith('?PreloadPrimitives@CEngineScene@@')and norm(H['und'](s))==norm(H['und'](sym))];assert hits;originals.append(dict(compiled=sym,original_symbols=hits,records=[pdb[s]for s in hits]))
ctors=['??0CIVCountedPointeeBase@@QAE@XZ','??0CEnginePersistentPrimitiveBase@@QAE@J@Z','??0CEngineInternalPrimitiveBase@@QAE@JK@Z','??0CBaseClass@@QAE@XZ','??0CBaseClassNonCopyable@@IAE@XZ','??0CEnginePrimitiveRenderUtil@@QAE@XZ','??0CEnginePrimitiveManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z']
for sym in ctors:assert sym in pdb;originals.append(dict(symbol=sym,signature=H['und'](sym),records=pdb[sym]))
(o/'original_signatures.json').write_text(json.dumps(originals,indent=2))
# Full emitted interfaces, not only invoked slots. Native purecall and all parent tables stay explicit.
tables=[('CIVCountedPointeeBase',0x12354d0,2),('CEnginePersistentPrimitiveBase',0x12a3d60,5),('CEngineInternalPrimitiveBase',0x12a3d7c,28),('CBaseClass',0x1231710,1),('CBaseClassNonCopyable',0x129a7c4,1),('CEnginePrimitiveRenderUtil',0x12a3bd0,1),('CEnginePrimitiveManager',0x12a53e8,13)]
slots=[];hier={}
for cl,vt,count in tables:
 col=u(vt-4);name=rd(u(col+12)+8,160).split(b'\0')[0].decode();assert name=='.?AV'+cl+'@@';ch=u(col+16);items=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);items.append([rd(u(bc)+8,180).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[cl]=dict(vtable=hex(vt),hierarchy=items)
 for a in obs:
  sy,se=H['coff'](next((o/'dry_work_extended').rglob(a+'.t.obj')));n,v,s=next((n,v,s)for n,v,s in sy.values()if n=='??_7'+cl+'@@6B@'and s>0);arr=[q for k,q in sorted(se[s-1][1].items())if k>=v];assert len(arr)==count,(a,cl,len(arr))
  for i,sym in enumerate(arr):
   target=u(vt+4*i)
   if i==0:assert sym.startswith(('??_G','??_E'));hits=['deleting destructor; original full hierarchy audited']
   elif target==0xbfea08:assert sym=='__purecall';hits=['native purecall']
   else:
    # Compiler names inherited slots under their declaring class, matching the original declaration owner.
    method=sym[1:].split('@')[0];hits=[q for q in pdb if q.startswith('?'+method+'@')and norm(H['und'](q))==norm(H['und'](sym))];assert hits,(a,cl,i,H['und'](sym))
   slots.append(dict(address=a,table=cl,slot=i,target=hex(target),compiled=sym,signature=H['und'](sym),original_symbols=hits))
assert len(slots)==255
assert [x[0]for x in hier['CEngineInternalPrimitiveBase']['hierarchy']]==['.?AVCEngineInternalPrimitiveBase@@','.?AVCEnginePersistentPrimitiveBase@@','.?AVCIVCountedPointeeBase@@']
assert [x[0]for x in hier['CEnginePrimitiveManager']['hierarchy']]==['.?AVCEnginePrimitiveManager@@','.?AVCEnginePrimitiveRenderUtil@@','.?AVCBaseClassNonCopyable@@','.?AVCBaseClass@@']
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2));(o/'native_virtual_signatures.json').write_text(json.dumps(slots,indent=2))
# Native slot order of overloaded SetChildPrimitive is explicit; table ends at27, next float pool is not a virtual entry.
assert u(0x12a3d7c+7*4)==0xb8fdb0 and u(0x12a3d7c+8*4)==0xb8fda0 and u(0x12a3d7c+28*4)==0x3dcccccd
records=[]
for a,total in [('00b91f50',1),('00b92d30',1),('00b92660',1),('00b92f80',3),('00b92e50',3)]:
 for i in range(1,total+1):
  p=r/'luna236_whole/attempts'/a/f'grade{i:02d}';sha=(p/'input.sha256').read_text().split()[0];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==sha;cmd=(p/'command.txt').read_text();assert '--flags'not in cmd;records.append(dict(address=a,source=str(p/'source.cpp'),sha256=sha,command=cmd,effective_flags=['/O2','/Oy','/W3'],log=str(p/'stdout.txt'),status=(p/'stdout.txt').read_text().splitlines()[0]))
assert len(records)==9;(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
assert(o/'dry.log').read_text().count('RELOCATION_MATCH PASS')==5
print('PASS19relocations/255fullVTsignatures/7nativeRTTIhierarchies/9immutablegrades/originalfullboundaryandconstructorproof')

assert(o/'dry_extended.log').read_text().count('RELOCATION_MATCH PASS')==5
for q in json.loads((o/'fixture_extensions.json').read_text()):
 assert hashlib.sha256(Path(q['fixture']).read_bytes()).hexdigest()==q['sha256']
 assert hashlib.sha256(Path(q['original_fixture']).read_bytes()).hexdigest()==q['original_sha256']
print('PASS extended scene-tree fixtures in VC7.1, unchanged production and attempt counts')
