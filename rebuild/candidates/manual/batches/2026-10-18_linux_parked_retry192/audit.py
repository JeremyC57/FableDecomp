from pathlib import Path
import struct,runpy,sys,json,hashlib,re,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry192';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?OnDeactivate@ModeStandNative':0x861d00,'?SuspendAI@ThingNative':0x833a10,'?LowerBound@ComponentMap':0x40f020,'?GetBriar@NativeDefinition':0x8e7790,'?GetWorld@ModeBriar':0x73a680,'?TransitionToDefaultThemeAllChannels@EnvironmentMethods':0x6b3950,'??0HitParameters':0x7a3bc0,'?AddDamageWithAttribute@HitParameters':0x868ca0,'?SetThingHitting@HitParameters':0x8685f0,'?GetPItem@BaseIntelligentPointer':0xa01b50,'?RemoveTC@ThingNative':0x4c9840,'?Kill@ThingNative':0x4c9b80,'??1HitParameters':0x789358,'?BaseChangeState@CMenuEntry':0x52cf40}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert[val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==26
assert rd(0x73a680,9)==bytes.fromhex('a11c8a3b018b4024c3')
assert u(0x8e7798)==0x123562c and rd(0x123562c,14)==b'CBriarRoseDef\0'
# Native hit constructor's packed defaults, real intelligent-pointer fields and matching destructor cleanup.
assert rd(0x7a3bc9,5)==bytes.fromhex('881e885e01')
for at,target in [(0x7a3c39,0xa01b10),(0x7a3c49,0xa01b10),(0x78935e,0xa01c10),(0x789366,0xa01c10)]:assert rd(at,1)==b'\xe8' and at+5+struct.unpack('<i',rd(at+1,4))[0]==target
assert rd(0x7a3c2b,3)==bytes.fromhex('8d7e50')and rd(0x7a3c44,3)==bytes.fromhex('8d7e58')
assert u(0x7a3c40)==u(0x7a3c50)==0x1238c6c
pdb={}
for fn in ['ghidra_out/egor_pdb_names.tsv','ghidra_out/fablewin_pdb_names.tsv']:
 for i,l in enumerate(Path(fn).read_text().splitlines(),1):
  if '\t'in l:pdb.setdefault(l.split('\t',1)[1],[]).append([fn,i,l.split('\t')[0]])
# Full original signatures; source projections differ in owner spelling and documented opaque container identities.
originals=['?SuspendAI@CThingAICreature@@QAEX_NPAX@Z','?TransitionToDefaultThemeAllChannels@CEnvironment@@QAEXJM@Z','?SetAsDrawable@CThing@@UAEX_N@Z','?OnHit@CThingCreatureBase@@UAEXAAVCHitParameters@@@Z','?SetParent@CComponent@NUISystem@@UAEXPAV12@@Z','?LoadAndReturnDef@CComponent@NUISystem@@UBE?AV?$CDefPointer@$$CBVCUIDef@NUISystem@@@@XZ','?ExecuteActions@CComponent@NUISystem@@UAEXPAV?$list@V?$CCountedPointer@VCAction@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCAction@NUISystem@@@@@std@@@std@@@Z','?OnUnhovered@CMenuEntry@NUISystem@@MAEXXZ','?Delete@CIVCountedPointeeBase@@UBEXXZ','??0CHitParameters@@QAE@XZ','??1CHitParameters@@QAE@XZ','?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z','?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z','?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z','?Kill@CThing@@QAEX_N@Z','?ChangeState@CChangingStateComponent@NUISystem@@UAEXK@Z']
originals += ['?OnDeactivate@CModeBriarRoseTeleportIntangible@NCreatureMode@@UAEXXZ','?OnDeactivate@CModeBriarRoseConcentrate@NCreatureMode@@UAEXXZ']
for vt,address,name in [(0x128026c,0x8e7cf0,b'.?AVCModeBriarRoseTeleportIntangible@NCreatureMode@@'),(0x1268548,0x8e7e70,b'.?AVCModeBriarRoseConcentrate@NCreatureMode@@')]:
 assert u(vt+6*4)==address
 assert rd(u(u(vt-4)+12)+8,len(name)+1)==name+b'\0'
for s in originals:assert s in pdb,s
(o/'original_signatures.json').write_text(json.dumps([{ 'symbol':s,'undecorated':H['und'](s),'PDB_records':pdb[s]}for s in originals],indent=2))
checks=[('008e7cf0',0x1239abc,28,0x4c8e50,'?SetAsDrawable@ThingFixture@@QAEX_N@Z',originals[2]),('008e7e70',0x125ac0c,65,0x667d30,'?Hit@CreatureFixture@@QAEXAAUHitParameters@@@Z',originals[3]),('0053d3b0',0x124662c,51,0x52f330,'?Parent@ChildFixture@@QAEXPAUCComponent@NUISystem@@@Z',originals[4]),('0053d3b0',0x124662c,108,0x530ec0,'?Load@MenuFixture@@QBE?AUDefinitionPointer@NUISystem@@XZ',originals[5]),('0053d3b0',0x124662c,131,0x5309b0,'?Actions@MenuFixture@@QAEXPAUActionList@NUISystem@@@Z',originals[6]),('0053d3b0',0x124662c,144,0x53d970,'?Unhover@MenuFixture@@QAEXXZ',originals[7]),('008e7cf0',0x12354d0,1,0x42d930,'?Delete@DefinitionFixture@@QBEXXZ',originals[8]),('008e7cf0',0x12354d0,1,0x42d930,'?Delete@BriarFixture@@QBEXXZ',originals[8]),('0053d3b0',0x12354d0,1,0x42d930,'?Dispose@DefFixture@@QBEXXZ',originals[8])]
slots=[]
for a,vt,slot,target,sym,original in checks:
 assert u(vt+4*slot)==target,(a,slot)
 sy,se=H['coff'](next((o/'dry_work').rglob(a+'.t.obj')));assert any(n==sym and s>0 for n,v,s in sy.values()),sym
 slots.append(dict(address=a,table=hex(vt),slot=slot,target=hex(target),compiled=H['und'](sym),original=H['und'](original),scope='Typed PMF fixture interface with original parameter/return/const widths; aliases and access differences explicit, no full native class/constructor claim.'))
# Genuine observed-pointer tables have only a deleting destructor and actual inheritance.
hier={}
for vt,cl in [(0x129c95c,'BaseIntelligentPointer'),(0x1238c6c,'IntelligentThingPointer')]:
 sy,se=H['coff'](next((o/'dry_work').rglob('008e7e70.t.obj')));n,v,s=next((n,v,s)for n,v,s in sy.values()if n=='??_7'+cl+'@@6B@'and s>0);size,rel=se[s-1];ss=[rel[k]for k in range(v,size,4)if k in rel];assert len(ss)==1 and ss[0].startswith(('??_G','??_E'))
 ch=u(u(vt-4)+16);items=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);items.append([rd(u(bc)+8,180).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]={'hierarchy':items,'emitted_slots':[H['und'](s)for s in ss],'native_destructor':hex(u(vt))}
assert hier['0x1238c6c']['hierarchy']==[['.?AV?$CIntelligentPointer@VCThing@@@@',0],['.?AVCBaseIntelligentPointer@@',0]]
assert u(0x1238c6c)==0x4a9710
(o/'native_virtual_signatures.json').write_text(json.dumps(slots,indent=2));(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2))
records=[json.loads(l)for l in(r/'astra241_whole/ATTEMPTS.jsonl').read_text().splitlines()];records=[q for q in records if q['address']in obs and q['kind']=='grade'];assert collections.Counter(q['address']for q in records)=={'008e7cf0':3,'008e7e70':1,'0053d3b0':1}
for q in records:
 p=Path(q['snapshot']);assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==q['sha256'];assert q['command']and q['base_flags']==['/O2','/Oy','/W3']and not q['extra_flags'];assert(p/'stdout.txt').exists()and(p/'stderr.txt').exists()
q=json.loads((o/'root_amendments/grade.json').read_text());assert hashlib.sha256(Path(q['source']).read_bytes()).hexdigest()==q['sha256'];assert 'RELOCATION_MATCH'in(o/'root_amendments/grade.log').read_text();records.append(q);(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
assert(o/'dry.log').read_text().count('RELOCATION_MATCH PASS')==3
print('PASS26relocations/18originalsignatures/2nativeownerRTTI/9typedvirtualinterfaces/2pointerVT+RTTI/6sourcegrades/nativehelpers')
