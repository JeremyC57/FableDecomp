#!/usr/bin/env python3
"""resetdd.py <selfprefix>: per family row whose self leaf starts with <selfprefix> (Reset / ctor(T*) stand-ins): T from the callers' PDB
spelling, the deleter the retail body stores (`mov dword [reg+4], imm32`, C7 4x 04), and the table binding at that deleter address.
-> evidence/dd_<tag>.tsv: addr T deleter dd_status dd_names blocked bytes_len"""
import sys,os,json,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from pe import PE; import nf
A=os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R='/home/user/FableDecomp'
pre=sys.argv[1]; tag=sys.argv[2]
fam=json.load(open(A+'/fam.json')); blk=json.load(open(A+'/evidence/blocked.json')); cat=nf.catalog()
rb=collections.defaultdict(set)
for l in open(R+'/rebuild/oracles/reloc_bindings.tsv'):
    f=l.rstrip('\n').split('\t')
    if len(f)>=2: rb[f[1].lower()].add(f[0])
ss={}
for l in open(R+'/rebuild/oracles/self_symbols.tsv'):
    f=l.rstrip('\n').split('\t')
    if len(f)>=2: ss.setdefault(f[0].lower().replace('0x',''),f[1])
out=[];C=collections.Counter()
for r in fam:
    if not r['self_leaf'].startswith(pre): continue
    a=r['target']; b=bytes.fromhex(cat[a]['bytes'])
    ds=[int.from_bytes(b[m.start()+3:m.start()+7],'little') for m in re.finditer(rb'\xc7[\x40-\x47]\x04',b)]; ds=[x for x in ds if x]
    d='%08x'%ds[0] if len(ds)==1 else ''
    pdb=r['pdb_spelling']
    mm=re.match(r'^(?:\?Reset@|\?\?0)\?\$CCountedPointer@(.*?)@@@@QAE',pdb)
    T=mm.group(1) if mm else ''
    names=rb.get(d,set())|({ss[d]} if d in ss else set())
    want='?DeleteData@?$CCountedPointer@%s@@@@SIXPAX@Z'%T
    st='NO_T' if not T else ('NO_DELETER' if not d else ('DD_OK' if want in names and len(names)==1 else ('DD_MULTI' if want in names else ('DD_OTHER' if names else 'DD_NONE'))))
    C[(st,bool(blk.get(a)))]+=1
    out.append([a,T,d,st,' ; '.join(sorted(names)),','.join(blk.get(a,[])),str(len(b))])
open(A+'/evidence/dd_%s.tsv'%tag,'w').write(''.join('\t'.join(x)+'\n' for x in out))
for k,v in C.most_common(): print(v,k)
for x in out:
    if x[3]!='DD_OK': print(x[:5])
