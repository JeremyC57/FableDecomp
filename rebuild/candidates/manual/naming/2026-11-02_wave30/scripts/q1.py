# q1.py: per queue row: correct symbol in Ego_r / FableWin, and current bindings at the target address
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
NM = inputs.names(); BT, BS = inputs.bindings()
for l in open(sys.argv[1]):
    q = l.rstrip('\n').split('\t')
    ch = q[0]
    if ch.startswith('mig'): addr, wrong, cor = q[1], q[2], q[3]
    else: addr, cor, wrong = q[1], q[2], q[4] if len(q) > 4 else ''
    a = addr.lower().replace('0x', '')[:8].zfill(8) if re.match(r'^(0x)?[0-9a-fA-F]{6,8}$', addr.strip()) else addr
    syms = re.findall(r'\?\S+', cor)
    print('==', ch, a)
    for s in syms or [cor[:150]]:
        print('   cor', ('E' if s in NM['Ego_r.pdb'] else '-') + ('F' if s in NM['FableWin.pdb'] else '-'), 'bound->' + BS[s][0] if s in BS else 'unbound', s[:200])
    for s in sorted(BT.get(a, ())):
        print('   bnd', ('E' if s in NM['Ego_r.pdb'] else '-') + ('F' if s in NM['FableWin.pdb'] else '-'), s[:200])
