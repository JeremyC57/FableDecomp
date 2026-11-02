# mklabels26.py (from mklabels25.py): LABELS.tsv -- for every queue target this wave respelled (fully or partly) and the decision targets,
# the qualified name of the settled spelling, when no ghidra_out/labels_*.tsv row already carries that exact name.
import os, sys, re, glob, collections
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(W, 'scripts'))
import und
L = collections.defaultdict(set)
for p in glob.glob('/home/user/FableDecomp/ghidra_out/labels_*.tsv'):
    for l in open(p, errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2: L[q[0].lower().replace('0x', '').zfill(8)].add(q[1])
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W26.tsv'))][1:]
out = [('address', 'name', 'evidence')]
seen = set()
def bare(n):
    # the catalog's bare spelling: no class/struct/enum/union keywords, `const T`, `T*`, no space after a comma, `> >`,
    # and a ctor / dtor named without its repeated template arguments
    n = re.sub(r'\b(class|struct|enum|union) ', '', n)
    n = re.sub(r'\b((?:unsigned |signed )?(?:long long|long|int|short|char)) const\b', r'const \1', n)
    n = re.sub(r'([\w:]+(?:<[^<>]*>)?) const\b', r'const \1', n)
    n = re.sub(r'\s*\*', '*', n); n = re.sub(r',\s+', ',', n)
    while '>>' in n: n = n.replace('>>', '> >')
    d = 0; cut = -1
    for i, ch in enumerate(n):
        if ch == '<': d += 1
        elif ch == '>': d -= 1
        elif ch == ':' and d == 0 and n[i:i + 2] == '::': cut = i
    if cut > 0:
        head, tail = n[:cut], n[cut + 2:]
        cls = re.split(r'::', re.sub(r'<.*', '', head))[-1]
        mm = re.match(r'^(~?)(\w+)<.*>$', tail)
        if mm and mm.group(2) == cls: n = head + '::' + mm.group(1) + mm.group(2)
    return n
def SQ(n):
    n = re.sub(r'\s+', '', re.sub(r'\b(class|struct|enum|union) ', '', n))
    prev = None
    while prev != n: prev = n; n = re.sub(r',std::allocator<[^<>]*(?:<[^<>]*>[^<>]*)*>', '', n)
    return n
def qn(sym):
    d = und.get([sym])[sym]
    m = re.search(r"([\w:<>,*&]+)::`scalar deleting dtor'", re.sub(r',\s+', ',', re.sub(r'\b(class|struct|enum|union) ', '', d)))
    if m: return m.group(1).split(' ')[-1] + "::`scalar deleting destructor'"
    return und.qname(d) if d != sym else None
FIX = {  # queue spellings proven wrong / display-string finals: the settled mangled spelling (0099a300: the PDB access is MAE, not the queue's UAE)
 '0099a300': '??1CBaseClassNonCopyable@@MAE@XZ',
 '013ca828': '?GDefStringTable@@3VCDefStringTable@@A',
}
for q in Q:
    ch, a, fin, ev, ws, o = q[:6]
    if o not in ('respelled', 'respelled-partial'): continue
    t = a.lower().replace('0x', '').zfill(8)
    fin = FIX.get(t, fin)
    if not fin.startswith('?') or fin == '?g_framesPerSecond@@3JA': continue   # the c682 descriptive row of 0x01375550: the cg10 ConstantFPS spelling was applied
    if '<' in fin:  # anonymous-namespace data symbol with the TU hash left open: the name without the hash
        m = re.match(r'\?(\w+)@\?A0x', fin); name = "`anonymous namespace'::" + m.group(1) if m else None
    else: name = qn(fin)
    if not name or (t, name) in seen: continue
    seen.add((t, name))
    name = bare(name).lstrip('*& ')
    if SQ(name) in set(SQ(x) for x in L.get(t, ())): continue
    out.append((t, name, 'naming wave 26 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = []
_rp = os.path.join(W, 'RENAMES.tsv')
if os.path.exists(_rp):
    for l in list(open(_rp))[1:]:
        r = l.rstrip('\n').split('\t')
        if len(r) >= 10: EXTRA.append((r[0], r[2], r[9] + '; rechecked MATCH / RELOCATION_MATCH + PASS'))
# every binding this wave ADDs names its target: the label follows when no labels_*.tsv row carries the name
for l in list(open(os.path.join(W, 'BINDINGS.tsv')))[1:]:
    r = l.rstrip('\n').split('\t')
    if r[0] != 'ADD' or '?A0x' in r[1]: continue
    n = qn(r[1])
    # stand-in scopes (address-suffixed / row namespaces / std helpers over row-local types) are not catalog names
    if n and re.search(r'[0-9a-f]{8}|\bRow|Fable|NFactory|NCreatureMode|CodeSections|^std::', n): continue
    if n: EXTRA.append((r[2], bare(n).lstrip('*& '), 'naming wave 26: %s, bound this wave (first named by %s); rechecked MATCH / RELOCATION_MATCH + PASS' % (r[1], r[3])))
for t, n, e in EXTRA:
    if SQ(n) not in set(SQ(x) for x in L.get(t, ())) and (t, n) not in seen and not any(r[0] == t and SQ(r[1]) == SQ(n) for r in out[1:]): out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
