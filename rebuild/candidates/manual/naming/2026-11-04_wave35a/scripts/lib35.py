import re,os,json,glob,collections,subprocess
from common import *
from hdrparse import *
W=os.environ['S']+'/wave35a'
_ev=None
def evidence():
    global _ev
    if _ev is None:
        _ev={}
        A=ROOT+'/rebuild/candidates/manual/naming/2026-11-04_anniversary/'
        for f in (A+'labels_waves23.tsv',A+'labels_high.tsv',A+'wave3/labels_alloc.tsv'):
            for l in open(f):
                q=l.rstrip('\n').split('\t')
                if len(q)>=3 and q[0]!='address': _ev[q[0]]=(q[1],q[2],os.path.relpath(f,ROOT))
    return _ev
_pdb=None
def pdbmap():
    global _pdb
    if _pdb is None:
        _pdb=collections.defaultdict(set)
        for fn,tag in (('egor_pdb_names.tsv','Ego_r.pdb'),('fablewin_pdb_names.tsv','FableWin.pdb')):
            for l in open(ROOT+'/ghidra_out/'+fn):
                if '\t' in l: _pdb[l.rstrip('\n').split('\t',1)[1]].add(tag)
    return _pdb
def pdbs(sym): return ' / '.join(sorted(pdbmap().get(sym,())))
_incl=None
def includers(h):
    global _incl
    if _incl is None:
        _incl=collections.defaultdict(list)
        for p in glob.glob(ROOT+'/rebuild/src/compiled/*/*/*.cpp'):
            s=open(p,errors='replace').read()
            for m in re.finditer(r'#include\s+"(engine/[^"]+)"',s): _incl[m.group(1)].append(p)
    return _incl.get(h,[])
def defines_for(h):
    """#define lines (FABLE_*) that a landed includer of h puts before the include"""
    c=collections.Counter()
    for p in includers(h)[:40]:
        s=open(p,errors='replace').read()
        i=s.find('#include "%s"'%h)
        ds=tuple(sorted(set(re.findall(r'^#define\s+(FABLE_\w+)\s*$',s[:i],re.M))))
        c[ds]+=1
    if c: return list(c.most_common(1)[0][0])
    return ['FABLE_RETAIL_NSCRIPT']
def strip_head(src):
    lines=src.split('\n'); i=0
    while i<len(lines) and lines[i].startswith('//'): i+=1
    return lines[:i],'\n'.join(lines[i:])
def access_of(sym):
    # ?M@...@@<A>... where A: U/V public, E/F private, M/N protected (virtual)
    m=re.match(r'^\?[^@]+@(?:[^@]+@)+@([A-Z])',sym)
    if not m: return None
    return {'U':'public','E':'private','M':'protected','Q':'public','A':'private','I':'protected'}.get(m.group(1))
def first_comment(src):
    out=[]
    for l in src.split('\n'):
        if not l.startswith('//'): break
        if l.startswith('// Leaf:'): continue
        out.append(l[2:].strip())
        if len(out)==2: break
    t=' '.join(out)
    return t if len(t)<=220 else t[:217]+'...'
def objsyms(obj):
    out=subprocess.run(['objdump','-t',obj],capture_output=True,text=True).stdout
    return set(re.findall(r'\s(\S+)$',out,re.M))
def fix_cp(s):
    """wave 33 D4: replace an Alloc row's local CCountedPointer / CCPPointerInfo stand-in (deleter Destroy_00cdee00) by the shared header"""
    i=s.find('class CCPPointerInfo'); t=s.find('template <class T> class CCountedPointer')
    if i<0 or t<0 or t<i: return s,False
    k=s.find('\n};\n',t)
    if k<0: return s,False
    k+=4
    pre=s[:i]
    pre2=re.sub(r'^void __fastcall Destroy_00cdee00\(void\* object\);[^\n]*\n\n?','',pre,flags=re.M)
    if pre2==pre: return s,False
    s=pre2+'// CCountedPointer<T> / CCPPointerInfo: the shared retail model (engine/CCountedPointer.h; 0x00CDEE00 is\n//   CCountedPointer<CEntityScriptBase>::DeleteData), in place of the Alloc row\'s local stand-in (deleter Destroy_00cdee00; wave 33 D4).\n#include "engine/CCountedPointer.h"\n'+s[k:]
    return s,True
_V=None
def qcheck(addr, src, leaf, extra=()):
    """quick parity: (status, objpath) -- own work dir under $W/qwork"""
    global _V
    import sys as _s
    from pathlib import Path
    if _V is None:
        _s.path.insert(0,ROOT+'/tools/decomp_pipeline'); import verify_and_land as V; _V=V
    V=_V
    cat=catalog() if not hasattr(qcheck,'cat') else qcheck.cat
    qcheck.cat=cat
    retail=bytes.fromhex(cat[addr][3])
    while retail and retail[-1] in (0xCC,0x90): retail=retail[:-1]
    work=Path(W)/'qwork'/addr; work.mkdir(parents=True,exist_ok=True)
    st,built,sec=V.parity_of(V.vc71(src),addr,leaf,retail,work,V.env(),extra=tuple(extra))
    return st,str(work/(addr+'.obj'))
def prelude_for(h):
    """the #define / #include lines a landed includer of h puts before (and including) its include of h; shortest first"""
    cands=[]
    for p in includers(h):
        s=open(p,errors='replace').read()
        i=s.find('#include "%s"'%h)
        if i<0: continue
        pre=[l for l in s[:i].split('\n') if l.startswith(('#define','#include','#undef'))]
        cands.append((len(pre),pre,os.path.relpath(p,ROOT)))
    cands.sort(key=lambda x:x[0])
    return [(c[1]+['#include "%s"'%h],c[2]) for c in cands]
def call_targets(addr):
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    cat=catalog() if not hasattr(call_targets,'cat') else call_targets.cat
    call_targets.cat=cat
    b=bytes.fromhex(cat[addr][3]); md=Cs(CS_ARCH_X86,CS_MODE_32); out=[]
    for i in md.disasm(b,int(addr,16)):
        if i.mnemonic in ('call','jmp') and i.op_str.startswith('0x'):
            t=int(i.op_str,16)
            if not (int(addr,16)<=t<int(addr,16)+len(b)): out.append(t)
    return out
