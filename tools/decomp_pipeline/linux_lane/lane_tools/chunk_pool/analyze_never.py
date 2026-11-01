#!/usr/bin/env python3
"""Analyse every known function with no landed row and no parked attempt ("never attempted") and cut the authorable
ones into chunk-ready groups.

    export S=<lane scratch>; python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/analyze_never.py [outdir]

Writes (default outdir rebuild/candidates/manual/pending/never_attempted/; no retail bytes, only hashes):
  ANALYSIS.tsv  one line per row: extent-scan facts (extent vs manifest size, padding before/after, first instruction,
                int3 inside, EH scope, jump tables, tail calls), references (callers, data/vtable refs), retail-asm
                instruction signatures, vendor region, masked-body twin groups (identical bodies among the pool and
                against landed rows), calling convention, and the resulting KIND + a one-line reason.
  CHUNKS.tsv    proposed chunk cuts of the authorable rows, class siblings kept together, ordered easiest first
                (twins of landed rows, then by size band and residue of evidence); 10 rows per chunk, ids NA001..
  SUMMARY.md    counts per kind / band / module and the cut order.
KIND values: funclet (EH funclet, lands with its parent through families/funclets.py), thunk (jmp-only stub),
tiny (<= 16 bytes), fragment (no reference and not after a terminator, or decode failure), overlong (extent runs past
the next start), libpk/coda/vorbis (vendor audio region; names from the embedded sources), crt (between
toolchain_library neighbours), asm_sig (carries an instruction VC7.1 cannot emit from C++: retail_asm candidate),
twin_landed (masked body equal to a landed row: land from that source), retry_skipped (in SKIPPED_ROWS for a reason the
retail_asm rule or a later lesson removed: cut last), authorable (everything else).
"""
import sys, os, re, csv, glob, struct, hashlib, bisect, collections
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'bounds'))
from extent import Exe, fmt, is_filler, ROOT
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

OUT = sys.argv[1] if len(sys.argv) > 1 else ROOT + '/rebuild/candidates/manual/pending/never_attempted'
os.makedirs(OUT, exist_ok=True)
na = lambda a: a.strip().lower().replace('0x', '').zfill(8)

