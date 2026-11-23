# tri2.py [idx..]: NF319 alias-class triage. Per ENTRIES line: the class model branches of engine/<X>.h (#if stack of each
# `class|struct X` definition), the base list, whether landed rows define every switch of a branch (count), the PDB name-set
# hits for the exact pdb_spelling (egor / fablewin), the RTTI placement, and whether a base header declares M (override).
# -> work/tri2.tsv
import re, sys, subprocess, os
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
R = Path('/home/user/FableDecomp')
INC = R / 'rebuild/include'
GUARD = re.compile(r"#\s*ifndef\s+(FABLE_ENGINE_\w+_H|FABLE_RETAIL_HEADER_\w+|\w+_H|\w+_HPP|\w+_INCLUDED)\s*$")
def load(p):
    d = {}
    for l in open(p, encoding='utf-8', errors='replace'):
        a, _, s = l.rstrip('\n').partition('\t'); d.setdefault(s, set()).add(a)
    return d
EG = load(R / 'ghidra_out/egor_pdb_names.tsv'); FW = load(R / 'ghidra_out/fablewin_pdb_names.tsv')
RT = {}
for l in open(R / 'ghidra_out/labels_rtti_port.tsv'):
    a, _, s = l.rstrip('\n').partition('\t'); RT[a.lower()] = s
_defcache = {}
def landed_definers(sw):
    if sw in _defcache: return _defcache[sw]
    r = subprocess.run(['grep', '-rlE', r'^\s*#\s*define\s+' + sw + r'\b', str(R / 'rebuild/src/compiled'), '--include=*.cpp'], capture_output=True, text=True).stdout.split()
    _defcache[sw] = set(r); return _defcache[sw]
def branches(h, X):
    L = h.read_text(encoding='utf-8', errors='replace').split('\n'); stack = []; out = []
    for n, ln in enumerate(L, 1):
        s = ln.strip()
        if re.match(r"#\s*if", s): stack.append([s, False]); continue
        if re.match(r"#\s*(else|elif)", s) and stack: stack[-1] = [stack[-1][0] + ' |ELSE:' + s, True]; continue
        if re.match(r"#\s*endif", s) and stack: stack.pop(); continue
        m = re.match(r"\s*(class|struct)\s+(?:FABLE_\w+\s+)?" + re.escape(X) + r"\s*(:[^{;]*)?(\{|$)", ln)
        if m and not re.match(r"\s*(class|struct)\s+" + re.escape(X) + r"\s*;", ln):
            inner = [x for x in stack if not GUARD.match(x[0])]
            out.append((n, ' & '.join(x[0] for x in inner) or '-', (m.group(2) or '').strip(), any(x[1] for x in inner)))
    return out
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
sel = set(int(x) for x in sys.argv[1:]) if len(sys.argv) > 1 else None
o = open(A / 'work/tri2.tsv', 'w')
for i, e in enumerate(E, 1):
    if sel and i not in sel: continue
    pdb = e[3]; m = re.match(r"\?([^@]+)@([^@]+)@@", pdb); M, X = m.group(1), m.group(2)
    eg = 'EG' if pdb in EG else '-'; fw = 'FW' if pdb in FW else '-'
    rt = 'RT' if RT.get(e[0].lower()) == pdb else 'RT?' + RT.get(e[0].lower(), 'none')[:60]
    h = INC / 'engine' / (X + '.h')
    if not h.exists(): print(i, e[0], X, M, eg, fw, rt, 'NOHEADER', sep='\t', file=o); continue
    for n, st, base, inelse in branches(h, X):
        sws = re.findall(r'#\s*ifdef\s+(\w+)', st) + re.findall(r'defined\s*\(\s*(\w+)\s*\)', st)
        if inelse or 'ifndef' in st and not sws: kind = 'FLAT?'
        else: kind = 'CLASS' if base else 'NOBASE'
        cnt = [len(landed_definers(s)) for s in sws]
        both = len(set.intersection(*[landed_definers(s) for s in sws])) if sws else 0
        print(i, e[0], X, M, eg, fw, rt, 'L%d' % n, kind, base[:60], st[:140], ','.join(map(str, cnt)), 'all=%d' % both, sep='\t', file=o)
