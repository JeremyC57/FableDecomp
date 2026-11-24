# namecensus.py (NF325): for every rows/<addr>.cpp, its new self symbol (the // Leaf: line): other landed sources that define X::M( (a
# definition-shaped line, not a call) or carry a Leaf naming the same symbol, and landed headers with an inline body of X::M. -> evidence/namecensus.tsv
import re, subprocess, glob, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338'
R = '/home/user/FableDecomp/rebuild'
out = open(A + '/evidence/namecensus.tsv', 'w'); out.write('addr\tsymbol\tother_definers\tleaf_claims\n')
for f in sorted(glob.glob(A + '/rows/*.cpp')):
    if f.endswith('_test.cpp'): continue
    a = os.path.basename(f)[:8]; s = re.search(r'//\s*Leaf:\s*(\S+)', open(f).read()).group(1)
    m = re.match(r'\?([^@?]+)@([^@]+)@@', s)
    if not m: out.write('%s\t%s\tSKIP (special member; ctor binder, not a new name)\t-\n' % (a, s)); continue   # NF327
    M, X = m.group(1), m.group(2)
    r = subprocess.run(['grep', '-rlE', r'^[A-Za-z_][^;=]*[^.>:\w]' + X + r'::' + M + r'\s*\(', R + '/src/compiled', '--include=*.cpp'], capture_output=True, text=True).stdout.split()
    r = [x for x in r if not x.endswith('_%s.cpp' % a)]
    lf = subprocess.run(['grep', '-rlF', 'Leaf: ' + s, R + '/src/compiled'], capture_output=True, text=True).stdout.split()
    lf = [x for x in lf if not x.endswith('_%s.cpp' % a)]
    out.write('%s\t%s\t%s\t%s\n' % (a, s, ' '.join(os.path.basename(x) for x in r) or '-', ' '.join(os.path.basename(x) for x in lf) or '-'))
out.close()
