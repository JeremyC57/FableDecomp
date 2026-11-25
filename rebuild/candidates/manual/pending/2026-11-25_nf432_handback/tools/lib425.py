"""lib425.py: shared helpers for the NF425 CLONE / COPY generators (header discovery after gen417, compile, symbol sets, evidence)."""
import csv, re, os, sys, glob, subprocess, json, collections
R = '/home/user/FableDecomp'
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, A + '/tools'); import nf
csv.field_size_limit(10**9)
def tsv(p): return list(csv.reader(open(p, newline=''), delimiter='\t', quoting=csv.QUOTE_NONE))
WL = {r['address']: r for r in csv.DictReader(open(A + '/evidence/worklist.tsv'), delimiter='\t', quoting=csv.QUOTE_NONE)}
SHAPE = {l.split('\t')[0]: l.rstrip('\n').split('\t')[1] for l in open(A + '/evidence/shapes.tsv')}
BASES = {r[1]: r for r in tsv(R + '/ghidra_out/rtti_bases.tsv')[1:]}
REC = json.load(open(A + '/trk/rec.json'))
PAY = {x['address']: x for x in json.load(open(A + '/work/p_all/payload.json'))['result']['authored']}
TAB = collections.defaultdict(set); TABW = collections.defaultdict(set)
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t', quoting=csv.QUOTE_NONE):
    if not r.get('target') or not r.get('address'): continue
    TAB[r['target'].lower()].add(r['symbol']); TABW[r['symbol']].add((r['target'].lower(), r['address'].lower()))
PDB_E = set(l.rstrip('\n').split('\t')[1] for l in open(R + '/ghidra_out/egor_pdb_names.tsv') if '\t' in l)
PDB_F = set(l.rstrip('\n').split('\t')[1] for l in open(R + '/ghidra_out/fablewin_pdb_names.tsv') if '\t' in l)
def pdb_src(sym):
    s = []
    if sym in PDB_E: s.append('Ego_r')
    if sym in PDB_F: s.append('FableWin')
    return s
OM = {}
for f in ('predictions_n2.tsv', 'predictions_n1.tsv'):
    p = R + '/rebuild/candidates/manual/naming/audits/2026-11-25_order_match/' + f
    for r in csv.DictReader(open(p), delimiter='\t'):
        OM.setdefault(r['target'], (r['predicted'], f[12:14]))
EXCL = ('_quarantine/', 'campfire_types.h')
HDRDEFS = {}
for p in sorted(glob.glob(R + '/rebuild/include/engine/*.h')):
    rel = os.path.relpath(p, R + '/rebuild/include')
    if any(e in rel for e in EXCL): continue
    t = open(p, errors='replace').read()
    for m in re.finditer(r'^\s*class\s+(\w+)\s*:\s*public\s+[\w:<>, ]+\s*\{', t, re.M):
        HDRDEFS.setdefault(m.group(1), []).append(rel)
def header_of(C):
    h = HDRDEFS.get(C, []); own = 'engine/%s.h' % C
    if own in h: return own
    if h: return h[0]
    if os.path.exists(R + '/rebuild/include/' + own): return own
    return ''
def ancestors(q):
    if q not in BASES: return []
    return [x.split('@')[0] for x in BASES[q][4].split(';') if x]
def direct(q):
    return BASES[q][3].split(';')[0].split('@')[0] if q in BASES else ''
sys.path.insert(0, R + '/tools/decomp_pipeline'); import verify_and_land as V
E = V.env()
def compile_(src, obj, flags=None):
    fl = flags or ['/O2', '/Oy']
    cmd = ([V.WINE] if V.WINE else []) + [str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/W3', '/Fo' + obj, src])
    try: r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=E, timeout=int(os.environ.get('CCT', '60')))
    except subprocess.TimeoutExpired: return 99, 'TIMEOUT'
    return r.returncode, (r.stdout + r.stderr)
def syms(obj):
    st = subprocess.run(['objdump', '-t', obj], capture_output=True, text=True).stdout
    ext = [l.split()[-1] for l in st.splitlines() if '(scl   2)' in l]
    und = sorted(set(l.split()[-1] for l in st.splitlines() if '(scl   2)' in l and '(sec  0)' in l))
    dfn = sorted(set(s for s in ext if s not in und))
    return dfn, und
