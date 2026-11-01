# fam_stb.py <ov> <rows.txt>: the CScriptGameResourceObjectScriptedThingBase slot forwarders (vtable 0x0127094C slots 2-29, every
# slot in FableWin.pdb as UAE/UBE, slots 2-29 also in Ego_r.pdb; vtprobe 30/30, migration wave 5) declared non-virtual (QAE/QBE)
# or with stand-in parameter types in a row-local polymorphic model: each declaration becomes the header's exact one.
# Calls on objects stay direct calls (retail: `call 0x007E7xxx`); the recheck proves no call became a virtual dispatch.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
H = open('/home/user/FableDecomp/rebuild/include/engine/CScriptGameResourceObjectScriptedThingBase.h').read()
_i = H.index('class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {'); hb = H[_i:H.index('#else', _i)]
hb = re.sub(r',\s*\n\s+', ', ', hb)
SLOT = {}
for m in re.finditer(r'^\s*(virtual [^;]*?(\w+)\(([^;]*)\)( const)?;)\s*//\s*(\d+) 0x([0-9A-F]{8})', hb, re.M):
    decl, name, prm, cst, n, a = m.groups()
    if int(n) < 2: continue
    SLOT.setdefault(name, []).append((decl, prm, a.lower()))
ENUM_ARG = {'MoveToPosition': 2, 'MoveToThing': 2, 'FollowPreCalculatedRoute': 1, 'MoveToAndPickUpGenericBox': 1, 'Speak': 2}
ENUM_T = {'MoveToPosition': 'EScriptEntityMoveType', 'MoveToThing': 'EScriptEntityMoveType', 'FollowPreCalculatedRoute': 'EScriptEntityMoveType',
          'MoveToAndPickUpGenericBox': 'EScriptEntityMoveType', 'Speak': 'ETextGroupSelectionMethod'}
BT, BS = inputs.bindings(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
SLOTADDR = set(a for v in SLOT.values() for _, _, a in v)
def split_args(s):
    out, d, cur = [], 0, ''
    for ch in s:
        if ch in '([{': d += 1
        elif ch in ')]}': d -= 1
        if ch == ',' and d == 0: out.append(cur); cur = ''
        else: cur += ch
    out.append(cur); return out
def find_call_args(t, i):
    # t[i] == '(' ; return index of matching ')'
    d = 0
    for j in range(i, len(t)):
        if t[j] == '(': d += 1
        elif t[j] == ')':
            d -= 1
            if d == 0: return j
    return -1
USED = {}
def tr(t, a):
    m = re.search(r'\b(class|struct)\s+CScriptGameResourceObjectScriptedThingBase\b[^;{]*\{', t)
    if not m: return None, 'no STB class body'
    d = 0
    for j in range(m.end() - 1, len(t)):
        d += {'{': 1, '}': -1}.get(t[j], 0)
        if d == 0: break
    body = t[m.end():j]
    if 'virtual' not in body: return None, 'non-polymorphic local model'
    names = []
    def rep(mm):
        ind, name, rest = mm.group(1), mm.group(3), mm.group(0)
        cands = SLOT[name]
        decl = cands[0][0]
        if len(cands) > 1:   # Speak: pick the overload by the second parameter
            p2 = split_args(mm.group(4))[1] if len(split_args(mm.group(4))) > 1 else ''
            decl = [c[0] for c in cands if ('char' in c[1].split(',')[1]) == ('char' in p2)][0]
        names.append(name)
        return ind + decl
    pat = re.compile(r'^([ \t]*)(?:virtual\s+)?((?:const\s+)?[\w:&*<> ]+?)\s*\b(' + '|'.join(SLOT) + r')\s*\(([^;{]*)\)\s*(?:const\s*)?;', re.M)
    nb = pat.sub(rep, body)
    if nb == body: return None, 'no slot declarations'
    t = t[:m.end()] + nb + t[j:]
    # stand-in typedefs of the two enums become the (empty) enums the PDB spellings carry (W4...; same 4-byte argument)
    for e in ('EScriptEntityMoveType', 'ETextGroupSelectionMethod'):
        t = re.sub(r'\btypedef\s+(?:long|int|unsigned\s+long)\s+' + e + r'\s*;', 'enum %s { };' % e, t)
    m = re.search(r'\b(class|struct)\s+CScriptGameResourceObjectScriptedThingBase\b[^;{]*\{', t)
    # forward enums the header declarations need
    for e in ('EScriptEntityMoveType', 'ETextGroupSelectionMethod'):
        if e in nb and not re.search(r'\b' + e + r'\b', t[:m.start()] + t[m.end() + len(nb):]):
            t = t[:m.start()] + 'enum %s;\n' % e + t[m.start():]
    # enum arguments at call sites
    for name in set(names) & set(ENUM_ARG):
        k = ENUM_ARG[name]; et = ENUM_T[name]; pos = 0
        while True:
            mm = re.compile(r'(?:\.|->)\s*' + name + r'\s*\(').search(t, pos)
            if not mm: break
            i = mm.end() - 1; e = find_call_args(t, i)
            args = split_args(t[i + 1:e])
            if len(args) > k and not re.match(r'^\s*\(\s*' + et + r'\s*\)', args[k]):
                lead = re.match(r'\s*', args[k]).group(0)
                args[k] = lead + '(%s)%s' % (et, args[k].strip() if re.match(r'^[\w.]+$', args[k].strip()) else '(' + args[k].strip() + ')')
                t = t[:i + 1] + ','.join(args) + t[e:]
                e = find_call_args(t, i)
            pos = e
    USED[a] = names
    return t, 'slots %s' % ','.join(sorted(set(names)))
def ck(S0, S1, a):
    if S0 == S1: return 'no symbol change'
    rem_ok = lambda s: '@CScriptGameResourceObjectScriptedThingBase@@Q' in s and (BS.get(s, ('',))[0] in SLOTADDR or s not in BS)
    add_ok = lambda s: s in PDB and '@CScriptGameResourceObjectScriptedThingBase@@' in s
    return F.diffcheck(S0, S1, rem_ok, add_ok)
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'stb')
