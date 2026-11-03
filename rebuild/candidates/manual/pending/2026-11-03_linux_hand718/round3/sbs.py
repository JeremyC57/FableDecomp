import sys,re,difflib
t=open(sys.argv[1]).read(); lim=int(sys.argv[2]) if len(sys.argv)>2 else 30
if '--- retail' not in t: print(t[:2500]); sys.exit()
r=t.split('--- retail')[1].split('--- built')[0].strip().split('\n')[1:]
b=t.split('--- built')[1].strip().split('\n')[1:]
def norm(l):
    p=l.split(None,2); s=p[2] if len(p)>2 else ''
    s=re.sub(r'0x[0-9a-f]{5,8}','X',s)   # mask addresses
    return s
R=[norm(x) for x in r]; B=[norm(x) for x in b]
sm=difflib.SequenceMatcher(None,R,B,autojunk=False)
out=[];nd=0
for op,a1,a2,b1,b2 in sm.get_opcodes():
    if op=='equal': continue
    nd+=max(a2-a1,b2-b1)
    out.append('@%s r[%d:%d] b[%d:%d]'%(r[a1].split()[0] if a1<len(r) else 'end',a1,a2,b1,b2))
    for i in range(max(a2-a1,b2-b1)):
        x=R[a1+i] if a1+i<a2 else ''; y=B[b1+i] if b1+i<b2 else ''
        out.append('  %-42s | %s'%(x[:42],y[:42]))
print('difflines=%d retail=%d built=%d'%(nd,len(R),len(B)))
print('\n'.join(out[:lim]))
