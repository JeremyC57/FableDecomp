import struct,ctypes,csv,re,json,sys
from pathlib import Path
D=Path(__file__).parent
class SV(ctypes.Structure):_fields_=[('n',ctypes.c_size_t),('p',ctypes.c_char_p)]
lib=ctypes.CDLL('/lib/x86_64-linux-gnu/libLLVM-17.so.1');dem=getattr(lib,'_ZN4llvm17microsoftDemangleESt17basic_string_viewIcSt11char_traitsIcEEPmPiNS_15MSDemangleFlagsE');dem.argtypes=[SV,ctypes.POINTER(ctypes.c_size_t),ctypes.POINTER(ctypes.c_int),ctypes.c_int];dem.restype=ctypes.c_void_p
free=ctypes.CDLL(None).free;free.argtypes=[ctypes.c_void_p]
def und(s):
 b=s.encode();n=ctypes.c_size_t();st=ctypes.c_int();p=dem(SV(len(b),b),ctypes.byref(n),ctypes.byref(st),0)
 if not p:return s
 out=ctypes.string_at(p).decode();free(p);return out
def coff(path):
 b=Path(path).read_bytes();_,ns,_,ps,ny,_,_=struct.unpack_from('<HHIIIHH',b,0);strings=b[ps+ny*18:];syms={};i=0
 while i<ny:
  off=ps+i*18;n=b[off:off+8]
  if n[:4]==b'\0'*4:n=strings[struct.unpack_from('<I',n,4)[0]:].split(b'\0')[0]
  else:n=n.split(b'\0')[0]
  val,sec,typ,storage,aux=struct.unpack_from('<IhHBB',b,off+8);syms[i]=(n.decode(),val,sec);i+=1+aux
 secs=[]
 for i in range(ns):
  o=20+i*40;size,raw,reloc,_,nr=struct.unpack_from('<IIIIH',b,o+16);rels={}
  for j in range(nr):v,idx,kind=struct.unpack_from('<IIH',b,reloc+j*10);rels[v]=syms[idx][0]
  secs.append((size,rels))
 return syms,secs

import runpy,hashlib,subprocess,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry174';w=r/'retry202c_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?SetupPrimitiveDesc@CLocalDetailPrimitiveRepeatedMesh':0xbf9bc0,'?SetupPrimitiveDesc@CLocalDetailPrimitiveMeshZSpriteBatch':0xbf9fa0,'?g_PrimitiveRenderer':0x1436e80,'?AddRepeatedStaticMesh':0xb32f70,'?AddRepeatedZSprite':0xb32ff0,'??_7CEnginePrimitiveHandle':0x122f598,'??1CBaseClass':0x99a2e0,'?IsObjectOffsetsInRange':0xbfb750,'?UpdateRepeatedStaticMesh':0xb31fe0,'??_7CLocalDetailPrimitiveMeshZSpriteBatch':0x12a82d8,'?g_LocalDetailEngine':0x1436ea8,'??1CObject':0xbfbbf0,'?FreeVirtual':0xbd9b90,'??_7CMovableResource':0x129b860}
for a,count in [('00bfaba0',5),('00bfacc0',5),('00bf9cc0',4),('00bfb590',8)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rels=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rels)==count;out=[]
 for off,typ,sym in rels:
  k=int(off,16);v=int(a,16)+k+4+struct.unpack('<i',rd(int(a,16)+k,4))[0]if typ=='DISP32'else u(int(a,16)+k);assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],sym;out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
expected={0x1231710:[0x4371f0],0x122f598:[0x41c500],0x12354d0:[0x4664e0,0x42d930],0x12a3d60:[0xb8ffe0,0x42d930,0xbfea08,0xb8ffc0,0xb8ffd0],0x129b860:[0x9d5110,0xbfea08,0x9d50f0,0x9d5100],0x12a82ac:[0xbfbc50,0xbf9cc0,0xbf9a80,0x9d5100,0xbdf6d0,0xbfaba0,0xbdf6e0,0xbf9b40,0xbfa2a0,0xbfbb90],0x12a82d8:[0xbfbd80,0xbfa8a0,0xbf9a80,0x9d5100,0xbfad90,0xbfacc0,0xbfbba0,0xbf9b90,0xbfb210,0xbfbbb0]};out={}
for vt,values in expected.items():
 assert [u(vt+4*k)for k in range(len(values))]==values
 col=u(vt-4);ch=u(col+16);h=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);h.append([rd(u(bc)+8,140).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 out[hex(vt)]={'slots':[hex(x)for x in values],'hierarchy':h}
for vt in [0x12a82ac,0x12a82d8]:assert out[hex(vt)]['hierarchy'][-2:]==[['.?AVCLocalDetailPrimitiveBase@@',0],['.?AVCMovableResource@@',0]]
# Fresh target objects preserve all real local-detail methods in their declared order.
methods=['destructor','RelocateData','CanRelocateData','Verify','RenderUpdate','AddObjectsToPrimitiveRenderer','NeedsRenderUpdate','OnPostRelocation','Load','GetType']
for a in ['00bfaba0','00bfacc0','00bf9cc0','00bfb590']:
 sy,secs=coff(next((o/'dry_work').rglob(a+'.obj')));tables={}
 for name,value,sec in sy.values():
  if not name.startswith('??_7CLocalDetailPrimitive'):continue
  size,rels=secs[sec-1];symbols=[rels[k]for k in range(value,size,4)if k in rels];got=['destructor'if x.startswith(('??_G','??_E'))else x.split('@')[0][1:]for x in symbols];assert got==methods,(a,name,got);tables[name]=[und(x)for x in symbols]
 out[a]={'fresh_CO​FF_virtuals':tables}
# Actual external result ABI and first-match range helper retain native signatures.
assert rd(0xb32f89,3)==bytes.fromhex('c20800')and rd(0xbfb758,3)==bytes.fromhex('c20400')
assert rd(0xbfb750,9)==bytes.fromhex('8b0185c0750532c0c2')
records=[json.loads(l)for l in (w/'ATTEMPTS.jsonl').read_text().splitlines()];assert len(records)==21
for x in records:
 p=w/'snapshots'/f"{x['address']}_{x['attempt']}.cpp";assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['effective_flags']==['/O2','/Oy','/W3']and x['prior_reserved_uncertain']==4
assert collections.Counter(x['address']for x in records)=={'00bfaba0':5,'00bfacc0':5,'00bf9cc0':5,'00bfb590':6}
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
for x in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(x[k]['path']).read_bytes()).hexdigest()==x[k]['sha256']
print('PASS22maskedoperands/33nativeVTslots+7RTTI/freshCOFFmethodorder/21immutablegrades+16reserved; BFB590 remains HELD missing actual renderer header at cap10')
