# census_report.py (NF328) <c2.tsv> <out CENSUS.tsv>: per unit member, from census2.py's per-object table: emitting copies (addr:verdict),
# strong definers (COMDAT selection 1), pick-any copies differing from retail, objects that only reference it (UND), and the objects whose
# ??_7CScriptThing@@6B@ has a slot count other than retail's 79 (truncated local-model vftables under the real name; pre-existing).
import sys, collections
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[1])][1:]
out = open(sys.argv[2], 'w')
out.write('member\tsymbol\tself\tobjects_defining\tstrong_definers\tpickany_bytes_equal\tpickany_bytes_diff\tund_only\treloc_shapes\n')
for name, sym, self, c in (('~CScriptThing', '??1CScriptThing@@UAE@XZ', '004aa840', 1), ('CScriptThing(const CScriptThing&)', '??0CScriptThing@@QAE@ABV0@@Z', '004abe90', 4)):
    d = [r for r in rows if r[c].startswith('DEF')]
    a = lambda r: r[0].replace('H_', '').replace('.obj', '')
    strong = [a(r) for r in d if r[c] == 'DEF/sel=1']
    eq = [a(r) for r in d if r[c] == 'DEF/sel=2' and r[c + 1] == 'BYTES_EQUAL']
    df = [a(r) for r in d if r[c] == 'DEF/sel=2' and r[c + 1] != 'BYTES_EQUAL']
    und = [a(r) for r in rows if r[c] == 'UND']
    shapes = collections.Counter(r[c + 2] for r in d)
    out.write('\t'.join([name, sym, self, str(len(d)), ','.join(strong) or '-', '%d: %s' % (len(eq), ','.join(eq)), ','.join(df) or '-',
                         '%d' % len(und), ' || '.join('%dx %s' % (k, s) for s, k in shapes.most_common())]) + '\n')
vt = collections.Counter(r[7] for r in rows if r[7] != '-')
out.write('# ??_7CScriptThing@@6B@ slot counts over the census objects: %s\n' % ', '.join('%s slots x%d' % kv for kv in sorted(vt.items(), key=lambda x: -x[1])))
out.write('# objects with a non-79-slot ??_7CScriptThing@@6B@: %s\n' % ','.join(r[0].replace('H_', '').replace('.obj', '') for r in rows if r[7] not in ('-', '79')))
out.close(); print(open(sys.argv[2]).read()[:4000])
