# genmodel.py <Class> [--base <Base> --basehdr <engine/Base.h> --basesw SW1,SW2] [--slot N=<mangled>] [--src <header>]
# NF322 (2026-11-23): writes hdr/engine/<Class>.h = the CURRENT repo header with an opt-in polymorphic model
#   #ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL  <new model>  #else  <flat model, text unchanged>  #endif
# inserted round the flat model (from its `#pragma pack(push, 1)` line to its last FABLE_STATIC_ASSERT line).
# Slots come from the retail vftable found through RTTI (tools/vtdump.py logic) and their RTTI-port labels:
#   - base-less mode (no --base): every slot must be the class's own (label names <Class>, or --slot override); the model declares
#     all of them in slot order (slot 0 `??_E` / `??_G` -> `virtual ~<Class>();`), replacing `void* __vftable`.
#   - base mode: only slots whose label names <Class> are declared (overrides; inherited slots stay the base's), and slots past the
#     base's retail slot count must all be <Class>'s (new virtuals, declared in slot order). Members below sizeof(base) are dropped.
# Data members are the flat model's lines, unchanged (same names, types, offset comments); the opt-in pins sizeof and every kept
# member offset with FABLE_STATIC_ASSERT inside the #ifdef. Forward declarations for every class / struct / enum the declarations
# name (key from the mangled name: V class, U struct, W4 enum), namespaces kept. Declarations only, no bodies.
# Prints a one-line report and writes work/model_<Class>.tsv (slot, addr, label, action).
import sys, re, struct, os, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF429')
R = Path('/home/user/FableDecomp')
args = sys.argv[1:]; X = args[0]; opt = {'slot': {}}
k = 1
while k < len(args):
    if args[k] == '--slot':
        n, m = args[k + 1].split('=', 1); opt['slot'][int(n)] = m; k += 2
    elif args[k] == '--extra':
        # NF429: a non-virtual member a row of this class calls (mangled PDB spelling); declared after the virtual slots, own access
        opt.setdefault('extra', []).append(args[k + 1]); k += 2
    elif args[k] == '--pure':
        # NF429: a new pure virtual slot (retail _purecall); the declaration is a derived class's override spelling with the
        # class swapped to <Class> (name, type and access from FableWin.pdb's LF_ONEMETHOD `pure intro virtual` at that vftable offset)
        n, m = args[k + 1].split('=', 1); opt.setdefault('pure', {})[int(n)] = m; k += 2
    else: opt[args[k][2:]] = args[k + 1]; k += 2
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; secs = []
for i in range(ns):
    name = d[so + i * 40: so + i * 40 + 8].rstrip(b'\0').decode('latin1')
    vs, va, rs, raw = struct.unpack_from('<IIII', d, so + i * 40 + 8)
    secs.append((name, base + va, max(vs, rs), raw, rs))
def sec_of(va):
    for nm, s, n, r, rs in secs:
        if s <= va < s + n: return nm
def rd(va):
    for nm, s, n, r, rs in secs:
        if s <= va < s + rs: return struct.unpack_from('<I', d, r + va - s)[0]
def off2va(o):
    for nm, s, n, r, rs in secs:
        if r <= o < r + rs: return s + o - r
