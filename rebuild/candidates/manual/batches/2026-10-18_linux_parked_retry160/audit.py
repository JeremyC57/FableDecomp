from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry160';w=r/'luna236_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'??_7CEnginePrimitiveRenderer2D':0x12a2308,'?ClearPrimitiveList':0xb8fee0,'?FableOperatorDelete':0xbfe9bc,'?DestroyManagerRange':0xb38930,'?FableFree':0xbfea14,'??1CEngineComponent':0xb59730,'??_7CEngineScreenEffectDisplacementRenderer':0x12a3a94,'?g_pEngine_01436e98':0x1436e98,'?FreeMemoryPage':0xf37030,'?g_pRenderStateManager_013cb508':0x13cb508,'?RestoreCaptureBlockAtGlobal':0xa05840,'??_7CPixelShader':0x129a054,'??_7CShaderBase':0x129a030,'??_7CVertexShader':0x129b3d0,'??_7CSurface':0x122f84c,'?Uninitialise@CSurface':0x9f2e20,'??1CBase@@':0x99a2e0}
count=0
for a,n in [('00b2ab80',0),('00b4aaa0',11),('00b870a0',16)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=''),re.M);assert len(rel)==n,(a,len(rel));out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 count+=n;(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
u=lambda a:struct.unpack('<I',rd(a,4))[0]
def hierarchy(vt):
 col=u(vt-4);ch=u(col+16);out=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);out.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 return out
expected={0x12a0f04:['CEngineLayerRenderer','CBaseClassNonCopyable','CBaseClass'],0x12a2308:['CEnginePrimitiveRenderer2D','CEngineComponent'],0x12a3a94:['CEngineScreenEffectDisplacementRenderer','CEngineComponent'],0x122f84c:['CSurface'],0x129a030:['CShaderBase'],0x129a054:['CPixelShader','CShaderBase'],0x129b3d0:['CVertexShader','CShaderBase']};out={}
for vt,names in expected.items():
 h=hierarchy(vt);assert h==[['.?AV'+n+'@@',0]for n in names],(hex(vt),h);out[hex(vt)]=h
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
slots=[(0x12a0f04,1,0xb2ab80),(0x12a2308,0,0xb4b720),(0x12a3a94,0,0xb89050),(0x129da50,0,0xa630d0),(0x129d58c,0,0xa5ed00),(0x122f84c,0,0x41c9c0)]
for vt,k,v in slots:assert u(vt+4*k)==v
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{v:08x}\t{k}\t{t:08x}\n'for v,k,t in slots))
assert rd(0x9f2e20,32)==bytes.fromhex('568bf18b460485c074148b0850ff5108c7460400000000c74608000000005ec3')
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe')
for thunk,iat,name in [(0xbfea14,0x1440154,b'free'),(0xbfe9bc,0x1440164,b'??3@YAXPAX@Z')]:
 assert rd(thunk,6)==b'\xff\x25'+struct.pack('<I',iat),(hex(thunk),rd(thunk,6).hex());assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==iat]==[(b'MSVCR71.dll',name)]
records=[]
for line in(o/'attempts.tsv').read_text().splitlines()[1:]:
 a,g,_=line.split('\t');ps=sorted((w/'attempts'/a).glob('grade??'));assert len(ps)==int(g),(a,len(ps),g)
 for p in ps:
  sh=hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest();assert sh in(p/'snapshot.sha256').read_text();cmd=(p/'command.txt').read_text();assert 'q.py' in cmd and 'timeout' in cmd;records.append(dict(address=a,snapshot=str(p),sha256=sh,command=cmd))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS',count,'relocations,7nativehierarchies,6VTslots,COMcleanupbody,free/deleteimports,',len(records),'immutablegrades/finalhashes')
