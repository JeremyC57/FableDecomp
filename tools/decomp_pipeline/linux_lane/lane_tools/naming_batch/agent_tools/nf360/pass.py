# pass.py <tag> <H|N> <addr>=<src>[:<len>] ...: one-path VC7.1 compile + grade. Each row compiles at the SAME path in every pass
# (work/one/<addr>/<addr>.cpp, so ?A0x hashes and __FILE__ strings agree between passes); the object is kept as work/obj/H_<addr>.obj
# (pass H) or work/obj/<addr>.obj (pass N); `<addr>@<tag>=<src>` keeps a variant as work/obj/<addr>_<tag>.obj. Headers come from FABLE_HDR_OVERLAY (the caller swaps the tree's files between passes).
# Flags: catalog CompilerFlags (nf.flags_of), else the source's // Flags: line. Oracle: catalog bytes, else Fable.exe at <addr> for <len>.
# Refuses a landed row whose lock is not NF352's. Log: work/pass_<tag>.log. Launch in the background with `timeout 900`.
import sys, os, re, shutil, subprocess
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
tag, kind = sys.argv[1], sys.argv[2]
log = open(A / 'work' / ('pass_%s.log' % tag), 'a')
print('# overlay=%s' % os.environ.get('FABLE_HDR_OVERLAY', ''), file=log); log.flush()
for spec in sys.argv[3:]:
    a, rest = spec.split('=', 1); a = a.lower(); suf = ''
    if '@' in a: a, suf = a.split('@', 1); suf = '_' + suf   # addr@tag: a variant source, object kept as <addr>_<tag>.obj
    src, ln = (rest.rsplit(':', 1) + [None])[:2] if re.search(r':\d+$', rest) else (rest, None)
    if nf.lock_owner(a) != "NF360": print('NOLOCK', a, file=log); log.flush(); continue
    text = Path(src).read_text(encoding='utf-8')
    c = nf.catalog().get(a)
    if c: retail = bytes.fromhex(c['bytes'])
    else:
        va = int(a, 16)
        for nm, base, d, ch in nf.exe():
            if base <= va < base + len(d): retail = d[va - base: va - base + int(ln)]
    while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
    wd = A / 'work' / 'one' / a; wd.mkdir(parents=True, exist_ok=True)
    leaf = nf.leaf_of(text, a) if c else re.search(r'^// Leaf:\s*(\S+)', text, re.M).group(1)
    fl = nf.flags_of(a, text) if c else [x for x in (re.search(r'^// Flags:[ \t]*(.*)', text, re.M) or re.search('()', '')).group(1).split() if x not in ('/O2', '/Oy', '/W3', 'none', '(none)')]
    st, built, sec = V.parity_of(V.vc71(text), a, leaf, retail, wd, V.env(), extra=tuple(fl))
    dst = A / 'work' / 'obj' / (('H_' if kind == 'H' else '') + a + suf + '.obj'); dst.parent.mkdir(exist_ok=True)
    if (wd / (a + '.obj')).exists(): shutil.copy(wd / (a + '.obj'), dst)
    print('%s %s %s leaf=%s flags=%s src=%s' % (kind, a + suf, st, leaf[:90], ' '.join(fl), src), file=log); log.flush()
print('PASS_DONE', file=log); log.close()
