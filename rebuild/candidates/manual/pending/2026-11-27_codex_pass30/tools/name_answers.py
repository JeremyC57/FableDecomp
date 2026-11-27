#!/usr/bin/env python3
"""name_answers.py (Codex pass 30, 2026-11-27): one line per retail address that landed code names, with the spelling to use.
usage: name_answers.py <tracker rec.json> <out NAME_ANSWERS.tsv> [--locks DIR] [--rtti rebuild/manifest/rtti-vtables.tsv]
The tracker record (tools/decomp_pipeline/naming_status.py --dump-rec) holds every spelling landed code gives an address (callers'
relocations + each row's own Leaf) and its verdict. This turns the verdict into an instruction:
  PROVEN            one spelling, a PDB name (agree_pdb) or a std / CRT / compiler / library-signature name (agree_std): use it exactly.
  PROVEN_RTTI       retail RTTI places the address in a vftable slot whose PDB name is known (rtti_identified / rtti_conflict): use it.
  PROVEN_LANDED_WRONG  the PDB spelling is known (split_fixable) but some landed rows spell it another way: use the PDB spelling; the
                    `landed_wrong` column lists the wrong spellings and the landed rows (witnesses) that carry them.
  REGISTERED_GUESS  a best guess registered in descriptive_names.tsv (agree_descriptive, owner rule 2026-11-25): use it exactly; it may be
                    replaced only by a name proven from a PDB, the GameCODA SDK or another original source (report that in NAME_FIXES).
  FOLDED            several PDB names at one address (split_pdb_multi: identical bodies the linker folded): any listed PDB spelling is a
                    true name; use the one the calling function's own source used (its Ego_d / Ego_r call-site record).
  UNSETTLED         no proven or registered name (agree_unverified / split_unknown / standin / unmeasured): the listed spellings are
                    hints only. Find the original name (docs/NAMING_FIRST_TIME.md section 2); else register a best guess.
`held` marks a landed row that is reserved or held (lock owner other than `released`, e.g. CODEX29 / COORD_HOLD_*): it cannot be
respelt in this pass; report its wrong spelling in NAME_CONFLICTS.tsv instead.
"""
import json, os, re, sys

rec_path, out_path = sys.argv[1], sys.argv[2]
locks = sys.argv[sys.argv.index('--locks') + 1] if '--locks' in sys.argv else None
rec = json.load(open(rec_path))
WORDS = re.compile(r'(^vfunc|\bsub_|\bFUN_|Unproven|^_v\d|_v\d$|Retail@|_Methods@@)', re.I)
HEX = re.compile(r'(?<![?])_(?=[0-9a-fA-F]*[0-9])[0-9a-fA-F]{8}(?![0-9A-Za-z])')        # an address suffix (needs a digit: `_ECBedDef` is not one)
def placeholder(s): return bool(WORDS.search(s) or HEX.search(s))
RTTI = {}                                                                       # retail vftable VA -> RTTI class (rtti-vtables.tsv)
if '--rtti' in sys.argv:
    for l in open(sys.argv[sys.argv.index('--rtti') + 1]).read().splitlines()[1:]:
        f = l.split('\t')
        if len(f) > 1: RTTI.setdefault(f[1].lower(), f[0])
def vft_check(t, s):
    """a ??_7 spelling counts only if its class is the retail RTTI class at that VA and its base qualifier names no stand-in."""
    m = re.match(r'\?\?_7(.*?)@@6(?:B(.*))?$', s)
    if not m: return None
    body = m.group(1)
    if body.startswith('?$'):   # template: ??_7?$Name@<args>@<scopes>@@6B@; RTTI's class column drops the template arguments
        cls = body[2:].split('@', 1)[0]; rc = re.sub(r'<.*', '', RTTI.get(t, '')); same = (not rc) or rc.split('::')[-1] == cls
    else:
        cls = '::'.join(reversed([x for x in body.split('@') if x])); rc = re.sub(r'<.*', '', RTTI.get(t, '')); same = (not rc) or rc == cls
    if not same: return f'retail RTTI names class {rc} at this vftable, the landed spelling names {cls}'
    if m.group(2) and placeholder(m.group(2)): return 'the base-subobject qualifier names a stand-in class'
    return None

