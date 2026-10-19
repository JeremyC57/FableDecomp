from pathlib import Path
import struct,runpy,sys,json,hashlib,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry183';w=r/'retry202f_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'??0CDisplayEngineInit':0x419270,'?g_DisplayDevice':0x13b8390,'?GetHeight@CSurface':0x415bf0,'?GetDimensions@CSurface':0x9f2f90,'?GetPixelFormat@CSurface':0x9f2f60,'?GetDefinitionManager':0x44c6b0,'??2@':0xbfea1a,'??0CDisplayEngine@@':0x434e10,'?reset@':0x41940c,'??0CCharString':0x99ebf0,'__real@bf800000':0x122dee0,'?DisplayProgress':0x9e9f40,'??1CCharString':0x99eae0,'?Init@CDisplayEngine':0x4350d0,'??_7CMainGameComponent@@6BCBaseClassNonCopyable':0x122f180,'??_7CMainGameComponent@@6BCDeviceResetCallback':0x122f174,'?Shutdown@':0x4175e5,'??1CMainGameComponentInit':0x4131d0,'??1?$CCountedPointer@UCGraphicDataBank':0x419108,'??1CFrameRateSmoother':0x419764,'??1CDiskFileWin32':0x401630,'??1?$CCountedPointer@UCFontBank':0x4057a0,'??1CNetworkClient':0x419352,'??1?$CCountedPointer@UCLUA':0x4190b6,'??1?$CCountedPointer@UCDataBank':0x403750,'??_7CGameComponent@@6BCBaseClassNonCopyable':0x122ecb0,'??_7CGameComponent@@6BCDeviceResetCallback':0x122eca4,'??1CBaseClass@@':0x99a300,'?GetGlobalIndexFromClassIndex':0x9ad390}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  if sym.startswith('??_C@'):
   lit={0x122eeb4:'Init Display Engine: Calling Init',0x1230018:'UI_MISC_THINGS_DEF',0x123002c:'UI_ICONS_DEF'}[v];assert rd(v,len(lit)+1)==lit.encode()+b'\0'
  elif sym.startswith('?GetDef@'):assert v==(0x42b312 if 'CUIMiscThingsDef' in sym else 0x42b374)
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==56
assert rd(0x122dee0,4)==struct.pack('<f',-1.0)
# Identical helper alias: native NonCopyable destructor99A300 and Base destructor99A2E0.
assert rd(0x99a300,7)==rd(0x99a2e0,7)==bytes.fromhex('c70110172301c3')
assert rd(0x9ad39d,4)==bytes.fromhex('8b542408') and rd(0x9ad3a1,3)==bytes.fromhex('8b0491')
helpers=[]
for a in ['0041d8e7','0041d98a']:
 obj=next((o/'dry_work').rglob(a+'.obj'));sy,sc=H['coff'](obj);b=obj.read_bytes()
 for n,v,sec in sy.values():
  if not n.startswith('?GetDef@') or sec<=0:continue
  target=0x42b374 if 'CUIIconsDef' in n else 0x42b312;size,rels=sc[sec-1];ptr=struct.unpack_from('<I',b,20+(sec-1)*40+20)[0];ours=bytearray(b[ptr+v:ptr+v+98]);native=bytearray(rd(target,98));count=0
  for off,sym in rels.items():
   off-=v
   if 0<=off<=94:
    assert sym.startswith('?GetPDefFromGlobalIndex@') and target+off+4+struct.unpack('<i',native[off:off+4])[0]==0x9ad9e0
    ours[off:off+4]=native[off:off+4]=b'\0'*4;count+=1
  assert count==1 and ours==native;helpers.append([a,n,hex(target),98,count])
assert len(helpers)==4
(o/'native_helper_bytes.json').write_text(json.dumps(helpers,indent=2))
hier={}
for vt in [0x122f180,0x122ecb0,0x122d06c,0x122edac,0x122f168,0x122e59c,0x1231574]:
 ch=u(u(vt-4)+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,200).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]=out
assert hier['0x122f180']==[['.?AVCMainGameComponent@@',0],['.?AVCGameComponent@@',0],['.?AVCBaseClassNonCopyable@@',0],['.?AVCBaseClass@@',0],['.?AVCDeviceResetCallback@@',4]]
(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2))
sig=json.loads((o/'native_virtual_signatures.json').read_text());assert len(sig['slots'])==71 and not sig['mismatches']
records=json.loads((w/'ATTEMPTS.json').read_text());assert collections.Counter(x['address']for x in records)=={'00417418':4,'00418b7a':4,'0041d8e7':5,'0041d98a':2}
for x in records:
 assert hashlib.sha256(Path(x['source']).read_bytes()).hexdigest()==x['sha256'];assert x['command'] and x['effective_flags']==['/O2','/Oy','/W3'];assert x['historical_uncertain']==4 and x['cumulative']==x['new_grade']+4<=10
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS56relocations/3fullstrings/4exact98bytehelpers/71emittedvirtualsignatures/7RTTIhierarchies/15grades+16reserved')
