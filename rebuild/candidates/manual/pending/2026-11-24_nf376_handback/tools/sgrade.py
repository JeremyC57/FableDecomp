#!/usr/bin/env python3
"""sgrade.py <tag> <src.cpp> <spec.tsv> [extra cl flags...]: compile ONE translation unit once (VC7.1, /O2 /Oy /W3 + extras, the
verify_and_land vc71() text) and grade EVERY unit address against its own catalog oracle at its own full-symbol leaf.
spec.tsv: address<TAB>leaf (full mangled symbol). Strict leaf: the graded symbol must EQUAL the leaf (no substring, no largest-function
fallback). Per address: status (MATCH / RELOCATION_MATCH / DIFFER(nvm)), graded symbol, reloc_identity hard errors / warnings.
Also prints the object's external DEF / UND symbols. Output: <W>/work/sg/<tag>/{<tag>.obj, grade.txt}. Run from the repo root,
lane env sourced (agent NF376, option S pilot)."""
import sys, os, re, subprocess, contextlib, io
from pathlib import Path
R = Path(os.environ.get('FABLE_ROOT', '/home/user/FableDecomp'))
sys.path.insert(0, str(R / 'tools/decomp_pipeline'))
import verify_and_land as V, reloc_identity as RI
W = Path(os.environ['S']) / 'w22/agentNF376'
sys.path.insert(0, str(W / 'tools')); import nf
tag, src, spec = sys.argv[1], sys.argv[2], sys.argv[3]; extra = sys.argv[4:]
wd = W / 'work/sg' / tag; wd.mkdir(parents=True, exist_ok=True)
sp = wd / (tag + '.cpp'); sp.write_text(V.vc71(Path(src).read_text(encoding='utf-8')), encoding='utf-8')
obj = wd / (tag + '.obj'); obj.unlink(missing_ok=True)
cp = V.cl(['/nologo', '/c'] + V.BASE_FLAGS + extra + ['/Fo%s' % obj, str(sp)], V.env())
out = []
if cp.returncode or not obj.exists():
    out.append('SRC_FAIL ' + ((cp.stdout or '') + (cp.stderr or ''))[-1500:])
else:
    for l in open(spec).read().splitlines():
        if not l.strip() or l.startswith('#'): continue
        a, leaf = l.split('\t')[:2]
        retail = bytes.fromhex(nf.catalog()[a]['bytes'])
        with contextlib.redirect_stderr(io.StringIO()) as er: built, sec, sym = V.obj_text(obj, leaf)
        rel = V.obj_relocs(obj, sec)
        st = 'MATCH' if built == retail else ('RELOCATION_MATCH' if len(built) == len(retail) and V.mask(retail, rel) == V.mask(built, rel) else 'DIFFER(%dv%d)' % (len(built), len(retail)))
        strict = 'LEAF_OK' if sym == leaf else 'LEAF_MISMATCH(%s)' % sym
        h, w_, pairs = RI.check(R, V.OBJDUMP, obj, leaf, a, retail, built=built) if st != 'DIFFER' and not st.startswith('DIFFER') else ([], [], [])
        out.append('%s\t%s\t%s\thard=%s\twarn=%s' % (a, st, strict, ';'.join(h) or '-', ';'.join(w_) or '-'))
        out.append('   relocs ' + ' '.join('%s->%s' % (p[2], p[3]) for p in pairs))
    out.append('# DEF/UND')
    out += ['   ' + x for x in sorted(nf.ext_syms(obj))]
t = '\n'.join(out) + '\n'; (wd / 'grade.txt').write_text(t); print(t)
