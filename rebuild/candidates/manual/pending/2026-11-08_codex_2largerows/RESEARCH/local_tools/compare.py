"""Instruction diagnostics only: addresses/branch destinations masked, never a byte-match gate."""
import argparse,difflib,json,os
from pathlib import Path
import grade
from coff_reader import coff
S=Path(__file__).resolve().parent
p=argparse.ArgumentParser();p.add_argument('address');p.add_argument('tag');a=p.parse_args()
d=S/'history'/a.address/a.tag;r=json.loads((d/'RESULT.json').read_text())
secs,syms=coff(d/(a.address+'.obj'));sym=next(x for x in syms.values()if x['name']==r['leaf']);sec=secs[sym['section']-1]
b=sec['data'];exe=Path(os.environ['FABLE_EXE']).read_bytes();va=int(a.address,16);off=grade.offset(exe,va);ret=exe[off:off+grade.LENGTHS[a.address]]
tail=1032 if a.address=='0041e6d3'else 0
for loose in (False,True):
 name='loose'if loose else 'strict';x=grade.disasm(ret[:len(ret)-tail],va,loose=loose);y=grade.disasm(b[:len(b)-tail],va,relocs=sec['relocs'],loose=loose)
 sm=difflib.SequenceMatcher(None,[i[1]for i in x],[i[1]for i in y],autojunk=False)
 hunks=[{'tag':t,'retail':[(hex(q[0]),q[2])for q in x[i:j]],'built':[(hex(q[0]),q[2])for q in y[k:l]],'indexes':[i,j,k,l]}for t,i,j,k,l in sm.get_opcodes()if t!='equal']
 (d/(name+'_hunks.json')).write_text(json.dumps(hunks,indent=2));r[name+'_instruction_ratio']=sm.ratio();r[name+'_hunks']=len(hunks);r[name+'_instruction_counts']=[len(x),len(y)]
 for label,lines in [('retail',x),('built',y)]:
  if not loose:(d/(label+'.disasm')).write_text('\n'.join('%08x %s'%(q[0],q[2])for q in lines)+'\n')
r['metric_note']='Diagnostic instruction similarity, relocation values and branch destinations normalized; not a byte-match score. Action excludes its 1032-byte jump tables only from instruction metric.'
(d/'RESULT.json').write_text(json.dumps(r,indent=2));print(json.dumps({k:v for k,v in r.items()if k in ['address','strategy','object_size','strict_instruction_ratio','loose_instruction_ratio','strict_hunks','loose_hunks']}))
