from pathlib import Path
import struct,runpy,sys,json,hashlib,collections,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry182';w=r/'astra240_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'??1QuadTreeElement':0xbddfb0,'?gEngine':0x1436ea8,'?Deallocate@LandscapeMemory':0xbd9b90,'??_7Patch@Native':0x12a803c,'?gRenderer':0x1436ea8,'?DeleteVertexBuffer':0xbda0d0,'?DeleteIndexBuffer':0xbda090,'?DeleteTexture':0xbda110,'?UnregisterSubPatchForZSorting':0xb677a0,'??1EdgeStrip':0xbfc410,'??_7Movable@Native':0x129b860,'__real@3f800000':0x122ded8,'__real@3ff0000000000000':0x1237f00,'__CIfmod':0xbfeb28,'?GetEmbeddedEventsBetweenTimes':0x6f9820,'?GetExtraEventsBetweenTimes':0x6f95a0}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==28
assert u(0x122ded8)==0x3f800000 and rd(0x1237f00,8)==struct.pack('<d',1.0)
assert rd(0xbfeb28,6)==bytes.fromhex('ff2518014401')
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');imports={x.address:(d.dll,x.name)for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports};assert imports[0x1440118]==(b'MSVCR71.dll',b'_CIfmod')
assert rd(0xbdfa33,3)==bytes.fromhex('c20400')and rd(0xbdfa36,10)==b'\xcc'*10
hier={}
for vt in [0x129b860,0x12a8014,0x12a803c,0x12a8228,0x12a8240,0x12a8334,0x12a7df0]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert hier['0x12a7df0']==[['.?AVCQuadTreeElement@CLocalDetailCacheMap@@',0],['.?AVCMovableResource@@',0]]
assert len(hier['0x12a803c'])==3 and len(hier['0x12a8240'])==3 and len(hier['0x12a8334'])==2
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2))
sig=json.loads((o/'native_virtual_signatures.json').read_text());assert len(sig['slots'])==57 and not sig['mismatches']
pdb=Path('ghidra_out/fablewin_pdb_names.tsv').read_text()
for s in ['??0CAddMeshDesc@NEnginePrimitiveManagerRepeatedStaticMeshes@@QAE@PBVC3DMeshInfo@@JPAVC4DVector@@1PBMPBUCSubsectionElement@1@PBEMABVC3DBoundingBox@@ABVC3DBoundingSphere@@MABVCEngineFadeDistance@@KJM_NK@Z','??0CEngineFadeDistance@@QAE@MM@Z','?UnregisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJ@Z','?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z','?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z']:assert s in pdb
records=[json.loads(l)for l in(w/'ATTEMPTS.jsonl').read_text().splitlines()];records=[x for x in records if x['address']in obs]
assert collections.Counter(x['address']for x in records)=={'00bdfa10':5,'00be6090':2,'00bf67f0':4,'00bf9bc0':4,'006f99b0':5}
for x in records:
 p=w/'snapshots'/(x['address']+'_'+str(x['attempt'])+'.cpp');assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['command'] and x['effective_flags']==['/O2','/Oy','/W3'];assert(w/'logs'/(x['address']+'_'+str(x['attempt'])+'.log')).exists()
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS28relocations/float+double/CRTimport/7RTTIhierarchies/57native-emittedsignatures/20immutablegrades')