# ---- inputs
landed = {m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", open(ROOT + '/rebuild/build_candidates.ps1', errors='ignore').read())}
differ = {os.path.basename(p)[:8].lower() for p in glob.glob(ROOT + '/rebuild/candidates/manual/pending/*/differ/*.cpp') if not p.endswith('.test.cpp')}
rows = {}
for r in csv.DictReader(open(ROOT + '/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t'):
    a = r['address'].lower()
    if re.fullmatch(r'[0-9a-f]{8}', a): rows[a] = r
never = sorted(a for a in rows if a not in landed and a not in differ)
skipped = {}
for r in csv.DictReader(open(ROOT + '/rebuild/candidates/manual/SKIPPED_ROWS.tsv'), delimiter='\t'): skipped[na(r['address'])] = r
bfix = {}
if os.path.exists(ROOT + '/rebuild/candidates/manual/BOUNDS_FIX.tsv'):
    for r in csv.DictReader(open(ROOT + '/rebuild/candidates/manual/BOUNDS_FIX.tsv'), delimiter='\t'): bfix[na(r['address'])] = r
reserved = set()
for p in glob.glob(ROOT + '/rebuild/candidates/manual/pending/*/RESERVED.tsv'):
    for r in csv.DictReader(open(p), delimiter='\t'): reserved.add(na(r['address']))
vt = collections.defaultdict(list)   # func -> [(class, slot)]
for r in csv.DictReader(open(ROOT + '/rebuild/manifest/rtti-vtables.tsv'), delimiter='\t'): vt[r['func_va'].lower()].append((r['class'], r['slot']))
# landed grades by address (toolchain_library / retail_asm) from the source headers
grade = {}
for p in glob.glob(ROOT + '/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', p)
    if not m: continue
    try: head = open(p, errors='replace').read(600)
    except Exception: continue
    grade[m.group(1)] = 'toolchain_library' if 'Toolchain-library:' in head else 'retail_asm' if 'Retail-asm:' in head else 'cpp'
lsorted = sorted(landed)
# landed masked-body hashes from the catalog bytes
IMG_LO, IMG_HI = 0x400000, 0x1500000
def masked(b):
    b = bytearray(b); i = 0; n = len(b)
    while i + 4 <= n:
        v = struct.unpack_from('<I', b, i)[0]
        if IMG_LO <= v < IMG_HI:
            b[i:i + 4] = b'\0\0\0\0'; i += 4; continue
        if b[i] in (0xE8, 0xE9) and i + 5 <= n:
            b[i + 1:i + 5] = b'\0\0\0\0'; i += 5; continue
        i += 1
    return hashlib.sha1(bytes(b)).hexdigest()[:16]
lhash = {}
for r in csv.DictReader(open(ROOT + '/rebuild/oracles/auto-re-candidates.tsv'), delimiter='\t'):
    try: lhash.setdefault(masked(bytes.fromhex(r['bytes'])), []).append(r['address'].lower())
    except Exception: pass

E = Exe()
md = Cs(CS_ARCH_X86, CS_MODE_32)
# instructions VC7.1 never emits from C++ (GOTCHAS retail_asm evidence); `fnstsw ax` is ordinary float-compare codegen and is NOT one
ASM_SIG = {'rdtsc', 'cpuid', 'pushal', 'popal', 'fnstcw', 'fldcw', 'fninit', 'frndint', 'fscale', 'f2xm1', 'fyl2x', 'emms'}
SSE = re.compile(r'^(movaps|movups|movss|movd|movq|addps|mulps|subps|divps|addss|mulss|subss|divss|sqrtss|sqrtps|cvt\w+|shufps|unpck\w+|xorps|andps|orps|pxor|paddw|paddd|pmul\w+|psub\w+|punpck\w+|pand|por|pshufw|prefetch\w*|movntq|movntps|sfence|maxps|minps|maxss|minss|rcpps|rsqrtps|cmpps|cmpss|comiss|ucomiss)$')
def region(a):
    v = int(a, 16)
    if 0x00C123B0 <= v < 0x00C57C90: return 'libpk'
    if 0x00C57C90 <= v < 0x00C5E180: return 'coda'
    if 0x00C89000 <= v < 0x00C9F000: return 'vorbis'
    i = bisect.bisect_left(lsorted, a)
    prev = lsorted[i - 1] if i else None; nxt = lsorted[i] if i < len(lsorted) else None
    if prev and nxt and grade.get(prev) == 'toolchain_library' and grade.get(nxt) == 'toolchain_library': return 'crt'
    return ''
def band(n):
    return '<=16' if n <= 16 else '17-96' if n <= 96 else '97-256' if n <= 256 else '257-512' if n <= 512 else '513-1024' if n <= 1024 else '1025-4096' if n <= 4096 else '>4096'
BANDS = ['<=16', '17-96', '97-256', '257-512', '513-1024', '1025-4096', '>4096']

out = []
nev_set = set(never)
pool_hash = collections.defaultdict(list)
for a in never:
    r = rows[a]; va = int(a, 16); name = r['name']; mod = r['module']
    ns = E.next_start(va); msize = min((ns - va) if ns else 65536, 65536)
    sc = E.scan(va); ext = sc['extent']; f = sc['flags']
    body = E.read(va, ext) if ext else b''
    # instructions
    mn = []; asm = set(); sse = 0; rets = 0; calls = 0; int3 = 0
    for ins in md.disasm(body, va):
        mn.append(ins.mnemonic)
        if ins.mnemonic in ASM_SIG: asm.add(ins.mnemonic)
        if ins.mnemonic == 'fistp' and 'dword' in ins.op_str: asm.add('fistp_dword')
        if SSE.match(ins.mnemonic): sse += 1
        if ins.mnemonic.startswith('ret'): rets += 1
        if ins.mnemonic == 'call': calls += 1
        if ins.mnemonic == 'int3': int3 += 1
    first = mn[0] if mn else '?'
    pad_before = E.after_terminator(va) or is_filler(E.read(va - 4, 4)[-1:])
    gap_after = (ns - (va + ext)) if ns else 0
    pad_after = gap_after >= 0 and (gap_after == 0 or is_filler(E.read(va + ext, min(gap_after, 64))))
    xr = E.xref_kinds(va); callers = xr.get('call', 0); jmps = xr.get('jmp', 0); dref = sum(v for k, v in xr.items() if k.startswith('data_') or k.startswith('imm'))
    vts = vt.get(a, [])
    h = masked(body) if body else ''
    if h: pool_hash[h].append(a)
    twin_l = lhash.get(h, []) if h else []
    # classification
    kind = 'authorable'; why = []
    if name.startswith(('Unwind@', 'Catch@')):
        kind = 'funclet'
        i = bisect.bisect_left([int(x, 16) for x in never], va)
        why.append('EH funclet; lands with its parent via families/funclets.py')
    elif a in skipped and skipped[a].get('status', '') == 'open':
        note = skipped[a].get('note', ''); sk = skipped[a].get('kind', '')
        if sk in ('multi', 'prefix', 'cold') or re.search(r'wrong bounds|fragment|not an entry|thunk|continuation|mid-function|two functions', note, re.I):
            kind = 'fragment'; why.append(f'SKIPPED_ROWS {sk}: ' + note[:90])
        elif re.search(r'inline asm|fnstcw|fldcw|fistp|rdtsc|lane gap', note, re.I):
            kind = 'retry_skipped'; why.append('SKIPPED_ROWS before the retail_asm rule (2026-10-24) / lane-gap note, re-attemptable: ' + note[:90])
        else:
            kind = 'retry_skipped'; why.append(f'SKIPPED_ROWS {sk} (re-attemptable): ' + note[:90])
    elif a in bfix and bfix[a]['action'] == 'skip':
        kind = 'fragment'; why.append('BOUNDS_FIX skip: ' + bfix[a]['evidence'][:80])
    elif ext <= 6 and first == 'jmp':
        kind = 'thunk'; why.append('jmp-only stub')
    elif f.get('bad_decode') or f.get('runaway'):
        kind = 'fragment'; why.append('decode failure / runaway scan')
    elif not (callers or jmps or dref or vts) and not pad_before:
        kind = 'fragment'; why.append('no reference and not after a terminator (mid-function entry?)')
    elif ns and ext > msize + 8 and f.get('fell_into_start'):
        kind = 'overlong'; why.append('scan runs into the next start: boundary needs a fix (BOUNDS_FIX)')
    elif msize <= 16:
        kind = 'tiny'; why.append('<= 16 bytes')
    elif region(a):
        kind = region(a); why.append('vendor/library region')
    elif asm:
        kind = 'asm_sig'; why.append('retail_asm candidate: ' + ','.join(sorted(asm)))
    elif twin_l:
        kind = 'twin_landed'; why.append('masked body equals landed ' + ','.join(twin_l[:3]))
    if sse and kind == 'authorable': why.append(f'{sse} SSE/MMX instructions: try the xmmintrin/mmintrin intrinsics first')
    if a in reserved: why.append('RESERVED (ChatGPT pass 10)')
    length = ext if (pad_after and ext) else msize
    if not pad_after and ext and ext < msize - 16: why.append(f'scan stops {msize - ext} bytes before the next start over non-filler bytes: length uncertain (cold arm after a noreturn call?)')
    out.append(dict(address=a, module=mod, name=name, cc=r.get('calling_convention', ''), size=msize, extent=ext, length=length, band=band(msize), kind=kind,
                    first=first, pad_before=int(bool(pad_before)), pad_after=int(bool(pad_after)), int3_inside=int3, rets=rets, calls=calls,
                    callers=callers, jmp_refs=jmps, data_refs=dref, vtable='|'.join(f'{c}#{s}' for c, s in vts[:3]),
                    eh=('seh' if f.get('seh_scope') else 'eh' if f.get('eh_catch') or f.get('eh_continuation') else ''),
                    jtables=len(f.get('jtable', [])), tailcalls=len(f.get('tailcall', [])), asm_sig=','.join(sorted(asm)) + (f' sse{sse}' if sse else ''),
                    region=region(a), body_hash=h, twin_landed=','.join(twin_l[:3]), twin_pool='', reason='; '.join(why), scan=fmt(sc)[:160]))
# pool twins
for o in out:
    g = pool_hash.get(o['body_hash'], [])
    if len(g) > 1: o['twin_pool'] = f'{len(g)}:' + ','.join(x for x in g if x != o['address'])[:60]
cols = list(out[0].keys())
with open(OUT + '/ANALYSIS.tsv', 'w') as fh:
    w = csv.DictWriter(fh, cols, delimiter='\t', lineterminator='\n'); w.writeheader(); w.writerows(out)

# ---- chunk cuts of the authorable rows (+ twin_landed first, asm_sig last as retail_asm work)
auth = [o for o in out if o['kind'] in ('authorable', 'twin_landed', 'retry_skipped') and o['address'] not in reserved]
def ease(o):
    # easiest first: twin of landed, then more callers/vtable evidence, then smaller inside the 97-1024 band, then the rest
    bi = BANDS.index(o['band']); order = {2: 0, 3: 1, 4: 2, 1: 3, 5: 4, 6: 5, 0: 6}[bi]
    return (0 if o['kind'] == 'twin_landed' else 2 if o['kind'] == 'retry_skipped' else 1, order, -(min(o['callers'], 5) + (2 if o['vtable'] else 0)), o['size'])
auth.sort(key=ease)
# group siblings: rows of the same module travel together inside one band order
groups = collections.OrderedDict()
for o in auth:
    key = (0 if o['kind'] == 'twin_landed' else 2 if o['kind'] == 'retry_skipped' else 1, {2: 0, 3: 1, 4: 2, 1: 3, 5: 4, 6: 5, 0: 6}[BANDS.index(o['band'])], o['module'], o['body_hash'] if o['twin_pool'] else '')
    groups.setdefault(key, []).append(o)
chunks = []; cur = []
for key, g in groups.items():
    for o in g:
        cur.append(o)
        if len(cur) == 10: chunks.append(cur); cur = []
if cur: chunks.append(cur)
with open(OUT + '/CHUNKS.tsv', 'w') as fh:
    fh.write('chunk\taddress\tmodule\tname\tlength\tband\tkind\tcallers\tvtable\ttwin_pool\treason\n')
    for i, c in enumerate(chunks, 1):
        for o in c: fh.write(f"NA{i:03d}\t{o['address']}\t{o['module']}\t{o['name']}\t{o['length']}\t{o['band']}\t{o['kind']}\t{o['callers']}\t{o['vtable']}\t{o['twin_pool']}\t{o['reason']}\n")
# ---- summary
K = collections.Counter(o['kind'] for o in out); B = collections.Counter((o['kind'], o['band']) for o in out)
M = collections.Counter(o['module'] for o in auth)
pt = sum(1 for o in out if o['twin_pool'])
with open(OUT + '/SUMMARY.md', 'w') as fh:
    fh.write(f"# Never-attempted pool analysis ({len(out)} rows)\n\nGenerated by `chunk_pool/analyze_never.py` from the extent scanner "
             f"(`lane_tools/bounds/extent.py`), the manifest, SKIPPED_ROWS / BOUNDS_FIX / RESERVED ledgers, rtti-vtables and the landed catalog. "
             f"No retail bytes: bodies appear as masked hashes only.\n\n## Kinds\n\n| kind | rows | " + ' | '.join(BANDS) + " |\n|---|---:|" + "---:|" * len(BANDS) + "\n")
    for k, n in K.most_common():
        fh.write(f"| {k} | {n} | " + ' | '.join(str(B.get((k, b), 0)) for b in BANDS) + " |\n")
    fh.write(f"\nRows with an identical masked body elsewhere in the pool: {pt} (column twin_pool; one authoring covers the group).\n"
             f"Reserved to ChatGPT pass 10 and left out of the cuts: {sum(1 for o in out if o['address'] in reserved)}.\n\n"
             f"## Authorable rows cut into {len(chunks)} chunks of 10 (CHUNKS.tsv, NA001..)\n\nOrder: twins of landed rows first, then bands 97-256, 257-512, 513-1024, 17-96, 1025-4096, >4096; "
             f"inside a band the rows with more callers / a vtable slot first; class siblings stay in one chunk.\n\nTop modules among the authorable rows: "
             + ', '.join(f'{m} {n}' for m, n in M.most_common(15)) + ".\n")
print(K.most_common()); print(len(chunks), 'chunks;', len(auth), 'authorable rows;', pt, 'pool twins')
