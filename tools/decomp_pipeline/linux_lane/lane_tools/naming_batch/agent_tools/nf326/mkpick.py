# mkpick.py (NF326): pick-any drafts of the CScriptThing unit selves (the (a) form) -> rows/<addr>.cpp.
# Each draft: the HEAD row's own leading comment lines (minus its `// Leaf:` and the lines that describe the stand-in model), the full PDB
# Leaf, a one-definition note naming the landed in-TU copies (from work/srccensus.tsv), the NF318 preamble (CCountedPointer before the
# NScript class model), the member defined `inline` on the real class (engine/CScriptThing.h, FABLE_RETAIL_NSCRIPT) with a local `imp`
# read once from PImp.Data (the HEAD self bodies' shape; the accessor spelling PImp.IsNull() / PImp-> gives the same bytes but emits
# unreferenced pick-any CCountedPointer IsNull / operator-> COMDATs, NF320 Q2), and the nf86 / nf88 / NF92 scaffolding: an
# anonymous-namespace helper that calls the member qualified (non-virtually) under `#pragma inline_depth(0)`, so this object emits the
# member out of line as a pick-any COMDAT. 0x004AA8D0 / 0x004AB150 were written by hand first (the pilot, local-`imp` bodies); this script regenerates all ten.
import re, sys
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf

M = {   # addr: (member, PDB leaf, declaration head, body lines, helper return, helper call args, helper params, extra defines)
 '004aa8d0': ('GetDefName', '?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ', 'CCharString CScriptThing::GetDefName() const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (imp == 0)', '        return NullString;', '    return imp->GetDefName();'],
              'CCharString', '', '', ['FABLE_RETAIL_CSCRIPTTHING_NULLSTRING']),
 '004aa900': ('GetDataString', '?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ', 'CCharString CScriptThing::GetDataString() const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (imp == 0)', '        return NullString;', '    return imp->GetDataString();'],
              'CCharString', '', '', ['FABLE_RETAIL_CSCRIPTTHING_NULLSTRING']),
 '004aa930': ('SetDataString', '?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z', 'void CScriptThing::SetDataString(const CCharString& data)',
              ['    CScriptThing* imp = PImp.Data;', '    if (imp)', '        imp->SetDataString(data);'],
              'void', 'data', 'CScriptThing& thing, const CCharString& data', []),
 '004aa980': ('GetPos', '?GetPos@CScriptThing@@UBEABVC3DVector@@XZ', 'const C3DVector& CScriptThing::GetPos() const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (imp == 0)', '        return NullCoord;', '    return imp->GetPos();'],
              'const C3DVector&', '', '', ['FABLE_RETAIL_CSCRIPTTHING_NULLCOORD']),
 '004aaa40': ('GetAngleXY', '?GetAngleXY@CScriptThing@@UBEMXZ', 'float CScriptThing::GetAngleXY() const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (imp == 0)', '        return 0.0f;', '    float angle = imp->GetAngleXY();', '    return angle;'],
              'float', '', '', []),
 '004aad20': ('MsgIsHitBySpecialAbilityFrom', '?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z',
              'bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& attacker) const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (!imp) return false;', '    return imp->MsgIsHitBySpecialAbilityFrom(ability, attacker);'],
              'bool', 'ability, attacker', 'const CScriptThing& thing, EHeroAbility ability, const CCharString& attacker', []),
 '004aad40': ('MsgIsHitByAnySpecialAbilityFrom', '?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z',
              'bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& attacker) const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (!imp) return false;', '    return imp->MsgIsHitByAnySpecialAbilityFrom(attacker);'],
              'bool', 'attacker', 'const CScriptThing& thing, const CCharString& attacker', []),
 '004aad60': ('MsgIsHitByAnyAggressiveSpecialAbilityFrom', '?MsgIsHitByAnyAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z',
              'bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (!imp) return false;', '    return imp->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);'],
              'bool', 'attacker', 'const CScriptThing& thing, const CCharString& attacker', []),
 '004aafe0': ('IsBeingCarriedBy', '?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z',
              'bool CScriptThing::IsBeingCarriedBy(const CCharString& carrier) const',
              ['    const CScriptThing* imp = PImp.Data;', '    if (!imp) return false;', '    return imp->IsBeingCarriedBy(carrier);'],
              'bool', 'carrier', 'const CScriptThing& thing, const CCharString& carrier', []),
 '004ab150': ('IsNull', '?IsNull@CScriptThing@@UBE_NXZ', 'bool CScriptThing::IsNull() const',
              ['    const CScriptThing* imp = PImp.Data;', '    return imp == 0 || imp->IsNull();'],
              'bool', '', '', []),
}
DROP = re.compile(r'^// (Leaf:|Switch route|Preamble:)|local slot view|slot view cast|Virtual-slot model|stand-in|CScriptThing_Methods|local .*model|vtable view')

