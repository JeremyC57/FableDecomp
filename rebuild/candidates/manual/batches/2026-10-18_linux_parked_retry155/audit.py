from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry155';w=r/'retry201b_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'?Reset@NativeCIME':0x4051c0,'?g_ImeDrawRect':0x143cd64,'?Callback_DrawRect':0x404d50,'?g_ImeMalloc':0x143e514,'?Callback_malloc':0x404a60,'?g_ImeFree':0x143cf08,'?Callback_free':0x404a70,'?g_ImeDrawFans':0x143cdfc,'?Callback_DrawFans':0x404e10,'?g_ImeOnChar':0x143e630,'?Callback_OnChar':0x404f00,'?ImeUI_Initialize':0xcb5180,'??2@':0xbfea1a,'??_7CIMEFont':0x122dcbc,'?ImeUi_SetAppearance':0xcb2e20,'?GFGetSystemManager':0x9a4ec0,'?GetRenderTargetDimensions':0x9bedc0,'?ImeUi_SetScreenDimension':0xcb2ee0,'?ImeUi_FinalizeString':0xcb2c60,'?ImeUi_EnableIme':0xcb5130,'??4CCharString':0x99efe0,'?Append@NativeCharString':0x99f100,'__real@4f800000':0x122dcb4,'__real@3daaaaab':0x122dca0,'__real@3d800000':0x122dc98,'__real@3d638e39':0x122dc90,'__real@3d2aaaab':0x122dc88,'__real@3d124925':0x122dc80,'??_C@_0L@BEDDKEGH':0x122dca8,'??_C@_02MIGHPNDL':0x122dca4,'??_C@_02KMALDIDP':0x122dc9c,'??_C@_02DCIIBFLB':0x122dc94,'??_C@_02JMHLOEOE':0x122dc8c,'??_C@_02DAMOKLOI':0x122dc84,'??0CChangingStateComponent':0x52cc50,'??_7CIconText@NUISystem@@6BCUIInterface':0x12470cc,'??_7CIconText@NUISystem@@6BCObserver':0x12470a8,'??_7CIconText@NUISystem@@6BCActionParam':0x12470a0,'??0CWideString':0x99aed0,'??0CBaseClass':0x99a2d0,'??_7CEnginePrimitiveHandle':0x122f598,'?UpdateFont@CIconText':0x541fd0}
count=0
for a,n in [('00405650',21),('00404970',18),('00542330',11)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
for a,h in [(0x122dcb4,'4f800000'),(0x122dca0,'3daaaaab'),(0x122dc98,'3d800000'),(0x122dc90,'3d638e39'),(0x122dc88,'3d2aaaab'),(0x122dc80,'3d124925')]:assert rd(a,4)==struct.pack('<I',int(h,16))
for a,s in [(0x122dca8,'ENG_ARIAL_'),(0x122dca4,'12'),(0x122dc9c,'16'),(0x122dc94,'18'),(0x122dc8c,'24'),(0x122dc84,'28')]:assert rd(a,len(s)+1)==s.encode()+b'\0'
assert rd(0x9bedc0,24)==bytes.fromhex('8b91980100008b89940100008b4424048908895004c20400')
font=[0x404940,0x404d20,0x404950,0x404f60,0x405020]
for i,v in enumerate(font):assert struct.unpack('<I',rd(0x122dcbc+4*i,4))[0]==v
# Independently inspect native RTTI complete-object locators and every base adjustment.
u=lambda a:struct.unpack('<I',rd(a,4))[0]
text=lambda a:rd(a,160).split(b'\0')[0].decode()
def hierarchy(vt):
 col=u(vt-4);ch=u(col+16);bases=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);bases.append([text(u(bc)+8),struct.unpack('<i',rd(bc+8,4))[0]])
 return u(col+4),text(u(col+12)+8),bases
icon=[['.?AVCIconText@NUISystem@@',0],['.?AVCChangingStateComponent@NUISystem@@',0],['.?AVCComponent@NUISystem@@',0],['.?AVCUIInterface@NUISystem@@',0],['.?AVCObserver@NUISystem@@',4],['.?AVCActionParam@NUISystem@@',24]]
out={}
for vt,offset in [(0x12470cc,0),(0x12470a8,4),(0x12470a0,24)]:
 h=hierarchy(vt);assert h==(offset,icon[0][0],icon);out[hex(vt)]=h
h=hierarchy(0x1245de4);assert h==(0,icon[1][0],icon[1:]);out['0x1245de4']=h
h=hierarchy(0x122f598);assert h==(0,'.?AVCEnginePrimitiveHandle@@',[['.?AVCEnginePrimitiveHandle@@',0],['.?AVCBaseClass@@',0]]);out['0x122f598']=h
h=hierarchy(0x122dcbc);assert h==(0,'.?AVCIMEFont@NUISystem@@',[['.?AVCIMEFont@NUISystem@@',0],['.?AVCImeUiFont_Base@@',0]]);out['0x122dcbc']=h
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');assert rd(0xbfea1a,6)==bytes.fromhex('ff2550014401');assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==0x1440150]==[(b'MSVCR71.dll',b'??2@YAPAXI@Z')]
records=[]
for a,g in [('00405650',5),('00404970',2),('00542330',2),('009f52a0',6)]:
 ps=sorted((w/'snapshots').glob(a+'-??'));assert len(ps)==g,(a,len(ps),g)
 for p in ps:
  m=json.loads((p/'command.json').read_text());assert m['command']and m['base_flags']and m['reserved_uncertain']==4;assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==m['source_sha256'];records.append(dict(snapshot=str(p),manifest=m))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS',count,'relocations,6float pools/6literals,hidden-result ABI,5 fontVTslots,6RTTI hierarchies,new import,15 immutable grades/final hashes')
