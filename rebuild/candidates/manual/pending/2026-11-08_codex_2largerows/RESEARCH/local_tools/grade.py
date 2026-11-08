"""Two-row, local-only RTM compiler experiments. Never lands or edits a repository."""
import argparse,difflib,hashlib,json,os,re,shutil,struct,subprocess,sys,time
from pathlib import Path
S=Path(__file__).resolve().parent
sys.path.insert(0,'/workspace/FableDecomp-pass9/tools/decomp_pipeline')
sys.path.insert(0,'/workspace/work/pass13_100_20261103/tools')
import verify_and_land as V
from coff_reader import coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
LENGTHS={'0041e6d3':35651,'00cbfb7d':74778}
def offset(exe,va):
 p=struct.unpack_from('<I',exe,0x3c)[0];n=struct.unpack_from('<H',exe,p+6)[0];o=struct.unpack_from('<H',exe,p+20)[0]
 for i in range(n):
  vs,rva,rs,rp=struct.unpack_from('<IIII',exe,p+24+o+40*i+8)
  if rva<=va-0x400000<rva+rs:return va-0x400000-rva+rp
 raise ValueError('outside PE sections')
def disasm(data,addr,relocs=None,loose=False):
 if relocs:
  data=bytearray(data)
  for o,idx,typ in relocs:
   if o+4<=len(data):struct.pack_into('<I',data,o,(0x1000000-addr-o-4)&0xffffffff if typ==20 else 0x1000000)
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;out=[]
 for i in md.disasm(data,addr):
  op=i.op_str
  if i.mnemonic.startswith('j') or i.mnemonic in ('call','loop'):
   op=re.sub(r'0x[0-9a-f]+','TARGET',op)
  else:
   op=re.sub(r'0x[0-9a-f]+',lambda m:'ADDR' if int(m[0],16)>=0x400000 else m[0],op)
  if loose:
   op=re.sub(r'\b(?:eax|ebx|ecx|edx|esi|edi)\b','REG',op)
   op=re.sub(r'\b(?:al|bl|cl|dl)\b','BYTE',op)
   op=re.sub(r'\[(e[bs]p)(?: [-+] 0x[0-9a-f]+)?\]',r'[\1 SLOT]',op)
  out.append((i.address,i.mnemonic+' '+op,i.mnemonic+' '+i.op_str))
 return out
