# evs.py json lo hi : print event stream condensed; refs only for rel lines lo..hi; new/drop/forward/chosen of pieces
import json,sys
d=json.load(open(sys.argv[1])); lo,hi=int(sys.argv[2]),int(sys.argv[3])
ev=d['functions'][0]['events']
gen=0; out=[]; refs={}
def flush():
    global refs
    if refs:
        out.append("   refs: "+" ".join("%d:%s"%(k,",".join(v)) for k,v in sorted(refs.items())))
    refs={}
for e in ev:
    t=e['e']
    if t=='ref':
        if lo<=e['line']<=hi: refs.setdefault(e['id'],[]).append("%s%d@%x"%(e['rw'],e['line'],e['blk']&0xfff))
        continue
    flush()
    if t=='new' and e['parent'] is not None: out.append(" new %d parent %s"%(e['id'],e['parent']))
    elif t=='new': pass
    elif t in('split',): out.append("SPLIT %s"%e['ids'])
    elif t in('drop','forward','lowspill'): out.append(" %s %d"%(t,e['id']))
    elif t=='chosen': out.append(" chosen %d reg %d"%(e['id'],e['reg']))
    elif t in('sorted','resort'): out.append("%s n=%d"%(t,len(e['list'])))
    else: out.append(t)
flush()
print("\n".join(out))