def copies(member):
    out = []
    for l in open(A / 'work/srccensus.tsv').read().splitlines()[1:]:
        m, a, k, lf, f = l.split('\t')
        if m == member and a not in out: out.append(a)
    return out

def draft(addr):
    member, leaf, head, body, hret, hargs, hparams, defs = M[addr]
    src = Path(nf.src_of(addr)).read_text(encoding='utf-8')
    lead = []
    for l in src.split('\n'):
        if not l.startswith('//'): break
        if not DROP.search(l): lead.append(l)
    stand = re.findall(r'(\w+)\s+CScriptThing_Methods::' + member + r'\s*\(', src)
    cp = copies(member)
    cps = ' / '.join('0x' + a.upper() for a in cp[:8]) + (' and %d more' % (len(cp) - 8) if len(cp) > 8 else '')
    L = []
    if not lead or not lead[0].startswith('// CScriptThing::'):
        L.append('// CScriptThing::%s -- forwards to the implementation object (PImp.Data) through its vtable; the null-handle arm returns the default' % member)
    L += lead[:1]
    L.append('// Leaf: ' + leaf)
    L += lead[1:]
    L.append('// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the script TUs (the')
    L.append('//   landed in-TU copies %s define it inline; the ones that compile CScriptThing\'s vftable emit it pick-any)' % cps)
    L.append('//   and 0x%s is the vftable\'s out-of-line copy. So this row defines the same inline member on the real class and emits it as a' % addr.upper())
    L.append('//   pick-any COMDAT, not a strong definition (a strong one is LNK2005 against every emitted copy: NF318 evidence/link_cs.txt, NF326 evidence/).')
    L.append('//   Was the stand-in ?%s@CScriptThing_Methods@@... on the flattened header (a local vtable view of the pointee).' % member)
    L.append('// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).')
    L.append('#include "engine/CCountedPointer.h"')
    for d in defs: L.append('#define ' + d)
    L.append('#define FABLE_RETAIL_NSCRIPT')
    L.append('#include "fable_string.h"')
    L.append('#include "engine/C3DVector.h"')
    L.append('#include "engine/CScriptThing.h"')
    L.append('')
    L.append('inline ' + head)
    L.append('{'); L += body; L.append('}')
    L.append('')
    hp = hparams or 'const CScriptThing& thing'
    L.append('namespace {')
    L.append('// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline %s above with inline expansion switched off for' % member)
    L.append('// this one function, so this object emits the body out of line as a pick-any COMDAT (retail\'s 0x%s, the vftable\'s copy; the' % addr.upper())
    L.append('// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.')
    L.append('// Internal to this TU; never called.')
    L.append('#pragma inline_depth(0)')
    L.append('%s ReferenceScriptThing%s(%s)' % (hret, member, hp))
    L.append('{')
    L.append('    %sthing.CScriptThing::%s(%s);' % ('' if hret == 'void' else 'return ', member, hargs))
    L.append('}')
    L.append('#pragma inline_depth()')
    L.append('}')
    return '\n'.join(L) + '\n'

if __name__ == '__main__':
    od = Path(sys.argv[sys.argv.index('--out') + 1]) if '--out' in sys.argv else A / 'rows'   # --out <dir>: reproduction check
    args = [x for x in sys.argv[1:] if re.match(r'^[0-9a-f]{8}$', x)]
    for a in (args or sorted(M)):
        p = od / (a + '.cpp')
        if p.exists() and '--force' not in sys.argv: print('KEEP', p); continue
        p.write_text(draft(a), encoding='utf-8'); print('WROTE', p)
