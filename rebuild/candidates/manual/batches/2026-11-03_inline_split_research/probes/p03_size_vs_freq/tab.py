import re,sys
asm=open(sys.argv[1]).read()
funcs=re.split(r'\n(?=[0-9a-f]{8} <)',asm)
res={}
for f in funcs:
    m=re.match(r'[0-9a-f]{8} <\?s(\d+)_(\d+)@',f)
    if not m: continue
    k,d=int(m.group(1)),int(m.group(2))
    res[(k,d)]='C' if re.search(r'DISP32\s+\?F\d+@',f) else 'i'
ks=sorted({k for k,_ in res}); ds=sorted({d for _,d in res})
print('k\\d '+' '.join(str(d) for d in ds))
for k in ks: print(f'{k:3} '+' '.join(res[(k,d)] for d in ds))
