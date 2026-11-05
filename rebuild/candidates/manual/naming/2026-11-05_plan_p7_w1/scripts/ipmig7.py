# ipmig7.py W [addr...|@file] [--base W0 ...] : plan phase 7 wave 3, the CIntelligentPointer leftovers that the CThing header wave
# unblocks. For a landed row that already includes engine/CThing.h (phase 6 / phase 7 w2) and still defines the canonical local
# CBaseIntelligentPointer / CIntelligentPointer<T> stand-ins (vptr + PData; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50,
# SetPItem 0x00A01B90; a typed `T* GetPItem() const` / `void SetPItem(T*)` in the template), the two blocks are replaced by the shared
# engine/CIntelligentPointer.h under FABLE_RETAIL_CINTELLIGENTPOINTER (the byte-packed retail branch every phase 5 w3 row took) and
# the typed calls take the header's names: `p.GetPItem()` -> `p.Get()`, `p.SetPItem(x)` -> `p.Set(x)` on CIntelligentPointer<T>
# objects (FableWin.pdb ?Get@?$CIntelligentPointer@..@@QBEPAV..@XZ, inline: the same GetPItem 0x00A01B50 call). Objects declared as
# the plain CBaseIntelligentPointer keep GetPItem / SetPItem. Anything else defers the row. The gate decides the bytes.
import sys,os,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; args=sys.argv[2:]; BASES=[]
while '--base' in args: i=args.index('--base'); BASES.append(args[i+1]); del args[i:i+2]
addrs=[]
for a in args: addrs+=[l.split()[0] for l in open(a[1:]) if l.strip()] if a.startswith('@') else [a]
def text(a):
    src=L.src_rel(a); t=L.read(L.ROOT+'/'+src)
    for W0 in BASES:
        if os.path.exists(L.ov_path(W0,src)): t=L.read(L.ov_path(W0,src))
    return t
class Defer(Exception): pass
def cut(t,rx):
    b=L.blank_comments(t); m=re.search(rx,b)
    if not m: return t,None
    s=b.find('{',m.start()); blk=L.block(t,s,b); k=t.find(';',s+len(blk))
    if k<0 or t[s+len(blk):k].strip(): raise Defer('block end')
    e=t.find('\n',k); e=len(t) if e<0 else e
    if t[k+1:e].strip() and not t[k+1:e].strip().startswith('//'): e=k+1
    return t[:m.start()]+'@@IPCUT@@'+t[e:],blk
def migrate(a):
    t=text(a)
    if 'engine/CThing.h' not in t: raise Defer('no engine/CThing.h')
    if 'engine/CIntelligentPointer.h' in t: raise Defer('already includes the header')
    t,bb=cut(t,r'(?<![\w:])(class|struct)\s+CBaseIntelligentPointer\s*\{')
    if bb is None: raise Defer('no local CBaseIntelligentPointer block')
    t,tb=cut(t,r'template\s*<\s*class\s+\w+\s*>\s*(class|struct)\s+CIntelligentPointer\s*:\s*public\s+CBaseIntelligentPointer\s*\{')
    if tb is None: raise Defer('no local CIntelligentPointer<T> block')
    # the canonical stand-ins only: vptr + PData, the four retail members, the typed accessors
    bc=re.sub(r'//[^\n]*','',bb)
    for need in (r'virtual\s+~CBaseIntelligentPointer\s*\(\s*\)',r'GetPItem\s*\(\s*\)\s*const',r'\bPData\b'):
        if not re.search(need,bc): raise Defer('non-canonical CBaseIntelligentPointer model')
    if re.search(r'\b(operator|IsValid|IsNull|Release|Reset)\b',bc): raise Defer('local base model has extra members')
    tc=re.sub(r'//[^\n]*','',tb)
    KW=('return','sizeof','static_cast','reinterpret_cast','const_cast','if','CBaseObject','const')
    extra=[n for n in re.findall(r'\b(\w+)\s*\(',tc) if n not in ('GetPItem','SetPItem','CIntelligentPointer','CBaseIntelligentPointer','T')+KW]
    subs=[]
    if re.search(r'\boperator\s*=\s*\(',tc): subs.append('FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN')
    if re.search(r'\bCIntelligentPointer\s*\(\s*const\s+CIntelligentPointer\b',tc): subs.append('FABLE_RETAIL_CINTELLIGENTPOINTER_COPY')
    if re.search(r'\boperator\s*(?!=)\S+\s*\(',tc): raise Defer('local CIntelligentPointer<T> has an extra operator')
    if extra: raise Defer('local CIntelligentPointer<T> has extra members: '+','.join(sorted(set(extra))[:4]))
    t=t.replace('@@IPCUT@@','#define FABLE_RETAIL_CINTELLIGENTPOINTER\n'+''.join('#define %s\n'%x for x in subs)+'#include "engine/CIntelligentPointer.h"',1).replace('@@IPCUT@@','')
    if len(re.findall(r'#\s*define\s+FABLE_RETAIL_CINTELLIGENTPOINTER\b',t))>1: raise Defer('macro already defined')
    b=L.blank_comments(t)
    base_names=set(re.findall(r'(?<![\w:<])CBaseIntelligentPointer\s*[&*]?\s*(\w+)\s*[;,=)\[]',b))
    if re.search(r'\bCBaseIntelligentPointer\s*::\s*(GetPItem|SetPItem)\b',b) and not re.search(r'\bclass\s+\w+\s*:\s*public\s+CBaseIntelligentPointer\b',b): pass
    out=[]; i=0; n=0
    for m in re.finditer(r'(\b\w+(?:\s*\[[^\]]*\])?|\))\s*(\.|->)\s*(GetPItem|SetPItem)\s*\(',b):
        if m.start()<i: continue
        recv=m.group(1)
        if recv in base_names: continue
        out.append(t[i:m.start(3)]); out.append('Get' if m.group(3)=='GetPItem' else 'Set'); i=m.end(3); n+=1
    out.append(t[i:]); t=''.join(out)
    if not n and not re.search(r'CIntelligentPointer\s*<',L.blank_comments(t)): raise Defer('no typed use')
    return t,'migrated GetPItem/SetPItem->Get/Set x%d%s%s'%(n,' +'+'+'.join(x.split('_')[-1] for x in subs) if subs else '',' base:'+','.join(sorted(base_names)) if base_names else '')
res=[]
for a in addrs:
    try: txt,note=migrate(a); L.write(L.ov_path(W,L.src_rel(a)),txt); res.append((a,'MIGRATED: '+note))
    except Defer as d: res.append((a,'UNCHANGED: '+str(d)))
    except Exception as ex: res.append((a,'UNCHANGED: error '+type(ex).__name__+' '+str(ex)[:100]))
with open(W+'/rows.tsv','w') as o:
    o.write('address\trule\n')
    for a,r in res: o.write(a+'\t'+r+'\n')
print(collections.Counter(r.split(':')[0]+(':'+r.split(':')[1][:40] if r.startswith('UNCH') else '') for a,r in res).most_common(12))
