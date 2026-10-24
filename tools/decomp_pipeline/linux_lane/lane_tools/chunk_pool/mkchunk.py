import json,os,sys,re
# usage: mkchunk.py N [K] [LO HI]   cut K chunks of 40 class-labelled rows (LO..HI bytes, default 17..96) starting at chunk N
# rows come from $S/batch/families512.json, or families1024.json when HI > 512 (famscan.py 1024 with FAMOUT=...)
# POOL=small takes the rows the class filter leaves out instead: `_global`, template and namespaced labels
# (unwind funclets, unnamed `_gapscan` starts and families of 3+ stay with the family generators)
S=os.environ['S']; N=int(sys.argv[1]); K=int(sys.argv[2]) if len(sys.argv)>2 else 1
LO=int(sys.argv[3]) if len(sys.argv)>3 else 17; HI=int(sys.argv[4]) if len(sys.argv)>4 else 96
FJ=f'{S}/batch/families{1024 if HI>512 else 512}.json'
if not os.path.exists(FJ): FJ=f'{S}/batch/families1024.json'
d=json.load(open(FJ))
SMALL=os.environ.get('POOL')=='small'
def wanted(m,n):
    plain=re.match(r'^C[A-Z0-9]',m) and '::' not in m and '<' not in m
    if not SMALL: return plain
    return not plain and not m.startswith('_gapscan') and not n.startswith(('Unwind@','Catch@','sub_','FUN_'))
cat={l.split('\t')[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv')}
used=set()
HIST='rebuild/candidates/manual/chunk_history.tsv'   # committed record of every cut row (fresh workspaces have no $S/w22/chunkNN.tsv)
if os.path.exists(HIST): used|={l.split('\t')[1] for l in open(HIST).read().splitlines()[1:]}
for i in range(N):
    p=f'{S}/w22/chunk{i:02d}.tsv'
    if os.path.exists(p): used|={l.split('\t')[0] for l in open(p).read().splitlines()[1:]}
rows=[]
for shape,fam in d:
    if len(fam)>2: continue
    for a,m,n,l,h in fam:
        if not wanted(m,n) or not (LO<=l<=HI): continue
        if a.lower() in cat or a in used: continue
        rows.append((m,a,n,l,h))
# graphics-API wrapper rows (D3D9 device/display, buffers, shaders, textures/surfaces, Win32 window/video) go last: game logic,
# CPU math, scene graph, draw-order lists and mesh processing (renderers, stripifier) stay in normal order (owner, 2026-10-23)
GFX=re.compile(r'D3D|Direct3D|VertexBuffer|IndexBuffer|Shader|VertexDecl|^CTexture$|VolumeTexture|CubeTexture|^CSurface$|RenderTarget|DisplayManager|DisplayEngine|VideoSys|Vsync|GraphicsDevice|MainWindow|Win32Window')
rows.sort(key=lambda r:(bool(GFX.search(r[0])),r[0],r[1]))
print('graphics rows deferred',sum(1 for r in rows if GFX.search(r[0])))
print('pool left',len(rows))
hist=open(HIST,'a') if os.path.exists(HIST) else open(HIST,'w'); hist.write('' if os.path.getsize(HIST) else 'chunk\taddress\tmodule\tname\tlength\n')
for k in range(K):
    sel=rows[k*40:(k+1)*40]; nn=N+k
    with open(f'{S}/w22/chunk{nn:02d}.tsv','w') as f:
        f.write('address\tmodule\tname\tlength\tbytes\n')
        for m,a,n,l,h in sel: f.write(f'{a}\t{m}\t{n}\t{l}\t{h}\n'); hist.write(f'{nn:02d}\t{a}\t{m}\t{n}\t{l}\n')
    with open(f'{S}/batch/chunk{nn:02d}_oracle.tsv','w') as f:
        f.write('address\tname\tlength\tbytes\n')
        for m,a,n,l,h in sel:
            b=bytes.fromhex(h)
            while b and b[-1] in (0xcc,0x90): b=b[:-1]   # trailing int3/nop padding is not part of the function (the lander compares the whole row)
            f.write(f'{a}\t{m}::{n}\t{len(b)}\t{b.hex()}\n')
    print(nn, len(sel), sel[0][0] if sel else '-', sel[-1][0] if sel else '-')
