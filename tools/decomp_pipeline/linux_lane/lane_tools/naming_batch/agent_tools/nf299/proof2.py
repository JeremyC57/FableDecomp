# proof2.py <tag> <addr>:<old>=<new>[,<old>=<new>...] ...: per row, map each HEAD-only external symbol through the given identifier
# renames (`?old@` -> `?new@`, the respell this agent made), require the result among the new object's new symbols (explicit map, not
# sorted pairing; NF299: an item starting with '?' is an exact full-symbol pair), then objsec HEAD v new under that map; also tables count. -> evidence/proof_<tag>.txt
import sys,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299'
tag=sys.argv[1]; out=open(A+'/evidence/proof_%s.txt'%tag,'w')
def syms(p): return set(l.split()[-1] for l in subprocess.run(['objdump','-t',p],capture_output=True,text=True).stdout.splitlines() if '(scl   2)' in l)
for spec in sys.argv[2:]:
    a,rn=spec.split(':',1); rn=[x.split('=') for x in rn.split(',')]
    h=A+'/work/obj/H_%s.obj'%a; n=A+'/work/obj/N_%s.obj'%a; sh,sn=syms(h),syms(n)
    old=sorted(sh-sn); new=sorted(sn-sh); mp={}; bad=[]
    for s in old:
        t=s
        for o,w in rn:
            if o.startswith('?') and t==o: t=w; continue   # NF299: an exact full-symbol pair (name + const / access change)
            t=t.replace('?%s@'%o,'?%s@'%w,1) if t.startswith('?%s@'%o) else t
        if t not in new and any(o=='?A0x' for o,w in rn):
            import re as _re
            k=_re.sub(r'\?A0x[0-9a-f]{8}','?A0x',t); c=[x for x in new if _re.sub(r'\?A0x[0-9a-f]{8}','?A0x',x)==k]
            if len(c)==1: t=c[0]
        if t in new: mp[s]=t
        else: bad.append(s)
    left=[s for s in new if s not in mp.values()]
    r=subprocess.run(['python3',A+'/tools/objsec.py',h,n]+['%s=%s'%kv for kv in mp.items()],capture_output=True,text=True).stdout
    tb=(sum(1 for s in sh if s.startswith('??_7')),sum(1 for s in sn if s.startswith('??_7')))
    out.write('##### %s renames: %s\n'%(a,' '.join('%s=%s'%kv for kv in mp.items())))
    if bad or left: out.write('UNMAPPED old=%s new=%s\n'%(bad,left))
    out.write(''.join(l+'\n' for l in r.splitlines() if l.startswith(('sections','DIFF','ALL_EQ','bad','  '))))
    out.write('tables H=%d N=%d\n'%tb)
out.close(); print(open(A+'/evidence/proof_%s.txt'%tag).read())
