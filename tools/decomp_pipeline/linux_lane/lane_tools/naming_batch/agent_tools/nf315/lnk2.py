# lnk2.py <out-prefix> <watch-regex> <obj>...: /DLL /NOENTRY /FORCE /NODEFAULTLIB link (LINK unset); prints duplicate lines and the
# unresolved symbols matching the watch regex with the number of referencing objects
import sys,os,re,collections
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
import verify_and_land as V
e=V.env(); e.pop('LINK',None)
out=sys.argv[1]; watch=re.compile(sys.argv[2]); objs=sys.argv[3:]
r=V.run_tool(V.VC/"bin"/"link.exe",["/nologo","/DLL","/NOENTRY","/FORCE","/NODEFAULTLIB","/out:"+out+".dll"]+objs,e)
t=(r.stdout or '')+(r.stderr or '')
open(out+'.log','w').write(t)
d=[l for l in t.splitlines() if 'LNK4006' in l or 'LNK2005' in l]
print('duplicates',len(d)); print('\n'.join(l[:260] for l in d))
u=collections.Counter()
for l in t.splitlines():
    m=re.search(r'unresolved external symbol (?:"[^"]*" \()?(\S+?)\)?(?: referenced|$)',l)
    if m and watch.search(m.group(1)): u[m.group(1)]+=1
for s,c in sorted(u.items()): print('UNRESOLVED',c,s)
