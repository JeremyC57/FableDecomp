# fam_cmp.py <ov> <rows.txt>: wave 14 anonymous-namespace re-authoring (DECISIONS_W13_RESOLVED anonymous-namespace row), part 1:
# the CThingCompare_InDirectionOfAndNearestToHeuristicBitch comparators 0x00978790 / 0x0097ABF0 are Ego_r/FableWin
# ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x<hash>@@QBE_NPBVCThing@@0@Z -- operator()(const CThing*, const CThing*)
# const. The rows declare it out of line inside their anonymous namespace with `CThing*` parameters; the parameters become
# `const CThing*` (callers pass CThing*: the conversion emits nothing). check: only the operator() spelling changes PAV -> PBV.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, acclib, f2m
CLS = 'CThingCompare_InDirectionOfAndNearestToHeuristicBitch'
def tr(t, a):
    c = f2m.code_only(t)
    ms = [x for x in re.finditer(r'\boperator\s*\(\s*\)\s*\(\s*CThing\s*\*(\s*\w*)\s*,\s*CThing\s*\*(\s*\w*)\s*\)(\s*const\s*;)', c)
          if (acclib.class_span(c, x.start()) or (0, ''))[1] == CLS]
    if len(ms) != 1: return None, '%d operator() declarations' % len(ms)
    m = ms[0]
    t = t[:m.start()] + 'operator()(const CThing*' + m.group(1) + ', const CThing*' + m.group(2) + ')' + m.group(3) + t[m.end():]
    # a same-TU stand-in definition of operator() (and its inline score helper) follows the declaration
    t, nd = re.subn(r'(\b' + CLS + r'\s*::\s*operator\s*\(\s*\)\s*\()\s*CThing\s*\*(\s*\w*)\s*,\s*CThing\s*\*(\s*\w*)\s*\)', r'\1const CThing*\2, const CThing*\3)', t)
    if nd: t = re.sub(r'\b(float|double|long|int)(\s+)GetScore(\s*)\(\s*CThing\s*\*', r'\1\2GetScore\3(const CThing*', t)
    return t, 'cmp const CThing*' + (' (+ same-TU definition)' if nd else '')
def ck(S0, S1, a):
    n = lambda s: re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', s)
    lost = {n(s) for s in S0 - S1}; got = {n(s) for s in S1 - S0}
    exp = {s.replace('QBE_NPAVCThing@@0@Z', 'QBE_NPBVCThing@@0@Z').replace('QBEMPAVCThing@@@Z', 'QBEMPBVCThing@@@Z') for s in lost}
    if not lost or exp != got or any(CLS not in s or not (s.startswith('??R') or s.startswith('?GetScore@')) for s in lost): return 'lost %r gained %r' % (sorted(lost)[:2], sorted(got)[:2])
    return None
F.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tr, ck, 'cmp')
