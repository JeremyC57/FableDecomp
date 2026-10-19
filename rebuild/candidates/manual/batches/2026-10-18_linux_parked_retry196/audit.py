from pathlib import Path
import struct,runpy,sys,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry196';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?g_ResourceManager':0x1436e98,'?GetVertexBufferUsage':0xb3b030,'?Initialise@CVertexBufferWin32_Retail':0xa63150,'?Lock@CVertexBufferWin32_Retail':0xac20a0,'?Unlock@CVertexBufferWin32_Retail':0xac20b0,'?GetIndexBufferMemPool':0xb3b0a0,'?GetIndexBufferUsage':0xb3b060,'?Initialise@CIndexBuffer_Methods':0xac1f70,'?GetSize@CIndexBuffer_Methods':0xac1f10,'?Lock@CIndexBuffer_Methods':0xac1fb0,'?Unlock@CIndexBuffer_Methods':0xac1f00}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert[val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==26
assert rd(0xb3b050,1)==rd(0xb3b080,1)==b'\xc3' and rd(0xb3b0a0,6)==bytes.fromhex('b801000000c3')
assert rd(0xac20a0,5)==bytes.fromhex('8b01ff6004') and rd(0xac20b0,5)==bytes.fromhex('8b01ff6008')
for call,target in [(0xbc2069,0xbbf380),(0xbc52c0,0xbc3e10)]:assert rd(call,1)==b'\xe8' and call+5+struct.unpack('<i',rd(call+1,4))[0]==target
identities=[]
for vt,owner,start,end in [(0x12a6140,'CEnginePrimitiveManagerParticleSpriteGroup',0xbc14e0,0xbc2079),(0x12a6590,'CEnginePrimitiveManagerParticleLineGroup',0xbc4d90,0xbc52d0)]:
 col=u(vt-4);name=rd(u(col+12)+8,160).split(b'\0')[0].decode();assert name=='.?AV'+owner+'@@';assert struct.pack('<I',vt)in rd(start,end-start);identities.append(dict(table=hex(vt),owner=name,constructor=hex(start)))
(o/'native_hierarchy_audit.json').write_text(json.dumps(identities,indent=2))
pdb={}
for fn in ['ghidra_out/egor_pdb_names.tsv','ghidra_out/fablewin_pdb_names.tsv']:
 for i,l in enumerate(Path(fn).read_text().splitlines(),1):
  if '\t'in l:pdb.setdefault(l.split('\t',1)[1],[]).append([fn,i,l.split('\t')[0]])
originals=['?GetVertexBufferUsage@CEngineResourceManager@@QAEKXZ','?GetIndexBufferUsage@CEngineResourceManager@@QAEKXZ','?GetIndexBufferMemPool@CEngineResourceManager@@QAE?AW4EIndexBufferPool@@XZ','?BuildBuffers@CEnginePrimitiveManagerParticleSpriteGroup@@AAEXXZ','?BuildBuffers@CEnginePrimitiveManagerParticleLineGroup@@AAEXXZ','?Unlock@CIndexBuffer@@QAEXXZ','?GetSize@CIndexBuffer@@QBEJXZ','?Initialise@CIndexBuffer@@QAEXKKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z','?Lock@CIndexBuffer@@QAEPAEKKK@Z','?DoUnlock@CVertexBufferWin32@@EAEXXZ','?Initialise@CVertexBufferWin32@@QAE_NW4EVertexType@@JJKW4EVertexBufferPool@@@Z','?DoLock@CVertexBufferWin32@@EAEPAEKKK@Z']
for s in originals:assert s in pdb,s
(o/'original_signatures.json').write_text(json.dumps([dict(symbol=s,undecorated=H['und'](s),PDB_records=pdb[s])for s in originals],indent=2))
# Original wrappers Lock/Unlock are unnamed/inlined in the donor exports; their entire native thunks and DoLock/DoUnlock signatures establish the external boundary.
for a in obs:
 sy,se=H['coff'](next((o/'dry_work').rglob(a+'.t.obj')));assert any(n=='?GetSize@CIndexBuffer_Methods@@QBEJXZ'and s>0 for n,v,s in sy.values())
fields=[l.split('\t')for l in Path('ghidra_out/struct_layouts_egor.tsv').read_text().splitlines()if l.startswith(('CEnginePrimitiveManagerParticleSpriteGroup\t','CEnginePrimitiveManagerParticleLineGroup\t'))]
assert len(fields)==26
(o/'original_member_names.json').write_text(json.dumps(fields,indent=2))
records=[]
for a,total in [('00bbf380',4),('00bc3e10',1)]:
 for i in range(1,total+1):
  p=r/'luna236_whole/attempts'/a/f'grade{i:02d}';sha=(p/'input.sha256').read_text().split()[0];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==sha;cmd=(p/'command.txt').read_text();assert '--flags'not in cmd;records.append(dict(address=a,source=str(p/'source.cpp'),sha256=sha,command=cmd,effective_flags=['/O2','/Oy','/W3'],log=str(p/'stdout.txt'),status=(p/'stdout.txt').read_text().splitlines()[0]))
for q in json.loads((o/'root_amendments/grades.json').read_text()):
 assert hashlib.sha256(Path(q['source']).read_bytes()).hexdigest()==q['sha256'];assert 'RELOCATION_MATCH'in(o/'root_amendments'/(q['address']+'.log')).read_text();records.append(q)
assert len(records)==7;(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for fn in ['author_frozen_hashes.json','frozen_hashes.json']:
 for row in json.loads((o/fn).read_text()):
  for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
assert(o/'dry.log').read_text().count('RELOCATION_MATCH PASS')==2
print('PASS26relocations/12originalsignatures/2nativeownerRTTI+constructorcalls/26layoutrecords/7immutablegrades/exactwrapperthunks')
