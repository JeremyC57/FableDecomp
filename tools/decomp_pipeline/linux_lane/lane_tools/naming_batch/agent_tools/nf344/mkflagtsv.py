#!/usr/bin/env python3
"""mkflagtsv.py <out.tsv> <results.jsonl>... (agent NF344): FLAG_CHANGES.tsv from flagrun results. One line per proposed change:
address, catalog flags old -> new, grade before / after (flagrun parity grade), pk_all leaf-called copies EQ/NEQ before / after, old checker
(pickany_vs_retail.py) EQ/NEQ before / after, the copies repaired, and the decision class."""
import json, re, sys
from collections import defaultdict
HASH = re.compile(r'\?A0x[0-9a-f]{8}')
CH = [  # address, variant, new catalog flags, class, note
 ('0064b1ac', 'NOD', '/O2 /Oy /W3 /Os /Oy-', 'drop-unneeded (DECISION: Codex pass 27 RESERVED.tsv still lists the row)', 'object identical without /d2inlT59 (every function COMDAT, bytes + relocations)'),
 ('00e4aa00', 'NOD', '(remove CompilerFlags; = /O2 /Oy /W3)', 'drop-unneeded', 'object identical without /d2inlT200'),
 ('00e4fa20', 'NOD', '(remove CompilerFlags; = /O2 /Oy /W3)', 'drop-unneeded (DECISION: Codex pass 27 RESERVED.tsv still lists the row)', 'object identical without /d2inlT69'),
 ('00b2c8e0', 'NOD', '(remove CompilerFlags; = /O2 /Oy /W3)', 'drop-repairs-nonleaf-copy', 'only change: NOADDR vector<unsigned char>::resize(n) COMDAT 94 B -> 25 B; the 25-B body occurs in Fable.exe (0x00412880 vector<char>::resize, 0x007B9E50), the 94-B one nowhere'),
 ('007677e0', 'T60', '/O2 /Oy /W3 /d2inlT60', 'value-change-repairs', 'repairs 0x007696E0 / 0x007698C0 __uninitialized_copy<AttackParticleAndEndPoints> (leaf-reached); also changes NOADDR __uninitialized_fill_n<AttackParticleAndEndPoints> 45 B -> 113 B (neither form found in Fable.exe); T58 DIFFER, T59 HEAD, T61 also changes ??E _Rb_tree_iterator'),
 ('00b47bc0', 'T39', '/O2 /Oy /W3 /d2inlT39', 'value-change-repairs', 'repairs 0x00B49DF0 _M_insert_overflow<CManagerLocalLight*> (leaf-called); also NOADDR ??2CManagerLocalLight 12 B -> 51 B, the 51-B body occurs in Fable.exe at 0x00B45310 / 0x00B45350 / 0x00B45390 (and the CParticle-pool siblings 0x00AF8680 / 0x00AF86C0 / 0x00AF8B00); T30..T38 no repair, T39..T45 repair'),
 ('00afcc30', 'T1537', '/O2 /Oy /W3 /d2inlT1537', 'drop-unneeded (S15 only)', 'object identical without /d2inlS15'),
 ('00b03ca0', 'T1537', '/O2 /Oy /W3 /d2inlT1537', 'drop-unneeded (S15 only)', 'object identical without /d2inlS15'),
]
def stats(r):
    eq = neq = 0; neqs = set(); eqs = set()
    for x in r.get('copies', []) or []:
        if x.get('leafref') == 'y' or x.get('reach') == 'y':
            k = HASH.sub('?A0x', x['symbol']) + '@' + x['address']
            if x['verdict'].startswith('EQ'): eqs.add(k)
            elif x['verdict'].startswith('NEQ'): neqs.add(k)
    o = {'EQ': 0, 'NEQ': 0}
    for l in r.get('old', []) or []:
        f = l.split()
        if f and f[0] in o: o[f[0]] += 1
    return eqs, neqs, o
def main():
    out = sys.argv[1]; R = defaultdict(dict)
    for f in sys.argv[2:]:
        for l in open(f):
            r = json.loads(l); R[r['row']][r['variant']] = r
    with open(out, 'w') as o:
        o.write('address\tclass\tcatalog_flags_old\tcatalog_flags_new\tgrade_before\tgrade_after\tpk_all_leafcalled_EQ/NEQ_before\tpk_all_leafcalled_EQ/NEQ_after\t'
                'old_checker_EQ/NEQ_before\told_checker_EQ/NEQ_after\tcopies_repaired\tcopies_broken\tnote\n')
        for a, v, nf, cls, note in CH:
            h = R[a]['CAT']; n = R[a][v]
            he, hn, ho = stats(h); ne, nn, no = stats(n)
            rep = sorted(k.split('@')[-1] for k in hn if k in ne); brk = sorted(k.split('@')[-1] for k in nn if k not in hn)
            o.write('\t'.join([a, cls, h['flags'], nf, h['grade'], n['grade'], '%d/%d' % (len(he), len(hn)), '%d/%d' % (len(ne), len(nn)),
                               '%d/%d' % (ho['EQ'], ho['NEQ']), '%d/%d' % (no['EQ'], no['NEQ']), ','.join(rep) or '-', ','.join(brk) or '-', note]) + '\n')
main()
