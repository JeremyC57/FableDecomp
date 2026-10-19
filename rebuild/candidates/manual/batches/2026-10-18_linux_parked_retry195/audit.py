from pathlib import Path
import struct,runpy,sys,json,hashlib,re,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry195';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?OnActivate@ModeStandNative':0x8621a0,'?GetSnowTroll@NativeDefinition':0x8c7a70,'?SuspendAI@ThingNative':0x833a10,'?GetPos@ThingNative':0x4c73d0,'?GetPItem@BaseIntelligentPointer':0xa01b50,'?GetWorld@ModeIceTroll':0x73a680,'?GetValidPositions@@':0x74a5d0,'??_C@_05MEANFIJN@STAND':0x12674dc,'??0CCharString':0x99ebf0,'??4CCharString':0x99efb0,'?EmptyNarrowString':0x129aaf4,'?CalculateCRC':0x4014a0,'??1CCharString':0x99eae0,'?OnActivate@BaseModeNative':0x73aac0,'?GetStrafingAnimationName':0x8e8ff0,'?GetAnimDistanceAndIndexFromAnimName':0x8e9080,'?LowerBound@ComponentMap':0x40f020,'?GetAnimation@AnimationComponentMethods':0x70b460,'?GetFirstCombinationAnimFromType':0x70c4e0,'?RemoveCombinationAnimsOfType':0x70d8c0,'?InitialiseMovementSpeeds':0x6a9a00}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert[val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==30
assert rd(0x12674dc,6)==b'STAND\0' and rd(0x129aaf4,1)==b'\0'
assert rd(0x73a680,9)==bytes.fromhex('a11c8a3b018b4024c3')
assert u(0x8c7a78)==0x123565c and rd(0x123565c,14)==b'CSnowTrollDef\0'
assert rd(0x70b460,4)==bytes.fromhex('8b410cc3')
assert rd(0x6a9a06,6)==bytes.fromhex('8b86e0000000')
pdb={}
for fn in ['ghidra_out/egor_pdb_names.tsv','ghidra_out/fablewin_pdb_names.tsv']:
 for i,l in enumerate(Path(fn).read_text().splitlines(),1):
  if '\t'in l:pdb.setdefault(l.split('\t',1)[1],[]).append([fn,i,l.split('\t')[0]])
originals=['?GetPos@CThing@@QBEABVC3DVector@@XZ','?InitialiseMovementSpeeds@CThingPlayerCreature@@QAEXXZ','?SuspendAI@CThingAICreature@@QAEX_NPAX@Z','?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z','?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z','?GetAnimation@CTCAnimationComplex@@QAEAAVCAnimateThing@@XZ','?GetValidPositions@NCreatureHiding@@YIXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@ABVCWorldMap@@ABVCThingSearchTools@@ABVC3DVector@@M_NPBVCTCShapeManager@@@Z','?OnActivate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXXZ','??$GetSubDef@VCSnowTrollDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSnowTrollDef@@@@@Z','?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z','?GetAnimDistanceAndIndexFromAnimName@NCreatureMode@@YI_NABVCThingCreatureBase@@ABVCCharString@@AAJAAM@Z','?OnActivate@CModeStrafe@NCreatureMode@@UAEXXZ','?FrameUpdate@CModeStrafe@NCreatureMode@@UAEXXZ','?Delete@CIVCountedPointeeBase@@UBEXXZ']
for s in originals:assert s in pdb,s
(o/'original_signatures.json').write_text(json.dumps([dict(symbol=s,undecorated=H['und'](s),PDB_records=pdb[s])for s in originals],indent=2))
for vt,address,name in [(0x1268774,0x8d7630,b'.?AVCModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@'),(0x1268588,0x8e9a30,b'.?AVCModeStrafe@NCreatureMode@@')]:
 assert u(vt+20)==address and rd(u(u(vt-4)+12)+8,len(name)+1)==name+b'\0'
assert u(0x1268588+12)==0x8ea220
sy,se=H['coff'](next((o/'dry_work').rglob('008d7630.t.obj')))
world=[0x4ae8d0,0x49d970,0x49e220,0x49e1b0,0x49e1c0,0x49da00,0x49da60,0x4a6550,0x49dbe0,0x4a6e30,0x4a2e90,0x4a1cb0,0x4ae8b0,0x4ae8c0]
assert[u(0x12390f0+i*4)for i in range(14)]==world
n,v,s=next((n,v,s)for n,v,s in sy.values()if n=='??_7WorldNative@@6B@'and s>0);size,rels=se[s-1];slots=[rels[k]for k in range(v,size,4)if k in rels];assert len(slots)==14
virtuals=[]
def norm(s):return re.sub(r'^(public|private|protected): ', '',re.sub(r'\b(class|struct) ', '',s)).replace('WorldNative','CWorld').replace('CWorldBase::','CWorld::')
for i,sym in enumerate(slots):
 if i==0:
  assert sym.startswith(('??_G','??_E'));matches=['??1CWorld@@UAE@XZ'];assert matches[0]in pdb
 else:
  matches=[s for s in pdb if ('@CWorld@@'in s or '@CWorldBase@@'in s) and norm(H['und'](s))==norm(H['und'](sym))];assert matches,(i,H['und'](sym))
 virtuals.append(dict(slot=i,native_target=hex(world[i]),compiled_symbol=sym,compiled_signature=H['und'](sym),original_symbols=matches))
(o/'world_virtual_signatures.json').write_text(json.dumps(virtuals,indent=2))
hier={}
for vt,cl in [(0x129c95c,'BaseIntelligentPointer'),(0x1238c6c,'IntelligentThingPointer')]:
 n,v,s=next((n,v,s)for n,v,s in sy.values()if n=='??_7'+cl+'@@6B@'and s>0);size,rel=se[s-1];ss=[rel[k]for k in range(v,size,4)if k in rel];assert len(ss)==1 and ss[0].startswith(('??_G','??_E'))
 ch=u(u(vt-4)+16);items=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);items.append([rd(u(bc)+8,180).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]={'hierarchy':items,'emitted_slots':[H['und'](s)for s in ss],'native_destructor':hex(u(vt))}
assert hier['0x1238c6c']['hierarchy']==[['.?AV?$CIntelligentPointer@VCThing@@@@',0],['.?AVCBaseIntelligentPointer@@',0]]
assert u(0x1238c6c)==0x4a9710
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2))
records=[json.loads(l)for l in(r/'astra241_whole/ATTEMPTS.jsonl').read_text().splitlines()];records=[q for q in records if q['address']in obs and q['kind']=='grade'];assert collections.Counter(q['address']for q in records)=={'008d7630':3,'008e9a30':2}
for q in records:
 p=Path(q['snapshot']);assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==q['sha256'];assert q['command']and q['base_flags']==['/O2','/Oy','/W3']and not q['extra_flags'];assert(p/'stdout.txt').exists()and(p/'stderr.txt').exists()
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
assert(o/'dry.log').read_text().count('RELOCATION_MATCH PASS')==2
print('PASS30relocations/14originalsignatures/14WorldVTsignatures/2nativeownerRTTI/2pointerVT+RTTI/5sourcegrades/nativehelpers')
