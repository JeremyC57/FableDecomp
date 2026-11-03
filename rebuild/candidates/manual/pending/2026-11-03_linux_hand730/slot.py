# slot.py <hexoff>...: CGameScriptInterfaceBase slot declaration for a vtable byte offset
import sys,re
L=open('/home/user/FableDecomp/rebuild/include/engine/CGameScriptInterfaceBase.h').read().split('\n')
m={}
for l in L:
    r=re.search(r'//\s+(\d+) 0x',l)
    if r: m[int(r.group(1))]=l.strip()[:200]
for a in sys.argv[1:]:
    s=int(a,16)//4; print(a,s,m.get(s))
