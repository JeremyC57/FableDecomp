from pathlib import Path
import struct,runpy,sys,json,hashlib,collections,re
r=Path('work/codex-lane/w22');o=r/'coordinator_retry180';w=r/'astra242_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?PackedBitMasks':0x13979e8,'?AllocateArrays':0xadf2a0,'?ReadWithSrcChunkOverflow':0x993ca0,'??4CParentDefClassBase':0x431ed0,'??4CDefStringVector':0x46078c,'??4CFloatVector':0x45ef2d,'??4OpinionOffsetList':0x471b73,'??4ReactionVector':0x471cc5,'??4WarningVector':0x432ee9}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32' else u(int(a,16)+k);assert v==int(target,16)
  assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==22
assert [u(0x13979e8+i*4)for i in range(33)]==[(1<<i)-1 for i in range(33)]
assert rd(0x460652,5)==bytes.fromhex('e900000000')and rd(0x471a72,5)==bytes.fromhex('e900000000')
assert rd(0x471ce5,3)==bytes.fromhex('6a505f')and rd(0x471ba5,2)==bytes.fromhex('6a06')
assert rd(0x995f29,6)==bytes.fromhex('c7065c672301')
assert json.loads((o/'native_hierarchy_audit.json').read_text())[:2]==[['.?AVCMemoryDataInputStream@@',0],['.?AVCDataInputStream@@',0]]
h=runpy.run_path(str(o/'coff_helpers.py'));signatures={}
for a,prefix in [('00a62b70','?CountBits@'),('00ae1030','?SetParticles@'),('00bde190','?LoadHeader@')]:
 sy,secs=h['coff'](next((o/'dry_work').rglob(a+'.obj')));s=[n for n,v,sec in sy.values()if sec>0 and n.startswith(prefix)];assert len(s)==1;signatures[a]=h['und'](s[0])
assert 'void const *' in signatures['00a62b70'] and signatures['00a62b70'].endswith('const')
assert 'CMovableResourceMemoryPool &' in signatures['00ae1030']
assert 'CLocalDetailCacheMap::CObjectCacheGroupCollection' in signatures['00bde190'] and 'CMemoryDataInputStream &'in signatures['00bde190']
pdb=Path('ghidra_out/fablewin_pdb_names.tsv').read_text()
for sig in ['?CountBits@CPackedUIntArray@@ABEKPBXKJK@Z','?AllocateArrays@CParticleSimulation@@AAE_NAAVCMovableResourceMemoryPool@@@Z','??0C4DVector@@QAE@ABVC3DVector@@M@Z','??0C4DVector@@QAE@XZ','??0C3DBoundingSphere@@QAE@XZ','?LoadHeader@CObjectCacheGroupCollection@CLocalDetailCacheMap@@QAEXAAVCMemoryDataInputStream@@@Z']:assert sig in pdb
records=[];expected={'00a62b70':9,'00ae1030':4,'00bde190':4,'00460657':1,'00471a77':1}
for a,count in expected.items():
 ps=sorted((w/'snapshots'/a).glob('a[0-9][0-9]/manifest.json'));assert len(ps)==count
 for p in ps:
  x=json.loads(p.read_text());assert hashlib.sha256((p.parent/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['base_flags']==['/O2','/Oy','/W3'];assert(p.parent/'stdout.txt').exists()and(p.parent/'stderr.txt').exists();x['snapshot']=str(p.parent);records.append(x)
assert len(records)==19
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));(o/'native_signature_audit.json').write_text(json.dumps(signatures,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS22relocations/33masks/true const+reference signatures/real math constructors/streamRTTI/19immutablegrades')
