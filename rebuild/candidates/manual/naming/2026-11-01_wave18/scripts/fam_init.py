# fam_init.py <ov> <rows.txt>: wave 14 queue codex50 -- 0x00857520 is Ego_r ?Initialise@CCombatActionBase@@UAEXXZ (virtual). The
# rows' local CCombatActionBase model declares `void Initialise();` non-virtual; it becomes `virtual void Initialise();` (the
# rows call it qualified, `CCombatActionBase::Initialise()`, a direct call either way). A derived Initialise the row defines
# becomes the virtual override it is in retail (its own symbol QAE -> UAE). check: only those spellings change.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, acclib, f2m
OLD = '?Initialise@CCombatActionBase@@QAEXXZ'; FIN = '?Initialise@CCombatActionBase@@UAEXXZ'
def tr(t, a):
    c = f2m.code_only(t)
    ms = [x for x in re.finditer(r'(?<![\w ])([ \t]*)void(\s+)Initialise(\s*\(\s*\)\s*;)', c) if (acclib.class_span(c, x.start()) or (0, ''))[1] == 'CCombatActionBase']
    if len(ms) != 1: return None, '%d declarations in CCombatActionBase' % len(ms)
    m = ms[0]
    k = acclib.class_span(c, m.start())
    t = t[:m.start()] + m.group(1) + 'virtual void' + m.group(2) + 'Initialise' + m.group(3) + t[m.end():]
    body = c[k[2]:k[3]]
    vf = re.search(r'\bvoid\s*\*\s*__vftable\s*;', body)
    if vf and 'virtual' not in body and not body[:vf.start()].replace('public:', '').strip():
        # the flattened vptr field goes: the now-virtual class carries the real vptr at +0 (same layout)
        t = t[:k[2] + vf.start()] + t[k[2] + vf.end():]
        return t, 'virtual Initialise (flattened __vftable field -> real vptr)'
    return t, 'virtual Initialise'
def ck(S0, S1, a):
    lost = S0 - S1; got = S1 - S0
    ok_l = {s for s in lost if s == OLD or re.match(r'^\?Initialise@\w+@@QAEXXZ$', s)}
    ok_g = {s for s in got if s == FIN or re.match(r'^\?Initialise@\w+@@UAEXXZ$', s)}
    if lost - ok_l or got - ok_g: return 'lost %r gained %r' % (sorted(lost - ok_l)[:3], sorted(got - ok_g)[:3])
    if FIN not in S1: return 'final not emitted'
    if {s.replace('@@QAE', '@@UAE') for s in ok_l} != ok_g: return 'unpaired %r %r' % (sorted(ok_l), sorted(ok_g))
    return None
F.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tr, ck, 'init')
