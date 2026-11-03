# fix_cp.py <famdir> <rows...>: replace the row-local CCountedPointer / CCPPointerInfo stand-in (deleter Destroy_00cdee00) by the shared header
import sys, os, re, json
F=sys.argv[1]; rows=set(sys.argv[2:])
g={l.split('\t')[0]:l.rstrip('\n').split('\t') for l in open(F+'/gen.tsv')}
n=0
for a in rows:
    d=json.loads(g[a][2]); p=F+'/src/'+d['new_src']; s=open(p).read()
    i=s.find('class CCPPointerInfo'); t=s.find('template <class T> class CCountedPointer')
    if i<0 or t<0: print('skip',a); continue
    k=s.find('\n};\n',t)+4
    pre=s[:i]
    pre2=re.sub(r'^void __fastcall Destroy_00cdee00\(void\* object\);[^\n]*\n\n?','',pre,flags=re.M)
    assert pre2!=pre, a
    s=pre2+'// CCountedPointer<T> / CCPPointerInfo: the shared retail model (engine/CCountedPointer.h; 0x00CDEE00 is\n//   CCountedPointer<CEntityScriptBase>::DeleteData). Naming wave 33 replaced the Alloc row\'s local stand-in (deleter Destroy_00cdee00).\n#include "engine/CCountedPointer.h"\n'+s[k:]
    open(p,'w').write(s); n+=1
    g[a][2]=json.dumps(dict(d,cpfix=True))
open(F+'/gen.tsv','w').write(''.join('\t'.join(v)+'\n' for v in g.values()))
print('fixed',n)
