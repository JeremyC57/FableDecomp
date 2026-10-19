from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry173';w=r/'astra237_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?Die@CBase':0x84d0a0,'?TransferPersistantBaseData@CBase':0x84cf80,'?Init@CBase':0x84e110,'??3@':0xbfe9bc,'?gModeGlobalRoot':0x13b86a0,'?TellEngineThatCameraHasMoved':0x697860,'?GetRandom@CQuakeWorld':0x49d810,'?FloatRandom':0x4978a0,'?Rotate@CQuakeVector':0xa13c60,'?GetQuakeFrame':0x49d870,'?RotateRandom':0x497890,'__real@4f800000':0x122dcb4,'__real@3f000000':0x122f59c,'?ConstructQuake':0x69c860,'?Insert@CQuakeArray':0x69d300,'?PrisonerNumberString':0x99f830,'??H@':0x99f690,'??4CCharString':0x99efb0,'??1CCharString':0x99eae0,'?IsActiveThreadTerminating@CFH_Prisoner':0xf35b30,'?Append@CPrisonerSpeechString':0x99f100,'?gSubBookmarkManager':0x13cb508,'?gSubRenderManager':0x1436e18,'?gSubAlphaTestFunc':0x1396de8,'?SortList@CEngineSubPrimitiveRenderer':0xb84990,'?RenderListTwoPass':0xb84320,'?RenderList@':0xb842d0,'?DeleteList@':0xb84400,'?PopBookmark':0xa05840,'?GetPItem@CCameraObservedPointer':0xa01b50,'?GetDef@CGameDefinitionManager':0x56d0c0,'?LowerBound@CCameraComponentMap':0x4365b0,'?GetProjectileWeaponFirstPersonTargetingModeZoomFOV':0x789cc0,'??2@':0xbfea1a,'??0CProjectileWeapon':0x879620,'??0CCameraModePointer':0x56c880,'?SetCameraMode@':0x697e20,'??0CCharString':0x99ebf0,'?PlayCameraSoundThroughObservedThing':0x698a30,'?Find@CCameraRegisteredMap':0x69cf90,'?AllocMain@CCameraBaseInit':0x84d270,'?Reset@CCameraModePointer':0x69c2f0}
strings={0x12cd118:'TEXT_QST_B01_PROPHET',0x12cd410:'_FREED',0x12cd3fc:'_FREED_ASEXPECTED',0x12cd3ec:'_FREED_DEFAULT',0x12cd3dc:'_FREED_SURPRISE',0x125c344:'PUSH_SPECIALITY_CAMERA_MODE'}
for a,s in strings.items():assert rd(a,len(s)+1)==s.encode()+b'\0'
for a,n in [('00697e20',8),('00699560',10),('00d3abe0',20),('00b849f0',9),('00698d10',10),('00698f50',10)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k)
  if sym.startswith('??_C@'):assert v in strings and strings[v] in sym
  else:assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert u(0x122dcb4)==0x4f800000 and u(0x122f59c)==0x3f000000
slots=[(0x1260f0c,58,0x8942c0),(0x1255f74,2,0x5ed0e0),(0x12a53e8,6,0xb4a460)]
for vt,k,v in slots:assert u(vt+4*k)==v
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{a:08x}\t{k}\t{v:08x}\n'for a,k,v in slots))
# Typed owner record and helper contracts independently decoded from PE.
assert rd(0x56c89d,6)==bytes.fromhex('c70001000000')and u(0x56c8a6)==0x56c0f0
assert rd(0x56c0f0,8)==bytes.fromhex('85c974068b016a01')
assert rd(0x69cfc9,3)==bytes.fromhex('c20800') and rd(0x69cfd4,3)==bytes.fromhex('c20800')
assert rd(0x497894,4)==bytes.fromhex('c10c240d')
assert rd(0xf35b30,7)==bytes.fromhex('8b01ff500c8bc8')
assert rd(0xcb7940,14)==bytes.fromhex('8b412c85c0750332c0c38a4005c3')
records=[];counts={'00697e20':1,'00699560':3,'00d3abe0':4,'00b849f0':5,'00698d10':4,'00698f50':10}
for a,n in counts.items():
 for i in range(1,n+1):
  d=w/'snapshots'/f'{a}_{i:02d}';m=json.loads((d/'manifest.json').read_text());assert hashlib.sha256((d/'source.cpp').read_bytes()).hexdigest()==m['sha256'];assert m['command']and m['compiler_flags']and(d/'output.txt').exists();records.append({'address':a,'grade':i,'snapshot':str(d),'metadata':m})
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS67relocations/3nativePMFslots/6literals/2floatpools/typedowner+hiddeniteratorABI/27immutablegrades')
