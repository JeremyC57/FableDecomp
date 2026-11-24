# lvsize.py <lvdir> <addr...>: size of the GetSubDef<CAppearanceDef|CWeaponDef|CEntitySoundDef|CInventoryItemDef> COMDAT in vland's kept row object
# (<lvdir>/xx/yy/<addr>/<addr>.obj): proves which header the vland run compiled with (182 = HEAD body, 162 = size body).
import sys, glob, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361/tools'); import nf
for a in sys.argv[2:]:
    g = glob.glob('%s/*/*/%s/%s.obj' % (sys.argv[1], a, a))
    if not g: print(a, 'NOOBJ'); continue
    secs, sl = nf.coff(g[0]); out = []
    for i, n, v, sec, t, scl in sl:
        m = re.match(r'\?\?\$GetSubDef@V(CAppearanceDef|CWeaponDef|CEntitySoundDef|CInventoryItemDef)@@@CDefClassBase', n)
        if m and scl == 2 and sec > 0: out.append('%s=%d' % (m.group(1), len(secs[sec - 1]['data'])))
    print(a, ' '.join(out) or 'none')
