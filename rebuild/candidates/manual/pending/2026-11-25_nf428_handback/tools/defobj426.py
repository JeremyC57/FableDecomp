#!/usr/bin/env python3
"""defobj426.py <list file of repo-relative sources> <out tsv>: compile each landed (HEAD) source with VC7.1 (nf.compile_one, the row's
flags) into work/def/<addr>.obj; print every DEF COMDAT named ?Release@?$CDefPointer / ??1?$CDefPointer / ?Release@?$CIVCountedPointer /
??1?$CIVCountedPointer with its selection, size and raw bytes hex (address<TAB>symbol<TAB>sel<TAB>size<TAB>hex<TAB>nrelocs)."""
import sys, os, re, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, coff
L = [l.strip() for l in open(sys.argv[1]) if l.strip()]; out = open(sys.argv[2], 'a')
os.makedirs(str(nf.W / 'def'), exist_ok=True)
PAT = re.compile(r'^(\?Release@|\?\?1)\?\$(CDefPointer|CIVCountedPointer)@')
for f in L:
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if not m: print('NOADDR', f, file=out, flush=True); continue
    a = m.group(1); dst = nf.W / 'def' / (a + '.obj')
    if not dst.exists():
        try:
            st, leaf, fl, obj, wd = nf.compile_one(a, '/home/user/FableDecomp/rebuild/src/compiled/' + f.lstrip('./'), 'D')
            if obj.exists(): shutil.copy(obj, dst)
        except Exception as ex:
            print('ERR', a, f, ex, file=out, flush=True); continue
    if not dst.exists(): print('NOOBJ', a, f, file=out, flush=True); continue
    n = 0
    for s in coff.sections(str(dst)):
        if s['name'].startswith('.text') and s['sym'] and PAT.match(s['sym']):
            print('\t'.join([a, s['sym'], str(s['sel']), str(s['size']), s['raw'].hex()]), file=out, flush=True); n += 1
    print('DONE', a, n, file=out, flush=True)
