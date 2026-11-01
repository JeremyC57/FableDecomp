# vtowner.py: for each nesting row, the vtable(s) holding the address, slot, the code storing the vtable start, and the PDB check
import sys, re, pe
sys.path.insert(0, '.')
from rows import na
PDB = {}
for tag, p in (('E', '/home/user/FableDecomp/ghidra_out/egor_pdb_names.tsv'), ('F', '/home/user/FableDecomp/ghidra_out/fablewin_pdb_names.tsv')):
    for l in open(p, errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2: PDB.setdefault(q[1], set()).add(tag)
def vts(a):
    out = []
    for s, va in pe.find(a, ('.rdata',)):
        # walk back to the vtable start: the code-referenced dword address at or before va (slots 0..6)
        for k in range(0, 8):
            st = va - 4 * k
            refs = pe.find(st, ('.text',))
            if refs: out.append((st, k, [(r, pe.owner(r)) for _, r in refs])); break
    return out
if __name__ == '__main__':
    for l in open(sys.argv[1]):
        q = l.rstrip('\n').split('\t'); a = int(q[0], 16); meth, ent, scr = q[1:4]
        want = '?%s@%s@%s@NScript@@EAEXXZ' % (meth, ent, scr)
        res = []
        for st, k, refs in vts(a):
            own = [o for r, o in refs]
            m = [re.search(r'NScript::(\w+)::(\w+)::Alloc', o or '') for o in own]
            al = [(x.group(1), x.group(2)) for x in m if x]
            res.append('vt %08x slot %d by %s' % (st, k, ';'.join(o or hex(r) for r, o in refs)))
            for s2, e2 in al:
                pdbn = '?%s@%s@%s@NScript@@EAEXXZ' % (meth, e2, s2)
                res.append('=> %s %s pdb:%s %s' % (s2, e2, ''.join(sorted(PDB.get(pdbn, ()))), 'SAME' if pdbn == want else 'DIFF'))
        print(q[0], meth, ent, scr, 'want-pdb:' + ''.join(sorted(PDB.get(want, ()))), '|', ' | '.join(res))
