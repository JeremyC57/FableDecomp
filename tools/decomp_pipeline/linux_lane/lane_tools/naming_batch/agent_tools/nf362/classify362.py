# classify362.py <addr...>: objsec352 HEAD v new (work/obj/H_<a>.obj v work/obj/<a>.obj, leaf = the HEAD source's // Leaf:) into
# evidence/objsec/<a>.txt, then every differing line classified by script. Allowed classes:
#   TARGET   the repaired copy COMDAT: GetDef<CUIMiscThingsDef> / <CThingObjectDef> / <CThingBaseDef> (REREAD rows) or Reset<CComponent> (005cdd86)
#   SDC      a CHANGED ??$SetDynamicCast@ COMDAT whose pk_all address is NOADDR in both objects (the REREAD body itself)
#   ASSIGN   a DROPPED ?Assign@?$CDefPointer@ COMDAT (and its SYM- DEF line) that pk_all lists NOADDR in H (NF346 / NF349 class)
# Anything else (and leaf != same) is OTHER and holds the row. Prints `ROW <a> verdict=<OK|HOLD> leaf=<..> target=n sdc=n assign=n other=n`.
import sys, os, re, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF362'
sys.path.insert(0, A + '/tools'); import nf
TGT = ('??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@', '??$GetDef@VCThingObjectDef@@@CDefinitionManager@@',
       '??$GetDef@VCThingBaseDef@@@CDefinitionManager@@', '?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z')
def pkaddr(p):
    d = {}
    for l in open(p):
        f = l.rstrip('\n').split('\t')
        if len(f) >= 10: d.setdefault(f[9], set()).add(f[0])   # pk_all verdict column (NOADDR = no retail address)
    return d
os.makedirs(A + '/evidence/objsec', exist_ok=True)
for a in sys.argv[1:]:
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    if not leaf.startswith('?'):   # no // Leaf: line (catalog name tail): take the one external DEF of the HEAD object that contains it
        c_ = [x[4:] for x in nf.ext_syms('%s/work/obj/H_%s.obj' % (A, a)) if x.startswith('DEF ') and ('?' + leaf + '@') in x]
        if len(c_) == 1: leaf = c_[0]
    r = subprocess.run(['python3', A + '/tools/objsec352.py', '%s/work/obj/H_%s.obj' % (A, a), '%s/work/obj/%s.obj' % (A, a), leaf], capture_output=True, text=True)
    out = r.stdout + r.stderr; open('%s/evidence/objsec/%s.txt' % (A, a), 'w').write(out)
    H = pkaddr('%s/evidence/pk_gate/H_%s.pkall.tsv' % (A, a)); N = pkaddr('%s/evidence/pk_gate/N_%s.pkall.tsv' % (A, a))
    c = dict(target=0, sdc=0, assign=0, other=0); oth = []; leafv = '?'
    for l in out.splitlines():
        if l.startswith('VERDICT'):
            m = re.search(r'leaf=(\S+)', l); leafv = m.group(1) if m else '?'; continue
        m = re.match(r"(CHANGED|DROPPED|ADDED)\s+\('(\w+)', '([^']*)'", l)
        s = re.match(r'SYM([+-]) (DEF|UND) (\S+)', l)
        if m:
            kind, sym = m.group(1), m.group(3)
            if kind == 'CHANGED' and sym.startswith(TGT): c['target'] += 1
            elif kind == 'CHANGED' and sym.startswith('??$SetDynamicCast@') and H.get(sym) == {'NOADDR'} and N.get(sym) == {'NOADDR'}: c['sdc'] += 1
            elif kind == 'DROPPED' and sym.startswith('?Assign@?$CDefPointer@') and H.get(sym) == {'NOADDR'} and sym not in N: c['assign'] += 1
            else: c['other'] += 1; oth.append(l[:160])
        elif s:
            if s.group(1) == '-' and s.group(2) == 'DEF' and s.group(3).startswith('?Assign@?$CDefPointer@') and H.get(s.group(3)) == {'NOADDR'}: pass
            else: c['other'] += 1; oth.append(l[:160])
    ok = leafv == 'same' and c['other'] == 0 and c['target'] >= 1
    print('ROW %s verdict=%s leaf=%s target=%d sdc=%d assign=%d other=%d' % (a, 'OK' if ok else 'HOLD', leafv, c['target'], c['sdc'], c['assign'], c['other']))
    for x in oth: print('   OTHER', x)
