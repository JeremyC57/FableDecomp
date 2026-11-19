import json,sys
t=json.load(open(sys.argv[1])); r=0
for e in t['events']:
    if e[0]=='dag':
        r+=1; print(f'REGION {r} n={len(e[1])}')
        for n in e[1]: print(f"  idx {n['idx']:3} {str(n['op']):10} prio {n['prio']:#x} p2 {n['prio2']:#x} early {n['early']} depth {n['depth']} unit {n['unit']} fl {n['fl']} succ {[(s[0],s[1],s[2],s[3]) for s in n['succ']]}")
    elif e[0]=='pick': print(f"  PICK cyc {e[1]} idx {e[2]['idx']} {e[2]['op']}")
    elif e[0]=='cyc': print(f"  CYC {e[1]} ready {[(x['idx'],x['op']) for x in e[3]]}")
    elif e[0]=='err': print('ERR',e[1])
