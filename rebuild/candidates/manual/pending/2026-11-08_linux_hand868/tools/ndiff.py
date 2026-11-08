import sys, re, difflib
def norm(path, keep_regs=False):
    out=[]
    for l in open(path):
        p=l.split(None,2)
        if len(p)<3: continue
        ins=p[2].strip()
        ins=re.sub(r'esp \+ 0x[0-9a-f]+','esp+S',ins)
        ins=re.sub(r'0x[0-9a-f]+','I',ins)
        if not keep_regs:
            ins=re.sub(r'\b(eax|ebx|ecx|edx|esi|edi|ebp)\b','R',ins)
            ins=re.sub(r'\b(al|bl|cl|dl)\b','r8',ins)
        out.append((p[0],ins))
    return out
a=norm(sys.argv[1], '--regs' in sys.argv); b=norm(sys.argv[2], '--regs' in sys.argv)
sm=difflib.SequenceMatcher(None,[x[1] for x in a],[x[1] for x in b],autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    print('--- %s retail[%s..%s] ours[%s..%s]' % (tag, a[i1][0] if i1<len(a) else '-', a[i2-1][0] if i2>i1 else '-', b[j1][0] if j1<len(b) else '-', b[j2-1][0] if j2>j1 else '-'))
    for k in range(i1,i2): print('  - %s %s' % a[k])
    for k in range(j1,j2): print('  + %s %s' % b[k])
