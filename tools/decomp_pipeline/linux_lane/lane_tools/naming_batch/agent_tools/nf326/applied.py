# applied.py (NF326): APPLIED.tsv = one line per changed row (rows.txt): repo file + what changed (selfs / out-of-class copy respells / in-class respells),
# the members whose pick-any COMDAT bodies changed read from evidence/copyproof_vland.txt.
import sys, re
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf
SELFS = {'004aa8d0','004aa900','004aa930','004aa980','004aaa40','004aad20','004aad40','004aad60','004aafe0','004ab150'}
cp = {}
for l in open(A / 'evidence/copyproof_vland.txt'):
    a = l.split()[0]; cp[a] = sorted(set(re.findall(r'changed \?(\w+)@CScriptThing@@', l)))
out = open(A / 'APPLIED.tsv', 'w'); out.write('address\tfile\tchange\n')
for a in open(A / 'rows.txt').read().split():
    f = str(nf.src_of(a)).split('rebuild/src/compiled/')[1]
    src = (A / 'rows' / (a + '.cpp')).read_text(encoding='utf-8')
    if a in SELFS:
        ch = 'self (HELD, Q1): stand-in CScriptThing_Methods member -> the PDB member defined inline on engine/CScriptThing.h + pick-any scaffolding helper'
    else:
        heads = re.findall(r'// NF326 one-definition respell[^\n]*\n([^\n]*)', src)
        kind = 'in-class respell (local CScriptThing model kept)' if any('CScriptThing::' not in h for h in heads) else 'copy respell'
        ms = cp.get(a, [])
        ch = '%s (HELD with the unit, Q1): body of the in-TU inline %s in the self row shape; only those pick-any COMDATs change (now = retail)' % (kind, ', '.join(ms) or '?')
    out.write('0x%s\t%s\t%s\n' % (a.upper(), f, ch))
out.close()
