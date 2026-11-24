# p2legs361.py: Part 2 object / closure / fold legs for GetDef<CMaterialDef>.
#  defscan: every object in work/obj/H_*.obj (all NF361 HEAD compiles, Part 1 and Part 2) that DEFs or UNDs the symbol.
#  closure: in each DEF object, every OTHER function COMDAT whose relocations name the symbol (intermediates), with its pk_all address.
#  fold:    every landed row whose source quotes (transitively, inside rebuild/src/compiled) the Source of an emitter row.
import sys, os, re, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
GM = '??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMaterialDef@@@@@Z'
W = str(nf.W); C = '/home/user/FableDecomp/rebuild/src/compiled'
emit = []
for o in sorted(glob.glob(W + '/obj/H_*.obj')):
    a = os.path.basename(o)[2:10]; secs, sl = nf.coff(o)
    st = [('DEF' if sec > 0 else 'UND') for i, n, v, sec, t, scl in sl if n == GM and scl == 2]
    if st: print('defscan', a, ','.join(st))
    if 'DEF' in st:
        emit.append(a)
        byidx = {}
        for i, n, v, sec, t, scl in sl:
            if scl == 2 and sec > 0 and secs[sec - 1]['name'].startswith('.text'): byidx.setdefault(sec, n)
        for k, s in enumerate(secs, 1):
            if s['name'].startswith('.text') and any(r[1] == GM for r in s['rel']) and byidx.get(k) != GM:
                print('closure', a, 'intermediate', byidx.get(k))
srcs = {os.path.normpath(os.path.join(C, nf.field(a, 'Source'))): a for a in emit if nf.field(a, 'Source')}
rows = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", nf.PS, re.S):
    a = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1)); s = re.search(r"\bSource = '([^']*)'", m.group(1))
    if a and s: rows.setdefault(a.group(1).lower(), os.path.normpath(os.path.join(C, s.group(1))))
def incs(p, seen):
    if p in seen or not os.path.exists(p): return seen
    seen.add(p)
    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', open(p, encoding='utf-8', errors='replace').read(), re.M):
        q = os.path.normpath(os.path.join(os.path.dirname(p), m.group(1)))
        if not os.path.exists(q): q = os.path.normpath(os.path.join('/home/user/FableDecomp/rebuild/include', m.group(1)))
        if q.startswith(C): incs(q, seen)
    return seen
nfold = 0
for a, p in rows.items():
    inc = incs(p, set()) - {p}
    for s, e in srcs.items():
        if s in inc: nfold += 1; print('fold', a, 'includes emitter', e)
print('emitters', ' '.join(emit), '| fold hits', nfold)
