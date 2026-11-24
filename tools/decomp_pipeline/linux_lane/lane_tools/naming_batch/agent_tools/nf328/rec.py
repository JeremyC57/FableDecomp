# rec.py <tag>: STATUS lines from evidence/libcp_<tag>_vland.txt + work/recheck_<tag>n.log (done when .drectve equal or a flatten row;
# skipped-held READY (Q1) for an alias row whose .drectve gains /DEFAULTLIB:"libcp")
import subprocess, sys
t = sys.argv[1]
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open('ENTRIES.tsv')}
g = {}
for l in open('work/recheck_%sn.log' % t):
    p = l.split()
    if len(p) > 2 and len(p[0]) == 8 and p[2] == 'PASS': g[p[0]] = p[1]
for l in open('evidence/libcp_%s_vland.txt' % t):
    a = l.split()[0]
    if 'LIBCP_OK' not in l or a not in g: print('SKIP', a); continue
    old, new = l.split('renames=')[1].split(' ;')[0].split('=', 1)
    assert new == E[a][3], (a, new)
    lc = 'libcp"' in l
    base = '%s; vland %s PASS (%s); objsec ALL_EQ under the one rename (%s), UND sets equal, DEF set = HEAD under the rename, no ??_7/??_R; graded symbol = the PDB/RTTI symbol; evidence/libcp_%s_vland.txt' % (
        E[a][2], g[a], 'landed test unchanged', '.drectve = HEAD + /DEFAULTLIB:"libcp"' if lc else '.drectve equal', t)
    if lc and E[a][2] != 'flatten-switch1':
        subprocess.run(['python3', 'tools/st.py', a, 'skipped-held', 'READY (full proof) but held for Q1: the alias route adds /DEFAULTLIB:"libcp" (the 2026-11-23 ruling names the flatten switch route); ' + base])
    else: subprocess.run(['python3', 'tools/st.py', a, 'done', base])
    print(a, 'recorded')