def find_all(pat):
    i = d.find(pat)
    while i >= 0:
        yield i; i = d.find(pat, i + 1)
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open(R / 'ghidra_out' / f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
def vtable(cls):
    res = []
    for o in find_all(b'.?AV' + cls.encode() + b'@@\0'):
        tdva = off2va(o) - 8
        for o1 in find_all(struct.pack('<I', tdva)):
            col = off2va(o1) - 12
            if col is None or rd(col) != 0 or rd(col + 4) != 0: continue
            for o2 in find_all(struct.pack('<I', col)):
                vt = off2va(o2) + 4
                if sec_of(vt) not in ('.rdata', '.data'): continue
                sl = []; j = 0
                while True:
                    t = rd(vt + 4 * j)
                    if t is None or sec_of(t) != '.text': break
                    sl.append(t); j += 1
                res.append((vt, sl))
    return res
V = vtable(X)
if len(V) != 1: print(X, 'VTABLES', len(V)); sys.exit(1)
vt, slots = V[0]
nbase = 0
if opt.get('base'):
    BV = vtable(opt['base'])
    if len(BV) == 1: nbase = len(BV[0][1])
    elif 'basen' in opt: nbase = int(opt['basen'])
    else: print(X, 'BASE_VTABLES', len(BV)); sys.exit(1)
def undname(m):
    return subprocess.run(['llvm-undname'], input=m + '\n', capture_output=True, text=True).stdout.strip().split('\n')[-1]
def owner(lab):
    m = re.match(r'\?\?_[EG](\w+)@@', lab)
    if m: return m.group(1)
    m = re.match(r'\?\?\w(\w+)@@', lab) if lab.startswith('??') else re.match(r'\?[^@]+@(\w+)@@', lab)
    return m.group(1) if m else None
fwd = {}
def ty(t):
    t = t.strip()
    for key, nm in re.findall(r'\b(class|struct|enum|union)\s+([\w:]+)', t):
        if not nm.startswith('std::'): fwd[nm] = key
    t = re.sub(r'\b(class|struct|enum|union)\s+', '', t)
    t = re.sub(r'>>', '> >', t)
    m = re.match(r'(.*?)\s+const\s*([&*])$', t)
    if m: return 'const %s%s' % (m.group(1), m.group(2))
    m = re.match(r'(.*?)\s*([&*])$', t)
    if m: return '%s%s' % (m.group(1), m.group(2))
    return t
def split_params(s):
    out = []; dd = 0; cur = ''
    for ch in s:
        if ch in '<(': dd += 1
        if ch in '>)': dd -= 1
        if ch == ',' and dd == 0: out.append(cur); cur = ''
        else: cur += ch
    if cur.strip(): out.append(cur)
    return out
def decl_of(mangled):
    if re.match(r'\?\?_[EG]', mangled): return 'public', 'virtual ~%s();' % X
    dm = undname(mangled)
    m = re.match(r'(public|protected|private): (virtual )?(.*?)__thiscall ([\w:]+)::([\w=!<>+\-*/\[\]~ ]+?)\((.*)\)( const)?$', dm)
    if not m: return None, 'UNPARSED ' + dm
    acc, virt, ret, cls, M, params, cst = m.groups()
    ps = [] if params.strip() == 'void' else [ty(p) for p in split_params(params)]
    return acc, 'virtual %s %s(%s)%s;' % (ty(ret), M, ', '.join(ps), ' const' if cst else '')
rows = []; decls = []; bad = []
for j, t in enumerate(slots):
    lab = opt['slot'].get(j) or L.get('%08x' % t, '-')
    own = owner(lab) if lab != '-' else None
    if opt.get('base') and j < nbase:
        if own == X:
            acc, dl = decl_of(lab)
            if lab.startswith('??_') and re.match(r'\?\?_[GE]%s@@[ME]AE' % re.escape(X), lab):
                # NF429: retail's deleting destructor is protected / private, so the class declares its destructor (FableWin.pdb
                # LF_ONEMETHOD ~X `protected virtual`, not compiler-generated; ??1X@@MAE@XZ in both PDBs)
                decls.append((j, t, {'M': 'protected', 'E': 'private'}[lab[lab.index('@@') + 2]], 'virtual ~%s();' % X, lab)); act = 'dtor-access'
            elif lab.startswith('??_'): act = 'inherit-dtor'
            elif acc is None: bad.append('slot%d:%s' % (j, dl)); act = 'BAD'
            else: decls.append((j, t, acc, dl, lab)); act = 'override'
        else: act = 'inherit' if own != X else 'override'
        if lab == '-': act = 'inherit?'
    elif j in opt.get('pure', {}):
        dm0, ov0 = opt['pure'][j].split('!'); own0 = owner(dm0); lab = dm0.replace('@%s@@' % own0, '@%s@@' % X, 1)
        acc, dl = decl_of(lab)
        if acc is None: bad.append('slot%d:%s' % (j, dl)); act = 'BAD'
        else: decls.append((j, t, acc, dl[:-1] + ' = 0;', 'pure (FableWin.pdb LF_ONEMETHOD pure intro virtual, its access; name and type as the retail override %s)' % ov0)); act = 'pure'
    else:
        if own != X: bad.append('slot%d:%s:%s' % (j, '%08X' % t, lab)); act = 'BAD'
        else:
            acc, dl = decl_of(lab)
            if acc is None: bad.append('slot%d:%s' % (j, dl)); act = 'BAD'
            else: decls.append((j, t, acc, dl, lab)); act = 'declare' if not opt.get('base') else 'new'
    rows.append('%d\t%08X\t%s\t%s' % (j, t, lab, act))
(A / 'work' / ('model_%s.tsv' % X)).write_text('vftable\t%08X\tslots\t%d\tbase_slots\t%d\n' % (vt, len(slots), nbase) + '\n'.join(rows) + '\n')
if bad: print(X, 'BAD', ' '.join(bad)); sys.exit(1)
# header
src = Path(opt['src']) if 'src' in opt else R / 'rebuild/include/engine' / (X + '.h')
H = src.read_text(encoding='utf-8').split('\n')
ip = next(i for i, l in enumerate(H) if l.startswith('#pragma pack(push'))
last = max(i for i, l in enumerate(H) if l.startswith('FABLE_STATIC_ASSERT'))
ci = next(i for i in range(ip, last) if re.match(r'(class|struct)\s+' + re.escape(X) + r'\s*(:.*)?\{', H[i]))
ce = next(i for i in range(ci, last) if H[i].startswith('};'))
mem = []; carry = []
pre = [l for l in H[ip + 1:ci] if l.strip()]          # lines between `#pragma pack(push` and the class line (existing opt-ins), kept verbatim
for l in H[ci + 1:ce]:
    if l.strip() in ('public:', ''): continue
    if '__vftable' in l: continue
    m = re.search(r'//\s*\+0x([0-9a-fA-F]+)', l)
    if not m or l.lstrip().startswith('#'): carry.append(l); continue   # existing opt-in lines inside the class, kept verbatim
    off = int(m.group(1), 16)
    nm = re.match(r'\s*(.*?)\s*([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*;', l)
    mem.append((off, nm.group(2), l))
if opt.get('base') and 'basesize' not in opt: print(X, 'NEED --basesize'); sys.exit(1)
bsz = int(opt['basesize'], 16) if 'basesize' in opt else 4
keep = [(o, n, l) for o, n, l in mem if o >= bsz]
if opt.get('base') and keep and keep[0][0] > bsz:
    # a flat `_pad_*` member straddles the end of the base (e.g. +0x24[4] over a 0x25-byte packed base): its tail bytes are this
    # class's alignment padding; keep them as an unnamed `_pad_*` member (the generated headers' convention for unnamed bytes)
    g = keep[0][0] - bsz
    keep.insert(0, (bsz, '_pad_0x%02x' % bsz, '    unsigned char _pad_0x%02x[0x%x];%s// +0x%02x (tail of the flat member that straddles the base end)' % (bsz, g, ' ' * 20, bsz)))
szm = [l for l in H[ce:last + 1] if re.search(r'sizeof\(' + re.escape(X) + r'\)', l)]
size = re.search(r'==\s*(0x[0-9a-fA-F]+)', szm[0]).group(1)
SW = 'FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper()
key = 'class'
# forward declarations (after decls computed)
fl = []
for nm, kk in sorted(fwd.items()):
    if nm == X: continue
    if '::' in nm:
        nsp, leaf = nm.rsplit('::', 1)
        fl.append('namespace %s { %s %s; }' % (nsp, kk, leaf))
    else: fl.append('%s %s;' % (kk, nm))
out = []
out.append('#ifdef %s' % SW)
out.append('// Opt-in (NF429, 2026-11-25; generator NF322 genmodel.py as run by NF329 / NF427, NF325 style): the polymorphic retail class, declarations only (no bodies, no layout change). Retail vftable')
out.append('// 0x%08X (RTTI .?AV%s@@%s), %d slots; %s' % (vt, X, (' : ' + opt['base']) if opt.get('base') else '', len(slots),
           ('the declared slots are this class\'s overrides (RTTI-port labels name %s); every other slot stays the base\'s.' % X) if opt.get('base')
           else 'every slot is this class\'s own (RTTI-port labels), so `void* __vftable` becomes these virtual declarations, in slot order.'))
out.append('// Members: the flat model\'s lines below, same offsets (asserts). Slot order checked against retail by an emitted-vftable probe (NF429 NOTES).')
if opt.get('needs'): out.append('// Needs (defined by the including row, above its first #include): %s.' % ', '.join(opt['needs'].split(',')))
if opt.get('prefwd'): out += [x.strip() + ';' for x in opt['prefwd'].split(';') if x.strip()]   # names the base header uses undeclared
if opt.get('base'): out.append('#include "%s"' % opt['basehdr'])
if any('std::' in d[3] for d in decls): out.append('#include "stlport/fable_containers.h"')   # NF429: STLport std::vector in a slot signature
out += fl
out.append('#pragma pack(push, 1)')
out += pre
out.append('%s %s%s {' % (key, X, (' : public ' + opt['base']) if opt.get('base') else ''))
if carry: out.append('public:')                       # the flat model was a struct / public: keep the carried opt-in lines public
out += carry
cur = None
for j, t, acc, dl, lab in decls:
    if acc != cur: out.append('%s:' % acc); cur = acc
    out.append('    %s%s// slot %d 0x%08X %s' % (dl, ' ' * max(1, 70 - len(dl)), j, t, lab))
for xm in opt.get('extra', []):
    dmx = undname(xm)
    mx = re.match(r'(public|protected|private): (.*?)__thiscall ([\w:]+)::(\w+)\((.*)\)( const)?$', dmx)
    if not mx or mx.group(3) != X: print(X, 'BAD_EXTRA', dmx); sys.exit(1)
    psx = [] if mx.group(5).strip() == 'void' else [ty(q) for q in split_params(mx.group(5))]
    dlx = '%s %s(%s)%s;' % (ty(mx.group(2)), mx.group(4), ', '.join(psx), ' const' if mx.group(6) else '')
    if mx.group(1) != cur: out.append('%s:' % mx.group(1)); cur = mx.group(1)
    out.append('    %s%s// %s (non-virtual; PDB name set, called by a row)' % (dlx, ' ' * max(1, 70 - len(dlx)), xm))
out.append('public:')
for o, n, l in keep: out.append(l)
out.append('};')
out.append('#pragma pack(pop)')
out.append('FABLE_STATIC_ASSERT(sizeof(%s) == %s);' % (X, size))
for o, n, l in keep: out.append('FABLE_STATIC_ASSERT(offsetof(%s, %s) == 0x%x);' % (X, n, o))
out.append('#else')
NH = H[:ip] + out + H[ip:last + 1] + ['#endif // %s' % SW] + H[last + 1:]
dst = A / 'hdr/engine' / (X + '.h'); dst.parent.mkdir(parents=True, exist_ok=True)
dst.write_text('\n'.join(NH), encoding='utf-8')
print(X, 'OK carried=%d' % (len(pre) + len(carry)), 'vt=%08X slots=%d base=%d declared=%d members=%d/%d basesize=0x%x size=%s' % (vt, len(slots), nbase, len(decls), len(keep), len(mem), bsz, size))
