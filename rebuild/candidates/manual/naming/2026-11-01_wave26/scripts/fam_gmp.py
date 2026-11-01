# fam_gmp.py <ov> <rows.txt>: 0x00449970 -> ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (Ego_r 0x004450F0, FableWin 0x01A596C0)
# rows whose player-manager stand-in `CPlayerManager_Retail` declares `CPlayer* GetMainPlayer();` / `CPlayer* GetPlayer();`:
# the stand-in becomes class CPlayerManager, CPlayer a class (PDB key V), the getter returns CPlayer& (same code: the address).
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
FIN = '?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ'
BT, BS = inputs.bindings(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
B70 = BT['00449970']
COMMENT70 = set()
DECL = re.compile(r'\bCPlayer\s*\*\s*(GetMainPlayer|GetPlayer)\s*\(\s*\)\s*;')
class Body:
    def __init__(s, a, b, t): s.a, s.b, s.t = a, b, t
    def start(s, i): return s.a
    def end(s, i): return s.b
    def group(s, i): return s.t[s.a:s.b]
CHAIN = r'((?:[A-Za-z_]\w*(?:\(\))?\s*(?:->|\.)\s*)+)'
def tr(t, a):
    if not re.search(r'\bCPlayerManager_Retail\b', t): return None, 'no CPlayerManager_Retail'
    code = re.sub(r'//[^\n]*', '', t)
    if re.search(r'\bCPlayerManager\b', code.replace('CPlayerManager_Retail', '')): return None, 'file already names CPlayerManager'
    if re.search(r'\bCPlayer_Retail\s*\*\s*(GetMainPlayer|GetPlayer)\s*\(\s*\)\s*;', code):
        if re.search(r'\bCPlayer\b', code): return None, 'CPlayer_Retail getter and a CPlayer type'
        t = re.sub(r'\bCPlayer_Retail\b', 'CPlayer', t)
    m = re.search(r'\b(?:struct|class)\s+CPlayerManager_Retail\b[^;{]*\{', t)
    if not m: return None, 'no CPlayerManager_Retail body'
    depth = 0
    for j in range(m.end() - 1, len(t)):
        depth += {'{': 1, '}': -1}.get(t[j], 0)
        if depth == 0: break
    m = Body(m.end(), j, t)
    d = DECL.findall(m.group(1))
    if len(d) != 1: return None, 'getter decls %r' % d
    g = d[0]
    dl = re.search(r'[^\n]*\bCPlayer\s*\*\s*' + g + r'\s*\(\s*\)\s*;[^\n]*', m.group(1)).group(0)
    if '0x00449970' in dl or '0x449970' in dl.lower(): COMMENT70.add(a)
    if g == 'GetPlayer' and len(re.findall(r'\bGetPlayer\s*\(', t)) != len(re.findall(r'\bGetPlayer\s*\(\s*\)', t)): return None, 'GetPlayer with arguments elsewhere'
    if g == 'GetPlayer' and re.search(r'\bGetMainPlayer\b', t): return None, 'both GetPlayer and GetMainPlayer'
    body = m.group(1); nb = DECL.sub('CPlayer& GetMainPlayer();', body)
    t = t[:m.start(1)] + nb + t[m.end(1):]
    if g == 'GetPlayer':
        # only calls through the player manager stand-in may be renamed: every other GetPlayer() call stays -> require none
        others = [x for x in re.finditer(r'\bGetPlayer\s*\(\s*\)', t)]
        t = re.sub(r'\bGetPlayer(\s*)\(\s*\)', r'GetMainPlayer\1()', t)
    t = t.replace('// 0x00449970 (labelled "Catch@0040d959", propagated)', '// 0x00449970')
    t = re.sub(r'\bCPlayerManager_Retail\b', 'CPlayerManager', t)
    t = re.sub(r'\bstruct(\s+CPlayerManager\s*)\{', r'class\1{ public:', t)
    t = re.sub(r'\bstruct(\s+CPlayerManager\s*);', r'class\1;', t)
    t = re.sub(r'\bstruct(\s+CPlayer\b)([^;{(]*)\{', r'class\1\2{ public:', t)
    t = re.sub(r'\bstruct(\s+CPlayer\s*);', r'class\1;', t)
    t = re.sub(r'GetMainPlayer\(\)\s*->', 'GetMainPlayer().', t)
    # remaining pointer uses: take the address of the whole postfix chain
    def amp(mm):
        pre = t[max(0, mm.start() - 2):mm.start()]
        return mm.group(0) if pre.endswith('&') else '&' + mm.group(0)
    t = re.sub(CHAIN + r'GetMainPlayer\(\)(?!\s*\.)', amp, t)
    return t, 'GetMainPlayer& on CPlayerManager (was %s)' % g
def ck(S0, S1, a):
    if not (S0 & B70) and a not in COMMENT70: return 'emits no 0x00449970 spelling before'
    ren = lambda s: s.replace('CPlayerManager_Retail', 'CPlayerManager').replace('UCPlayer@@', 'VCPlayer@@').replace('UCPlayerManager@@', 'VCPlayerManager@@')
    R0 = set(ren(s) for s in S0)
    add_ok = lambda s: s == FIN or s in R0
    rem_ok = lambda s: s in B70 or ren(s) in S1 or 'CPlayerManager_Retail' in s
    if S0 == S1 and a in COMMENT70 and not (S0 & B70): return None   # dead declaration (never called): respelled for the model only
    return F.diffcheck(S0, S1, rem_ok, add_ok, [FIN])
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'gmp')
