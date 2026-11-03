# mk.py <src-with-#include "st_head.inc"> : expand the include in place into <src>.x.cpp (prints its path)
import sys,os,re
p=sys.argv[1]; s=open(p).read(); d=os.path.dirname(p)
s=re.sub(r'#include "(\w+\.inc)"\n',lambda m:open(os.path.join(d,m.group(1))).read(),s)
o=p[:-4]+'.x.cpp'; open(o,'w').write(s); print(o)