def main():
 ap=argparse.ArgumentParser();ap.add_argument('address',choices=LENGTHS);ap.add_argument('tag');ap.add_argument('--source');ap.add_argument('--headers');a=ap.parse_args()
 addr=a.address;source=Path(a.source or S/'rows'/addr/(addr+'.cpp'));headers=Path(a.headers or S/'rows'/addr/'hdr')
 text=source.read_text();leaf=re.findall(r'^// Leaf:\s*(\S+)',text,re.M);assert len(leaf)==1;leaf=leaf[0]
 flags=re.findall(r'^// Flags:\s*(.*)',text,re.M);flags=flags[0].split()if flags else []
 d=S/'history'/addr/a.tag;d.mkdir(parents=True,exist_ok=False)
 shutil.copy2(source,d/'candidate.cpp');shutil.copytree(headers,d/'hdr')
 r={'address':addr,'strategy':a.tag,'leaf':leaf,'flags':flags,'source_sha256':hashlib.sha256(text.encode()).hexdigest(),'started':time.time(),'status':'STARTED'}
 (d/'RESULT.json').write_text(json.dumps(r,indent=2))
 inc=[d/'hdr',S/'support/rebuild/include',V.VC/'include']
 e=V.env();e['INCLUDE']=';'.join(V.wpath(x)for x in inc)
 # Native preprocessing catches missing headers and fatal directives before Wine.
 pre=subprocess.run(['g++','-E','-P','-std=gnu++03','-D_WIN32','-D_MSC_VER=1310','-D_M_IX86=600','-nostdinc++','-I'+str(inc[0]),'-I'+str(inc[1]),'-I'+str(V.VC/'include'),str(d/'candidate.cpp')],capture_output=True,text=True)
 (d/'preprocess.log').write_text(pre.stderr)
 if pre.returncode:
  r.update(status='PREPROCESS_FAIL',errors=pre.stderr[-5000:]);(d/'RESULT.json').write_text(json.dumps(r,indent=2));print(json.dumps(r));return
 obj=d/(addr+'.obj')
 cmd=[V.WINE,str(V.CL_EXE)]+V.tool_args(['/c','/O2','/Oy','/W3']+flags+['/Fo'+str(obj),str(d/'candidate.cpp')])
 t=time.time()
 try:
  cp=subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e,cwd=d,timeout=240)
 except subprocess.TimeoutExpired:
  r.update(status='COMPILE_TIMEOUT');(d/'RESULT.json').write_text(json.dumps(r,indent=2));print(json.dumps(r));return
 (d/'compiler.log').write_text(cp.stdout+cp.stderr);r['compile_seconds']=time.time()-t
 if cp.returncode or not obj.exists():
  r.update(status='COMPILE_FAIL',errors=(cp.stdout+cp.stderr)[-6000:]);(d/'RESULT.json').write_text(json.dumps(r,indent=2));print(json.dumps(r));return
 sec,syms=coff(obj);hits=[x for x in syms.values()if x['name']==leaf and x['section']>0]
 assert len(hits)==1 and hits[0]['value']==0, 'exact Leaf must select one whole section'
 section=sec[hits[0]['section']-1];built=section['data'];rels=section['relocs'];rel=[x[0]for x in rels]
 exe=Path(os.environ['FABLE_EXE']).read_bytes();assert hashlib.sha1(exe).hexdigest()=='39fd416ba23a91a89713ec80c789f57ddb22e2c8'
 off=offset(exe,int(addr,16));retail=exe[off:off+LENGTHS[addr]]
 status='MATCH'if built==retail else 'RELOCATION_MATCH'if V.mask(built,rel)==V.mask(retail,rel)else 'DIFFER'
 r.update(status=status,object_size=len(built),retail_size=len(retail),first_difference=next((i for i,(x,y)in enumerate(zip(V.mask(built,rel),V.mask(retail,rel)))if x!=y),None))
 # Raw bytes remain local evidence only, excluded from handback artifacts.
 (d/'built.bin').write_bytes(built)
 (d/'relocations.json').write_text(json.dumps([{'offset':o,'type':typ,'symbol':syms[idx]}for o,idx,typ in rels],indent=2))
 (d/'symbols.txt').write_text(subprocess.run(['objdump','-t',str(obj)],capture_output=True,text=True,check=True).stdout)
 for loose in (False,True):
  name='loose'if loose else 'strict'
  tail=1032 if addr=='0041e6d3'else 0
  x=disasm(retail[:len(retail)-tail],int(addr,16),loose=loose);y=disasm(built[:len(built)-tail],int(addr,16),relocs=rels,loose=loose)
  xx=[i[1]for i in x];yy=[i[1]for i in y]
  sm=difflib.SequenceMatcher(None,xx,yy,autojunk=False)
  r[name+'_instruction_ratio']=sm.ratio();r[name+'_instruction_counts']=[len(x),len(y)]
  hunks=[]
  for tag,i,j,k,l in sm.get_opcodes():
   if tag=='equal':continue
   hunks.append({'tag':tag,'retail':[(hex(q[0]),q[2])for q in x[i:j]],'built':[(hex(q[0]),q[2])for q in y[k:l]],'indexes':[i,j,k,l]})
  (d/(name+'_hunks.json')).write_text(json.dumps(hunks,indent=2));r[name+'_hunks']=len(hunks)
  if not loose:
   for label,lines in [('retail',x),('built',y)]:
    (d/(label+'.disasm')).write_text('\n'.join('%08x %s'%(q[0],q[2])for q in lines)+'\n')
 r['finished']=time.time();(d/'RESULT.json').write_text(json.dumps(r,indent=2));print(json.dumps(r),flush=True)
if __name__=='__main__':main()
