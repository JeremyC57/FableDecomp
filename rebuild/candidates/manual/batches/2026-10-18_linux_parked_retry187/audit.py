from pathlib import Path
import struct,runpy,sys,json,hashlib,collections,re
r=Path('work/codex-lane/w22');o=r/'coordinator_retry187';w=r/'retry202g_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?g_pMainGameComponent':0x13b86a0,'?g_pFrontendRuntime':0x13b871c,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?GetGameDefinitionManager':0x44c6b0,'?GetFrontendDefinitionManager':0x43368d,'?GetGlobalIndexFromClassIndex':0x9ad390,'?GetDef@':0x410890,'?g_pActiveScrollingViewport':0x13b8aac,'?g_ActiveListHead':0x13b8a98,'__real@41c80000':0x123003c,'??3@':0xbfe9bc,'??4CCharString':0x99efb0,'?GetAnimationInfo@':0xa242c0,'?GetAnimationStats@':0xa269c0,'?g_MinEmbeddedAnimationDuration':0x129ba3c,'?GetEventTime@':0xa4eae0,'__real@00000000':0x122dedc}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  if sym.startswith('??_C@'):assert v==0x122e078 and rd(v,len(b'CONFIG_OPTIONS_DEFAULTS_DEF\0'))==b'CONFIG_OPTIONS_DEFAULTS_DEF\0'
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==30
assert rd(0x123003c,4)==struct.pack('<f',25.0)and rd(0x129ba3c,4)==struct.pack('<f',0.0001)and u(0x122dedc)==0
assert rd(0xa242c0,len(bytes.fromhex('8b81800300008b4c24048b0488c20400')))==bytes.fromhex('8b81800300008b4c24048b0488c20400')
assert rd(0xa4eae0,len(bytes.fromhex('8b4424045083c120e823580000d94014c20400')))==bytes.fromhex('8b4424045083c120e823580000d94014c20400')
# Independently compare the emitted entire108-byte config helper, masking only its real call operand.
obj=next((o/'dry_work').rglob('004083e0.obj'));b=obj.read_bytes();sy,se=H['coff'](obj);n,v,s=next((n,v,s)for n,v,s in sy.values()if n.startswith('?GetDef@')and s>0);size,rels=se[s-1];ptr=struct.unpack_from('<I',b,20+(s-1)*40+20)[0];ours=bytearray(b[ptr+v:ptr+v+108]);native=bytearray(rd(0x410890,108));actual=[]
for k,sym in rels.items():
 off=k-v
 if 0<=off<=104:
  assert sym.startswith('?GetPDefFromGlobalIndex@') and 0x410890+off+4+struct.unpack('<i',native[off:off+4])[0]==0x9ad9e0
  ours[off:off+4]=native[off:off+4]=b'\0'*4;actual.append(off)
assert len(actual)==1 and ours==native
(o/'native_helper_bytes.json').write_text(json.dumps({'symbol':n,'length':108,'masked_actual_call_offset':actual,'target':'009ad9e0'},indent=2))
pdb=[l.split('\t',1)[1]for l in(Path('ghidra_out/egor_pdb_names.tsv').read_text()+Path('ghidra_out/fablewin_pdb_names.tsv').read_text()).splitlines()if'\t'in l]
for part in ['?GetConfigOptionsDefaultsDef@CUserProfileManager@@QBE','?ScrollMenu@CManager@NUISystem@@QAEXJ@Z','?GetEmbeddedEventTime@CManager@NAnimationEvents@@ABEMKABVCCharString@@@Z','?GetAnimationStats@C3DAnimationInfo@@QBE','?GetEventTime@C3DAnimationStats@@QBEMABVCCharString@@@Z','??4CMapInfo@@']:assert any(q.startswith(part)for q in pdb),part
sy,se=H['coff'](next((o/'dry_work').rglob('0041da2d.t.obj')))
def norm(s):
 s=re.sub(r'public: |private: |protected: |virtual |class |struct |enum ','',s)
 s=re.sub(r'NUISystem::[A-Za-z0-9_]+::[A-Za-z0-9_]+(?=\()','METHOD',s)
 return re.sub(r'\s+','',s)
slots=[]
for vt,i,t,method,cl,fixture in [(0x1230134,0,0x55cb10,'SetEvent','CObservable','?Event@ManagerDouble'),(0x12468d4,144,0x53fc00,'GetScrollingSpeed','CScrollingViewport','?Speed@ViewportDouble'),(0x124b7e4,150,0x55a330,'IsDragged','CDraggable','?Drag@ComponentDouble'),(0x12462e4,175,0x53ce5f,'GetScrollBar','CList','?Bar@ComponentDouble'),(0x124608c,33,0x52eaf0,'ChangePositionDelta','CComponent','?Delta@ComponentDouble')]:
 assert u(vt+4*i)==t
 expected=set(q for q in pdb if q.startswith('?'+method+'@'+cl+'@NUISystem@@'));assert len(expected)==1;expected=expected.pop()
 compiled=set(n for n,v,s in sy.values()if n.startswith(fixture));assert len(compiled)==1;compiled=compiled.pop();assert norm(H['und'](compiled))==norm(H['und'](expected)),(compiled,expected)
 slots.append({'native_table':hex(vt),'slot':i,'native_target':hex(t),'fixture':H['und'](compiled),'PDB':H['und'](expected)})
# Real counted base table is actually emitted by the getter fixture, two slots only.
sy,se=H['coff'](next((o/'dry_work').rglob('004083e0.t.obj')))
for n,v,s in sy.values():
 if n.startswith('??_7CIVCountedPointeeBase@@'):
  assert se[s-1][1][v].startswith(('??_G','??_E'));assert H['und'](se[s-1][1][v+4])=='public: virtual void __thiscall CIVCountedPointeeBase::Delete(void) const'
(o/'native_virtual_signatures.json').write_text(json.dumps({'typed_PMF_slots':slots,'counted_base':'actual emitted dtor0/Deleteconst1; no fabricated UI intermediate virtual methods'},indent=2))
records=json.loads((w/'ATTEMPTS.json').read_text());assert collections.Counter(x['address']for x in records)=={'004083e0':4,'0041da2d':4,'00513bf0':1,'006f9130':2}
for x in records:assert hashlib.sha256(Path(x['source']).read_bytes()).hexdigest()==x['sha256']and x['command']and x['effective_flags']==['/O2','/Oy','/W3']and x['cumulative']==x['new_grade']+4<=10
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS30relocations/fullconfigliteral/3floatpools/108bytehelper/5nativePMFsignatures+countedbase/11grades+16reserved')