def lock_of(a):
    if not locks: return ''
    p = f'{locks}/{a}/owner'
    if not os.path.exists(p): return ''
    o = open(p).read().strip()
    return '' if o == 'released' else o

def parse_spellings(s):
    """'sym (00401234,self) || sym2 (00405678)' -> [(sym, [witness...])]"""
    out = []
    for part in [x.strip() for x in (s or '').split(' || ') if x.strip()]:
        m = re.match(r'^(.*) \(([^()]*)\)$', part)
        out.append((m.group(1), [w for w in m.group(2).split(',') if w]) if m else (part, []))
    return out

rows, bad = [], []
for t, r in sorted(rec.items()):
    st = r['status']; sp = parse_spellings(r.get('spellings', ''))
    names = [s for s, _ in sp]
    use, verdict, basis, wrong = '', 'UNSETTLED', '', []
    if st in ('agree_pdb', 'agree_std') and len(names) == 1:
        use, verdict, basis = names[0], 'PROVEN', 'PDB name set (Ego_r / FableWin / Ego_d)' if st == 'agree_pdb' else 'std / CRT / compiler / high-confidence library signature'
    elif st in ('rtti_identified', 'rtti_conflict') and r.get('rtti'):
        use, verdict, basis = r['rtti'], 'PROVEN_RTTI', 'retail RTTI vftable slot + PDB name'
        wrong = [(s, w) for s, w in sp if s != use]
    elif st == 'split_fixable' and r.get('pdb_spelling'):
        use, verdict, basis = r['pdb_spelling'], 'PROVEN_LANDED_WRONG', 'PDB name set; other landed spellings are wrong'
        wrong = [(s, w) for s, w in sp if s != use]
    elif st == 'agree_descriptive' and len(names) == 1:
        use, verdict, basis = names[0], 'REGISTERED_GUESS', 'rebuild/oracles/descriptive_names.tsv (best guess, owner rule 2026-11-25)'
    elif st == 'split_pdb_multi':
        verdict, basis = 'FOLDED', 'several PDB names, one folded body: use the one the caller\'s source used'
        use = ' | '.join(names)
    else:
        basis = f'tracker status {st}: spellings below are hints, none proven'
    if verdict in ('PROVEN', 'PROVEN_RTTI', 'PROVEN_LANDED_WRONG', 'REGISTERED_GUESS'):
        why = vft_check(t, use) or ('placeholder-looking spelling' if placeholder(use) and st != 'agree_pdb' else None)
        if why:   # demoted: a compiler-name shape (??_7) does not prove the class inside it
            verdict, basis, use = 'UNSETTLED', f'not proven ({why}); tracker said {st}', ''
        elif placeholder(use): bad.append((t, verdict, use))
    wl = []
    for s, w in wrong:
        ws = [x for x in w if x != 'self']
        if 'self' in w: ws.append(t)
        wl.append(s + ' @ ' + ','.join((x + ('[held:' + lock_of(x) + ']' if lock_of(x) else '')) for x in ws))
    rows.append([t, verdict, use, basis, ' || '.join(wl), r.get('spellings', '') if verdict in ('UNSETTLED', 'FOLDED') else '',
                 r.get('catalog', ''), 'y' if r.get('landed') == 'y' else 'n', lock_of(t)])
with open(out_path, 'w') as o:
    o.write('address\tverdict\tuse_this_spelling\tbasis\tlanded_wrong (spelling @ landed rows that carry it)\thint_spellings (UNSETTLED / FOLDED only)\tcatalog_name\tself_landed\tself_lock\n')
    for x in rows: o.write('\t'.join(c.replace('\t', ' ') for c in x) + '\n')
from collections import Counter
print('lines', len(rows), dict(Counter(x[1] for x in rows)))
if bad:
    print('GATE FAILED: placeholder-looking spelling marked usable:', len(bad))
    for b in bad[:20]: print('  ', *b)
    sys.exit(1)
