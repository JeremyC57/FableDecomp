import re, os, json
ROOT='/home/user/FableDecomp'
def ps1_entries():
    ps=open(ROOT+'/rebuild/build_candidates.ps1',encoding='utf-8-sig').read()
    ent={}
    for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
        kv=dict(re.findall(r"(\w+) = '([^']*)'", blk))
        if 'Address' in kv: ent[kv['Address'].lower()]=kv
    return ent
def catalog():
    cat={}
    for l in open(ROOT+'/rebuild/oracles/auto-re-candidates.tsv'):
        q=l.rstrip('\n').split('\t')
        if q[0]!='address' and len(q)>=4: cat[q[0].lower()]=q
    return cat
_pdb=None
def pdb():
    global _pdb
    if _pdb is None:
        E=set(l.rstrip('\n').split('\t',1)[1] for l in open(ROOT+'/ghidra_out/egor_pdb_names.tsv') if '\t' in l)
        F=set(l.rstrip('\n').split('\t',1)[1] for l in open(ROOT+'/ghidra_out/fablewin_pdb_names.tsv') if '\t' in l)
        _pdb=(E,F)
    return _pdb
def fname(qual):
    return re.sub(r'[^A-Za-z0-9]','',qual)
