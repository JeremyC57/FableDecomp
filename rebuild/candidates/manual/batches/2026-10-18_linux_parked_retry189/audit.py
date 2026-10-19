from pathlib import Path
import struct,runpy,sys,json,hashlib,re,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry189';w=r/'astra239_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'??0DamageMap@@':0x4f97b3,'??0AugmentationVector@@':0x811d40,'??0CBaseIntelligentPointer@@':0xa01b10,'?GetPItem@CBaseIntelligentPointer@@':0xa01b50,'?SetPItem@CBaseIntelligentPointer@@':0xa01b90,'??_7ThingPointer@@':0x1238c6c,'??0CCharString@@QAE@XZ':0x99e4b0,'??0CCharString@@QAE@ABU0@@Z':0x99ec30,'?Sprintf@CCharString@@':0x99f1f0,'??YCCharString@@':0x99f0a0,'??8BasicString@@':0x4115a0,'??4CCharString@@':0x99efe0,'?InsertOverflow@ObjectiveVector@@':0x47c030,'??1CCharString@@':0x99eae0,'?ReadWithSrcChunkOverflow@':0x993ca0,'?Resize@LongArray@@':0x4428f0,'?Sizeof@BankFileData@@':0x9d5620,'?SetResourceSize@':0x9fbf20,'_malloc':0xbfea0e,'_free':0xbfea14,'?WriteSLONG@':0x993eb0,'?TransferBinaryOut@':0x6db0f0,'?Erase@InnerMap@@':0x4da252}
strings={0x122d70e:b'\0',0x12366b4:b'%d-\0',0x1236688:b'This quest objective has no description!\0'}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings and rd(v,len(strings[v]))==strings[v]
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==33
pdb=[l.split('\t',1)[1]for l in(Path('ghidra_out/egor_pdb_names.tsv').read_text()+Path('ghidra_out/fablewin_pdb_names.tsv').read_text()).splitlines()if'\t'in l]
parts=['??0CHitParameters@@QAE@ABV0@@Z','?AddObjectiveToQuest@CHeroLogBook@@QAE_NKKABVCCharString@@@Z','?Sprintf@CCharString@@QAAXPBDZZ','??YCCharString@@QAEABV0@ABV0@@Z','??4CCharString@@QAEABV0@PBD@Z','?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z','?SetResourceSize@CResource@@QAEXK@Z','?WriteSLONG@CDataOutputStream@@QAEXJ@Z','?TransferBinaryOut@?$CPersistTraits@VCHeroStatIncreaseData@@@@QAEXAAVCDataOutputStream@@ABVCHeroStatIncreaseData@@@Z']
assert all(p in pdb for p in parts);template=next(x for x in pdb if x.startswith('??$GFSerialiseMapBinaryOut@VCHeroStatIncreaseData@@W4EDamageAttribute@@'))
assert 'YIXAAVCDataOutputStream@@ABV' in template
assert 'private:void ReadWithSrcChunkOverflow' in(o/'root_amendments/src/009c8470.cpp').read_text()
# Root verifies emitted pointer vtables and actual native two-class RTTI, reused in188 independently.
hier={}
for vt in [0x1238c6c,0x129c95c]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert hier['0x1238c6c']==[['.?AV?$CIntelligentPointer@VCThing@@@@',0],['.?AVCBaseIntelligentPointer@@',0]] and u(0x1238c6c)==0x4a9710
sy,se=H['coff'](next((o/'dry_work_final').rglob('00812170.t.obj')));tables=[]
for cl in ['ThingPointer','CBaseIntelligentPointer']:
 n,v,s=next((n,v,s)for n,v,s in sy.values()if n.startswith('??_7'+cl+'@@'))
 assert se[s-1][0]==4 and list(se[s-1][1])==[0];assert se[s-1][1][0].startswith(('??_G'+cl,'??_E'+cl));tables.append(H['und'](se[s-1][1][0]))
# Real format ABI is cdecl member with varargs; verify final emitted symbol exactly.
sy,se=H['coff'](next((o/'dry_work_final').rglob('0047a8e0.t.obj')));assert any(n=='?Sprintf@CCharString@@QAAXPBDZZ'for n,v,s in sy.values())
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2));(o/'native_signature_audit.json').write_text(json.dumps({'pointer_tables':tables,'PDB_direct_signatures':[H['und'](p)for p in parts],'actual_template':H['und'](template),'format':'member cdecl %d of native32bit ulong bit pattern; fixture high IDs80000000 andffffffff verified'},indent=2))
records=[x for x in json.loads((w/'SOURCE_ATTEMPTS.json').read_text())if x['address']in obs and x['counted_source_grade']]
assert collections.Counter(x['address']for x in records)=={'00812170':3,'0047a8e0':8,'009c8470':4,'006dc450':2}
for x in records:assert hashlib.sha256(Path(x['source_path']).read_bytes()).hexdigest()==x['source_sha256'] and x['command'] and x['effective_flags']
root=json.loads((o/'root_amendments/grade.json').read_text());assert root['cumulative']==5 and hashlib.sha256(Path(root['source']).read_bytes()).hexdigest()==root['sha256'] and 'RELOCATION_MATCH' in(o/'root_amendments/grade.log').read_text()
(o/'attempt_audit.json').write_text(json.dumps({'author':records,'root':[root],'fixture_only':'0047a8e0 native signed %d high IDs, no production change'},indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS33relocations/3fullstrings/10PDBsignatures/2pointertables/2RTTIhierarchies/17author+1rootgrade')
