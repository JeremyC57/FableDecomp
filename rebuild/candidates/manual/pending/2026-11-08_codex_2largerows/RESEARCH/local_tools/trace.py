"""Local diagnostic-only compiler trace; always compare output with a normal RTM grade."""
import argparse,hashlib,json,os,subprocess,sys,time
from pathlib import Path
import grade
from coff_reader import coff
V=grade.V;S=grade.S
p=argparse.ArgumentParser();p.add_argument('address',choices=grade.LENGTHS);p.add_argument('tag');a=p.parse_args()
d=S/'history'/a.address/a.tag;r=json.loads((d/'RESULT.json').read_text());assert r['status']in ('DIFFER','MATCH','RELOCATION_MATCH')
e=V.env();e['INCLUDE']=';'.join(V.wpath(x)for x in[d/'hdr',S/'support/rebuild/include',V.VC/'include'])
log=d/'allocator_trace.txt';obj=d/'diagnostic.obj'
e.update(C2_TRACE_ORIGINAL=V.wpath(V.VC/'bin/c2.dll'),C2_TRACE_LOG=V.wpath(log),C2_TRACE_FUNCTION=r['leaf'])
e.pop('C2_TRACE_OFF',None)
cmd=[V.WINE,str(V.CL_EXE)]+V.tool_args(['/c','/O2','/Oy','/W3']+r['flags']+['/B2'+V.wpath(S/'runtime/c2trace_jumps.dll'),'/Fo'+str(obj),str(d/'candidate.cpp')])
t=time.time();cp=subprocess.run(cmd,env=e,capture_output=True,text=True,cwd=d,timeout=240)
(d/'diagnostic_compiler.log').write_text(cp.stdout+cp.stderr)
assert cp.returncode==0,(cp.returncode,cp.stdout,cp.stderr)
def leaf_data(path):
 sec,syms=coff(path);x=next(x for x in syms.values()if x['name']==r['leaf']and x['section']>0);return sec[x['section']-1]['data']
same=leaf_data(obj)==leaf_data(d/(a.address+'.obj'))
rec={'ordinary_leaf_byte_identical':same,'seconds':time.time()-t,'trace':str(log),'original_c2_sha256':hashlib.sha256((V.VC/'bin/c2.dll').read_bytes()).hexdigest(),'diagnostic_only':True}
(d/'DIAGNOSTIC_VALIDATION.json').write_text(json.dumps(rec,indent=2));print(json.dumps(rec));assert same
