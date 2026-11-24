# census_report_op.py (NF331, from NF328's census_report.py) <census_op.tsv> <out CENSUS.tsv>: for ??4CScriptThing@@QAEAAV0@ABV0@@Z
# (self 0x008AB980): emitting objects, strong definers (COMDAT selection 1), pick-any copies equal / differing from retail (with length),
# objects that only reference it (UND), relocation shapes, and the non-79-slot ??_7CScriptThing@@6B@ objects (pre-existing, report only).
import sys, collections
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[1])][1:]
a = lambda r: r[0].replace('H_', '').replace('.obj', '')
d = [r for r in rows if r[1].startswith('DEF')]
strong = [a(r) for r in d if r[1] != 'DEF/sel=2']
eq = [a(r) for r in d if r[1] == 'DEF/sel=2' and r[2] == 'BYTES_EQUAL']
df = ['%s(len %s)' % (a(r), r[3]) for r in d if r[1] == 'DEF/sel=2' and r[2] != 'BYTES_EQUAL']
und = [a(r) for r in rows if r[1] == 'UND']
none = [a(r) for r in rows if r[1] == '-']
shapes = collections.Counter(r[4] for r in d)
out = open(sys.argv[2], 'w')
out.write('member\tsymbol\tself\tobjects\tobjects_defining\tstrong_definers\tpickany_bytes_equal\tpickany_bytes_diff\tund_only\tneither\treloc_shapes\n')
out.write('\t'.join(['CScriptThing::operator=', '??4CScriptThing@@QAEAAV0@ABV0@@Z', '008ab980', str(len(rows)), str(len(d)), ','.join(strong) or '-',
                     '%d: %s' % (len(eq), ','.join(eq)), '%d: %s' % (len(df), ','.join(df)), '%d' % len(und), '%d' % len(none),
                     ' || '.join('%dx %s' % (k, s) for s, k in shapes.most_common())]) + '\n')
vt = collections.Counter(r[6] for r in rows if r[6] != '-')
out.write('# ??_7CScriptThing@@6B@ slot counts over the census objects: %s\n' % ', '.join('%s slots x%d' % kv for kv in sorted(vt.items(), key=lambda x: -x[1])))
out.close(); print(open(sys.argv[2]).read()[:5000])
