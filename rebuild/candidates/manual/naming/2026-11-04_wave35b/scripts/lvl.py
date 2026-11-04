# level-script helpers: data type of CLevelScriptBase<T>, Ego_r member at retail offset (Ego_r - 0x14)
from common35 import *
_D=None; _L=None
def datatype(cls):
    global _D
    if _D is None:
        _D={l.split('\t')[0]:l.split('\t')[1] for l in open(W+'/an/lsb_data.tsv')}
    return _D.get(cls)
def egor(cls):
    global _L
    if _L is None:
        _L={}
        for l in open(ROOT+'/ghidra_out/struct_layouts_egor.tsv'):
            a=l.rstrip('\n').split('\t')
            if a[0]=='@STRUCT' or len(a)<5: continue
            _L.setdefault(a[0],[]).append((int(a[2]),a[3],a[4],int(a[1])))
    return _L.get(cls)
def egor_member(cls,retail_off,delta=0x14):
    for o,ty,nm,sz in egor(cls) or []:
        if o==retail_off+delta and nm!='_padding_': return ty,nm
    return None
def egor_size(cls):
    e=egor(cls); return e[0][3] if e else None
def plain_script(cls):
    """CGlobal_* / CExpression_* scripts: PDB ctor (CGameScriptInterfaceBase&) only -> CScriptBase + Game +0x40"""
    E,F=pdb()
    return ('??0%s@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z'%cls) in (E|F)
STL_DELTA=[('vector<',0x10-0xc),('list<',0xc-4),('map<',0x10-0xc),('multimap<',0x10-0xc),('set<',0x10-0xc),('basic_string<',0x1c-0xc),('deque<',0x18-0x28)]
def egor_retail(cls):
    """[(retail_off, type, name)] for a level script: Ego_r - 0x14 (script base), minus the STLport size deltas of earlier containers.
    Stops (returns None after) at an unknown-size container or a field whose retail offset would need re-alignment."""
    e=egor(cls)
    if not e: return None
    out=[]; delta=0x14
    for o,ty,nm,sz in sorted(e):
        if nm=='_padding_' or o<0x54: continue
        out.append((o-delta,ty,nm))
        for p,dd in STL_DELTA:
            if ty.startswith(p) or ty.startswith('std::'+p): delta+=dd; break
    return out
def egor_member_rt(cls,off):
    for o,ty,nm in egor_retail(cls) or []:
        if o==off: return ty,nm
    return None
