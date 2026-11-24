# status_other.py (NF326): STATUS lines for the ENTRIES outside the ten-member CScriptThing pick-any unit (step 4 of the prefix).
# Each one is re-read from today's tracker (status/OPEN.tsv, spellings column = what landed rows bind / define now, with the witness):
# when a non-PDB spelling is still bound, the queue reason still holds and the entry is a port / held line; nothing here is worked.
# -> work/status_other.tsv (address, status, reason)
import re
from pathlib import Path
A = Path(__file__).resolve().parent.parent; R = Path('/home/user/FableDecomp')
O = {}
for l in open(R / 'rebuild/candidates/manual/naming/status/OPEN.tsv', encoding='utf-8').read().splitlines()[1:]:
    p = l.split('\t'); O[p[0].lower()] = p
E = [l.split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8').read().splitlines()[1:]]
UNIT = {'004aa8d0', '004aa900', '004aa930', '004aa980', '004aaa40', '004aad20', '004aad40', '004aad60', '004aafe0', '004ab150'}
SCRIPTED = {'009037e0', '00903ca0', '00903d60', '009040f0', '00904310', '00904430', '00904860', '00904940', '009049f0', '00904af0',
            '00904bd0', '00904e20', '00904ee0', '00904f60'}
SPECIAL = {
    '004aa840': 'binders bind ~CScriptThing through stand-in classes (CScriptThingCopy / CScriptThingParam / CScriptThingRuntime / '
                'ScriptMethods / ScriptThingCopy) or the non-virtual QAE form (0x00F051D0): binder ports. Also found here: the self '
                '0x004AA840 defines ??1CScriptThing@@UAE@XZ strongly (COMDAT selection 1) while the landed NScript rows that write '
                '`inline CScriptThing::~CScriptThing() { }` (484 sources by text) emit it pick-any: a real link of 0x004AA840 + 0x00E6A0B0 '
                'is LNK2005 + LNK1169 at HEAD (evidence/link2_dtor_head.txt). Same one-definition shape as the unit; see NOTES Q4',
    '004abe90': 'binders bind the copy constructor through stand-in classes (CAliveScriptThing, CElement12_004abe90, CScriptThingCopy, '
                'CScriptThingRetail, CScriptThingStored, CScriptThing_Calls, ScriptThingCopy) or `(void*)`: binder ports. Also: the self '
                'row is a local model (struct CBase / class CScriptThing) defining ??0CScriptThing@@QAE@ABV0@@Z strongly while the NScript '
                'rows that write the inline copy constructor (369 sources by text) emit it pick-any; see NOTES Q4',
    '00904380': None,
    '00905320': None,
}
out = open(A / 'work/status_other.tsv', 'w')
for e in E:
    a, src, st0, pdb, why = (e + [''] * 5)[:5]
    if a in UNIT: continue
    o = O.get(a)
    sp = o[11] if o and len(o) > 11 else ''
    standins = [s.strip() for s in sp.split('||') if s.strip() and not s.strip().startswith((pdb or '#') + ' ')]
    cur = '; '.join(standins)[:400]
    if a == '00904380':
        st, r = 'skipped-held', 'NF318 Q2: declared only under FABLE_RETAIL_SCRIPTEDTHING_FOLLOW_ROUTE, which no landed row defines; also a ScriptedThing-family body port (NF321)'
    elif a in SCRIPTED:
        st, r = 'skipped-port', ('ScriptedThing family (NF321): engine/CScriptGameResourceObjectScriptedThing.h declares none of the members and the '
                                 'self body runs on local CThing_Retail / action models; not a CScriptThing in-TU copy, so the unit decision does not reach it')
    elif a == '00905320':
        st, r = 'skipped-port', 'NF319 / NF323: the base class model does not declare the virtual (override probe FAIL); a base class-model port, outside the unit'
    elif a in SPECIAL:
        st, r = 'skipped-port', SPECIAL[a]
    else:
        st, r = 'skipped-port', ('outside the CScriptThing member unit (stand-in class / parameter / container binders, not in-TU copies of a '
                                 'CScriptThing member); queue reason (%s) re-read' % src)
    if cur: r += '. Tracker 2026-11-24 still binds: ' + cur
    elif o is None: r += '. Not in today\'s OPEN.tsv'
    out.write('%s\t%s\t%s\n' % (a, st, r))
out.close(); print(open(A / 'work/status_other.tsv').read().count('\n'), 'lines')
