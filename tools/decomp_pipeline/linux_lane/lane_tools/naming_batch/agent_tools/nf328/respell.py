# respell.py (NF326) <addr> <member>[,<member>...] [--write]: respell a copy row's in-TU inline definition(s) of CScriptThing members to the
# self row's body shape (a local `imp` read once from PImp.Data; the HEAD self bodies that match retail), so the pick-any COMDAT the copy
# emits equals retail's out-of-line copy. Only the body between the definition's braces changes: the qualifier (inline / FABLE_FORCEINLINE),
# the signature text and parameter names, and the null object the row already names (NullString, CScriptThing::NullString,
# g_EmptyCharString, NullCoord, ...) are kept. Source: the landed row (or rows/<addr>.cpp if a previous respell exists); output
# rows/<addr>.cpp, with one comment line above each changed definition. Prints the old and new definition texts.
# Forms: default = the local-`imp` body (what rows/ holds); --accessor = the PImp.IsNull() / PImp-> body (work/v2_rows, dropped). Rows in
# PAREN_ANGLE take the parenthesised GetAngleXY return. In-class bodies of LOCAL `class CScriptThing` models: tools/respell_inclass.py.
import re, sys
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf

def params(sig):
    inner = sig[sig.index('(') + 1: sig.rindex(')')].strip()
    if not inner or inner == 'void': return []
    return [re.findall(r'(\w+)\s*$', p.strip())[0] for p in inner.split(',')]

NULLS = {'GetDefName': r'(CScriptThing::NullString|NullString|g_\w+)', 'GetDataString': r'(CScriptThing::NullString|NullString|g_\w+)',
         'GetPos': r'(CScriptThing::NullCoord|NullCoord|g_\w+)'}
FORM = 'accessor' if '--accessor' in sys.argv else 'imp'
PAREN_ANGLE = {'00dd9520', '00ddae40'}   # rows whose own leaf needs the parenthesised float return (probe prB); see NOTES
ROW = None
def body(m, sig, old, imp_t='const CScriptThing*', data='PImp.Data'):
    # FORM 'imp' (the landed hand-back): a local `imp` read once from the counted pointer's Data, the self rows' shape. FORM 'accessor'
    # (work/v2_rows, not handed back): PImp.IsNull() / PImp->, the spelling of the copies that already equal retail; it makes each respelled
    # object emit unreferenced pick-any CCountedPointer IsNull / operator-> COMDATs (NF320 Q2), so it was dropped.
    ps = params(sig); a = ', '.join(ps)
    nm = re.search(NULLS[m], old) if m in NULLS else None
    nul = nm.group(1) if nm else ('NullCoord' if m == 'GetPos' else 'NullString')
    mut_t = imp_t.replace('const ', '')
    if FORM == 'accessor':
        if m in NULLS: return '{ if (PImp.IsNull()) return %s; return PImp->%s(%s); }' % (nul, m, a)
        if m == 'SetDataString': return '{ if (!PImp.IsNull()) PImp->SetDataString(%s); }' % a
        if m == 'GetAngleXY': return '{ if (PImp.IsNull()) return 0.0f; return (PImp->GetAngleXY()); }'
        if m == 'IsNull': return '{ return PImp.IsNull() || PImp->IsNull(); }'
        return '{ if (PImp.IsNull()) return false; return PImp->%s(%s); }' % (m, a)
    if m in NULLS: return '{ %s imp = %s; if (imp == 0) return %s; return imp->%s(%s); }' % (imp_t, data, nul, m, a)
    if m == 'SetDataString': return '{ %s imp = %s; if (imp) imp->SetDataString(%s); }' % (mut_t, data, a)
    if m == 'GetAngleXY':
        if ROW in PAREN_ANGLE: return '{ %s imp = %s; if (imp == 0) return 0.0f; return (imp->GetAngleXY()); }' % (imp_t, data)
        return '{ %s imp = %s; if (imp == 0) return 0.0f; float angle = imp->GetAngleXY(); return angle; }' % (imp_t, data)
    if m == 'IsNull': return '{ %s imp = %s; return imp == 0 || imp->IsNull(); }' % (imp_t, data)
    return '{ %s imp = %s; if (!imp) return false; return imp->%s(%s); }' % (imp_t, data, m, a)

def respell(src, m):
    t = src; out = []; pos = 0; n = 0
    pat = re.compile(r'^([^\n/;{}]*?\bCScriptThing::' + m + r'\s*\([^)]*\)\s*(?:const)?)\s*(//[^\n]*)?\s*\{', re.M)
    while True:
        mm = pat.search(t, pos)
        if not mm: break
        head = mm.group(1)
        if re.search(r'(->|\.)\s*$', head.split('CScriptThing::')[0]): pos = mm.end(); continue
        k = mm.end() - 1; d = 0
        for e in range(k, len(t)):
            if t[e] == '{': d += 1
            elif t[e] == '}':
                d -= 1
                if d == 0: break
        old = t[mm.start():e + 1]
        sig = head[head.index('CScriptThing::'):]
        new = head.rstrip() + ' ' + body(m, sig, t[k:e + 1]) + (('    ' + mm.group(2).strip()) if mm.group(2) else '')
        note = ('// NF326 one-definition respell: the body in the shape of the self row 0x%s (retail\'s out-of-line copy), so this TU\'s pick-any COMDAT equals it.\n' % SELF[m]) if FORM == 'imp' else ('// NF326 one-definition respell: the CCountedPointer accessor body of the self row 0x%s (retail\'s out-of-line copy), so this TU\'s pick-any COMDAT equals it.\n' % SELF[m])
        if FORM == 'imp' and m == 'GetAngleXY' and ROW in PAREN_ANGLE:
            note = "// NF326 one-definition respell: the self row 0x004AAA40 body with the float result parenthesised as the landed copies spell it (retail's out-of-line copy), so this TU's pick-any COMDAT equals it.\n"
        t = t[:mm.start()] + note + new + t[e + 1:]
        out.append((old, new)); pos = mm.start() + len(note) + len(new); n += 1
    return t, out

SELF = {'GetDefName': '004AA8D0', 'GetDataString': '004AA900', 'SetDataString': '004AA930', 'GetPos': '004AA980', 'GetAngleXY': '004AAA40',
        'MsgIsHitBySpecialAbilityFrom': '004AAD20', 'MsgIsHitByAnySpecialAbilityFrom': '004AAD40',
        'MsgIsHitByAnyAggressiveSpecialAbilityFrom': '004AAD60', 'IsBeingCarriedBy': '004AAFE0', 'IsNull': '004AB150'}
if __name__ == '__main__':
    a, ms = sys.argv[1], sys.argv[2].split(',')
    ROW = a
    od = Path(sys.argv[sys.argv.index('--out') + 1]) if '--out' in sys.argv else A / 'rows'   # --out <dir>: write there (reproduction check)
    p = od / (a + '.cpp')
    src = p.read_text(encoding='utf-8') if p.exists() else Path(nf.src_of(a)).read_text(encoding='utf-8')
    for m in ms:
        src, ch = respell(src, m)
        if not ch: print('NOMATCH', a, m)
        for o, n in ch: print('##', a, m, '\n  OLD:', o.replace('\n', '\\n')[:300], '\n  NEW:', n[:300])
    if '--write' in sys.argv: p.write_text(src, encoding='utf-8'); print('WROTE', p)
