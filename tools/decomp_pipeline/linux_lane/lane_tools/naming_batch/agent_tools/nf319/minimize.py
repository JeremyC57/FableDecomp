# minimize.py <idx..>: greedy minimisation of a draft's preamble. Candidate lines: every `#include` / forward `class|struct X;` line
# not in the HEAD row, then every `#define FABLE_*` except the class's own branch switches (work/feas.tsv) and its _RTTI_DECLS opt-in.
# A line is dropped when, without it, (1) clang -fsyntax-only on work/tree passes, (2) the override probe passes (tools/probe.py),
# (3) gcc cpp shows no #error. Rewrites rows/<addr>.cpp; log -> work/minimize.log
import re, sys, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
S = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad'
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
F = {int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/feas.tsv')}
F.update({int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/feas_decl.tsv')})
D = {int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/decls.tsv')}
def clang_ok(f):
    cmd = ['clang++', '-target', 'i686-pc-windows-msvc', '-fms-extensions', '-fms-compatibility', '-fms-compatibility-version=13.10', '-fsyntax-only', '-w',
           '-Wno-address-of-temporary', '-ferror-limit=5', '-I' + str(A / 'work/tree'), '-isystem', S + '/wine/vc71/include', str(f)]
    try: r = subprocess.run(cmd, capture_output=True, text=True, timeout=90)
    except subprocess.TimeoutExpired: return False
    return not re.search(r': (fatal )?error', r.stdout + r.stderr)
def cpp_ok(f):
    r = subprocess.run(['g++', '-E', '-x', 'c++', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-I' + str(A / 'work/tree'), '-isystem', S + '/wine/vc71/include', str(f), '-o', '/dev/null'], capture_output=True, text=True, timeout=60)
    return '#error' not in r.stderr
def probe_ok(a, X, decl):
    r = subprocess.run(['python3', str(A / 'tools/probe.py'), a, X, decl], capture_output=True, text=True)
    return ' PROBE PASS ' in r.stdout
log = open(A / 'work/minimize.log', 'a')
for i in [int(x) for x in sys.argv[1:]]:
    e = E[i - 1]; a = e[0]; X = F[i][1]; keep_sw = set(F[i][3].split(',')) | {'FABLE_RETAIL_%s_RTTI_DECLS' % X.upper()}
    decl = D[i][2]; rp = A / 'rows' / (a + '.cpp')
    head = set(x.strip() for x in (A / 'orig' / (a + '.cpp')).read_text(encoding='utf-8').split('\n'))
    L = rp.read_text(encoding='utf-8').split('\n')
    if not (clang_ok(rp) and probe_ok(a, X, decl) and cpp_ok(rp)): print(i, a, 'START_FAIL', file=log); log.flush(); print(i, a, 'START_FAIL'); continue
    cand = [k for k, l in enumerate(L) if (re.match(r'\s*#\s*include\s', l) or re.match(r'\s*(class|struct)\s+\w+\s*;\s*$', l)) and l.strip() not in head]
    cand += [k for k, l in enumerate(L) if re.match(r'\s*#\s*define\s+FABLE_\w+\s*$', l) and l.split()[1] not in keep_sw]
    drop = set(); tmp = A / 'work/probe' / ('min_' + a + '.cpp')
    for k in cand:
        trial = [l for j, l in enumerate(L) if j not in drop and j != k]
        rp.write_text('\n'.join(trial), encoding='utf-8')
        if clang_ok(rp) and cpp_ok(rp) and probe_ok(a, X, decl): drop.add(k)
    final = [l for j, l in enumerate(L) if j not in drop]
    rp.write_text('\n'.join(final), encoding='utf-8')
    ok = clang_ok(rp) and probe_ok(a, X, decl) and cpp_ok(rp)
    print(i, a, 'MIN', 'OK' if ok else 'FINAL_FAIL', 'dropped %d of %d' % (len(drop), len(cand)), file=log); log.flush()
    print(i, a, 'OK' if ok else 'FINAL_FAIL', len(drop), len(cand))
