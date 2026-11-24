# sigdiff.py <idx...>: per signature entry, the demangled PDB spelling and every other spelling (with its binders), flagged
# STANDIN when the wrong spelling names a class absent from both PDB name sets (a stand-in type -> port), else INPLACE?
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF323'
sys.path.insert(0, A + '/tools'); sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline/naming'); import nf
import msvc_demangle as M
G = '/home/user/FableDecomp/ghidra_out/'
txt = open(G + 'egor_pdb_names.tsv', encoding='utf-8', errors='replace').read() + open(G + 'fablewin_pdb_names.tsv', encoding='utf-8', errors='replace').read()
def dm(s):
    try: return M.pretty(s) if hasattr(M, 'pretty') else M.demangle(s)
    except Exception as e: return '?' + s
def classes(s): return set(re.findall(r'(?<=[VU])([A-Za-z_]\w*)@@', s)) | set(re.findall(r'@([A-Za-z_]\w*)@@', s))
E = nf.entries()
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; pdb = e[6]; sp = e[11]
    print('#%d %s %s' % (i, e[0], dm(pdb)))
    for part in sp.split(' || '):
        m = re.match(r'(\S+) \(([^)]*)\)', part)
        if not m: continue
        s, who = m.groups()
        if s == pdb: continue
        bad = sorted(c for c in classes(s) - classes(pdb) if ('V%s@@' % c) not in txt and ('U%s@@' % c) not in txt and ('@%s@@' % c) not in txt)
        print('   %s [%s] %s' % ('STANDIN:' + ','.join(bad) if bad else 'INPLACE?', who, dm(s)))
