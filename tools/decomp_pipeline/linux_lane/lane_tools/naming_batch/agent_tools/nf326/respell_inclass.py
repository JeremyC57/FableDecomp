# respell_inclass.py (NF326) <addr> <member>[,<member>...] [--write] [--out <dir>]: respell the IN-CLASS bodies of the unit members inside
# a row's LOCAL `class CScriptThing` / `struct CScriptThing` model (rows that model the handle locally instead of including
# engine/CScriptThing.h; header-policy debt, left as it is) to the self rows' body shape, so the pick-any COMDAT the row emits equals retail.
# The existing body is parsed, not replaced blindly: the data expression the row reads (PImp.Data / PImp_Data), the pointee's callee name
# and argument text (a local pointee model may spell it differently, e.g. GetPosition), and the null object it returns (NullCoord,
# g_ZeroPos, CScriptThing::NullString, ...) are all kept; only the shape changes to `X* imp = <data>; if (imp == 0) return <null>;
# return imp-><callee>(<args>);` with X the pointee type the model declares (CCountedPointer<X> PImp / X* PImp_Data / a local counted
# pointer struct with `X* Data`). Only the brace block changes; a one-line note goes above the function head.
import re, sys
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf
SELF = {'GetDefName': '004AA8D0', 'GetDataString': '004AA900', 'SetDataString': '004AA930', 'GetPos': '004AA980', 'GetAngleXY': '004AAA40',
        'MsgIsHitBySpecialAbilityFrom': '004AAD20', 'MsgIsHitByAnySpecialAbilityFrom': '004AAD40',
        'MsgIsHitByAnyAggressiveSpecialAbilityFrom': '004AAD60', 'IsBeingCarriedBy': '004AAFE0', 'IsNull': '004AB150'}

def block(t, k):
    d = 0
    for e in range(k, len(t)):
        if t[e] == '{': d += 1
        elif t[e] == '}':
            d -= 1
            if d == 0: return e
    raise ValueError('unbalanced')

OVERRIDE_T = {'00d73c70': 'CScriptThingImp*'}   # its local CCountedPointer<CScriptThing> model stores a CScriptThingImp* Data (clang showed it)
ROW = None
def pointee(cls, src):
    if ROW in OVERRIDE_T:
        D = 'PImp_Data' if re.search(r'\bPImp_Data\s*;', cls) else 'PImp.Data'
        return OVERRIDE_T[ROW], D
    m = re.search(r'CCountedPointer\s*<\s*(\w+)\s*>\s+PImp\s*;', cls)
    if m: return m.group(1) + '*', 'PImp.Data'
    m = re.search(r'(\w+)\s*\*\s*PImp_Data\s*;', cls)
    if m: return m.group(1) + '*', 'PImp_Data'
    m = re.search(r'\b(\w+)\s+PImp\s*;', cls)
    if m:
        s = re.search(r'\b(?:struct|class)\s+' + m.group(1) + r'\b[^;{]*\{(.*?)\};', src, re.S)
        if s:
            d = re.search(r'(\w+)\s*\*\s*Data\s*;', s.group(1))
            if d: return d.group(1) + '*', 'PImp.Data'
    raise ValueError('pointee type not found')

def respell(src, member):
    out = []
    for cm in list(re.finditer(r'\b(?:class|struct)\s+CScriptThing\b[^;{]*\{', src)):
        k0 = cm.end() - 1; e0 = block(src, k0); cls = src[k0:e0 + 1]
        T, D = pointee(cls, src)
        pat = re.compile(r'(?<![:.>\w])' + member + r'\s*\(([^)]*)\)\s*(?:const)?\s*(?://[^\n]*)?\s*\{')
        mm = pat.search(cls)
        if not mm: continue
        k = mm.end() - 1; e = block(cls, k); old = cls[k:e + 1]
        call = re.search(r'(?:PImp\.Data|PImp_Data)\s*->\s*(\w+)\s*\(([^)]*)\)', old)
        if not call: raise ValueError('no forwarded call in ' + old)
        cal, args = call.group(1), call.group(2).strip()
        if member == 'SetDataString':
            new = '{ %s imp = %s; if (imp) imp->%s(%s); }' % (T, D, cal, args)
        elif member == 'IsNull':
            new = '{ %s imp = %s; return imp == 0 || imp->%s(); }' % (T, D, cal)
        elif member in ('GetPos', 'GetDefName', 'GetDataString'):
            nul = re.search(r'return\s+([A-Za-z_][\w:]*)\s*;|\?\s*([A-Za-z_][\w:]*)\s*:', old)
            nul = nul.group(1) or nul.group(2)
            new = '{ %s imp = %s; if (imp == 0) return %s; return imp->%s(%s); }' % (T, D, nul, cal, args)
        elif member == 'GetAngleXY':
            new = '{ %s imp = %s; if (imp == 0) return 0.0f; float angle = imp->%s(); return angle; }' % (T, D, cal)
        else:
            new = '{ %s imp = %s; if (!imp) return false; return imp->%s(%s); }' % (T, D, cal, args)
        # line start of the function head, for the note
        hs = cls.rfind('\n', 0, mm.start()) + 1
        ind = re.match(r'\s*', cls[hs:]).group(0)
        note = ind + '// NF326 one-definition respell: the body in the shape of the self row 0x%s (retail\'s out-of-line copy), so this TU\'s pick-any COMDAT equals it.\n' % SELF[member]
        cls2 = cls[:hs] + note + cls[hs:k] + new + cls[e + 1:]
        src = src[:k0] + cls2 + src[e0 + 1:]
        out.append((old, new))
        break
    return src, out

if __name__ == '__main__':
    a, ms = sys.argv[1], sys.argv[2].split(',')
    ROW = a
    od = Path(sys.argv[sys.argv.index('--out') + 1]) if '--out' in sys.argv else A / 'rows'
    p = od / (a + '.cpp')
    src = p.read_text(encoding='utf-8') if p.exists() else Path(nf.src_of(a)).read_text(encoding='utf-8')
    for m in ms:
        src, ch = respell(src, m)
        if not ch: print('NOMATCH', a, m)
        for o, n in ch: print('##', a, m, '\n  OLD:', re.sub(r'\s+', ' ', o)[:260], '\n  NEW:', n[:260])
    if '--write' in sys.argv: p.write_text(src, encoding='utf-8'); print('WROTE', p)
