import json,sys
# usage: blkshare.py json id1,id2,...  -> per-pass block shares for these candidate ids
j=json.load(open(sys.argv[1])); ev=j['functions'][0]['events']
ids=set(int(x) for x in sys.argv[2].split(','))
passno=0; cur=None; shares={}
marks=[i for i,e in enumerate(ev) if e['e'] in ('sorted','resort')]
for i,e in enumerate(ev):
    if e['e']=='blk': cur=e; continue
    if e['e']=='blkend':
        w,wk=e['w'],e['wK']
        for cid,f6,cost,rn in e['refl']:
            if rn==0 and cid in ids: shares.setdefault(cid,[]).append((cur['lines'],cur['depth'],w,wk,'ref',wk*cost))
        for cid,f6,cost,rn in e['live']:
            if cid not in ids: continue
            if f6&0x10: shares.setdefault(cid,[]).append((cur['lines'],cur['depth'],w,wk,'ref',wk*cost))
            elif wk: shares.setdefault(cid,[]).append((cur['lines'],cur['depth'],w,wk,'live',-wk))
        continue
    if e['e'] in ('sorted','resort'):
        pr={s['id']:s['prio'] for s in e['list']}
        if any(c in pr for c in ids):
            print(f"== pass {passno} ({e['e']} at {i})")
            for c in sorted(ids):
                if c in pr or c in shares:
                    tot=sum(x[-1] for x in shares.get(c,[]))
                    print(f"  #{c}: table {pr.get(c)} sum {tot}")
                    for x in shares.get(c,[]): print('     ',x)
        shares={}; passno+=1
    if e['e']=='new' and e['id'] in ids:
        print('new',e['id'],'parent',e['parent'],'at',i)
