# incl.py <header-relpath>...: landed rows (source or test) that include the given rebuild/include headers transitively (by basename,
# through other repo headers), plus every row whose source #includes a relative source path (funclets / folded TUs) -> addresses
import os,re,sys
R='/home/user/FableDecomp/rebuild/'
names=set(os.path.basename(h) for h in sys.argv[1:]); hdrs=set(sys.argv[1:]); inc={}
for root,_,fs in os.walk(R+'include'):
    for f in fs:
        p=os.path.join(root,f)
        try: inc[os.path.relpath(p,R+'include')]=set(re.findall(r'#\s*include\s*[<"]([^">]+)[">]',open(p,encoding='utf-8',errors='replace').read()))
        except Exception: pass
ch=True
while ch:
    ch=False
    for h,s in inc.items():
        if h in hdrs: continue
        if any(os.path.basename(x) in names for x in s): hdrs.add(h); names.add(os.path.basename(h)); ch=True
bc=open(R+'build_candidates.ps1',encoding='utf-8').read(); out=set()
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    d=dict(re.findall(r"(\w+) = '([^']*)'",m.group(1)))
    for k in ('Source','TestSource'):
        if k not in d: continue
        p=R+('src/compiled/' if k=='Source' else 'tests/')+d[k]
        try: t=open(p,encoding='utf-8',errors='replace').read()
        except Exception: continue
        incs=re.findall(r'#\s*include\s*[<"]([^">]+)[">]',t)
        if any(os.path.basename(x) in names for x in incs) or any(x.startswith('../') for x in incs): out.add(d['Address'].lower())
sys.stderr.write('headers %d rows %d\n'%(len(hdrs),len(out)))
print('\n'.join(sorted(out)))
