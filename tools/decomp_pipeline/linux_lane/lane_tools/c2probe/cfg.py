"""cfg.py: crude function recovery for c2.dll from objdump output (basic-block-reordered code, no int3 padding).
Builds instruction map, call-target set, follows control flow (incl. jump tables) per entry. Saves work/funcs.json."""
import re,os,json,struct,bisect,sys
S=os.environ['S']; W=S+'/w22/c2probe/work'
d=open(S+'/wine/vc71rtm/bin/c2.dll','rb').read()
BASE=0x10700000
pe=struct.unpack_from('<I',d,0x3c)[0]; nsec=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(nsec):
    s=pe+24+opt+40*i; name=d[s:s+8].rstrip(b'\0').decode(); vs,va,rs,ra=struct.unpack_from('<IIII',d,s+8); secs.append((name,BASE+va,vs,ra,rs))
def rd(va,n):
    for name,sva,vs,ra,rs in secs:
        if sva<=va<sva+rs: return d[ra+va-sva:ra+va-sva+n]
    return None
TEXT_LO,TEXT_HI=0x10701000,0x10701000+0x164d47
ins={}   # addr -> (text, nextaddr)
order=[]
for l in open(W+'/c2.dis'):
    m=re.match(r'^\s*([0-9a-f]+):\s+(.*)$',l)
    if m:
        a=int(m.group(1),16); t=m.group(2).strip()
        if order: ins[order[-1]]=(ins[order[-1]][0],a)
        ins[a]=(t,None); order.append(a)
ins[order[-1]]=(ins[order[-1]][0],TEXT_HI)
def target(t):
    m=re.match(r'^(j\w+|call|loop\w*)\s+0x([0-9a-f]+)$',t)
    return int(m.group(2),16) if m else None
calls=set()
for a,(t,n) in ins.items():
    if t.startswith('call'):
        tg=target(t)
        if tg and TEXT_LO<=tg<TEXT_HI: calls.add(tg)
def jumptable(t):
    m=re.search(r'jmp\s+DWORD PTR \[\w+\*4\+0x([0-9a-f]+)\]',t)
    if not m: return []
    base=int(m.group(1),16); out=[]
    for i in range(512):
        b=rd(base+4*i,4)
        if not b: break
        v=struct.unpack('<I',b)[0]
        if not (TEXT_LO<=v<TEXT_HI) or v not in ins: break
        out.append(v)
    return out
def follow(entry):
    seen=set(); work=[entry]
    while work:
        a=work.pop()
        while a is not None and a not in seen and a in ins:
            seen.add(a); t,n=ins[a]
            if t.startswith('ret') or t.startswith('int3'): break
            if t.startswith('jmp'):
                tg=target(t)
                if tg: 
                    if tg in calls and tg!=entry: break   # tail call to another function
                    work.append(tg)
                else: work.extend(jumptable(t))
                break
            if t.startswith('j') or t.startswith('loop'):
                tg=target(t)
                if tg: work.append(tg)
            a=n
    return seen
if __name__=='__main__':
    funcs={}
    for e in sorted(calls):
        funcs[e]=sorted(follow(e))
    json.dump({hex(k):v for k,v in funcs.items()},open(W+'/funcs.json','w'))
    print(len(calls),'call targets')
    # map: address -> owning function(s)
    owner={}
    for e,addrs in funcs.items():
        for a in addrs: owner.setdefault(a,[]).append(e)
    json.dump({hex(k):[hex(x) for x in v] for k,v in owner.items()},open(W+'/owner.json','w'))
    multi=sum(1 for v in owner.values() if len(v)>1)
    print(len(owner),'addresses owned;',multi,'shared by >1 function')
