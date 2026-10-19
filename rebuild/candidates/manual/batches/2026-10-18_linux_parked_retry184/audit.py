from pathlib import Path
import struct,runpy,sys,json,hashlib,re,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry184';w=r/'astra242_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?AllocateAndCopy':0x4e2aab,'?Clear@':0x4e2aec,'?AccurateNormalise':0xa14510,'__real@38d1b717':0x129ba3c,'?GetSize@':0xa75b30,'__ehhandler$':0x1216f53,'__except_list':0,'??0CUnknown':0xcacb80,'??0CMediaType':0xcb1d00,'__real@3ff0000000000000':0x1237f00,'__imp__lstrlenW@4':0x143fe48,'??2@':0xbfea1a}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==16
assert rd(0x129ba3c,4)==struct.pack('<f',0.0001) and rd(0x1237f00,8)==struct.pack('<d',1)
assert rd(0xa14521,2)==bytes.fromhex('d9fa') # accurate sqrt, not approximate NormaliseA14540
assert rd(0x4e29a9,7)==bytes.fromhex('83c728578d4e28')
assert rd(0x1216f53,5)==bytes.fromhex('b884163701') and 0x1216f5d+struct.unpack('<i',rd(0x1216f59,4))[0]==0xbfeb34
assert [u(0x1371684+i*4)for i in range(3)]==[0x19930520,2,0x1371674]
assert [u(0x1371674+i*4)for i in range(4)]==[0xffffffff,0x1216f40,0,0x1216f48]
assert rd(0x1216f40,3)==bytes.fromhex('8b4df0') and 0x1216f48+struct.unpack('<i',rd(0x1216f44,4))[0]==0xca7780
assert rd(0x1216f48,6)==bytes.fromhex('8b4df083c134') and 0x1216f53+struct.unpack('<i',rd(0x1216f4f,4))[0]==0xcb1cc0
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');imports={x.address:(d.dll,x.name)for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports};assert imports[0x143fe48]==(b'KERNEL32.dll',b'lstrlenW')
pdb=Path('ghidra_out/fablewin_pdb_names.tsv').read_text()
for s in ['?AccurateNormalise@C2DVector@@QAEAAV1@XZ','?GetSize@CANavQuadTreeNode@@QBEMXZ','?IsAnyPartOfNodeWithinDistOfPos@CNavigatorBase@@IBE_NPBVCANavQuadTreeNode@@MABVC3DVector@@AAVC2DVector@@@Z']:assert s in pdb
# Emitted SDK interface signatures/order versus previously reviewed175 real DirectShow hierarchy.
# Override owner names differ, signatures and adjustor{4} must remain identical.
def tables(p):
 sy,se=H['coff'](p);out={}
 for n,v,s in sy.values():
  if n.startswith('??_7FixturePin@@') and s>0:out[n]=[H['und'](se[s-1][1][k])for k in range(v,se[s-1][0],4)if k in se[s-1][1]]
 return out
prior=tables(next((r/'coordinator_retry175/dry_work').rglob('00cac300.t.obj')));current=tables(next((o/'dry_work').rglob('00cab370.t.obj')))
def norm(s):return re.sub('CDynamicOutputPin|CBaseOutputPin|CBasePin|FixturePin','SDKOverrideOwner',s)
slots=[]
for table,actual in current.items():
 assert table in prior
 for i,s in enumerate(actual):assert norm(s)==norm(prior[table][i]),(table,i,s,prior[table][i]);slots.append([table,i,s,prior[table][i]])
assert len(slots)==37
(o/'sdk_virtual_signatures.json').write_text(json.dumps({'slots':slots,'scope':'37 emitted FixturePin slots checked against reviewed175 SDK hierarchy prefixes; owner-only override normalization; std::bad_alloc belongs to compiler library. No production native vtable emitted (SDK abstract novtable).'},indent=2))
records=[]
for a,count in [('004e29ba',3),('00a471d0',8),('00cab370',4)]:
 snapshots=sorted((w/'snapshots'/a).glob('a[0-9][0-9]'));assert len(snapshots)==count
 for p in snapshots:
  x=json.loads((p/'manifest.json').read_text());assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['command'] and x['base_flags']==['/O2','/Oy','/W3'];assert(p/'stdout.txt').exists();records.append(x)
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS16relocations/twofloatpools/2EHcleanupentries/lstrlenW/37SDKprefixsignatures/15immutablegrades')
