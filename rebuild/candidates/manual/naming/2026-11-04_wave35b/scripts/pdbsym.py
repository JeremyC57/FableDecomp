import re
from rows import *
_E=_F=None
def load():
    global _E,_F
    if _E is None:
        _E=set(l.rstrip('\n').split('\t',1)[1] for l in open(ROOT+'/ghidra_out/egor_pdb_names.tsv') if '\t' in l)
        _F=set(l.rstrip('\n').split('\t',1)[1] for l in open(ROOT+'/ghidra_out/fablewin_pdb_names.tsv') if '\t' in l)
    return _E,_F
def prefix(name):
    parts=name.split('::')
    # handle templates crudely: not expected
    meth=parts[-1]; cls=parts[:-1]
    q='@'.join(reversed(cls))
    if len(cls)>=1 and meth==cls[-1]: return '??0'+q+'@@'
    if meth.startswith('~'): return '??1'+q+'@@'
    return '?'+meth+'@'+q+'@@'
def find(name):
    E,F=load(); p=prefix(name)
    e=sorted(s for s in E if s.startswith(p)); f=sorted(s for s in F if s.startswith(p))
    return p,e,f
