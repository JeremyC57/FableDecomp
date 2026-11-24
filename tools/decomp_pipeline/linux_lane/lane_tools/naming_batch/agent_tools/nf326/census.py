# census.py (NF326): CENSUS.tsv of the CScriptThing pick-any unit, by object (work/objcensus_head.tsv: the HEAD objects of every
# census row compiled by VC7.1), with the text census (work/srccensus.tsv), the retail comparison of every emitted copy
# (work/copycmp.tsv: the copy's COMDAT v Fable.exe at the self address, relocation fields zeroed) and the exe census of retail call sites
# (work/census_exe.tsv + evidence/refbind.tsv: which symbol each calling row's object binds at the self address).
# (2nd pass) objects: work/objcensus_before.tsv = the HEAD objects of all 171 census rows; verdicts: evidence/comdat_census_before.tsv.
# Columns: member, pdb_spelling, self_row, self_head_spelling, copies_emitting (addr:BYTES_EQUAL|BYTES_DIFF v retail), copies_text_only
# (in-TU definition, no COMDAT emitted), und_rows (objects that reference the PDB symbol without defining it: vftable slots and calls),
# retail_call_rows (rows whose retail body calls the self address: bound symbol), strong_definers (COMDAT selection 1), spellings_now.
import collections, re
from pathlib import Path
A = Path(__file__).resolve().parent.parent
SELF = {'GetDefName': '004aa8d0', 'GetDataString': '004aa900', 'SetDataString': '004aa930', 'GetPos': '004aa980', 'GetAngleXY': '004aaa40',
        'MsgIsHitBySpecialAbilityFrom': '004aad20', 'MsgIsHitByAnySpecialAbilityFrom': '004aad40',
        'MsgIsHitByAnyAggressiveSpecialAbilityFrom': '004aad60', 'IsBeingCarriedBy': '004aafe0', 'IsNull': '004ab150',
        '~CScriptThing': '004aa840', 'CScriptThing(const CScriptThing&)': '004abe90'}
L = [l.rstrip('\n').split('\t') for l in open(A / 'work/objcensus_before.tsv')]   # all 171 HEAD objects (census rows + in-class rows)
for r in L[1:]: r[0] = r[0].replace('H_', '')
H = L[0]
txt = collections.defaultdict(set)
for l in open(A / 'work/srccensus.tsv').read().splitlines()[1:]:
    m, a, k, lf, f = l.split('\t'); txt[m].add(a)
for l in open(A / 'evidence/inclass_scan.txt'):
    p = l.split('\t')
    if len(p) > 2: txt[p[0]].add(p[1])
cmp = collections.defaultdict(dict)
for l in open(A / 'evidence/comdat_census_before.tsv').read().splitlines()[1:]:
    p = l.split('\t'); cmp[p[0]][p[1].replace('H_', '')[:8]] = p[3]
ref = collections.defaultdict(list)
for l in open(A / 'evidence/refbind.tsv').read().splitlines()[1:]:
    s, r, b = l.split('\t'); ref[s].append('%s:%s' % (r, b))
selfsp = {}   # the HEAD self object's own external definitions (not the text): the spelling the self row defines today
import sys; sys.path.insert(0, str(A / 'tools')); import nf
for a in SELF.values():
    for o in (A / 'work/hobj' / (a + '.obj'), A / 'work/obj' / ('H_' + a + '.obj')):
        if o.exists():
            selfsp[a] = ' + '.join(sorted(x[4:] for x in nf.ext_syms(str(o)) if x.startswith('DEF ') and not x.startswith('DEF ??_7'))); break
out = open(A / 'CENSUS.tsv', 'w')
out.write('member\tpdb_spelling\tself_row\tself_head_spelling\tcopies_emitting\tcopies_text_only\tund_rows\tretail_call_rows\tstrong_definers\tspellings_now\n')
for j, w in enumerate(H[1:-1], 1):
    m = w[1:].split('@')[0] if not w.startswith('??') else ('~CScriptThing' if w.startswith('??1') else 'CScriptThing(const CScriptThing&)')
    key = m if not w.startswith('??') else w[:3]
    s = SELF[m]
    defs = [r[0][:8] for r in L[1:] if r[j].startswith('DEF')]
    strong = [r[0][:8] for r in L[1:] if r[j].startswith('DEF/sel=1')]
    und = [r[0][:8] for r in L[1:] if r[j].startswith('UND')]
    em = ['%s:%s' % (a, cmp[key].get(a, '?')) for a in defs if a != s]
    to = sorted(txt.get(m, set()) - set(defs))
    sp = 'self %s; copies %s (pick-any)' % (selfsp.get(s, '?'), w) if defs else 'self %s' % selfsp.get(s, '?')
    out.write('\t'.join([m, w, s, selfsp.get(s, '?'), ','.join(em) or '-', ','.join(to) or '-', '%d: %s' % (len(und), ','.join(und)),
                         ','.join(ref.get(s, [])) or '-', ','.join(strong) or '-', sp]) + '\n')
out.close(); print(open(A / 'CENSUS.tsv').read()[:3000])
