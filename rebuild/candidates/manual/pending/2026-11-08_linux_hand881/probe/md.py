import sys,re,difflib
for fn in sys.argv[1:]:
    t=open(fn).read()
    try:
        r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
    except Exception: print(fn, t.splitlines()[0] if t else 'empty'); continue
    f=lambda L:[re.sub(r'0x[0-9a-f]{6,8}\b','ADDR',l.split(None,2)[2]) if len(l.split(None,2))>2 else l for l in L]
    a=f(r); c=f(b)
    sm=difflib.SequenceMatcher(None,a,c,autojunk=False)
    md=sum(max(i2-i1,j2-j1) for tag,i1,i2,j1,j2 in sm.get_opcodes() if tag!='equal')
    print(fn, t.splitlines()[0].split()[-3] if 'DIFFER' in t.splitlines()[0] else t.splitlines()[0], 'md', md)
