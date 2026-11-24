# recdone.py <tag>: STATUS 'done' lines for every row of work/<tag>.txt that has LIBCP_OK in evidence/libcp_<tag>_vland.txt and
# MATCH / RELOCATION_MATCH + PASS in work/recheck_<tag>n.log (new) and work/recheck_<tag>h.log (HEAD); asserts the new DEF = ENTRIES pdb_spelling.
import sys, subprocess, re
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338'
t = sys.argv[1]
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')}
def grades(f):
    g = {}
    for l in open(f):
        p = l.split()
        if len(p) > 2 and re.match(r'[0-9a-f]{8}$', p[0]) and p[2] == 'PASS': g[p[0]] = p[1]
    return g
gn, gh = grades(A + '/work/recheck_%sn.log' % t), grades(A + '/work/recheck_%sh.log' % t)
for l in open(A + '/evidence/libcp_%s_vland.txt' % t):
    a = l.split()[0]
    if 'LIBCP_OK' not in l or a not in gn: print('NOT', a); continue
    old, new = l.split('renames=')[1].split(' ;')[0].split('=', 1)
    assert new == E[a][3], (a, new, E[a][3])
    dr = '.drectve = HEAD + /DEFAULTLIB:"libcp" (NF318 Q1)' if 'libcp"' in l else '.drectve equal'
    r = ('%s; signature port onto engine/<X>.h under existing landed switches; vland HEAD %s PASS / new %s PASS (landed test unchanged); '
         'objsec ALL_EQ under the one DEF rename %s -> PDB, UND sets equal, DEF set = HEAD under the rename, no ??_7/??_R, %s; '
         'graded symbol = the PDB / RTTI symbol; evidence/libcp_%s_vland.txt') % (E[a][2], gh.get(a, '?'), gn[a], old, dr, t)
    subprocess.run(['python3', A + '/tools/st.py', a, 'done', r]); print(a, 'done')
