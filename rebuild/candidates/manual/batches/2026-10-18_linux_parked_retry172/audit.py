from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv,subprocess,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry172';w=r/'astra235_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'??0NativeInitBaseClass':0x99a310,'?InitialPreloadTime':0x122ed70,'??_7NativeDeviceResetCallback':0x122de50,'??_7EngineNative@@6BCIEngineNative':0x12a0f3c,'??_7EngineNative@@6BNativeDeviceResetCallback':0x12a0f30,'??0SimpleMemoryPoolNative':0xa5c570,'__ehhandler$':0x1217308,'__except_list':0,'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10,'__imp__WaitForSingleObject@8':0x143feac,'?Get@NativeCommandList':0xcb13a0,'?CheckTime@NativeDeferredCommandQueue':0xcae170,'?resize@NestedIDs':0xafe910,'?resize@PrimitiveIDs':0x4428f0,'?resize@Handles':0x443f90}
for a,n in [('00b260b0',7),('00caf8f0',12),('00afe9b0',3)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v];out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert rd(0x122ed70,8)==b'\0'*8
provenance=list(csv.reader((w/'logs/engine_vtable_provenance.tsv').open(),delimiter='\t'));api=Path('ghidra_out/engine_api.tsv').read_text();vt={'CIEngine':0x12a0d24,'CEngine':0x12a0f3c}
for owner,slot,target,name in provenance:
 assert u(vt[owner]+int(slot)*4)==int(target,16)
 if name and not name.startswith('`'):assert name in api,(owner,slot,name)
assert len(provenance)==171
for vt0,values in [(0x122de50,[0xbfea08]*2),(0x12a0f30,[0xb26010,0xb25d00]),(0x12a3d60,[0xb8ffe0,0x42d930,0xbfea08,0xb8ffc0,0xb8ffd0])]:assert [u(vt0+4*i)for i in range(len(values))]==values
out={}
for vt0,offset in [(0x12a0d24,0),(0x12a0f3c,0),(0x12a0f30,12),(0x122de50,0),(0x12a3d60,0)]:
 col=u(vt0-4);assert u(col+4)==offset;ch=u(col+16);h=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);h.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 out[hex(vt0)]={'object_offset':offset,'hierarchy':h}
assert out['0x12a0f3c']['hierarchy'][-1]==['.?AVCDeviceResetCallback@@',12]
assert out['0x12a3d60']['hierarchy']==[['.?AVCEnginePersistentPrimitiveBase@@',0],['.?AVCIVCountedPointeeBase@@',0]]
# Fresh production and fixture COFF signature checks against native engine_api prototypes.
for path in [o/'dry_work/00/b2/00b260b0/00b260b0.obj',o/'dry_work/00/b2/00b260b0/00b260b0.t.obj']:
 z=subprocess.run([sys.executable,str(o/'engine_vt_audit.py'),str(path)],capture_output=True,text=True);assert z.returncode==0,z.stdout+z.stderr
 report=json.loads((o/('engine_'+path.name+'_vtable_audit.json')).read_text());assert not report['errors'];out[path.name]=report
# Genuine one-state cleanup is lock release; four FS:0 operands are SEH chain, not globals.
assert [u(0x1371ab0+4*i)for i in range(7)]==[0x19930520,1,0x1371aa8,0,0,0,0]
assert [u(0x1371aa8+4*i)for i in range(2)]==[0xffffffff,0x1217300]
assert rd(0x1217300,3)==bytes.fromhex('8d4df0')
assert 0x1217308+struct.unpack('<i',rd(0x1217304,4))[0]==0xca48c0
out['queue_EH']={'info':'01371AB0','states':1,'unwind':'01217300','lock_destructor':'00CA48C0','cpp_handler':'00BFEB34','fs0_operands':4}
records=[json.loads(l)for l in (w/'ATTEMPTS.jsonl').read_text().splitlines()];assert len(records)==205
for x in records:
 p=Path(x['snapshot'])/'source.cpp';assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['compiler']and x['kind']=='grade';assert x['base_flags']==['/O2','/Oy','/W3']
counts=collections.Counter(x['address']for x in records);assert len(counts)==40 and max(counts.values())<=10
handoff=list(csv.DictReader((w/'HANDOFF.tsv').open(),delimiter='\t'));assert len(handoff)==40
for x in handoff:
 assert counts[x['address']]==int(x['grades_total']);assert int(x['grades_remaining'])==10-counts[x['address']]
 for key,hk in [('source','source_sha256'),('test','test_sha256')]:assert hashlib.sha256((w/x[key]).read_bytes()).hexdigest()==x[hk]
assert collections.Counter(x['status']for x in handoff)=={'LANDED':28,'READY':3,'DIFFER':9}
(o/'attempt_audit.json').write_text(json.dumps({'records':records,'whole40_handoff':handoff},indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
print('PASS22maskedoperands/180nativeVTslots/175production+177fixtureengine signatures/5RTTI/oneEHcleanup/205immutablegrades+whole40hashes')
