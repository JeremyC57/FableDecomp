# pkrun.py <H|N> <addr<TAB>src list> [addr=src:len ...]: run the NF340 pk_all.py (--tsv) and the official pickany_vs_retail.py on
# work/obj/(H_)<addr>.obj with the row's // Leaf, writing evidence/pk/<H|N>_<addr>.pkall.tsv / .pvr.txt. No compile.
import sys, os, re, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
PK = R + '/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py'
PVR = R + '/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py'
kind = sys.argv[1]; rows = []
if os.path.exists(sys.argv[2]):
    rows += [tuple(l.rstrip('\n').split('\t')[:2]) + (None,) for l in open(sys.argv[2]) if l.strip()]
for x in sys.argv[3:]:
    a, rest = x.split('=', 1); s, ln = rest.rsplit(':', 1); rows.append((a, s, ln))
os.makedirs(A + '/evidence/pk', exist_ok=True)
for a, src, ln in rows:
    obj = A + '/work/obj/' + ('H_' if kind == 'H' else '') + a + '.obj'
    if not os.path.exists(obj): print('MISSING', a); continue
    leaf = re.search(r'^// Leaf:\s*(\S+)', open(src, encoding='utf-8').read(), re.M)
    leaf = leaf.group(1) if leaf else nf.leaf_of(open(src, encoding='utf-8').read(), a)
    base = A + '/evidence/pk/%s_%s' % (kind, a)
    r = subprocess.run(['python3', '-I', '-B', PK, obj, a, '--leaf=' + leaf, '--tsv'] + (['--row-length=' + ln] if ln else []),
                       capture_output=True, text=True, cwd=R)
    open(base + '.pkall.tsv', 'w').write(r.stdout + r.stderr)
    r = subprocess.run(['python3', '-I', '-B', PVR, os.environ['FABLE_EXE'], R + '/rebuild/oracles/reloc_bindings.tsv', obj, a, '--leaf=' + leaf],
                       capture_output=True, text=True, cwd=R)
    open(base + '.pvr.txt', 'w').write(r.stdout + r.stderr)
print('PKRUN_DONE', kind, len(rows))
