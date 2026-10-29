# wave 7 family gpi7: the fam_gpi6 skips -- rows whose local CBaseIntelligentPointer declares `CThing* GetPItem() const;`
# (emitting ?GetPItem@CBaseIntelligentPointer@@QBEPAVCThing@@XZ) AND carry a typed CIntelligentPointer<T>::GetPItem() wrapper,
# so base calls and wrapper calls cannot be told apart textually. Final: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
# (Ego_r.pdb / FableWin.pdb; 0x00A01B50). Transform (idempotent; compile-driven):
#   1. the base declaration -> `CBaseObject* GetPItem() const;` (+ `class CBaseObject;` ahead of the class when absent);
#   2. compile; for each error line, try the uncast GetPItem() calls on that statement one at a time, wrapping the call chain as
#      `((CThing*)<chain>GetPItem())` (the item cast retail's inline CIntelligentPointer<CThing>::GetPItem() performs), and keep
#      a cast only when it lowers the error count; repeat until the file compiles (cap 40 compiles), else leave the file alone.
#   The cast is a pointer reinterpretation (no code), so bytes are unchanged; recheck proves it.
import re, sys, os
W = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, W); sys.path.insert(0, os.path.join(W, 'tools'))
from fam_ref6 import enclosing_class, chain_start
import cc7
DECL = re.compile(r'(^[ \t]*|(?<=[;{])[ \t]*)(?:class[ \t]+)?CThing[ \t]*\*[ \t]*GetPItem[ \t]*\([ \t]*\)[ \t]*const[ \t]*;', re.M)
CALL = re.compile(r'(?<![\w])GetPItem[ \t]*\(\)')
def lineno(t, pos): return t.count('\n', 0, pos) + 1
def calls(t):
    out = []
    for m in CALL.finditer(t):
        ls = t.rfind('\n', 0, m.start()) + 1
        if '//' in t[ls:m.start()]: continue
        pre = t[ls:m.start()]
        if re.search(r'\*[ \t]*(\w+::)?$', pre.rstrip()) or re.search(r'\b\w+[ \t]*\*[ \t]*(\w+::)?$', pre): continue   # declarations
        if re.search(r'(?<![.>\w])CBaseIntelligentPointer::$', pre): continue                                                    # wrapper body
        st = chain_start(t, m.start())
        if st is None: continue
        if t[max(0, st - 10):st].endswith('((CThing*)'): continue                                                   # already cast
        out.append((st, m.end()))
    return out
def cast(t, st, en): return t[:st] + '((CThing*)' + t[st:en] + ')' + t[en:]
def fix(t, tag):
    ds = [d for d in DECL.finditer(t) if enclosing_class(t, d.start()) == 'CBaseIntelligentPointer']
    if not ds:
        if re.search(r'CBaseObject\*[ \t]*GetPItem\(\)[ \t]*const;', t): return None, 'already'
        return None, 'decl shape'
    if len(ds) != 1: return None, 'two base decls'
    d = ds[0]
    t = t[:d.start()] + d.group(1) + 'CBaseObject* GetPItem() const;' + t[d.end():]
    cm = re.search(r'\b(?:class|struct)[ \t]+CBaseIntelligentPointer\b[^;{]*\{', t)
    if not re.search(r'\b(?:class|struct)[ \t]+CBaseObject\b', t[:cm.start()]):
        if re.search(r'\bstruct[ \t]+CBaseObject\b', t): return None, 'struct CBaseObject'
        ls = t.rfind('\n', 0, cm.start()) + 1
        t = t[:ls] + 'class CBaseObject;\n' + t[ls:]
    errs = cc7.errors(t, tag); n = 1; ncast = 0
    while errs and n < 40:
        if errs[0][0] == 0: return None, 'compile %s' % errs[0][1][:80]
        L = errs[0][0]
        cands = [(st, en) for st, en in calls(t) if L - 3 <= lineno(t, en) <= L]
        cands.sort(key=lambda c: -c[0])
        best = None
        before = sum(1 for x in errs if L - 3 <= x[0] <= L)
        for st, en in cands:
            t2 = cast(t, st, en); e2 = cc7.errors(t2, tag); n += 1
            if e2 and e2[0][0] == 0: continue
            after = sum(1 for x in e2 if L - 3 <= x[0] <= L)
            if after < before and len(e2) <= len(errs) + 2 and (best is None or after < best[2]): best = (t2, e2, after)
            if best is not None and best[2] == 0: break
            if n >= 40: break
        if best is None and len(cands) > 1:
            # several calls of one statement fail one at a time: cast them all, then drop each cast that is not needed
            keep = sorted({c[0]: c for c in cands}.values(), key=lambda c: -c[0])
            def apply(cs):
                u = t
                for st, en in sorted(cs, key=lambda c: -c[0]): u = cast(u, st, en)
                return u
            t2 = apply(keep); e2 = cc7.errors(t2, tag); n += 1
            if not (e2 and e2[0][0] == 0) and sum(1 for x in e2 if L - 3 <= x[0] <= L) == 0:
                for c in list(keep):
                    trial = [k for k in keep if k != c]
                    t3 = apply(trial); e3 = cc7.errors(t3, tag); n += 1
                    if not (e3 and e3[0][0] == 0) and sum(1 for x in e3 if L - 3 <= x[0] <= L) == 0: keep = trial; t2, e2 = t3, e3
                best = (t2, e2, 0)
        if best is None: return None, 'no cast clears line %d: %s | cands %s' % (L, errs[0][1][:90], [t[c[0]:c[1]] for c in cands])
        t, errs = best[0], best[1]; ncast += 1
    if errs: return None, 'cap'
    return t, 'ok casts %d compiles %d' % (ncast, n)
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t, os.path.basename(p)[:-4])
        if new is None: print('skip', p, why); sys.stdout.flush(); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why); sys.stdout.flush()
