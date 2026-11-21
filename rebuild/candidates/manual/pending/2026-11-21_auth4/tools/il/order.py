"""order.py <dump2.json>: per size-compare block of InitialiseFromFileData, which operand the IL evaluates first + leaf words."""
import json,struct,sys,re
t=json.load(open(sys.argv[1])); ev=t['events']
pi=[i for i,e in enumerate(ev) if e[0]=='pipe']; ti=[i for i in pi if 'InitialiseFromFileData' in ev[i][1]][0]
prev=max([i for i in pi if i<ti] or [-1]); seg=[e for e in ev[prev+1:ti] if e[0]=='t']
def D(e): return struct.unpack('<12I',bytes.fromhex(e[2]))
def ops(e): return [struct.unpack('<12I',bytes.fromhex(o[1])) for o in e[3]]
out=[]
for i,e in enumerate(seg):
    d=D(e)
    if d[1]==0x27e and any(o[1]==0x24b and o[6]==0x40 for o in ops(e)):   # AnimatedFlag |= 0x40
        j=i+1; seq=[]
        while j<len(seg) and D(seg[j])[1]!=0x288: seq.append(D(seg[j])[1]); j+=1
        iadd=seq.index(0x2d) if 0x2d in seq else 99
        imul=min([seq.index(x) for x in (0x27b,0x283) if x in seq] or [99])
        out.append("uncompressed-first" if imul<iadd else "compressed-first")
print(" ".join(out), "(rotation position scaling; retail: compressed uncompressed compressed)")
print("TARGET" if out==["compressed-first","uncompressed-first","compressed-first"] else "no")
# leaf words of the position block (2nd AnimatedFlag window): count (lea operand), pal start (sub), pal finish (add), pos finish/start (sub w/ two M/S)
M=0xffff; k=0
for i,e in enumerate(seg):
    d=D(e)
    if d[1]==0x27e and any(o[1]==0x24b and o[6]==0x40 for o in ops(e)):
        k+=1
        j=i+1; w={}
        subs=[]
        while j<len(seg) and D(seg[j])[1]!=0x288:
            dd=D(seg[j]); oo=ops(seg[j])
            if dd[1]==0x12 and 'c' not in w: w['c']=oo[1][4]&M
            if dd[1]==0x279: subs.append([o[4]&M for o in oo])
            if dd[1]==0x2d and 'pf' not in w: w['pf']=oo[1][4]&M
            j+=1
        if len(subs)>=2:
            w['ps']=subs[0][1]; w['qf'],w['qs']=subs[1][0],subs[1][1]
        if 'c' in w and 'pf' in w and 'ps' in w:
            L=(w['c']+2*w['ps']+2*w['pf']+0x99)&M; R=(w['qf']+2*w['qs']+0xc9)&M
            print("block%d c=%04x ps=%04x pf=%04x qf=%04x qs=%04x  L=%04x R=%04x %s"%(k,w['c'],w['ps'],w['pf'],w['qf'],w['qs'],L,R,"R>L" if R>L else "L>R"))
