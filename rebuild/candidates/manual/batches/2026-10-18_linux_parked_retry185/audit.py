from pathlib import Path
import struct,runpy,sys,json,hashlib,collections,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry185';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?GetType@':0xa03b40,'?InsertDefault@':0x9e4a20,'?GetJoystickPosition@':0xa043a0,'?GetJoystickButtonPressureFraction@':0xa03ba0,'?GetJoystickButton@':0xa03b90,'?SetAsJoystickButtonHeld@':0xa040d0,'_free':0xbfea14,'?CombineAreas@':0xa01e40,'?FindFreeArea@':0xa01ed0,'?MoveArea@':0xa028f0,'?CombineAdjacentAreas@':0xa02470,'__real@3f800000':0x122ded8,'?IsTimeBetween@':0x6bb5b0,'__ftol2':0xbfea70,'??2@':0xbfea1a,'??0ResourceNative':0x9fbec0,'??_7DataResource@':0x129d3a4,'?CreateEntry@':0x9d58d0,'?GetEntryDataSize@':0x9cc240,'?LipSyncLoadingPriority':0x1397700,'?BeginReadDataAutoAlloc@':0x9d6100,'??4?$Counted@':0x9a4440,'??3@':0xbfe9bc,'?AddResource@':0x9fc3d0}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==36
assert u(0x122ded8)==0x3f800000 and u(0x1397700)==3
assert rd(0xbfea14,6)==bytes.fromhex('ff2554014401')
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');imports={x.address:(d.dll,x.name)for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports};assert imports[0x1440154]==(b'MSVCR71.dll',b'free')
pdb=Path('ghidra_out/fablewin_pdb_names.tsv').read_text()+Path('ghidra_out/egor_pdb_names.tsv').read_text()
for s in ['?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ','?GetJoystickButton@CInputEvent@@QBEJXZ','?GetJoystickButtonPressureFraction@CInputEvent@@QBEMXZ','?GetJoystickPosition@CInputEvent@@QBE?AVC2DVector@@XZ','?SetAsJoystickButtonHeld@CInputEvent@@QAEXJMABVC2DVector@@N@Z','?IsTimeBetween@CGameTimeManager@@ABE_NMMM@Z','??0CGameTimeManager@@QAE@AAVCMainGameComponent@@AAVCWorld@@@Z','?GrowFreeArea@CHandleBasedMemoryPool@@AAE_NAAPAVCHeader@1@@Z','?BeginReadDataAutoAlloc@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KKW4EThreadedFilePriority@@@Z']:assert s in pdb,s
hier={}
for vt in [0x129c0e0,0x129c964,0x129d3a4,0x12354a4]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert hier['0x129c0e0']==[['.?AVCMaintainedEvent@@',0],['.?AVCBaseClass@@',0]]
assert hier['0x129c964']==[['.?AVCHandleBasedMemoryPool@@',0],['.?AVCBaseClassNonCopyable@@',0],['.?AVCBaseClass@@',0]]
assert hier['0x129d3a4']==[['.?AVCData@CLipSyncDataBankEntry@@',0],['.?AVCResource@@',0],['.?AVCIVCountedPointeeBase@@',0]]
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2));sig=json.loads((o/'native_virtual_signatures.json').read_text());assert len(sig['slots'])==30 and not sig['mismatches']
records=[]
w=r/'astra240_whole'
for a,count in [('009e4470',5),('00a42980',4)]:
 rr=[json.loads(l)for l in(w/'ATTEMPTS.jsonl').read_text().splitlines()if json.loads(l)['address']==a];assert len(rr)==count
 for x in rr:
  p=w/'snapshots'/(a+'_'+str(x['attempt'])+'.cpp');assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['command'] and x['effective_flags']==['/O2','/Oy','/W3'];records.append(x)
w=r/'astra239_whole'
for a in ['00a02c00','006bb990']:
 for i in range(1,5):
  p=w/'snapshots'/(a+'_%02d'%i);x=json.loads((p/'grade.json').read_text());assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['source_grade']==i and x['effective_flags']==['/O2','/Oy','/W3'];assert(p/'grade.log').exists();records.append(dict(address=a,**x))
assert len(records)==17
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS36relocations/4RTTIhierarchies/30emittedvirtualsignatures/trueenumtypes/17immutablegrades')
