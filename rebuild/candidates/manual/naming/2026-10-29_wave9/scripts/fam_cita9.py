# fam_cita9.py <ov> <rows.txt>: NScript::CIsThingAlive (Ego_r.pdb + FableWin.pdb: ??1CIsThingAlive@NScript@@UAE@XZ 0x00E241B0
# (Ego_r 0x00CC4A80), ??RCIsThingAlive@NScript@@UBE_NXZ, ?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ; class key V in
# 132 PDB names; vtable 0x012C32F0). Rows modelled the filter at global scope (struct, non-virtual dtor), emitting
# ??1CIsThingAlive@@QAE@XZ and ??_7CIsThingAlive@@6B@. Transform: the class definition is wrapped in `namespace NScript { }`
# (+ `using NScript::CIsThingAlive;`), `struct` becomes `class ... { public:` with public inheritance, the destructor is
# declared virtual. Kept only when the object then names no global CIsThingAlive symbol.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hand'))
import famlib9
from he import ns_chain, block_end
def tf(t, a):
    m = re.search(r'\b(class|struct)(\s+CIsThingAlive\b)([^;{]*)\{', t)
    if not m: return None, 'no CIsThingAlive definition'
    ch = ns_chain(t, m.start())
    if ch not in ([], ['NScript']): return None, 'scope %s' % ch
    s = m.start(); e = block_end(t, s); body = t[s:e]
    if m.group(1) == 'struct':
        bases = m.group(3)
        if bases.strip().startswith(':'):
            bl = bases.split(':', 1)[1]
            parts = [('public ' + x.strip()) if not re.match(r'\s*(public|private|protected)\b', x) else x.strip() for x in bl.split(',')]
            bases = ' : ' + ', '.join(parts) + ' '
        body = 'class' + m.group(2) + bases + '{ public:' + body[m.end() - s:]
    body = re.sub(r'(?<!virtual )(?<!virtual)(?<![\w~])~CIsThingAlive\s*\(', 'virtual ~CIsThingAlive(', body, count=1)
    if '~CIsThingAlive' not in body:
        k = body.rfind('}'); body = body[:k] + ' virtual ~CIsThingAlive() { }\n' + body[k:]
    t = t[:s] + (('namespace NScript {\n' + body + '\n}\nusing NScript::CIsThingAlive;') if not ch else body) + t[e:]
    t = re.sub(r'\bstruct(\s+CIsThingAlive\s*;)', r'class\1', t)
    return t, 'ok'
def ck(S0, S1, a):
    bad = [s for s in S1 if re.search(r'CIsThingAlive@@', s) and 'CIsThingAlive@NScript@@' not in s]
    if bad: return 'still global: %s' % bad[:3]
    if '??1CIsThingAlive@@QAE@XZ' in S0 and '??1CIsThingAlive@NScript@@UAE@XZ' not in S1: return 'no final dtor'
    if '??_7CIsThingAlive@@6B@' in S0 and '??_7CIsThingAlive@NScript@@6B@' not in S1: return 'no final vtable'
    return None
famlib9.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tf, ck, 'cita9')
