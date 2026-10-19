from pathlib import Path
import struct,runpy,sys,json,hashlib,re,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry188';w=r/'astra240_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?GetPControlledCreature@':0x487dc0,'?LowerBound@':0x40f020,'?GetType@':0xa03b40,'?GetJoystickPosition@':0xa043a0,'?SetAsTargetNextThingInDirection@':0x8023c0,'?SetAsPerformedEvent@':0xa0d390,'__real@3e800000':0x125bec0,'__real@be800000':0x1230c8c,'__real@3f266666':0x125bebc,'__real@bf266666':0x125bec4,'?GetInstance@':0x41e5f2,'?GetChar@':0xa03b80,'?IsEventButtonHeld@':0x687a90,'?IsEventGameAction@':0x687cf0,'??0BaseIntelligentPointer@':0xa01b10,'??1BaseIntelligentPointer@':0xa01c10,'?GetPItem@':0xa01b50,'?SetPItem@':0xa01b90,'??_7ThingPointer@':0x1238c6c,'_malloc':0xbfea0e}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k)
  assert v==int(target,16) and [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  if sym.startswith('__real@'):assert u(v)==int(sym[7:],16)
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==48
# Actual original names/signatures from PDB, never donor-address identity alone.
pdb=[l.split('\t',1)[1]for l in(Path('ghidra_out/egor_pdb_names.tsv').read_text()+Path('ghidra_out/fablewin_pdb_names.tsv').read_text()).splitlines()if'\t'in l]
parts=['?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ','?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ','?GetJoystickPosition@CInputEvent@@QBE?AVC2DVector@@XZ','?GetChar@CInputEvent@@QBE_WXZ','?SetAsTargetNextThingInDirection@CTCTargetingPlayer@@QAEXW4ETargetNextThingMethod@1@@Z','?IsEventButtonHeld@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z','?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z','?GetMouseMovement@CInputProcessGameBase@@QBE?AVC2DVector@@ABVCInputEvent@@@Z','?GetInstance@CManager@NUISystem@@SIPAV12@XZ','??0CLightningBeam@CTCWillLightning@@QAE@PAVCThing@@0@Z','??0CBaseIntelligentPointer@@QAE@XZ','??1CBaseIntelligentPointer@@UAE@XZ','?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ','?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z']
assert all(p in pdb for p in parts),[p for p in parts if p not in pdb]
assert 'protected:bool IsEventButtonHeld' in (w/'src/00691ae0.cpp').read_text()
assert rd(0xa03b80,4)==bytes.fromhex('668b01c3') # UTF16 word getter; VC7.1 default wchar_t typedef ushort
assert rd(0x42d4f7,15)==bytes.fromhex('668b442404668981c0000000c20400')
sy,se=H['coff'](next((o/'dry_work_final').rglob('00691ae0.t.obj')))
virtual=[]
for i,addr,prefix,original in [(0,0x55cb10,'?SetEvent@Receiver','?SetEvent@CObservable@NUISystem@@UAEXW4EEvent@2@@Z'),(12,0x42d4f7,'?SetKeyPressed@Receiver','?SetKeyPressed@CManager@NUISystem@@UAEX_W@Z')]:
 assert u(0x1230134+4*i)==addr and original in pdb
 compiled=next(n for n,v,s in sy.values()if n.startswith(prefix));dec=H['und'](compiled);want=H['und'](original)
 def shape(s):
  s=re.sub('public: |virtual |enum ','',s).replace('unsigned short','wchar_t');s=re.sub(r'[A-Za-z0-9_:]+(?=\()', 'METHOD',s);s=s.replace('PhotoInput::EEvent','NUISystem::EEvent');return s
 assert shape(dec)==shape(want),(dec,want)
 virtual.append({'slot':i,'target':hex(addr),'compiled':dec,'original':want})
# Honest instantiated pointer hierarchy; no invented unused virtual slots.
hier={}
for vt in [0x1238c6c,0x129c95c]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert hier['0x1238c6c']==[['.?AV?$CIntelligentPointer@VCThing@@@@',0],['.?AVCBaseIntelligentPointer@@',0]]
assert hier['0x129c95c']==[['.?AVCBaseIntelligentPointer@@',0]] and u(0x1238c6c)==0x4a9710
sy,se=H['coff'](next((o/'dry_work_final').rglob('007a0630.t.obj')))
for cl in ['ThingPointer','BaseIntelligentPointer']:
 n,v,s=next((n,v,s)for n,v,s in sy.values()if n.startswith('??_7'+cl+'@LightningNative@@'))
 assert se[s-1][0]==4 and list(se[s-1][1])==[0];assert se[s-1][1][0].startswith(('??_G'+cl,'??_E'+cl))
 virtual.append({'class':cl,'emitted':H['und'](se[s-1][1][0]),'slots':1})
# Pointee is only external two-slot ABI view; fixture TestDef is a test double, not a native constructed CDefPointee.
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2));(o/'native_virtual_signatures.json').write_text(json.dumps({'slots':virtual,'direct_PDB_signatures':[H['und'](p)for p in parts]},indent=2))
records=[json.loads(l)for l in(w/'GRADE_AUDIT.jsonl').read_text().splitlines()if json.loads(l)['address']in obs]
assert collections.Counter(x['address']for x in records)=={'00689130':4,'00691ae0':4,'007a0630':2}
for x in records:
 p=w/x['snapshot'];assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['effective_flags']==['/O2','/Oy','/W3'];assert x['command'] and x['cumulative_after_grade']<=10
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS48relocations/4floatpools/14directPDBsignatures/2typedPMFslots/2emittedpointertables/2RTTIhierarchies/10immutablegrades')
