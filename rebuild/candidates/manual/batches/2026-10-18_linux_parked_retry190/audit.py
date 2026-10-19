from pathlib import Path
import struct,runpy,sys,json,hashlib,re,collections,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry190';w=r/'retry202h_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?GetNextValidDimensions@':0x9fa620,'??2@':0xbfea1a,'??0CBaseClass@@':0x99a2d0,'??_7CVolumeTexture@@':0x129dc18,'?Reset@':0xa6ae60,'?Initialise@':0xa68370,'?LowerBound@':0x6faf20,'?Allocate@':0x6fafe0,'?Copy@':0x6fb5c0,'?PushBack@':0x6fcd90,'??_V@':0xbfeb1c,'?SortEventPairs@':0x6fd380,'?resize@':0x6fade0,'_malloc':0xbfea0e,'_free':0xbfea14,'?GetThingsToAdd@':0x82e9c0,'?GetThingsToDelete@':0x82e910,'?GetPThingWithUID@':0x51ed80,'?SetAsActivated@':0x4c7770,'?Kill@':0x4c9b80}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k)
  assert v==int(target,16) and [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==26
# Verify complete same-TU native helper, not just caller relocations.
obj=next((o/'dry_work').rglob('006fa3f0.obj'));b=obj.read_bytes();sy,se=H['coff'](obj);n,v,s=next((n,v,s)for n,v,s in sy.values()if n.startswith('?LowerBound@EventMap@')and s>0);size,rels=se[s-1];raw=struct.unpack_from('<I',b,20+(s-1)*40+20)[0];assert size==82 and not rels and b[raw+v:raw+v+82]==rd(0x6faf20,82)
(o/'native_helper_bytes.json').write_text(json.dumps({'symbol':n,'native':'006faf20','length':82,'complete_unmasked_equal':True},indent=2))
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');imports={i.address:i.name.decode()for d in pe.DIRECTORY_ENTRY_IMPORT for i in d.imports if i.name}
for thunk,name in [(0xbfeb1c,'??_V@YAXPAX@Z'),(0xbfea0e,'malloc'),(0xbfea14,'free')]:assert rd(thunk,2)==b'\xff\x25' and imports[u(thunk+2)]==name
pdb=[l.split('\t',1)[1]for l in(Path('ghidra_out/egor_pdb_names.tsv').read_text()+Path('ghidra_out/fablewin_pdb_names.tsv').read_text()).splitlines()if'\t'in l]
parts=['??0CVolumeTexture@@QAE@XZ','?Initialise@CVolumeTexture@@QAE_NABVC3DExtentsI@@JABVCPixelFormat@@KW4EVolumePool@@_N@Z','?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z','?AddEventToAnim@CManager@NAnimationEvents@@QAEXKABVCEvent@2@@Z','?ScriptedMapBrushAddThings@CMap@@MAEXABVCScriptedMapBrush@@@Z','?ScriptedMapBrushRemoveThings@CMap@@MAEXABVCScriptedMapBrush@@@Z','?SetAsActivated@CThing@@QAEX_N@Z','?Kill@CThing@@QAEX_N@Z']
assert all(p in pdb for p in parts),[p for p in parts if p not in pdb]
sy,se=H['coff'](next((o/'dry_work').rglob('008209c0.t.obj')));slots=[]
def norm(s):
 s=re.sub('public: |private: |protected: |virtual |class |struct |enum ','',s);s=re.sub(r'[A-Za-z0-9_:]+(?=\()', 'METHOD',s);return re.sub(r'\s+','',s)
for vt,i,target,prefix,original in [(0x12390f0,3,0x49e1b0,'?Manager@WorldDouble','?GetThingManager@CWorld@@UAEAAVCThingManager@@XZ'),(0x12390f0,13,0x4ae8c0,'?Map@WorldDouble','?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ'),(0x1244aec,17,0x4fc080,'?Number@WorldMapDouble','?GetMapNumberAt@CWorldMap@@UBEJABVC2DCoordI@@@Z')]:
 assert u(vt+4*i)==target and original in pdb;compiled=next(n for n,v,s in sy.values()if n.startswith(prefix));assert norm(H['und'](compiled))==norm(H['und'](original));slots.append({'table':hex(vt),'slot':i,'target':hex(target),'PDB':H['und'](original),'fixture':H['und'](compiled)})
sy,se=H['coff'](next((o/'dry_work').rglob('00a699d0.t.obj')));tables=[]
for cl in ['CBaseClass','CVolumeTexture']:
 n,v,s=next((n,v,s)for n,v,s in sy.values()if n.startswith('??_7'+cl+'@@'));assert se[s-1][0]==4 and list(se[s-1][1])==[0];assert se[s-1][1][0].startswith(('??_G'+cl,'??_E'+cl));tables.append(H['und'](se[s-1][1][0]))
hier={}
for vt in [0x129dc18,0x1231710]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert hier['0x129dc18']==[['.?AVCVolumeTexture@@',0],['.?AVCBaseClass@@',0]] and u(0x129dc18)==0xa69590
# Native activation setter exactbytes + donor bit-field proof (not propagated canonical label).
assert rd(0x4c7770,28)==bytes.fromhex('8a4424048a9191000000c0e00332c2240832d0889191000000c20400')
layout=Path('ghidra_out/struct_layouts_egor.tsv').read_text().splitlines();proof=[l for l in layout if '\tCThing\t' in l and any(x in l for x in ['Activated','GameStatePersisted','Dead'])]
# Layout columns may start with CThing; keep exact original lines for review.
if not proof:proof=[l for l in layout if l.startswith('CThing\t')and any(x in l for x in ['Activated','GameStatePersisted','Dead'])]
assert len(proof)>=3,proof
(o/'activation_name_proof.txt').write_text('\n'.join(proof)+'\nNative setter91bit3; nativeKill testsDead91bit0. DonorA1 Activatedbit3 vsA2 GameStatePersistedbit5. Existing4C7770canonicalnameheldforseparatecorrection.\n')
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2));(o/'native_virtual_signatures.json').write_text(json.dumps({'typed_slots':slots,'volume_tables':tables,'direct_PDB':[H['und'](p)for p in parts]},indent=2))
records=json.loads((w/'ATTEMPTS.json').read_text());assert collections.Counter(x['address']for x in records)=={'00a699d0':2,'006fa3f0':2,'00820790':3,'008209c0':5}
for x in records:assert hashlib.sha256(Path(x['source']).read_bytes()).hexdigest()==x['sha256']and x['command']and x['effective_flags']==['/O2','/Oy','/W3']and x['cumulative']==x['new_grade']+4<=10
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS26relocations/82byteunmaskedhelper/3PMFsignatures/2volumeVTs/2RTTI/12sourcegrades+16reserved')
