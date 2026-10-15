import json,os,sys,re
# usage: mkchunk.py N [K] [LO HI]   cut K chunks of 40 class-labelled rows (LO..HI bytes, default 17..96) starting at chunk N
S=os.environ['S']; N=int(sys.argv[1]); K=int(sys.argv[2]) if len(sys.argv)>2 else 1
LO=int(sys.argv[3]) if len(sys.argv)>3 else 17; HI=int(sys.argv[4]) if len(sys.argv)>4 else 96
d=json.load(open(f'{S}/batch/families512.json'))
cat={l.split('\t')[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv')}
used=set()
for i in range(N):
    p=f'{S}/w22/chunk{i:02d}.tsv'
    if os.path.exists(p): used|={l.split('\t')[0] for l in open(p).read().splitlines()[1:]}
rows=[]
for shape,fam in d:
    if len(fam)>2: continue
    for a,m,n,l,h in fam:
        if not re.match(r'^C[A-Z0-9]',m) or '::' in m or '<' in m or not (LO<=l<=HI): continue
        if a.lower() in cat or a in used: continue
        rows.append((m,a,n,l,h))
rows.sort(key=lambda r:(r[0],r[1]))
print('pool left',len(rows))
for k in range(K):
    sel=rows[k*40:(k+1)*40]; nn=N+k
    with open(f'{S}/w22/chunk{nn:02d}.tsv','w') as f:
        f.write('address\tmodule\tname\tlength\tbytes\n')
        for m,a,n,l,h in sel: f.write(f'{a}\t{m}\t{n}\t{l}\t{h}\n')
    with open(f'{S}/batch/chunk{nn:02d}_oracle.tsv','w') as f:
        f.write('address\tname\tlength\tbytes\n')
        for m,a,n,l,h in sel:
            b=bytes.fromhex(h)
            while b and b[-1] in (0xcc,0x90): b=b[:-1]   # trailing int3/nop padding is not part of the function (the lander compares the whole row)
            f.write(f'{a}\t{m}::{n}\t{len(b)}\t{b.hex()}\n')
    print(nn, len(sel), sel[0][0] if sel else '-', sel[-1][0] if sel else '-')
