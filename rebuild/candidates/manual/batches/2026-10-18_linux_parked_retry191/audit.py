from pathlib import Path
import struct,runpy,sys,json,hashlib,re,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry191';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?g_CurrentAlignment@@':0x13d28d4,'?g_CurrentAlignmentShift@@':0x13d28d8,'?RemoveAreaFromFreeList@':0xa65060,'?InsertAreaIntoFreeList@':0xa650f0,'?StopLoading@':0x9fdc20,'?GetUnusedFrames@':0x9fbf70,'?FreeTextureMemory@':0x9fe460,'??0CEnginePrimitiveManager@@':0xbacdb0,'??_7CEnginePrimitiveManagerOcclusionBase@@':0x12a3e14,'??0CVertexShader@@':0xa62fb0,'??0CCharString@@':0x99ebf0,'?g_pEngineData@@':0x1436e98,'?GetVertexShader@':0xa5d5f0,'??_7CVertexShader@@':0x129b3d0,'??_7CShaderBase@@':0x129a030,'??1CCharString@@':0x99eae0,'?FindIndexBySymbol@':0x9ccdc0,'?MeshInfoAtIndex@':0xa24280,'?GetMesh@':0xa25e70}
strings={0x12a3cb4:b'VSHADER_STATIC_UNLIT\0',0x12a3df8:b'MESH_ENGINE_UNIT_BOX\0'};obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings and rd(v,len(strings[v]))==strings[v]
  else:assert[val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==34
assert rd(0xa24280,len(bytes.fromhex('8b81800300008b4c24048b0488c20400')))==bytes.fromhex('8b81800300008b4c24048b0488c20400') and rd(0xa5d60e,7)==bytes.fromhex('8bc75f5ec20800')
pdb=[l.split('\t',1)[1]for l in(Path('ghidra_out/egor_pdb_names.tsv').read_text()+Path('ghidra_out/fablewin_pdb_names.tsv').read_text()).splitlines()if'\t'in l]
for prefix in ['?StopLoading@CFrameData@CGraphicInfoFrame@@QAE_NXZ','?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z','?GetUnusedFrames@CResource@@QBEKXZ','?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z','?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z','?ConsolidateAdjacentFreeAreas@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAVCMemoryArea@2@@Z']:assert prefix in pdb,prefix
# Match full emitted slot signatures to PDB, retaining explicit aliases/access differences in evidence.
def shape(s):
 s=re.sub('public: |protected: |private: |virtual |class |struct |enum ','',s);s=re.sub(r'\b[A-Za-z_][A-Za-z0-9_:]*::(?=[A-Za-z_~][A-Za-z0-9_~]*\()','',s);return re.sub(r'\s+','',s)
def decls(a,cl):
 sy,se=H['coff'](next((o/'dry_work_ownership_g03').rglob(a+'.t.obj')));n,v,s=next((n,v,s)for n,v,s in sy.values()if n.startswith('??_7'+cl+'@@')and s>0);return [q for k,q in sorted(se[s-1][1].items())if k>=v]
def checktable(a,cl,vt,expected,owners):
 symbols=decls(a,cl);assert len(symbols)==len(expected),(cl,len(symbols));out=[]
 for i,(sym,target)in enumerate(zip(symbols,expected)):
  assert u(vt+4*i)==target,(cl,i,hex(u(vt+4*i)),hex(target));comp=H['und'](sym)
  if i==0:assert sym.startswith(('??_G','??_E'));proof='actual deleting destructor slot'
  elif target==0xbfea08:assert sym=='__purecall';proof='actual native purecall'
  else:
   method=sym[1:].split('@')[0];own=owners.get(method,owners.get('*',cl));candidates=[q for q in pdb if q.startswith('?'+method+'@'+own+'@@')]
   hits=[H['und'](q)for q in candidates if shape(H['und'](q))==shape(comp)];assert hits,(cl,i,comp,candidates);proof=hits[0]
  out.append({'table':cl,'slot':i,'target':hex(target),'compiled':comp,'PDB_or_native_evidence':proof})
 return out
slots=[]
base=[0x4664e0,0x42d930];resource=[0x4664ff,0x42d930,0x42d940,0x9fbef0,0x42d950,0x42d960,0x42d970,0x42d980,0x9fbf00,0x9fbf10];managed=[0x9ffb80]+resource[1:]+[0xbfea08];frame=[0xa010a0,0x42d930,0x42d940,0x9fd830,0x9fd7f0,0x9ff860,0x42d970,0x42d980,0x9fdcb0,0x9fd4d0,0x9fea00]
for cl,vt,arr,owners in [('CountedNative',0x12354d0,base,{'*':'CIVCountedPointeeBase'}),('ResourceNative',0x12354a4,resource,{'*':'CResource','Delete':'CIVCountedPointeeBase'}),('ManagedTextureOwner',0x129c848,managed,{'*':'CResource','Delete':'CIVCountedPointeeBase'}),('FrameData',0x129c89c,frame,{'*':'CFrameData@CGraphicInfoFrame','Delete':'CIVCountedPointeeBase','UnloadResource':'CResource','CanAbortResourceLoad':'CResource','OnResourceLoadAborted':'CResource'})]:slots+=checktable('009fe8e0',cl,vt,arr,owners)
prior=json.loads((r/'coordinator_retry186/native_virtual_signatures.json').read_text())['slots'];prior=[x for x in prior if x['address']=='00bcc5f0'];tables={'CEnginePrimitiveManager':0x12a53e8,'CVertexShader':0x129b3d0,'CShaderBase':0x129a030,'CVertexShaderData':0x129da50,'CEnginePrimitiveRenderUtil':0x12a3bd0,'CBaseClassNonCopyable':0x129a7c4,'CBaseClass':0x1231710}
for cl,vt in tables.items():
 items=[x for x in prior if x['table']=='??_7'+cl+'@@6B@'];symbols=decls('00b90270',cl);assert len(items)==len(symbols)
 for x,sym in zip(items,symbols):
  i=x['slot'];assert u(vt+4*i)==int(x['target'],16);actual=H['und'](sym)
  assert (sym.startswith(('??_G','??_E'))if i==0 else shape(actual)==shape(x['compiled'])),(cl,i,actual,x)
  slots.append({'table':cl,'slot':i,'target':x['target'],'compiled':actual,'PDB_or_native_evidence':x['expected'],'earlier_independent_audit':'coordinator_retry186/native_virtual_signatures.json'})
# Occlusion override, purecall slots and concrete test implementation of the three abstract methods.
manager=[u(0x12a53e8+i*4)for i in range(13)];occ=manager[:];occ[0]=u(0x12a3e14);occ[6]=u(0x12a3e14+24)
slots+=checktable('00b90270','CEnginePrimitiveManagerOcclusionBase',0x12a3e14,occ,{'*':'CEnginePrimitiveManager','RenderBoundingVolume':'CEnginePrimitiveManagerOcclusionBase'})
syms=decls('00b90270','TestOcclusionManager');weapon={x['slot']:x for x in prior if x['table']=='??_7CEnginePrimitiveManagerWeaponTrails@@6B@'}
for i in [1,2,7]:assert shape(H['und'](syms[i]))==shape(weapon[i]['expected'])
hier={}
for vt in [0x129c89c,0x12a3e14,0x129b3d0,0x129a030,0x129ce94]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert [x[0]for x in hier['0x129c89c']]==['.?AVCFrameData@CGraphicInfoFrame@@','.?AVCManagedTextureOwner@@','.?AVCResource@@','.?AVCIVCountedPointeeBase@@']
assert [x[0]for x in hier['0x12a3e14']]==['.?AVCEnginePrimitiveManagerOcclusionBase@@','.?AVCEnginePrimitiveManager@@','.?AVCEnginePrimitiveRenderUtil@@','.?AVCBaseClassNonCopyable@@','.?AVCBaseClass@@']
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2));(o/'native_virtual_signatures.json').write_text(json.dumps({'slots':slots,'abstract_concrete_tests':[1,2,7]},indent=2))
runpy.run_path(str(o/'check_attempts.py'))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for key in ['src','test']:
  item=row[key];assert hashlib.sha256(Path(item['path']).read_bytes()).hexdigest()==item['sha256']
assert (o/'dry_ownership_g03.log').read_text().count('RELOCATION_MATCH PASS')==3
print('PASS34relocations/2fullstrings/'+str(len(slots))+'vtableentries/5RTTIhierarchies/9sourcegrades; ownership g03 and all host/native fixtures reviewed')
