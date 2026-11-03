import sys,re,difflib
# like qdiff but masks esp offsets too (structure-only diff)
L=open(sys.argv[1]).read().split('\n')
i=[n for n,l in enumerate(L) if l.startswith('--- retail')][0]; j=[n for n,l in enumerate(L) if l.startswith('--- built')][0]
def norm(ls):
    out=[]
    for l in ls:
        p=l.split(None,2)
        if len(p)<3: continue
        ins=re.sub(r'0x[0-9a-f]{5,}','X',p[2].strip())
        ins=re.sub(r'\[0\]|\[X\]','[G]',ins)
        ins=re.sub(r'call X','call',ins); ins=re.sub(r'j(\w+) X',r'j\1',ins)
        ins=re.sub(r'\[esp \+ 0x[0-9a-f]+\]','[esp+N]',ins); ins=re.sub(r'\[esp\]','[esp+N]',ins)
        ins=re.sub(r'(sub|add) esp, 0x[0-9a-f]+',r'\1 esp, N',ins)
        ins=re.sub(r'^(push|mov e\w\w,) 0$',r'\1 X',ins)
        out.append(ins)
    return out
r=norm(L[i+1:j]); b=norm(L[j+1:])
for d in difflib.unified_diff(r,b,'retail','built',n=int(sys.argv[2]) if len(sys.argv)>2 else 1,lineterm=''): 
    try: print(d)
    except BrokenPipeError: break
