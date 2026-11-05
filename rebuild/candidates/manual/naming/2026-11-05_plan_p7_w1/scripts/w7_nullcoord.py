# w7_nullcoord.py W ADDR... : plan phase 7 wave 1, 0x0143E8E0 -> ?NullCoord@CScriptThing@@1VC3DVector@@A (Ego_r.pdb `dynamic initializer
# for 'CScriptThing::NullCoord'`; CScriptThing::GetPos 0x004AA980 returns this address for a null PImp; engine/CScriptThing.h
# declares it `protected: static C3DVector NullCoord;`; queue tag land0511h). For each binder row:
#   - the `extern [const] C3DVector g_ZeroVector|g_ZeroPos;` line goes;
#   - every use must sit in a CScriptThing member (inline in a local `class|struct CScriptThing` block, or an out-of-class
#     `CScriptThing::` / `CScriptThing_Methods::` definition, CScriptThing_Methods deriving CScriptThing): it becomes NullCoord;
#   - a local CScriptThing block gets `protected: static C3DVector NullCoord; public:` (the header's declaration) when it lacks it.
# A use anywhere else (a free function or another class: protected access) defers the row. Writes W/overlay + W/rows_nullcoord.tsv.
import sys,os,re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; addrs=sys.argv[2:]
if len(addrs)==1 and addrs[0].startswith('@'): addrs=[l.split()[0] for l in open(addrs[0][1:]) if l.strip()]
NAMES=r'(?:g_ZeroVector|g_ZeroPos)'
def text(a):
    p=L.ov_path(W,L.src_rel(a)); return L.read(p) if os.path.exists(p) else L.read(L.ROOT+'/'+L.src_rel(a))
def enclosing_def(b,pos):
    """the qualified name of the out-of-class function definition around pos (None at namespace scope / inside a class)"""
    best=None
    for m in re.finditer(r'(?m)^[^\n;{}#]*?\b((?:\w+::)+)(~?\w+)\s*\([^;{}]*\)\s*(?:const)?\s*\{',b[:pos]):
        s=b.find('{',m.end()-1); blk=L.block(b,s)
        if s+len(blk)>pos: best=m.group(1)
    return best
out=[]
for a in addrs:
    t=text(a); b=L.blank_comments(t)
    uses=[m.start() for m in re.finditer(r'(?<![\w:])'+NAMES+r'\b',b) if not re.match(r'extern',b[max(0,b.rfind('\n',0,m.start())+1):m.start()].strip())]
    uses=[u for u in uses if not re.search(r'extern\s+(const\s+)?C3DVector\s*$',b[b.rfind('\n',0,u)+1:u])]
    # the local CScriptThing blocks
    blocks=[(b.find('{',m.start()),L.block(t,b.find('{',m.start()),b)) for m in L.class_rx('CScriptThing').finditer(b)]
    bad=[]; inblock=set()
    for u in uses:
        ib=[i for i,(s,blk) in enumerate(blocks) if s<u<s+len(blk)]
        if ib: inblock.add(ib[0]); continue
        q=enclosing_def(b,u)
        if q and q.rstrip(':').split('::')[-1] in ('CScriptThing','CScriptThing_Methods'): continue
        bad.append(t[max(0,b.rfind('\n',0,u)+1):b.find('\n',u)].strip()[:90])
    if not uses: out.append((a,'UNCHANGED: no use found')); continue
    if bad: out.append((a,'UNCHANGED: use outside a CScriptThing member: '+bad[0])); continue
    # rewrite: uses -> NullCoord; local blocks get the static member; the extern lines go
    for i in sorted(inblock,reverse=True):
        s,blk=blocks[i]
        if not re.search(r'\bNullCoord\b',blk):
            ins='\nprotected:\n    static C3DVector NullCoord;                        // 0x0143E8E0 ?NullCoord@CScriptThing@@1VC3DVector@@A (Ego_r.pdb; engine/CScriptThing.h)\npublic:'
            # keep the block's first access label: insert after `{ public:` / `{` line
            mp=re.match(r'\{[ \t]*(public:)?[^\n]*',blk); k=s+mp.end()
            t=t[:k]+ins+t[k:]
    b=L.blank_comments(t)
    for m in list(re.finditer(r'(?<![\w:])'+NAMES+r'\b',b))[::-1]:
        line=b[b.rfind('\n',0,m.start())+1:m.start()]
        if re.search(r'extern\s+(const\s+)?C3DVector\s*$',line): continue
        t=t[:m.start()]+'NullCoord'+t[m.end():]
    t=re.sub(r'(?m)^[ \t]*extern\s+(const\s+)?C3DVector\s+'+NAMES+r'\s*;[^\n]*\n','',t)
    L.write(L.ov_path(W,L.src_rel(a)),t); out.append((a,'RESPELL: 0143e8e0 -> ?NullCoord@CScriptThing@@1VC3DVector@@A (%d uses)'%len(uses)))
with open(W+'/rows_nullcoord.tsv','w') as o:
    o.write('address\trule\n')
    for a,r in out: o.write(a+'\t'+r+'\n')
import collections; print(collections.Counter(r.split(':')[0] for a,r in out))
for a,r in out:
    if r.startswith('UNCHANGED'): print(a,r[:140])