def head_pragmas(src):
    """HEAD's #pragma optimize / auto_inline lines (kept verbatim) and its // Flags: line."""
    pr = [l for l in src.splitlines() if re.match(r'\s*#\s*pragma\s+(optimize|auto_inline|inline_depth)', l)]
    fl = [l for l in src.splitlines() if l.startswith('// Flags:')]
    return pr, fl
def catflags(a):
    return nf.field(a, 'CompilerFlags')
def wrap(prefix, text, width=136):
    out, line = [], prefix
    for w in text.split(' '):
        if len(line) + 1 + len(w) > width and line.strip() != '//':
            out.append(line); line = '//   ' + w
        else: line = (line + ' ' + w) if line else w
    out.append(line); return out
def owner_self(t):
    r = REC.get(t); return (r['self_leaf'] if r else ''), (r['pdb_spelling'] if r else ''), (r['status'] if r else '')
def cxx_flags(a):
    """catalog CompilerFlags as a cl argument list (for the generator's own symbol compile)."""
    f = catflags(a)
    if not f: return None
    return [x for x in re.findall(r"'([^']*)'", f) or f.split()]
def leafcmp(obj, sym, a):
    """compiled leaf bytes v retail (catalog) bytes with the object's relocation fields masked: 'EQ' / 'LEN a v b' / 'BYTES'."""
    secs, sl = nf.coff(obj)
    k = None
    for i, nm, v, sec, t, scl in sl:
        if nm == sym and sec > 0: k = sec; break
    if k is None: return 'NOLEAF'
    d = bytearray(secs[k - 1]['data']); rb = bytearray.fromhex(nf.catalog()[a]['bytes'])
    while rb and rb[-1] in (0xCC, 0x90) and len(rb) > len(d): rb = rb[:-1]
    if len(d) != len(rb): return 'LEN %d v %d' % (len(d), len(rb))
    for o, s, t in secs[k - 1]['rel']:
        for j in range(4): d[o + j] = 0; rb[o + j] = 0
    return 'EQ' if d == rb else 'BYTES'
PRE_CAB = ['#define FABLE_RETAIL_CCHARSTRING', '#define FABLE_RETAIL_BASE_OBJECT', '#define FABLE_RETAIL_CCREATUREACTIONBASE',
           '#include "fable_string.h"', '#include "engine/CCreatureActionBase.h"']
# the CCombatActionBase MELEE prelude of the landed 0x008CAA70 (CCreatureAction_NymphSummonOutOf::Clone), minus its class's own opt-ins
PRE_CCOMBAT = ['#define FABLE_RETAIL_CCHARSTRING', '#define FABLE_RETAIL_BASE_OBJECT', '#define FABLE_RETAIL_CTHING', '#define FABLE_RETAIL_CTHINGPHYSICAL',
    '#define FABLE_RETAIL_CTHINGGAMEOBJECT', '#define FABLE_RETAIL_CTHINGCREATUREBASE', '#define FABLE_RETAIL_CTCBASE', '#define FABLE_RETAIL_CINTELLIGENTPOINTER',
    '#define FABLE_RETAIL_C3DVECTOR', '#define FABLE_RETAIL_C3DVECTOR_PACKED', '#define FABLE_RETAIL_CTCPHYSICSBASE', '#define FABLE_RETAIL_CCREATUREACTIONBASE',
    '#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS', '#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27', '#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE',
    '#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR', '#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE', '#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE',
    '#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION', '#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED', '#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS',
    '#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR', '#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR',
    '#include "fable_string.h"', '#include "engine/fable_ai_action_enums.h"', '#include "engine/C3DVector.h"', '#include "engine/CThingCreatureBase.h"',
    '#include "engine/CIntelligentPointer.h"', '#include "engine/CCountedPointer.h"', '#include "stlport/fable_containers.h"', '#include "engine/CCreatureActionBase.h"',
    '#include "engine/CCombatActionBase.h"', '#include "engine/CSoundPair.h"', '#include "engine/CTCPhysicsBase.h"']
