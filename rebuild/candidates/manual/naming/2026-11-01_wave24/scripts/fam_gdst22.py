# fam_gdst21.py <ov> <rows.txt> [tag]: naming wave 21, family gdst -- the established ?GDefStringTable@@3VCDefStringTable@@A (0x013CA828,
# ESTABLISHED_SPELLINGS.tsv; DECISIONS_W20 item 1): every row naming 0x013CA828 under another spelling (?g_DefStringTable@@3UCDefStringTable@@A
# and its variants: other identifiers, other class names, struct key) is respelled name-only: the identifier -> GDefStringTable, the class ->
# `class CDefStringTable { public:` (PDB class key V). Kept only when the object loses exactly the old global, gains exactly the established
# one, and every other symbol change is a class-rename of a PDB / bound CDefStringTable member.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
ov, rf = sys.argv[1], sys.argv[2]; tag = sys.argv[3] if len(sys.argv) > 3 else 'gdst'
NEW = '?GDefStringTable@@3VCDefStringTable@@A'
BT, BS = inputs.bindings(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
ROWS = {}
for l in open(rf):
    q = l.split()
    if q: ROWS[q[0]] = q[1:]
def transform(t, a):
    syms = [s for s in ROWS.get(a, []) if s != NEW]
    if not syms: return None, 'already the established spelling'
    s = syms[0]
    m = re.match(r'\?(\w+)@(?:(\w+)@)?@3([UV])(\w+)@(?:1|)@A$', s)
    if not m: return None, 'unparsed ' + s
    ident, ns, key, cls = m.group(1), m.group(2), m.group(3), m.group(4)
    t2 = t
    if ident != 'GDefStringTable': t2 = re.sub(r'\b' + ident + r'\b', 'GDefStringTable', t2)
    if cls != 'CDefStringTable': t2 = re.sub(r'\b' + cls + r'\b', 'CDefStringTable', t2)
    # struct -> class { public:
    t2 = re.sub(r'\bstruct(\s+CDefStringTable\s*)\{(?=\s*public:)', r'class\1{', t2)
    t2 = re.sub(r'\bstruct(\s+CDefStringTable\s*)\{', r'class\1{ public:', t2)
    t2 = re.sub(r'\bstruct(\s+CDefStringTable\b)(?!\s*\{)', r'class\1', t2)
    if t2 != t: t2 = t2.replace('extern CDefStringTable GDefStringTable;', 'extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)', 1) if '?GDefStringTable@@3VCDefStringTable@@A' not in t2 else t2
    return t2, '%s/%s%s -> GDefStringTable/CDefStringTable' % (ident, key, cls)
def ok_member(x):
    # a CDefStringTable member spelling that is in the PDB or already bound, or a member of the old class name (removed)
    return ('@CDefStringTable@@' in x and (x in PDB or x in BS))
def check(S0, S1, a):
    old = set(ROWS.get(a, [])) - {NEW}
    def rem_ok(x): return x in old or x in ('??0CStatic_009d4db0@@QAE@XZ', '??1CStatic_009d4db0@@QAE@XZ') or ('CDefStringTable' in x and x not in PDB and x not in BS) or ('@CDefStringTable' in x) or any(re.search(r'@' + re.escape(c) + r'@', x) for c in ('CDefStringTable_Retail', 'Pool', 'GObj', 'Singleton', 'CDefTypeNames', 'CStatic_009d4db0'))
    def add_ok(x): return x == NEW or ok_member(x) or x in ('??0CDefStringTable@@QAE@XZ', '??1CDefStringTable@@QAE@XZ')   # 0x012240A0: the dynamic initializer of the table (ctor 0x009D4DB0 = catalog CDefStringTable::CDefStringTable)
    return F.diffcheck(S0, S1, rem_ok, add_ok, need=[NEW])
F.run(ov, F.rows_from(rf), transform, check, tag)
