# lin.py json root[,root..] : lineage of split pieces of root candidates; prints new/chosen/drop/forward/lowspill + prio at sorts
import json,sys
d=json.load(open(sys.argv[1])); roots=[int(x) for x in sys.argv[2].split(",")]
ev=d['functions'][0]['events']
name={}; cnt={}
REG={1:'eax',2:'ecx',3:'edx',4:'ebx',6:'ebp',7:'esi',8:'edi'}
seenroot=set(); drv=False
step=0
for e in ev:
    t=e['e']
    if t=='new':
        p=e['parent']
        if p is not None and p in name:
            cnt[p]=cnt.get(p,0)+1; name[e['id']]=name[p]+"."+chr(96+cnt[p]); print("new  %-10s id%d (of id%d)"%(name[e['id']],e['id'],p))
        elif e['id'] in roots and p is None and not drv:
            seenroot.add(e['id']); name[e['id']]=str(e['id'])
        else:
            name.pop(e['id'],None)
    elif t in('chosen',) and e['id'] in name: print("REG  %-10s id%d -> %s"%(name[e['id']],e['id'],REG.get(e['reg'],e['reg'])))
    elif t in('drop','forward','lowspill') and e['id'] in name: print("%-4s %-10s id%d"%(t,name[e['id']],e['id']))
    elif t in('sorted','resort'):
        step+=1
        for i,c in enumerate(e['list']):
            if c['id'] in name: print("  %s%d pos %d/%d %-10s prio %d key %d refs %d spill %d"%(t[:3],step,i,len(e['list']),name[c['id']],c['prio'],c['key'],c['refs'],c['spill']))
    elif t=='driver': drv=True
    elif t=='split':
        s=[name[i] for i in e['ids'] if i in name]
        if s: print("SPLIT",s)
